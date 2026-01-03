#pragma once
#include <kernel/runtime/state.hpp>

namespace kernel {
    class Optimizer {
      private:
        /*
            In the future,
            there may be some auxiliary variables here that save the optimizer
           state.
        */
      public:
        static void optimizeSimply(State &);

        State optimize(const State &);
    };
} // namespace kernel
