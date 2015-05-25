#include "ruleentityextra.h"

RuleEntityExtra::RuleEntityExtra(const string &name):
  entity_name(name)
{

}

RuleEntityExtra::~RuleEntityExtra()
{

}

RuleEntityType RuleEntityExtra::ruleType() const
{
  return RuleEntityType::EXTRA;
}

const string& RuleEntityExtra::name() const
{
  return entity_name;
}

