#ifndef RULEENTITYNONTERMINAL_H
#define RULEENTITYNONTERMINAL_H

#include "ruleentity.h"

class RuleEntityNonTerminal: public RuleEntity
{
  string entity_name;
public:
  RuleEntityNonTerminal(const string& name);
  ~RuleEntityNonTerminal();

  const string& name() const override;
  RuleEntityType ruleType() const override;
};

#endif // RULEENTITYNONTERMINAL_H
