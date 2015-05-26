#include "syntax.h"

#include <assert.h>
#include <cctype>
#include <iostream>

#include <lexical/plsqldelimeters.h>
#include <lexical/plsqlidentifiers.h>
#include <lexical/plsqllexem.h>
#include <lexical/plsqlliteral.h>


#include <ruleentityextra.h>
#include <ruleentitynonterminal.h>
#include <ruleentityterminal.h>

Syntax::Syntax()
{

}

Syntax::~Syntax()
{

}

void Syntax::readRules(QTextStream& rule_input)
{
  QString tmp;
#define readWord rule_input>>tmp;word=to_string(tmp)
  string word, prew_word;
  RulePtr rule;
  readWord;
  prew_word = word;
  readWord;
  while (!word.empty()) {
      if (word == STR(">>>")) {
          if (rule) {
              rules.insert(rule);
              revers_rules.insert(rule);
            }
          rule.reset(new Rule(prew_word));

          readWord;
        } else {
          insertEntity(rule,prew_word);
        }

      prew_word = word;
      readWord;
    }
  insertEntity(rule,prew_word);
  rules.insert(rule);
  revers_rules.insert(rule);
}

void Syntax::insertEntity(RulePtr rule, const string& word)
{
  if (isalpha(word[0])) {
      if (isupper(word[0])) {
          rule->addEntity(RuleEntityPtr(new RuleEntityNonTerminal(word)));
        } else {
          LexemPtr lexem;
          if (word == STR("idt")) {
              lexem.reset(new IdentifierLexem(word));
            } else if (word == STR("const")) {
              lexem.reset(new LiteralLexem(word));
            } else {
              string tmp_word = word;
              for (unsigned i=0; i<tmp_word.size(); i++) {
                  tmp_word[i] = toupper(tmp_word[i]);
                }
              if (isKeyword(tmp_word)) {
                  lexem.reset(new KeyWordLexem(tmp_word));
                } else if (isReservedWord(tmp_word)) {
                  lexem.reset(new ReservedWordLexem(tmp_word));
                } else {
                  bool this_terminal_not_implementer_yet = false;
                  assert(this_terminal_not_implementer_yet);
                }
            }
          rule->addEntity(RuleEntityPtr(new RuleEntityTerminal(lexem)));
        }
    } else {
      if (word == STR("{") || word == STR("}")) {
          rule->addEntity(RuleEntityPtr(new RuleEntityExtra(word)));
        } else {
          LexemPtr lexem(new DelimiterLexem(word));
          rule->addEntity(RuleEntityPtr(new RuleEntityTerminal(lexem)));
        }
    }
}

void Syntax::print() const
{
  std::cout << "Rules in alphabetic:" << std::endl;
  for (auto& rule: rules) {
      std::cout << '\t';
      rule->print();
    }

  std::cout << std::endl << "Reversed rules:" << std::endl;
  for (auto& rule: revers_rules) {
      std::cout << '\t';
      rule->print();
    }
}

SyntaxTree& Syntax::buildTree(LexicalAnalyzer &lex)
{
  syntax_tree.nodes.clear();
  stack.clear();
  LexemPtr next_lexem;
  try {
    auto lexem = lex.nextLexem();
    SyntaxTree el;
    el.tree_name = RuleEntityPtr(new RuleEntityTerminal(lexem));
    stack.push_back(std::move(el));

    try {
      next_lexem = lex.nextLexem();
    } catch (const LexicalExceptionEndOfStream&) {
      next_lexem = LexemPtr();
    }
  } catch (const LexicalExceptionEndOfStream&) {
    throw SyntaxException(STR("Empty input"));
  }
  try {
    while(true) {
        bool convolution_proceed = true;
        while (convolution_proceed) {
            convolution_proceed = false;
            int last_proceed = 0;
            for (auto rule: revers_rules) {
                int current_proceed;
                convolution_proceed = rule->convolute(stack, current_proceed, next_lexem);
                if (convolution_proceed) {
                    last_proceed = 0;
                    current_proceed = 0;
                    break;
                  }
                if (current_proceed<last_proceed) break;
                last_proceed = current_proceed;
              }
          }

        if (!next_lexem) {
            throw LexicalExceptionEndOfStream();
          }
        auto lexem = next_lexem;
        SyntaxTree el;
        el.tree_name = RuleEntityPtr(new RuleEntityTerminal(lexem));
        stack.push_back(std::move(el));

        try {
          next_lexem = lex.nextLexem();
        } catch (const LexicalExceptionEndOfStream&) {
          next_lexem = LexemPtr();
        }
      }
  } catch (const LexicalExceptionEndOfStream&) {
    if (stack.size() == 1) {
        auto tree_top = std::dynamic_pointer_cast<RuleEntityNonTerminal>(stack[0].tree_name);
        if (!(tree_top && tree_top->name() == STR("S"))) {
            throw SyntaxException(STR("Top stack must be \"S\"!"));
          }
      } else {
        throw SyntaxException(STR("Top stack must be single \"S\"!"));
      }
  }

  return syntax_tree;
}

SyntaxTree& Syntax::getCurTree()
{
  return syntax_tree;
}
