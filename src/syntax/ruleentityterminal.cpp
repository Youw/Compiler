#include "ruleentityterminal.h"

#include "syntaxtree.h"
#include <plsqllexem.h>
#include "plsqltypes.h"

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

LexemPtr RuleEntityTerminal::getTerminal()
{
  return lexem;
}

bool RuleEntityTerminal::hasSameName(SyntaxTree* node)
{
  RuleEntityTerminal* right = dynamic_cast<RuleEntityTerminal*>(node->tree_name.get());
  if (!right) return false;
  if (lexem->name() == STR("idt")) {
      ReservedWordLexem* test =dynamic_cast<ReservedWordLexem*>(right->getTerminal().get());
      if (test || isPlSqlType(right->getTerminal()->name()))
        return false;
      return right->getTerminal()->type() == LexemType::IDENTIFIER;
    }
  if (lexem->name() == STR("const")) {
      return right->getTerminal()->type() == LexemType::LITERAL;
    }
  if (lexem->name() == STR("TYPE")) {
      return isPlSqlType(right->getTerminal()->name());
    }
  return lexem->name() == right->getTerminal()->name();
}
