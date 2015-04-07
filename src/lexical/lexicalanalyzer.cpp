#include "lexicalanalyzer.h"
#include "plsqllexem.h"

LexicalAnalyzer::LexicalAnalyzer(istream& input):
  input(input)
{

}

LexemPtr LexicalAnalyzer::nextLexem()
{
  return LexemPtr(new ErrorLexem(L"test"));
}

LexemPtr LexicalAnalyzer::currentLexem()
{
  return LexemPtr(new ErrorLexem(L"test"));
}

const decltype(LexicalAnalyzer::current_read_pos)& LexicalAnalyzer::currentReadPos()
{
  return current_read_pos;
}
