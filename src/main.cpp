#include <lexical/lexicalanalyzer.h>
#include <lexical/plsqllexem.h>
#include <lexical/plsqlliteral.h>
#include <syntax/syntax.h>
#include <context/context.h>

#include <iostream>
#include <fstream>
#include <QFile>
#include <QTextStream>

#include "config.h"

void prinLexem(const LexemPtr& lexem, ostream& output)
{
  output << "Read lexem: " << LexemTypeName(lexem->type());
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

int main(int argc, char* argv[])
{
  if (argc != 2) {
      std::cout << "No input files!" << std::endl;
      return 1;
    }
  ifstream f(argv[1]);
  if (!f) {
      std::cout << "Cannot open file: " << argv[1] << std::endl;
      return 1;
    }

  QFile file_syntax(":/plsql.rules");
  if (!file_syntax.open(QIODevice::ReadOnly | QIODevice::Text))
  {
      return 1;
  }
  QTextStream syntax_input(&file_syntax);
  Syntax syntax;

  syntax.readRules(syntax_input);
  syntax.print();

//  string input_name = STR("T:/1.txt");
  LexicalAnalyzer lexems(f);
  try {
    syntax.buildTree(lexems);
  } catch (const SyntaxException& e) {
    cout << e.what() << std::endl;
    return 1;
  } catch (const LexicalException& e) {
    cout << e.what() << std::endl;
    return 1;
  }
  syntax.getCurTree()->print();
  std::cout << std::endl;

  Context context;

  context.parseBlocks(syntax.getCurTree());

  std::cout << "Found Blocks in syntax tree:" << std::endl;
  for (auto block: context.currentBlocks()) {
      std::cout << "\t<<< Block id = " << block->id() << " >>>" << std::endl;
      block->block->print("\t");
      std::cout << std::endl << "\t<<< End Block >>>" << std::endl;
    }

//  ostream& output = cout;

//  try {
//    while(true) {
//        LexemPtr lexem = lexems.nextLexem();
//        prinLexem(lexem,output);
//      }
//  } catch (const LexicalExceptionEndOfStream&) {
//    output << input_name
//           << ':' << lexems.currentReadPos().row
//           << ':' << lexems.currentReadPos().column << ": "
//           <<"End of file reached." << std::endl;
//  } catch (const LexicalException& e) {
//    output << input_name
//           << ':' << lexems.currentReadPos().row
//           << ':' << lexems.currentReadPos().column << ": "
//           << "Lexical error: " << e.what() << std::endl;
//  }

  return 0;
}
