#pragma once
#include <kernel/typedef.hpp>
#include <kernel/variant.hpp>

namespace kernel {
    Ref<Variant> import(const string &module_name, const string &chunk_name);
} // namespace kernel
