#ifndef RULEENTITYEXTRA_H
#define RULEENTITYEXTRA_H

#include "ruleentity.h"

class RuleEntityExtra : public RuleEntity
{
  string entity_name;
public:
  RuleEntityExtra(const string& name);
  ~RuleEntityExtra();

  RuleEntityType ruleType() const override;
  const string& name() const override;
};

#endif // RULEENTITYEXTRA_H
