#ifndef RULEENTITYNULL_H
#define RULEENTITYNULL_H

#include <ruleentity.h>

class RuleEntityNull : public RuleEntity
{
public:
  RuleEntityNull();
  ~RuleEntityNull();

  RuleEntityType ruleType() const override;
  const string& name() const override;

  bool hasSameName(SyntaxTree* node) override;
};

#endif // RULEENTITYNULL_H
