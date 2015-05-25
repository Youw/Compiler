#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <deque>

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

class LexicalExceptionLexemIndexOutOfRange: public LexicalException
{
public:
  LexicalExceptionLexemIndexOutOfRange(unsigned index=unsigned(-1)):
    LexicalException(index==unsigned(-1)?STR("Lexem index out of range."):STR("Lexem index \"")+to_string(index)+STR("\" out of range."))
  {

  }
};

class LexicalAnalyzer
{
  istream& input;
  std::deque<LexemPtr> lexems;

  struct {
    int row, column;
  } current_read_pos = {1,0}, begin_read_pos;

  unsigned current_lexem_index = 0;
public:
  LexicalAnalyzer(istream& input);

  LexemPtr nextLexem();

  LexemPtr currentLexem();
  unsigned currentLexemIndex();
  void setCurrentLexemIndex(unsigned index);

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
