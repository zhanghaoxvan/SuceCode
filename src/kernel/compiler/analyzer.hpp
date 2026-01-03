#pragma once
#include <kernel/compiler/ast.hpp>

namespace kernel {
    class Analyzer {
      public:
        static void analyze(Ref<ast::Node>);
    };
} // namespace kernel
