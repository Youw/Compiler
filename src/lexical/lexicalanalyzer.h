#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <deque>
#include <memory>
#include <stdexcept>

#include "config.h"
#include "lexem.h"

class LexicalException: public std::logic_error
{

};

class LexicalExceptionEndOfStream: public LexicalException
{

};

using LexemPtr=std::shared_ptr<Lexem>;

class LexicalAnalyzer
{
  istream& input;
  std::deque<LexemPtr> lexems;

  struct {
    int row, column;
  } current_read_pos;
public:
  LexicalAnalyzer(istream& input);

  LexemPtr nextLexem();
  LexemPtr currentLexem();

  const decltype(current_read_pos)& currentReadPos();
};

#endif // LEXICALANALYZER_H
