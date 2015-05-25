#ifndef RULEENTITYTERMINAL_H
#define RULEENTITYTERMINAL_H

#include "ruleentity.h"

#include <lexical/lexem.h>

class RuleEntityTerminal : public RuleEntity
{
  LexemPtr lexem;
public:

  RuleEntityTerminal(LexemPtr lexem);
  ~RuleEntityTerminal();

  RuleEntityType ruleType() const override;
  const string &name() const override;
};

#endif // RULEENTITYTERMINAL_H
