#include "rule.h"

Rule::Rule(const string &name):
  rule_name(name)
{

}

Rule::~Rule()
{

}

bool Rule::operator<(const Rule& right) const
{
  return rule_name<right.rule_name;
}

const string& Rule::name() const
{
  return rule_name;
}

void Rule::addEntity(const RuleEntity& entity)
{
  produce.push_back(entity);
}
