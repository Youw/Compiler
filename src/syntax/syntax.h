#ifndef SYNTAX_H
#define SYNTAX_H

#include "rule.h"
#include "syntaxtree.h"

#include <lexical/lexem.h>
#include <lexical/lexicalanalyzer.h>

#include <QTextStream>

#include <set>
#include <vector>

class SyntaxException
{
  string message;
public:
  SyntaxException(const string& s = STR("SyntaxException")):message(STR("Syntax error: ")+s) {

  }

  virtual const string& what() const {
    return message;
  }
};


class Syntax
{
  std::multiset<RulePtr> rules;
  std::multiset<RulePtr,CompareRuleEntityRevers> revers_rules;
  SyntaxTree syntax_tree;

  std::vector<SyntaxTree> stack;
public:
  Syntax();
  ~Syntax();

  void readRules(QTextStream &rule_input);

  SyntaxTree& buildTree(LexicalAnalyzer &lex);
  SyntaxTree& getCurTree();

  void print() const;
private:
  static void insertEntity(RulePtr rule, const string& word);
};

#endif // SYNTAX_H
