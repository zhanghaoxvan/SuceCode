#pragma once
#include <functional>
#include <kernel/compiler/scanner.hpp>
#include <kernel/typedef.hpp>
#include <kernel/variant.hpp>

namespace kernel {
    void interact();
    Ref<Variant> doFile(const string &path, const array &args,
                        const string &name = "__main__");
    void compileTo(Scanner *, const string &dest);
    void tryRun(const std::function<void()> what);
} // namespace kernel
