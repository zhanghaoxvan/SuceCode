#include "exception.hpp"
#include <kernel/runtime/evaluator.hpp>

namespace kernel {
    OperateNullException::OperateNullException(const VariantPacker &value,
                                               const string &operation)
        : _M_operation(operation),
          _M_value(value.hasName() ? "(\"" + value.name() + "\")" : "") {}
} // namespace kernel
