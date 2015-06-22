#ifndef PLSQLTYPES_H
#define PLSQLTYPES_H

#include <config.h>

bool isPlSqlType(const string& identifier);
bool isConvertiible(const string& type_from, const string& type_to);

#endif // PLSQLTYPES_H
