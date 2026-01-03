#pragma once
#include <kernel/compiler/generator.hpp>
#include <kernel/compiler/lexer.hpp>
#include <kernel/compiler/optimizer.hpp>
#include <kernel/compiler/parser.hpp>
#include <kernel/compiler/process_bar.hpp>

#include <kernel/compiler/preprocessor.hpp>
#include <kernel/function.hpp>

namespace kernel {
    struct CompileOption {
        enum OptimizeLevel {
            NONE,
            SIMPLE,
            COMPLEX // to be implemented
        };

        OptimizeLevel optimizeLevel = OptimizeLevel::NONE;
        Owner<PreprocessRule> preprocessRule;

        CompileOption() = default;
        CompileOption(const CompileOption &option)
            : optimizeLevel(option.optimizeLevel),
              preprocessRule(new PreprocessRule{*option.preprocessRule}) {}
    };

    class Compiler {
      private:
        CompileOption _M_option;
        Owner<Scanner> _M_scanner;
        ProgressBar _M_progressBar;


      public:
        static Borrower<CompileOption> globalOption;

        Compiler(Scanner *scanner)
            : _M_scanner(scanner),
              _M_option(globalOption ? *globalOption : CompileOption{}),
              _M_progressBar() {}

        Compiler(Scanner *scanner, CompileOption &&option)
            : _M_scanner(scanner), _M_option(std::move(option)),
              _M_progressBar() {}


        Ref<State> compile();
        Function load();
        token::tokens tokenize();
        Ref<ast::Node> parse();

        static void optimize(State &, const CompileOption &);
        static Ref<ast::Node> parse(token::tokens &);
        static Function load(Ref<ast::Node>, const CompileOption &option);
        static token::tokens preprocess(const token::tokens &,
                                        const CompileOption &);
        static Ref<State> gen(Ref<ast::Node>);

        void enableProgressBar(bool enable) {
            if (enable) {
                _M_progressBar.update(); // 显示初始进度条
            }
        }
    };
}
