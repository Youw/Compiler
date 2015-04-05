#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <deque>
#include <memory>

#include "config.h"
#include "lexem.h"

using LexemPtr=std::shared_ptr<Lexem>;

class LexicalAnalyzer
{
  istream& input;
  std::deque<LexemPtr> lexems;
public:
  LexicalAnalyzer(istream& input);

  LexemPtr nextLexem();
  LexemPtr currentLexem();
};

#endif // LEXICALANALYZER_H
