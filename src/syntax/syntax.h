#ifndef SYNTAX_H
#define SYNTAX_H

#include "rule.h"

#include <QTextStream>

#include <set>

class Syntax
{
  std::multiset<RulePtr> rules;
  std::multiset<RulePtr,CompareRuleEntityRevers> revers_rules;
public:
  Syntax();
  ~Syntax();

  void readRules(QTextStream &rule_input);

  void print() const;
private:
  static void insertEntity(RulePtr rule, const string& word);
};

#endif // SYNTAX_H
