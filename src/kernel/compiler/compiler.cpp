#include "compiler.hpp"
#include <kernel/function.hpp>

namespace kernel {
    Borrower<CompileOption> Compiler::globalOption;

    Ref<State> Compiler::compile() {
        _M_scanner->reset();
        _M_progressBar.update(); // 初始化进度条

        // 词法分析阶段
        token::tokens tokens = tokenize();
        _M_progressBar.nextStage(); // 词法分析完成

        // 预处理阶段
        Preprocessor preprocessor(tokens);
        token::tokens &processed_tokens = preprocessor.getTokens();
        _M_progressBar.nextStage(); // 预处理完成

        // 语法分析阶段
        Ref<ast::Node> ast = parse(processed_tokens);
        _M_progressBar.nextStage(); // 语法分析完成

        // 代码生成阶段
        Ref<State> state = gen(ast);
        _M_progressBar.nextStage(); // 代码生成完成

        // 优化阶段
        optimize(*state, _M_option);
        _M_progressBar.nextStage(); // 优化完成

        state->chunk(_M_scanner->chunk());
        _M_progressBar.finish(); // 完成进度条

        return state;
    }
    Function Compiler::load() { return Function{Method{compile()}}; }
    Function Compiler::load(Ref<ast::Node> tree, const CompileOption &option) {
        Ref<State> state = gen(tree);
        optimize(*state, option);
        return Function{Method{state}};
    }
    void Compiler::optimize(State &state, const CompileOption &option) {
        switch (option.optimizeLevel) {
        case CompileOption::OptimizeLevel::NONE:
            break;
        case CompileOption::OptimizeLevel::SIMPLE:
            Optimizer::optimizeSimply(state);
            break;
        case CompileOption::OptimizeLevel::COMPLEX:
            break;
        }
    }
    token::tokens Compiler::tokenize() {
        Lexer lexer(_M_scanner.data());
        return lexer.getTokens();
    }
    token::tokens Compiler::preprocess(const token::tokens &tokens,
                                       const CompileOption &option) {
        Preprocessor preprocessor(tokens);
        if (option.preprocessRule)
            preprocessor.setRule(*option.preprocessRule);
        return preprocessor.getTokens();
    }
    Ref<ast::Node> Compiler::parse(token::tokens &tokens) {
        Parser parser;
        return parser.parse(tokens);
    }
    Ref<ast::Node> Compiler::parse() {
        Preprocessor preprocessor(tokenize());
        token::tokens &tokens = preprocessor.getTokens();
        return parse(tokens);
    }
    Ref<State> Compiler::gen(Ref<ast::Node> ast) {
        Ref<Generator> generator = new Generator;
        return generator->gen(ast);
    }
} // namespace kernel
