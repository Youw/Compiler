#ifndef SYNTAX_H
#define SYNTAX_H

#include "rule.h"
#include "syntaxtree.h"

#include <lexical/lexem.h>
#include <lexical/lexicalanalyzer.h>

#include <set>
#include <vector>

class QTextStream;

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
  SyntaxTreePtr syntax_tree;

  std::vector<SyntaxTreePtr> stack;
public:
  Syntax();
  ~Syntax();

  void readRules(QTextStream &rule_input);

  SyntaxTreePtr& buildTree(LexicalAnalyzer &lex);
  SyntaxTreePtr &getCurTree();

  void print() const;
private:
  static void insertEntity(RulePtr rule, const string& word);
  static void insertNextTerm(RulePtr rule, const string& word, bool must_be);

  void dumpStack() const;
};

#endif // SYNTAX_H
