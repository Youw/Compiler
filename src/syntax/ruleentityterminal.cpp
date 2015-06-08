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
      ReservedWordLexem* test1 =dynamic_cast<ReservedWordLexem*>(right->getTerminal().get());
      KeyWordLexem* test2 =dynamic_cast<KeyWordLexem*>(right->getTerminal().get());
      bool test3 = isPlSqlType(right->getTerminal()->name());
      if (test1 || test2 || test3)
        return false;
      right->is_identifier = right->getTerminal()->type() == LexemType::IDENTIFIER;
      return right->is_identifier;
    }
  if (lexem->name() == STR("const")) {
      return right->getTerminal()->type() == LexemType::LITERAL;
    }
  if (lexem->name() == STR("TYPE")) {
      return isPlSqlType(right->getTerminal()->name());
    }
  return lexem->name() == right->getTerminal()->name();
}
