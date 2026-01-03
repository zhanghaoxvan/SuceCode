#pragma once
#include <kernel/variant.hpp>

namespace kernel {
    bool numeric(Variant::Type type);

    Variant::Type improve(Variant::Type, Variant::Type);
} // namespace kernel
