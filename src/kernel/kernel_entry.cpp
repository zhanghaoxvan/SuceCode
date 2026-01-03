#include "kernel_entry.hpp"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <kernel/branch_parser.hpp>
#include <kernel/kernel_handler.hpp>
#include <kernel/sout.hpp>
#include <kernel/serialize.hpp>
#include <string>
#include <vector>
#include <cstring>
#include <cstdint>

namespace kernel {
    static string exe;

    const string& getExe() {
        return exe;
    }

    bool embedBytecodeToExe(const string& bytecodeFile, const string& outputExe) {
        std::ifstream bcFile(bytecodeFile, std::ios::binary);
        if (!bcFile) {
            return false;
        }
        
        bcFile.seekg(0, std::ios::end);
        std::streamsize bcSizeStream = bcFile.tellg();
        if (bcSizeStream == -1) {
            return false;
        }
        size_t bcSize = static_cast<size_t>(bcSizeStream);
        bcFile.seekg(0, std::ios::beg);
        
        std::vector<char> bcData(bcSize);
        bcFile.read(bcData.data(), bcSize);
        bcFile.close();

        string suceExe = getExe();
        std::ifstream suceFile(suceExe, std::ios::binary);
        std::ofstream outFile(outputExe, std::ios::binary);
        
        if (!suceFile || !outFile) {
            return false;
        }

        outFile << suceFile.rdbuf();
        suceFile.close();
        

        const char* marker = "__SUCE_BYTECODE_START__";
        outFile.write(marker, strlen(marker));

        uint64_t bcSize64 = static_cast<uint64_t>(bcSize);

        for (int i = 0; i < sizeof(uint64_t); ++i) {
            char byte = static_cast<char>((bcSize64 >> (i * 8)) & 0xFF);
            outFile.write(&byte, 1);
        }

        if (!outFile.good()) {
            return false;
        }
        
  
        outFile.write(bcData.data(), bcSize);

        uint32_t checksum = 0;
        for (char c : bcData) {
            checksum += static_cast<unsigned char>(c);
        }

        outFile.write(reinterpret_cast<const char*>(&checksum), sizeof(checksum));

        outFile.close();

        return true;
    }
    int main(int argc, char **args) {
        exe = args[0];
        std::replace(exe.begin(), exe.end(), '\\', '/');

        ArgsParser parser;
        parser.addOption("o", ArgsParser::Type::SHORT);
        parser.addOption("exe", ArgsParser::Type::LONG); 
        parser.parse(argc, args);

        vector<string> parsed_args = parser.getArgs();

        if (parsed_args.empty() && parser.hasNoExtraArgs()) {
            interact();
        } else if (parser.hasNoExtraArgs()) {
            string file = parsed_args.front();

            size_t lastDotPos = file.find_last_of('.');
            size_t lastSlashPos = file.find_last_of("\\/");
            string fileName;
            
            if (lastSlashPos != string::npos) {
                fileName = file.substr(lastSlashPos + 1);
            } else {
                fileName = file;
            }
            
            string outputFile;
            if (lastDotPos != string::npos && lastDotPos > lastSlashPos) {
                outputFile = fileName.substr(0, fileName.find_last_of('.')) + ".sout";
            } else {
                outputFile = fileName + ".sout";
            }

            tryRun([&]() {
                compileTo(new FileScanner{file}, outputFile);
            });

            if (parser.has("exe")) {
                string exeOutput;
                if (lastDotPos != string::npos && lastDotPos > lastSlashPos) {
                    exeOutput = fileName.substr(0, fileName.find_last_of('.')) + ".exe";
                } else {
                    exeOutput = fileName + ".exe";
                }

                if (parser.has("o")) {
                    exeOutput = parser.get("o");

                    if (exeOutput.find_last_of('.') == string::npos ||
                        exeOutput.substr(exeOutput.find_last_of('.')) != ".exe") {
                        exeOutput += ".exe";
                    }
                }

                embedBytecodeToExe(outputFile, exeOutput);
            } else {

            }

            array converted_args{parsed_args.size(), nullptr};
            for (uinteger i = 0; i < parsed_args.size(); ++i)
                converted_args[i] = new Variant{parsed_args[i]};
            tryRun([&]() { doFile(file, {new Variant{converted_args}}); });
            return EXIT_SUCCESS;
        } else if (!parsed_args.empty()) {
            string output = parser.get("o");
            tryRun([&]() {
                compileTo(new FileScanner{parsed_args.front()}, output);
            });
        }
        return EXIT_SUCCESS;
    }
} // namespace kernel

