#include <kernel/kernel_entry.hpp>
#include <kernel/runtime/evaluator.hpp>
#include <kernel/deserialize.hpp>
#include <kernel/colors.hpp>

#include <fstream>
#include <vector>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>

uint64_t readUInt64LE(const byte* data) {
    uint64_t result = 0;
    for (int i = 0; i < 8; ++i) {
        result |= static_cast<uint64_t>(static_cast<uint8_t>(data[i])) << (i * 8);
    }
    return result;
}

bool loadEmbeddedBytecode(const std::string& exePath, kernel::State& state) {

    std::ifstream file(exePath, std::ios::binary | std::ios::ate);
    if (!file) {
     
        return false;
    }

    std::streamsize size = file.tellg();

    file.seekg(0, std::ios::beg);

    std::vector<byte> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {

        return false;
    }
    file.close();

    const char* marker = "__SUCE_BYTECODE_START__";
    size_t markerLen = strlen(marker);


    bool markerFound = false;
    size_t markerPos = 0;

    if (buffer.size() >= markerLen + sizeof(uint64_t)) {

        for (size_t i = buffer.size() - markerLen - sizeof(uint64_t); i > 0; --i) {
            if (memcmp(buffer.data() + i, marker, markerLen) == 0) {
                markerFound = true;
                markerPos = i;
                break;
            }
        }

        if (!markerFound && memcmp(buffer.data(), marker, markerLen) == 0) {
            markerFound = true;
            markerPos = 0;
        }
    }

    if (!markerFound) {

        if (buffer.size() > markerLen) {
            size_t checkPos = buffer.size() > 200 ? buffer.size() - 200 : 0;
        }
        return false;
    }


    if (markerPos + markerLen + sizeof(uint64_t) > buffer.size()) {

        return false;
    }

    uint64_t bcSize64 = readUInt64LE(buffer.data() + markerPos + markerLen);

    if ((bcSize64 >> 32) > 0x100000) { 

        // 交换高低位字节序
        bcSize64 = ((bcSize64 << 32) | (bcSize64 >> 32));
        // 再交换每32位中的高低16位
        bcSize64 = ((bcSize64 & 0xFFFF0000FFFF0000ULL) >> 16) | ((bcSize64 & 0x0000FFFF0000FFFFULL) << 16);
        // 再交换每16位中的高低8位
        bcSize64 = ((bcSize64 & 0xFF00FF00FF00FF00ULL) >> 8) | ((bcSize64 & 0x00FF00FF00FF00FFULL) << 8);
    }


    size_t bcSize = static_cast<size_t>(bcSize64);

    if (bcSize64 > buffer.size() - markerPos - markerLen - sizeof(uint64_t)) {
  
        bcSize64 = buffer.size() - markerPos - markerLen - sizeof(uint64_t);
        bcSize = static_cast<size_t>(bcSize64); 

    }

    if (bcSize64 == 0 || bcSize64 > 2000000000) {

        return false;
    }

    if (bcSize64 > static_cast<uint64_t>(buffer.size() * 0.5)) {

        bcSize64 = (buffer.size() - markerPos - markerLen - sizeof(uint64_t)) / 2;
        bcSize = static_cast<size_t>(bcSize64);
    }

    if (markerPos + markerLen + sizeof(uint64_t) + bcSize > buffer.size()) {

        return false;
    }
    

    std::vector<byte> bcData(buffer.begin() + markerPos + markerLen + sizeof(uint64_t),
                             buffer.begin() + markerPos + markerLen + sizeof(uint64_t) + bcSize);

    if (bcData.size() >= sizeof(uint32_t)) {

        uint32_t storedChecksum = *reinterpret_cast<const uint32_t*>(&bcData[bcData.size() - sizeof(uint32_t)]);

        uint32_t actualChecksum = 0;
        for (size_t i = 0; i < bcData.size() - sizeof(uint32_t); ++i) {
            actualChecksum += static_cast<unsigned char>(bcData[i]);
        }
        
    }


    try {
        
        state = kernel::deserialize<kernel::State>(bcData);
        return true;
    } catch (const kernel::RuntimeException& e) {
      
        return false;
    } catch (const std::exception& e) {

        return false;
    } catch (...) {

        return false;
    }
}
int main(int argc, char **args) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    kernel::State state;
    std::string exePath = args[0];

    #ifdef _WIN32

    if (exePath.size() < 4 || exePath.substr(exePath.size() - 4) != ".exe") {
        exePath += ".exe";
    }
    #endif


    if (loadEmbeddedBytecode(exePath, state)) {
        kernel::Evaluator evaluator;
        try {
            evaluator.setup(state);
            return EXIT_SUCCESS;
        } catch (const std::exception& e) {


            return EXIT_FAILURE;
        }
    } else {


        return kernel::main(argc, args);
    }
}
