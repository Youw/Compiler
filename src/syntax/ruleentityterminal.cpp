#include "ruleentityterminal.h"

RuleEntityTerminal::RuleEntityTerminal(LexemPtr lexem):
  lexem(lexem)
{

}

RuleEntityTerminal::~RuleEntityTerminal()
{

}

RuleEntityType RuleEntityTerminal::ruleType() const
{
  return RuleEntityType::TERMINAL;
}

const string& RuleEntityTerminal::name() const
{
  return lexem->name();
}
