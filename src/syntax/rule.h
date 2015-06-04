#ifndef RULE_H
#define RULE_H

#include <memory>
#include <vector>

#include <config.h>
#include <lexical/lexem.h>

#include "syntaxtree.h"
#include "ruleentity.h"

class Rule
{
  string rule_name;
  std::vector<RuleEntityPtr> produce;
  std::vector<LexemPtr> next_symbols;
  bool must_be;
public:
  Rule(const string& rule_name = STR(""));
  ~Rule();

  bool convolute(std::vector<SyntaxTreePtr> &stack, int& elements_proceed, LexemPtr next_lexem);

  const string& name() const;
  void addEntity(RuleEntityPtr entity);
  void addNextSymbol(LexemPtr symbol, bool must_be);

  bool operator<(const Rule& right) const;
  friend struct CompareRuleEntityRevers;

  void print() const;
};

using RulePtr=std::shared_ptr<Rule>;

struct CompareRuleEntityRevers
{
  bool operator() (const RulePtr& left, const RulePtr& right)
  {
    auto l = left->produce.rbegin();
    auto r = right->produce.rbegin();
    if (l == left->produce.rend() || r == right->produce.rend()) {
        return false;
      }
    while (!(l == left->produce.rend() || r == right->produce.rend())) {
        bool eq = (*l)->name() == (*r)->name();
        if (eq) {
            l++;
            r++;
          } else {
            return (*l)->name() < (*r)->name();
          }
      }
    return l != left->produce.rend();
  }
};

#endif // RULE_H
