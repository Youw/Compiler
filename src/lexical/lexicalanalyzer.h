#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <deque>
#include <memory>

#include "config.h"
#include "lexem.h"

class LexicalException
{
  string message;
public:
  LexicalException(const string& s = STR("LexicalException")):message(s) {

  }

  virtual const string& what() const {
    return message;
  }
};

class LexicalExceptionEndOfStream: public LexicalException
{
public:
  LexicalExceptionEndOfStream(): LexicalException(STR("Stream end reached."))
  {

  }
};

using LexemPtr=std::shared_ptr<Lexem>;

class LexicalAnalyzer
{
  istream& input;
  std::deque<LexemPtr> lexems;

  struct {
    int row, column;
  } current_read_pos = {1,0}, begin_read_pos;

public:
  LexicalAnalyzer(istream& input);

  LexemPtr nextLexem();
  LexemPtr currentLexem();

  const decltype(current_read_pos)& currentReadPos();

private:
  character getChar();
  int getRawChar();
  int skipEOL(int &c);
  LexemPtr readDelimiter(character current_char);
  LexemPtr readSingleLineComment();
  LexemPtr readMultiLineComment();
  LexemPtr readQuotedIdentifier();
  LexemPtr readNumber(character current_char);
  LexemPtr readStrLiteral();
  LexemPtr readIdentifier(character current_char);
};

#endif // LEXICALANALYZER_H
