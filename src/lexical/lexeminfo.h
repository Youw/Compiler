#ifndef LEXEMINFO
#define LEXEMINFO

#include <config.h>
#include <map>

enum class LexemType {DELIMITER, IDENTIFIER, LITERAL, COMMENT, ERROR};


inline const string& LexemTypeName(LexemType type) {
  static const std::map<LexemType,string> LexemTypeNames = {
    {LexemType::DELIMITER,STR(DELIMITER)},
    {LexemType::IDENTIFIER,STR(IDENTIFIER)},
    {LexemType::LITERAL,STR(LITERAL)},
    {LexemType::COMMENT,STR(COMMENT)},
    {LexemType::ERROR,STR(ERROR)}
  };
  return LexemTypeNames.at(type);
}

#endif // LEXEMINFO

