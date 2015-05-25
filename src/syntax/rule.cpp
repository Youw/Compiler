#include "rule.h"

#include <iostream>

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

void Rule::addEntity(RuleEntityPtr entity)
{
  produce.push_back(entity);
}

void Rule::print() const
{
  std::wcout << rule_name << " >>> ";
  for(auto& entry: produce) {
      std::wcout << entry->name() << " ";
    }
  std::cout << std::endl;
}
