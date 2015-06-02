#include "ruleentitynull.h"

RuleEntityNull::RuleEntityNull()
{

}

RuleEntityNull::~RuleEntityNull()
{

}


RuleEntityType RuleEntityNull::ruleType() const
{
  return RuleEntityType::T_NULL;
}

const string& RuleEntityNull::name() const
{
  static const string n = STR("NULL");
  return n;
}

bool RuleEntityNull::hasSameName(SyntaxTree* node)
{
  (void)node;
  return false;
}
