#ifndef PLSQLLITERAL
#define PLSQLLITERAL

#include <config.h>

enum class Literal: int {
  INTEGER,
  FLOAT,
  STRING,
  BOOLEAN
};

namespace std {
  template <>
  struct hash<Literal>
  {
    std::size_t operator()(Literal in) const
    {
       return std::hash<int>()(int(in));
    }
  };
}

enum class Boolean: int {
  TRUE,
  FALSE,
  THE_NULL
};


namespace std {
  template <>
  struct hash<Boolean>
  {
    std::size_t operator()(Boolean in) const
    {
       return std::hash<int>()(int(in));
    }
  };
}


extern string to_string(Boolean val);
extern string to_string(Literal val);
extern bool isBoolean(const string& str);
extern Boolean toBoolean(const string& str);

#endif // PLSQLLITERAL

