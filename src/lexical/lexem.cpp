#include "lexem.h"

Lexem::~Lexem()
{

}

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
