#ifndef RULE_H
#define RULE_H

#include <memory>
#include <vector>

#include <config.h>

#include "ruleentity.h"

class Rule
{
  string rule_name;
  std::vector<RuleEntityPtr> produce;
public:
  Rule(const string& rule_name = STR(""));
  ~Rule();

  bool operator<(const Rule& right) const;

  const string& name() const;
  void addEntity(RuleEntityPtr entity);

  void print() const;

  friend struct CompareRuleEntityRevers;
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
