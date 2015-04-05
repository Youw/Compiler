#ifndef LEXEM_H
#define LEXEM_H

#include <config.h>
#include "lexeminfo.h"

class Lexem
{
public:
  virtual LexemType type() const = 0;
  virtual const string &name() const = 0;

  virtual ~Lexem();
};

class ErrorLexem: Lexem
{
  string info;
public:
  ErrorLexem(const string& error_info);

  LexemType type() const override;
  const string &name() const override;
};

#endif // LEXEM_H
