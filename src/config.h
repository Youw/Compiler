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

#endif // CONFIG_H

