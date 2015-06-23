#ifndef RULEENTITYTERMINAL_H
#define RULEENTITYTERMINAL_H

#include "ruleentity.h"

#include <lexical/lexem.h>

class VarInfo;
class RuleEntityVariable;

class RuleEntityTerminal : public RuleEntity
{
  LexemPtr lexem;
public:
  bool isVariable();
  RuleEntityVariable* getVariable();


  RuleEntityTerminal(LexemPtr lexem);
  ~RuleEntityTerminal();

  RuleEntityType ruleType() const override;
  const string &name() const override;
  LexemPtr getTerminal();

  bool hasSameName(SyntaxTree* node) override;
};

class RuleEntityVariable : public RuleEntityTerminal
{
public:
  RuleEntityVariable();
  RuleEntityVariable(const RuleEntityTerminal& other);
  VarInfo* var_info = 0;
};


#endif // RULEENTITYTERMINAL_H
