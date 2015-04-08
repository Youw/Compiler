#include "lexeminfo.h"

static const std::unordered_map<LexemType,string> LexemTypeNames = {
  {LexemType::DELIMITER,STR("DELIMITER")},
  {LexemType::IDENTIFIER,STR("IDENTIFIER")},
  {LexemType::LITERAL,STR("LITERAL")},
  {LexemType::COMMENT,STR("COMMENT")},
  {LexemType::ERROR,STR("ERROR")}
};

const string& LexemTypeName(LexemType type)
{
 return LexemTypeNames.at(type);
}
