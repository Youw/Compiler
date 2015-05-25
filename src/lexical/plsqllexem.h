#ifndef PLSQLLEXEM_H
#define PLSQLLEXEM_H

#include <lexical/lexeminfo.h>
#include <lexical/lexem.h>

#include "plsqlliteral.h"

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

class KeyWordLexem: public IdentifierLexem
{
public:
  using IdentifierLexem::IdentifierLexem;
};

class ReservedWordLexem: public IdentifierLexem
{
public:
  using IdentifierLexem::IdentifierLexem;
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
  virtual Literal literalType() const;
};

class LiteralStringLexem: public LiteralLexem
{
public:
  LiteralStringLexem(const string& literal_string);

  const string& value();

  Literal literalType() const override;
};

class LiteralBooleanLexem: public LiteralLexem
{
  Boolean bool_val;
public:
  LiteralBooleanLexem(Boolean literal_bool, const string& name);

  Boolean value();

  Literal literalType() const override;
};

class LiteralIntegerLexem: public LiteralLexem
{
  long int_val;
public:
  LiteralIntegerLexem(const long& literal_int, const string& name);

  const long& value();

  Literal literalType() const override;
};

class LiteralFloatLexem: public LiteralLexem
{
  double float_val;
public:
  LiteralFloatLexem(const double& literal_float, const string& name);

  const double& value();

  Literal literalType() const override;
};

#endif // PLSQLLEXEM_H
