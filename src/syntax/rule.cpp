#include "rule.h"
#include "ruleentity.h"
#include "ruleentityextra.h"
#include "ruleentitynonterminal.h"
#include "ruleentitynull.h"
#include "ruleentityterminal.h"

#include <lexical/plsqllexem.h>

#include <assert.h>
#include <cctype>
#include <iostream>
#include <iterator>
#include <algorithm>

Rule::Rule(const string &name):
  rule_name(name)
{

}

Rule::~Rule()
{

}

bool Rule::convolute(std::vector<SyntaxTree>& stack, int& elements_proceed, LexemPtr next_lexem)
{
  (void)next_lexem;//for now
  elements_proceed = 0;
  auto s_c = stack.rbegin(), s_e = stack.rend();

  int i;
  for (i=produce.size()-1; i>=0 && s_c!=s_e; i--, s_c++) {
      RuleEntityPtr entity = produce[i];
      RuleEntityExtra* extra = dynamic_cast<RuleEntityExtra*>(entity.get());

      //   //process extra
      if (extra) {
          auto s_c_e = s_c;
          s_c--;
          int elements_proceed_extra = 0;
          bool extra_readed = false;
          for (i--; i>=0 && s_c_e!=s_e; i--, s_c_e++) {
              entity = produce[i];
              extra = dynamic_cast<RuleEntityExtra*>(entity.get());
              if (extra) {
                  extra_readed = true;
                  s_c_e--;
                  break;
                }
              if (entity->hasSameName(&*s_c_e)) {
                  elements_proceed_extra++;
                } else {
                  break;
                }
            }
          if (extra_readed || i==0) {
              elements_proceed += elements_proceed_extra;
              s_c = s_c_e;
            } else {
              for (i--; i>=0; i--) {
                  entity = produce[i];
                  extra = dynamic_cast<RuleEntityExtra*>(entity.get());
                  if (extra) {
                      break;
                    }
                }
            }
        } else {
      //  //
          if (entity->hasSameName(&*s_c)) {
              elements_proceed++;
            } else {
              return false;
            }
        }
      if (s_c==s_e) break;
    }
   if (i>=0) {
        RuleEntityPtr entity = produce[i];
        RuleEntityExtra* extra = dynamic_cast<RuleEntityExtra*>(entity.get());
        for (; extra && i>=0; i--) {
            if (extra) {
                for (i--; i>=0; i--) {
                    entity = produce[i];
                    extra = dynamic_cast<RuleEntityExtra*>(entity.get());
                    if (extra) {
                        break;
                      }
                  }
                if (i>0) {
                    i--;
                    entity = produce[i];
                    extra = dynamic_cast<RuleEntityExtra*>(entity.get());
                  }
              }
          }
     }
  if (i>=0) return false;
  SyntaxTree conv;
  conv.tree_name = RuleEntityPtr(new RuleEntityNonTerminal(rule_name));
  std::vector<SyntaxTree>::iterator it_beg = stack.begin() + std::distance(s_c,s_e), it_cur;
  std::vector<SyntaxTree>::iterator it_end = stack.end();
  for (it_cur = it_beg; it_cur!=it_end; it_cur++) {
      switch (it_cur->tree_name->ruleType()) {
      case RuleEntityType::TERMINAL: {
            TreeElementLeaf* el = new TreeElementLeaf();
            el->leaf = it_cur->tree_name;
            conv.nodes.push_back(TreeElementPtr(el));
            break;
          }
        case RuleEntityType::NON_TERMINAL: {
            if (it_cur->tree_name->name() == rule_name) {
                for (auto&& node: it_cur->nodes) {
                    conv.nodes.push_back(std::move(node));
                  }
              } else {
                TreeElementNode* el = new TreeElementNode();
                el->node = new SyntaxTree(std::move(*it_cur));
                conv.nodes.push_back(TreeElementPtr(el));
              }
            break;
          }
        case RuleEntityType::EXTRA: {
            bool this_is_very_bad_day = false;
            assert(this_is_very_bad_day);
            break;
          }
        default:
          bool this_is_the_worst_day = false;
          assert(this_is_the_worst_day);
      }
    }
  stack.erase(it_beg,it_end);
  stack.push_back(std::move(conv));

  return true;
}

bool Rule::operator<(const Rule& right) const
{
  return rule_name<right.rule_name;
}

const string& Rule::name() const
{
  return rule_name;
}

void Rule::addEntity(RuleEntityPtr entity)
{
  if (produce.size() == 1 && entity->ruleType() == RuleEntityType::TERMINAL) {
      if (produce[0]->ruleType() == RuleEntityType::TERMINAL) {
          string tmp = produce[0]->name();
          std::transform(tmp.begin(), tmp.end(),tmp.begin(), ::toupper);
          if (tmp == STR("NULL")) {
              DelimiterLexem* delim= dynamic_cast<DelimiterLexem*>(entity.get());
              if (delim && delim->name() == STR(";")) {
                  produce[0] == RuleEntityPtr(new RuleEntityNull());
                }
            }
        }
    }
  produce.push_back(entity);
}

void Rule::print() const
{
  std::wcout << rule_name << " >>> ";
  for(auto& entry: produce) {
      std::wcout << entry->name() << " ";
    }
  std::cout << std::endl;
}
