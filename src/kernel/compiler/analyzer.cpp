#include "analyzer.hpp"

namespace kernel {
    using namespace ast;
    void Analyzer::analyze(Ref<Node> node) {
        switch (node->type()) {
        case Node::Type::NODE:
            break;
        }
    }
} // namespace kernel
