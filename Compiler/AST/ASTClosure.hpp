//
//  ASTClosure.hpp
//  Emojicode
//
//  Created by Theo Weidmann on 16/08/2017.
//  Copyright © 2017 Theo Weidmann. All rights reserved.
//

#ifndef ASTClosure_hpp
#define ASTClosure_hpp

#include "ASTExpr.hpp"
#include "ASTBoxing.hpp"
#include "Scoping/CapturingSemanticScoper.hpp"
#include "MemoryFlowAnalysis/MFHeapAllocates.hpp"
#include <llvm/IR/Instructions.h>

namespace EmojicodeCompiler {

struct Capture {
    std::vector<VariableCapture> captures;
    /// The type of the captured type context. If this is TypeType::NoReturn, the type context is not captured.
    Type self = Type::noReturn();
    llvm::Type *type = nullptr;

    bool capturesSelf() const { return self.type() != TypeType::NoReturn; }
};

class ASTClosure : public ASTExpr, public MFHeapAutoAllocates {
public:
    ASTClosure(std::unique_ptr<Function> &&closure, const SourcePosition &p);

    Type analyse(FunctionAnalyser *analyser, const TypeExpectation &expectation) override;
    Value* generate(FunctionCodeGenerator *fg) const final;

    void toCode(PrettyStream &pretty) const override;
    void analyseMemoryFlow(MFFunctionAnalyser *analyser, MFFlowCategory type) override;

private:
    std::unique_ptr<Function> closure_;
    Capture capture_;

    llvm::Value* storeCapturedVariables(FunctionCodeGenerator *fg, const Capture &capture) const;

    void applyBoxingFromExpectation(FunctionAnalyser *analyser, const TypeExpectation &expectation);
};

class ASTCallableBox final : public ASTBoxing, public MFHeapAutoAllocates {
public:
    ASTCallableBox(std::shared_ptr<ASTExpr> expr, const SourcePosition &p, const Type &exprType,
                   std::unique_ptr<Function> thunk)
    : ASTBoxing(std::move(expr), p, exprType), thunk_(std::move(thunk)) {}

    void analyseMemoryFlow(MFFunctionAnalyser *analyser, MFFlowCategory type) override;

    llvm::Value* generate(FunctionCodeGenerator *fg) const override;
    void toCode(PrettyStream &pretty) const override {}
    
private:
    std::unique_ptr<Function> thunk_;
};

class ASTCallableThunkDestination : public ASTExpr {
public:
    ASTCallableThunkDestination(const SourcePosition &p, const Type &destinationType)
        : ASTExpr(p) { setExpressionType(destinationType); }

    Type analyse(FunctionAnalyser *, const TypeExpectation &) final { return expressionType(); }
    void toCode(PrettyStream &pretty) const override {}
    llvm::Value* generate(FunctionCodeGenerator *fg) const override;
    virtual void analyseMemoryFlow(MFFunctionAnalyser *analyser, MFFlowCategory type) override {}
};

}  // namespace EmojicodeCompiler

#endif /* ASTClosure_hpp */
