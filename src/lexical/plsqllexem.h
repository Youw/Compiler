#ifndef PLSQLLEXEM_H
#define PLSQLLEXEM_H

#include <lexical/lexeminfo.h>
#include <lexical/lexem.h>

class ErrorLexem: public Lexem
{
  string info;
public:
  ErrorLexem(const string& error_info);

  LexemType type() const override;
  const string& name() const override;
};

class DelimiterLexem: public Lexem
{
  string delemiter;
public:
  DelimiterLexem(const string& delimiter);

  LexemType type() const override;
  const string& name() const override;

  static bool canBeDelimiter(character c);
  static bool canHasSecondChar(character c);
  static bool isDelimiter(const string& s);
};

class IdentifierLexem: public Lexem
{
  string identifier;
public:
  IdentifierLexem(const string& identifier);

  LexemType type() const override;
  const string& name() const override;
};

class CommentLexem: public Lexem
{
  string text;
public:
  CommentLexem(const string& text);

  LexemType type() const override;
  const string& name() const override;
};

class LiteralLexem: public Lexem
{
  string literal;
public:
  LiteralLexem(const string& literal);

  LexemType type() const override;
  const string& name() const override;
};

class LiteralStringLexem: public LiteralLexem
{
public:
  LiteralStringLexem(const string& literal_string);

  const string& value();
};

class LiteralIntegerLexem: public LiteralLexem
{
  long int_val;
public:
  LiteralIntegerLexem(const long& literal_int);

  const long& value();
};

class LiteralFloatLexem: public LiteralLexem
{
  double float_val;
public:
  LiteralFloatLexem(const double& literal_float);

  const double& value();
};

#endif // PLSQLLEXEM_H
