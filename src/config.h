#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>
#include <string>
#include <QString>

using string=std::wstring;
using istream=std::wistream;
using ifstream=std::wifstream;
using ostream=std::wostream;
using character=wchar_t;
static auto& cout=std::wcout;

template<typename type>
inline string to_string(type param)
{
  return std::to_wstring(param);
}

template<>
inline string to_string(QString param)
{
  return param.toStdWString();
}

#define ADD_PREFIX(x,pre) pre##x

#define TO_WSTR(x) ADD_PREFIX(x,L)

#define STR(x) TO_WSTR(x)

class StrHasher {
public:
  using hash_type = std::uint64_t;
  static constexpr hash_type hash_const(const character* str, hash_type last = basis) {
    return *str ? hash_const(str+1, (*str ^ last) * prime) : last;
  }

  static hash_type hash(const string& str) {
      hash_type ret{basis};
      for (auto c : str) {
          ret ^= c;
          ret *= prime;
        }
      return ret;
  }
private:
  static constexpr hash_type prime = 0x100000001B3ull;
  static constexpr hash_type basis = 0xCBF29CE484222325ull;
};

#endif // CONFIG_H

