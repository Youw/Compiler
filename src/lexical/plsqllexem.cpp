#include "plsqllexem.h"

#include "plsqldelimeters.h"

ErrorLexem::ErrorLexem(const string& error_info): info(error_info)
{

}

LexemType ErrorLexem::type() const
{
  return LexemType::ERROR;
}

const string& ErrorLexem::name() const
{
  return info;
}

DelimiterLexem::DelimiterLexem(const string& delimiter): delemiter(delimiter)
{

}

LexemType DelimiterLexem::type() const
{
  return LexemType::DELIMITER;
}

const string& DelimiterLexem::name() const
{
  return delemiter;
}

bool DelimiterLexem::canBeDelimiter(character c)
{
  return DELEMITER_FIRST_PARTS.find(c)!=DELEMITER_FIRST_PARTS.end();
}

bool DelimiterLexem::isDelimiter(const string& s)
{
  return DELEMITER_NAMES.find(s)!=DELEMITER_NAMES.end();
}

bool DelimiterLexem::canHasSecondChar(character c)
{
  return DELEMITER_FIRST_PARTS.at(c);
}

IdentifierLexem::IdentifierLexem(const string& identifier): identifier(identifier)
{

}

LexemType IdentifierLexem::type() const
{
  return LexemType::IDENTIFIER;
}

const string& IdentifierLexem::name() const
{
  return identifier;
}

CommentLexem::CommentLexem(const string& text): text(text)
{

}

LexemType CommentLexem::type() const
{
  return LexemType::COMMENT;
}

const string& CommentLexem::name() const
{
  return text;
}

LiteralLexem::LiteralLexem(const string& literal): literal(literal)
{

}

LexemType LiteralLexem::type() const
{
  return LexemType::LITERAL;
}

const string& LiteralLexem::name() const
{
  return literal;
}

LiteralStringLexem::LiteralStringLexem(const string& literal_string):
  LiteralLexem(literal_string)
{

}

const string& LiteralStringLexem::value()
{
  return name();
}

LiteralIntegerLexem::LiteralIntegerLexem(const long& literal_int):
  LiteralLexem(to_string(literal_int)),
  int_val(literal_int)
{

}

const long& LiteralIntegerLexem::value()
{
  return int_val;
}

LiteralFloatLexem::LiteralFloatLexem(const double& literal_float):
  LiteralLexem(to_string(literal_float)),
  float_val(literal_float)
{

}

const double& LiteralFloatLexem::value()
{
  return float_val;
}
