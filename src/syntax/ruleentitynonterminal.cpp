#include "ruleentitynonterminal.h"

RuleEntityNonTerminal::RuleEntityNonTerminal(const string& name):
  entity_name(name)
{

}

RuleEntityNonTerminal::~RuleEntityNonTerminal()
{

}

const string& RuleEntityNonTerminal::name() const
{
  return entity_name;
}

RuleEntityType RuleEntityNonTerminal::ruleType() const
{
  return RuleEntityType::NON_TERMINAL;
}
