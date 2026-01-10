#include "kernel_handler.hpp"
#include <fstream>
#include <iomanip>
#include <kernel/compiler/compiler.hpp>
#include <kernel/sout.hpp>
#include <kernel/runtime/follower.hpp>
#include <kernel/serialize.hpp>
#include <kernel/version>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

namespace kernel {
    void handleInput(const string &code) {
        if (code.empty())
            return;
        if (code == "exit")
            exit(EXIT_SUCCESS);
        if (code == "clear") {
            system(CLEAR);
            return;
        }
        static Environment env;
        tryRun([&] {
            try {
                Compiler compiler{new StringScanner{"return " + code}};
                auto pair = compiler.load().getMethod().call(env, {});
                cout << *pair.first << endl;
                env = std::move(pair.second);
            } catch (...) {
                Compiler compiler{new StringScanner{code}};
                auto pair = compiler.load().getMethod().call(env, {});
                cout << *pair.first << endl;
                env = std::move(pair.second);
            }
        });
    }

    void interact() {
        cout << std::left << std::setw(5) << "Suce Code Kernel " kernel_VERSION " "
             << std::right << std::setw(11) <<  KERNEL_COR << endl
             << std::left << std::setw(5) << "Build in "  __DATE__  "   " __TIME__  << endl
             << std::right << std::setw(20) << "Enter exit to exit" << endl;
        string input;
        cout << "/>> ";
        while (std::getline(cin, input)) {
            handleInput(input);
            cout << "/>> ";
        }
    }
    Ref<Variant> doFile(const string &path, const array &args,
                        const string &name) {
        size_t dot_pos = path.find_last_of('.');
        string ext;
        if (dot_pos != string::npos) {
            ext = path.substr(dot_pos + 1);

            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        }
        if (ext != "sce" && ext != "sout") {
            throw Exception{"Invalid file extension. Only .sce files are allowed."};
        } 

        if (Sout::isSout(path))
            return Sout::load(path).call(args);
        Compiler compiler{new FileScanner{path}};
        Function func = compiler.load();
        func.setProperty("__name__", new Variant{name});
        return func.call(args);
        
    }
    void compileTo(Scanner *scanner, const string &dest) {
        Compiler compiler{scanner};
        compiler.enableProgressBar(true);

        Function func = compiler.load();
        vector<byte> bytes = Sout::dump(func);
        std::ofstream os(dest, std::ios::binary);
        os.write((const char *)bytes.data(), bytes.size());
    }
    void tryRun(const std::function<void()> what) {
        try {
            what();
        } catch (const std::exception &e) {
            std::cerr << Singleton<ProgramFollower>::instance()->position()
                      << endl;
            std::cerr << e.what() << endl;
            auto stack = Singleton<ProgramFollower>::instance()->getCallStack();
            if (!stack.empty()) {
                std::cerr << "Call Stack:" << endl;
                while (!stack.empty()) {
                    std::cerr << "<- " << stack.top() << endl;
                    stack.pop();
                }
            }
        } catch (const CompileException &e) {
            std::cerr << e.className() << " "
                      << Singleton<ProgramFollower>::instance()->position()
                      << endl;
            std::cerr << e.what() << endl;
        } catch (const RuntimeException &e) {
            std::cerr << e.className() << " "
                      << Singleton<ProgramFollower>::instance()->position()
                      << endl;
            std::cerr << e.what() << endl;
            auto stack = Singleton<ProgramFollower>::instance()->getCallStack();
            if (!stack.empty()) {
                std::cerr << "Call Stack:" << endl;
                while (!stack.empty()) {
                    std::cerr << "<- " << stack.top() << endl;
                    stack.pop();
                }
            }
        } catch (const Exception &e) {
            std::cerr << e.className() << " "
                      << Singleton<ProgramFollower>::instance()->position()
                      << endl;
            std::cerr << e.what() << endl;
            auto stack = Singleton<ProgramFollower>::instance()->getCallStack();
            if (!stack.empty()) {
                std::cerr << "Call Stack:" << endl;
                while (!stack.empty()) {
                    std::cerr << "<- " << stack.top() << endl;
                    stack.pop();
                }
            }
        }
        Generator::clearInstance();
    }
} // namespace kernel
