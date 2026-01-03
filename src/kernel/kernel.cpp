#define BUILD_DLL
#include "kernel.h"
#include <cstring>
#include <kernel/compiler/compiler.hpp>
#include <kernel/exception.hpp>
#include <kernel/sout.hpp>
#include <kernel/runtime/follower.hpp>
#include <kernel/version>

using namespace kernel;

#define CHECK_kernel                                                              \
    if (kernel == nullptr)                                                        \
    NO_KERNEL_ERROR
#define NO_KERNEL_ERROR                                                           \
    return new KernelError { .msg = to_c_str("") }

struct Kernel {
    Environment env;
};

char *to_c_str(const string &str) {
    char *res = new char[str.length() + 1];
    std::strcpy(res, str.c_str());
    return res;
}

const char *kernelVersion() { return kernel_VERSION; }

Kernel *kernelCreate() { return new Kernel; }


void kernelDestroy(Kernel *kernel) { delete kernel; }

KernelError *kernelDoString(Kernel *kernel, const char *chunk) {
    CHECK_kernel;
    try {
        Compiler compiler{new StringScanner{chunk}};
        kernel->env =
            std::move(compiler.load().getMethod().call(kernel->env, {}).second);
    } catch (const Exception &e) {
        return new KernelError{.msg = to_c_str(e.what()), .type = to_c_str(e.className())};
    }
    return nullptr;
}

KernelError *kernelDoStringWithName(Kernel *kernel, const char *chunk,
                              const char *chunk_name) {
    CHECK_kernel;
    try {
        Compiler compiler{new StringScanner{chunk, chunk_name}};
        kernel->env =
            std::move(compiler.load().getMethod().call(kernel->env, {}).second);
    } catch (const Exception &e) {
        return new KernelError{.msg = to_c_str(e.what()), .type = to_c_str(e.className())};
    }
    return nullptr;
}

KernelError *kernelDoFile(Kernel *kernel, const char *filename) {
    CHECK_kernel;
    if (Sout::isSout(filename)) {
        kernel->env = std::move(
            Sout::load(filename).getMethod().call(kernel->env, {}).second);
        return nullptr;
    }
    try {
        Compiler compiler{new FileScanner{filename}};
        kernel->env =
            std::move(compiler.load().getMethod().call(kernel->env, {}).second);
    } catch (const Exception &e) {
        return new KernelError{.msg = to_c_str(e.what()), .type = to_c_str(e.className())};
    }
    return nullptr;
}
