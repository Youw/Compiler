#include <lexical/lexicalanalyzer.h>
#include <iostream>

#include "config.h"

void prinLexem(const Lexem& lexem, ostream& output)
{
  output << "Readed lexem: " << LexemTypeName(lexem.type()) << " Name: < "
             << lexem.name() << " > At" << std::endl;
}

int main()
{
  string input_name = STR(Keyboard);
  LexicalAnalyzer lexems(std::wcin);
  ostream& output = std::wcout;

  try {
    while(true) {
        LexemPtr lexem = lexems.nextLexem();
        prinLexem(*lexem,output);
      }
  } catch (LexicalExceptionEndOfStream&) {
    output << input_name
           << ':' << lexems.currentReadPos().row
           << ':' << lexems.currentReadPos().column << ": "
           <<"End of file reached." << std::endl;
  } catch (LexicalException& e) {
    output << input_name
           << ':' << lexems.currentReadPos().row
           << ':' << lexems.currentReadPos().column << ": "
           << "Lexical error: " << e.what() << std::endl;
  }

  return 0;
}
