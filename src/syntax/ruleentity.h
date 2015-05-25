#ifndef RULEENTITY_H
#define RULEENTITY_H

#include <memory>

#include <config.h>

enum class RuleEntityType {
  TERMINAL,
  NON_TERMINAL,
  EXTRA
};

class RuleEntity
{
public:
  RuleEntity() {}
  virtual ~RuleEntity() {}

  virtual RuleEntityType ruleType() const { return RuleEntityType(-1); }
  virtual const string& name() const { static const string none; return none; }
};

using RuleEntityPtr = std::shared_ptr<RuleEntity>;

#endif // RULEENTITY_H
