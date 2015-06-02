#include "ruleentitynonterminal.h"

#include "syntaxtree.h"

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

bool RuleEntityNonTerminal::hasSameName(SyntaxTree* node)
{
  return entity_name == node->tree_name->name();
}
