#ifndef PLSQLTYPES_H
#define PLSQLTYPES_H

#include <config.h>

enum class TypeGroup{
  Numeric,
  String,
  Boolean,
  Unknown
};

bool isPlSqlType(const string& identifier);
bool isConvertible(const string& type_from, const string& type_to);
bool isCompationConvertible(TypeGroup grp_left, TypeGroup grp_right);

TypeGroup groupOfType(const string& type);

#endif // PLSQLTYPES_H
