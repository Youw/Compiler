#include <lexical/lexicalanalyzer.h>
#include <lexical/plsqllexem.h>
#include <lexical/plsqlliteral.h>
#include <syntax/syntax.h>

#include <iostream>
#include <fstream>
#include <QFile>

#include "config.h"

void prinLexem(const LexemPtr& lexem, ostream& output)
{
  output << "Readed lexem: " << LexemTypeName(lexem->type());
  if(lexem->type()==LexemType::LITERAL) {
      output << ' ' << to_string(std::dynamic_pointer_cast<LiteralLexem>(lexem)->literalType());
    }
  if(dynamic_cast<KeyWordLexem*>(&*lexem)) {
      output << " KEYWORD";
    }
  if(dynamic_cast<ReservedWordLexem*>(&*lexem)) {
      output << " RESERVED_WORD";
    }
  output << " Name: < "
             << lexem->name() << " > At:" << lexem->pos.row << ':' << lexem->pos.col << std::endl;
}

int main()
{
  QFile file_syntax(":/plsql.rules"); // this is a name of a file text1.txt sent from main method
  if (!file_syntax.open(QIODevice::ReadOnly | QIODevice::Text))
  {
      return 1;
  }
  QTextStream syntax_input(&file_syntax);
  Syntax syntax;

  syntax.readRules(syntax_input);

  string input_name = STR("T:/1.txt");
  std::wifstream f("T:/1.txt");
  LexicalAnalyzer lexems(f);
  ostream& output = cout;

  try {
    while(true) {
        LexemPtr lexem = lexems.nextLexem();
        prinLexem(lexem,output);
      }
  } catch (const LexicalExceptionEndOfStream&) {
    output << input_name
           << ':' << lexems.currentReadPos().row
           << ':' << lexems.currentReadPos().column << ": "
           <<"End of file reached." << std::endl;
  } catch (const LexicalException& e) {
    output << input_name
           << ':' << lexems.currentReadPos().row
           << ':' << lexems.currentReadPos().column << ": "
           << "Lexical error: " << e.what() << std::endl;
  }

  return 0;
}
