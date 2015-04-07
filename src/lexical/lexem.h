#ifndef LEXEM_H
#define LEXEM_H

#include <config.h>

#include "lexeminfo.h"

class Lexem
{
public:
  struct {
    int row, col;
  } pos;

  virtual LexemType type() const = 0;
  virtual const string &name() const = 0;

  virtual ~Lexem();
};

#endif // LEXEM_H
