#ifndef CONFIG_H
#define CONFIG_H

#include <istream>
#include <string>

using string=std::wstring;
using istream=std::wistream;
using ostream=std::wostream;
using character=wchar_t;

template<typename type>
inline string to_string(type param)
{
  return std::to_wstring(param);
}

#define ADD_PREFIX(x,pre) pre##x

#define TO_WSTR(x) ADD_PREFIX(x,L)

#define STR(x) TO_WSTR(x)

#endif // CONFIG_H

