#include "plsqltypes.h"

#include <unordered_set>

static const std::unordered_set<string> TYPES = {
  STR("NUMERIC"),
  STR("PLS_INTEGER"),
  STR("BINARY_INTEGER"),
  STR("BINARY_FLOAT"),
  STR("BINARY_DOUBLE"),
  STR("NUMBER"),
  STR("NATURAL"),
  STR("NATURALN"),
  STR("POSITIVE"),
  STR("POSITIVEN"),
  STR("SIGNTYPE"),
  STR("SIMPLE_INTEGER"),
  STR("DEC"),
  STR("DECIMAL"),
  STR("FLOAT"),
  STR("INT"),
  STR("INTEGER"),
  STR("SMALLINT"),
  STR("REAL"),
  STR("CHARACTER"),
  STR("CHAR"),
  STR("VARCHAR2"),
  STR("RAW"),
  STR("NCHAR"),
  STR("NVARCHAR2"),
  STR("LONG"),
  STR("ROWID"),
  STR("UROWID"),
  STR("BOOLEAN"),
};

bool isPlSqlType(const string& identifier)
{
  return TYPES.find(identifier)!=TYPES.end();
}

