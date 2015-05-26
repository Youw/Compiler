#ifndef RULEENTITY_H
#define RULEENTITY_H

#include <memory>

#include <config.h>

class SyntaxTree;

enum class RuleEntityType {
  TERMINAL,
  NON_TERMINAL,
  EXTRA,
  T_NULL
};

class RuleEntity
{
public:
  RuleEntity() {}
  virtual ~RuleEntity() {}

  virtual RuleEntityType ruleType() const { return RuleEntityType(-1); }
  virtual const string& name() const { static const string none; return none; }

  virtual bool hasSameName(SyntaxTree* node) { (void)node; return false; }
};

using RuleEntityPtr = std::shared_ptr<RuleEntity>;

#endif // RULEENTITY_H
