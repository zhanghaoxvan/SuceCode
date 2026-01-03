#pragma once
#include <kernel/typedef.hpp>
#include <kernel/variant.hpp>

namespace kernel {
    map<string, Ref<Variant>> &getGlobals();
    void setGlobal(const string &, Ref<Variant>);
    Ref<Variant> getGlobal(const string &);
    Borrower<std::pair<const string, Ref<Variant>>>
    getGlobalPair(const string &);
    bool hasGlobal(const string &);
} // namespace kernel
