#include "rule.h"
#include "ruleentity.h"
#include "ruleentityextra.h"
#include "ruleentitynonterminal.h"
#include "ruleentityterminal.h"
#include "plsqltypes.h"

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

bool Rule::convolute(std::vector<SyntaxTreePtr>& stack, int& elements_proceed, LexemPtr next_lexem)
{
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
              if (entity->hasSameName(s_c_e->get())) {
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
          if (entity->hasSameName(s_c->get())) {
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
  if (!next_symbols.empty()) {
      if (must_be) {
          if (next_symbols[0]->name() != next_lexem->name()) {
              return false;
            }
        } else {
          for (auto next_symbol: next_symbols) {
              if (next_symbol->name() == STR("TYPE") && isPlSqlType(next_lexem->name())) {
                  return false;
                }
              if (next_symbol->name() == next_lexem->name()) {
                  return false;
                }
            }
        }
    }
  SyntaxTreePtr conv(new SyntaxTree);
  conv->tree_name = RuleEntityPtr(new RuleEntityNonTerminal(rule_name));
  std::vector<SyntaxTreePtr>::iterator it_beg = stack.begin() + std::distance(s_c,s_e), it_cur;
  std::vector<SyntaxTreePtr>::iterator it_end = stack.end();

  it_cur = it_beg;
  if ((*it_cur)->tree_name->ruleType() == RuleEntityType::NON_TERMINAL && (*it_cur)->tree_name->name() == rule_name) {
      for (auto&& node: (*it_cur)->nodes) {
          conv->nodes.push_back(std::move(node));
        }
      it_cur++;
    }

  for (; it_cur!=it_end; it_cur++) {
      switch ((*it_cur)->tree_name->ruleType()) {
      case RuleEntityType::TERMINAL: {
            TreeElementLeaf* el = new TreeElementLeaf();
            el->leaf = (*it_cur)->tree_name;
            conv->nodes.push_back(TreeElementPtr(el));
            break;
          }
        case RuleEntityType::NON_TERMINAL: {
            TreeElementNode* el = new TreeElementNode();
            el->node = *it_cur;
            conv->nodes.push_back(TreeElementPtr(el));
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
  stack.push_back(conv);

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
  produce.push_back(entity);
}

void Rule::addNextSymbol(LexemPtr symbol, bool must_be)
{
  this->must_be = must_be;
  next_symbols.push_back(symbol);
}

void Rule::print() const
{
  std::wcout << rule_name << " >>> ";
  for(auto& entry: produce) {
      std::wcout << entry->name() << " ";
    }
  if (!next_symbols.empty()) {
      std::cout << " <<< ";
      if (!must_be) {
          std::cout << " ! ";
        }
      std::wcout << next_symbols[0]->name();
      if (!must_be) {
          for (unsigned i = 1; i < next_symbols.size(); i++) {
              std::wcout << " , " << next_symbols[i]->name();
            }
        }
    }
  std::cout << std::endl;
}
