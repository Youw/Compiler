#ifndef LEXEMINFO
#define LEXEMINFO

#include <config.h>
#include <functional>
#include <unordered_map>

enum class LexemType: int {DELIMITER, IDENTIFIER, LITERAL, COMMENT, ERROR};

namespace std {
  template <>
  struct hash<LexemType>
  {
    std::size_t operator()(LexemType in) const
    {
       return std::hash<int>()(int(in));
    }
  };
}

const string& LexemTypeName(LexemType type);

#endif // LEXEMINFO

