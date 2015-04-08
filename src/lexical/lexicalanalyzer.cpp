#include "lexicalanalyzer.h"
#include "plsqldelimeters.h"
#include "plsqlidentifiers.h"
#include "plsqllexem.h"

#include <cctype>

LexicalAnalyzer::LexicalAnalyzer(istream& input):
  input(input)
{

}

character LexicalAnalyzer::getChar()
{
  int c;
  do{
      c = getRawChar();
      skipEOL(c);
    }while(isspace(c));
  return c;
}

int LexicalAnalyzer::getRawChar()
{
  int c = input.get();
  current_read_pos.column++;
  if(!input) throw LexicalExceptionEndOfStream();
  return c;
}

int LexicalAnalyzer::skipEOL(int&c)
{
  if (c==STR('\r')) {
reading_carriage_return:
      current_read_pos.row++;
      current_read_pos.column=0;
      int c_new = input.peek();
      if(c_new==STR('\r')) {
          c = getRawChar();
          goto reading_carriage_return;
        }
      if(c_new==STR('\n')) {
          c = getRawChar();
        }
    }
  if (c==STR('\n')) {
reading_newline:
      current_read_pos.row++;
      current_read_pos.column=0;
      int c_new = input.peek();
      if(c_new==STR('\n')) {
          c = getRawChar();
          goto reading_newline;
        }
      else if (c_new==STR('\r')) {
          c = getRawChar();
          goto reading_carriage_return;
        }
    }
  return c;
}

LexemPtr LexicalAnalyzer::nextLexem()
{
  character current_raw_character = getChar();
  character current_char = toupper(current_raw_character);
  begin_read_pos=current_read_pos;
  LexemPtr delimiter = readDelimiter(current_char);
  if(delimiter) {
      LexemPtr lexem;
      try {
        switch(DELEMITER_NAMES.at(delimiter->name())) {
          case Delimiter::SINGLELINE_COMMENT_INDICATOR:{
              lexem = readSingleLineComment();
              break;
            }
          case Delimiter::MULTILINE_COMMENT_DELIMITER_BEGIN:{
              lexem = readMultiLineComment();
              break;
            }
          case Delimiter::MULTILINE_COMMENT_DELIMITER_END:{
              throw LexicalException(string(STR("Uncomplete comment delimiter: "))+delimiter->name());
              break;
            }
          case Delimiter::QUOTED_IDENTIFIER_DELIMITER:{
              lexem = readQuotedIdentifier();
              break;
            }
          case Delimiter::CHARACTER_STRING_DELIMITER:{
              lexem = readStrLiteral();
              break;
            }
          case Delimiter::COMPONENT_INDICATOR:{
              if(isdigit(input.peek())) {
                  lexem = readNumber(current_char);
                  break;
                }
            }
          default:
            lexem = delimiter;
          }
      } catch (const LexicalException& e) {
        current_read_pos = begin_read_pos;
        throw e;
      }
      lexem->pos = {begin_read_pos.row,begin_read_pos.column};
      lexems.push_back(lexem);
      return lexems.back();
    }
  if(isdigit(current_char)) {
      LexemPtr lexem = readNumber(current_char);
      lexem->pos = {begin_read_pos.row,begin_read_pos.column};
      lexems.push_back(lexem);
      return lexems.back();
    }

  if(isalpha(current_char)) {
      LexemPtr lexem = readIdentifier(current_char);
      lexem->pos = {begin_read_pos.row,begin_read_pos.column};
      lexems.push_back(lexem);
      return lexems.back();
    }

  throw LexicalException(string(STR("Illegal character: "))+current_raw_character);
  return lexems.back();
}

LexemPtr LexicalAnalyzer::readDelimiter(character current_char)
{
  if(DelimiterLexem::canBeDelimiter(current_char)) {
      string temp(&current_char,1);
      if(DelimiterLexem::canHasSecondChar(current_char)) {
          character c2 = input.peek();
          if(DelimiterLexem::isDelimiter(temp+c2)) {
              temp=temp+c2;
              getChar();
            }
        }
      if(DelimiterLexem::isDelimiter(temp)) {
          return LexemPtr(new DelimiterLexem(temp));
        }
    }
  return LexemPtr();
}

LexemPtr LexicalAnalyzer::readSingleLineComment()
{
  string buffer(STR("--"));
  try {
    while(true) {
      int c = getRawChar();
      if((c==STR('\r')) || (c==STR('\n'))) {
          skipEOL(c);
          break;
        }
      buffer+=(character)c;
      }
  } catch(const LexicalExceptionEndOfStream&) {

  }
  return LexemPtr(new CommentLexem(buffer));;
}

LexemPtr LexicalAnalyzer::readMultiLineComment()
{
  string buffer(STR("/*"));
  try {
    while(true) {
      int c = getRawChar();
      skipEOL(c);
      if((c==STR('*') && (input.peek()==STR('/')))) {
          getRawChar();
          buffer+=STR("*/");
          break;
        }
      buffer+=(character)c;
      }
  } catch(const LexicalExceptionEndOfStream&) {
    throw LexicalException(STR("ORA-01742: comment not terminated properly"));
  }
  return LexemPtr(new CommentLexem(buffer));;
}

LexemPtr LexicalAnalyzer::readQuotedIdentifier()
{
  string buffer(STR("\""));
  try {
    while(true) {
      int c = getRawChar();
      if(c==STR('\r') || c==STR('\n')) {
          throw LexicalException(STR("PLS-00112: end-of-line in quoted identifier"));
        }
      if(c==STR('\"')) {
          int c_new = input.peek();
          if(c_new==STR('\"')) {
              getRawChar();
            } else {
              buffer+=(character)c;
              break;
            }
        }
      buffer+=(character)c;
      if((buffer.size()-1)>MAX_IDENTIFIER_SIZE) {
          throw LexicalException(string(STR("PLS-00114: identifier "))+buffer+STR("\" too long"));
        }
      }
  } catch(const LexicalExceptionEndOfStream&) {
    throw LexicalException(STR("ORA-01740: missing double quote in identifier"));
  }
  return LexemPtr(new CommentLexem(buffer));
}

LexemPtr LexicalAnalyzer::readNumber(character current_char)
{
  if(!(isdigit(current_char) || (current_char==STR('.')))) {
      return LexemPtr();
    }
  string buffer(&current_char,1);
  bool has_dot = false;
  if(current_char==STR('.')) {
      has_dot=true;
    }
  try {
    int c;
    do {
        current_char = getChar();
        if(current_char==STR('.')) {
            has_dot=true;
          } else {
            if((buffer.size()-int(has_dot))>38) {
                if(buffer[0]!=STR('.')) {
                    buffer.erase(0,1);
                  } else {
                    buffer.erase(buffer.end()-1);
                  }
              }
          }
        buffer+=(character)current_char;
        c = input.peek();
      } while(isdigit(c) || ((c==STR('.'))&&(!has_dot)));
  } catch(const LexicalExceptionEndOfStream&) {

  }
  if(has_dot) {
      return LexemPtr(new LiteralFloatLexem(std::stod(buffer),buffer));
    } else {
      return LexemPtr(new LiteralIntegerLexem(std::stol(buffer),buffer));
    }
}

LexemPtr LexicalAnalyzer::readStrLiteral()
{
  string buffer(STR("'"));
  try {
    int c;
    auto local_read_raw_char = [&] () {
        c = getRawChar();
        buffer+=(character)c;
        return c;
      };
    while(true) {
         c = local_read_raw_char();

        if (c==STR('\r')) {
      reading_carriage_return:
            current_read_pos.row++;
            current_read_pos.column=0;
            int c_new = input.peek();
            if(c_new==STR('\r')) {
                c = local_read_raw_char();
                goto reading_carriage_return;
              }
            if(c_new==STR('\n')) {
                c = local_read_raw_char();
              }
          }
        if (c==STR('\n')) {
      reading_newline:
            current_read_pos.row++;
            current_read_pos.column=0;
            int c_new = input.peek();
            if(c_new==STR('\n')) {
                c = local_read_raw_char();
                goto reading_newline;
              }
            else if (c_new==STR('\r')) {
                c = local_read_raw_char();
                goto reading_carriage_return;
              }
          }

        if(c==STR('\'')) {
            int c_new = input.peek();
            if(c_new==STR('\'')) {
                getRawChar();
              } else {
                break;
              }
          }
      }
  } catch(const LexicalExceptionEndOfStream&) {
    throw LexicalException(STR("ORA-01756: quoted string not properly terminated"));
  }
  return LexemPtr(new LiteralStringLexem(buffer));
}

LexemPtr LexicalAnalyzer::readIdentifier(character current_char)
{
  string buffer;
  if(!isOrdinaryIdentifierCharacter(current_char)) {
      return LexemPtr();
    } else {
      buffer+=(character)toupper(current_char);
    }
  try {
    while(isOrdinaryIdentifierCharacter(input.peek())) {
        current_char = getChar();
        buffer+=(character)toupper(current_char);
      };
  } catch (const LexicalException&) {

  }

  if(isBoolean(buffer)) {
      return LexemPtr(new LiteralBooleanLexem(toBoolean(buffer),buffer));
    }
  if(isKeyword(buffer)) {
      return LexemPtr(new KeyWordLexem(buffer));
    }
  if(isReservedWord(buffer)) {
      return LexemPtr(new ReservedWordLexem(buffer));
    }
  return LexemPtr(new IdentifierLexem(buffer));
}

LexemPtr LexicalAnalyzer::currentLexem()
{
  if(!lexems.size())
    return LexemPtr(new ErrorLexem(L"No lexem readed yet."));
  else
    return lexems.back();
}

const decltype(LexicalAnalyzer::current_read_pos)& LexicalAnalyzer::currentReadPos()
{
  return current_read_pos;
}
