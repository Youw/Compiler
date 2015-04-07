#include "plsqldelimeters.h"

const std::unordered_map<string,Delimiter> DELEMITER_NAMES = {
  {STR("+") ,Delimiter::ADDITION_OPERATOR},
  {STR(":="),Delimiter::ASSIGNMENT_OPERATOR},
  {STR("=>"),Delimiter::ASSOCIATION_OPERATOR},
  {STR("\%"),Delimiter::ATTRIBUTE_INDICATOR},
  {STR("'") ,Delimiter::CHARACTER_STRING_DELIMITER},
  {STR(".") ,Delimiter::COMPONENT_INDICATOR},
  {STR("||"),Delimiter::CONCATENATION_OPERATOR},
  {STR("/") ,Delimiter::DIVISION_OPERATOR},
  {STR("**"),Delimiter::EXPONENTIATION_OPERATOR},
  {STR("(") ,Delimiter::EXPRESSION_OR_LIST_DELIMITER_BEGIN},
  {STR(")") ,Delimiter::EXPRESSION_OR_LIST_DELIMITER_END},
  {STR(":") ,Delimiter::HOST_VARIABLE_INDICATOR},
  {STR(",") ,Delimiter::ITEM_SEPARATOR},
  {STR("<<"),Delimiter::LABEL_DELIMITER_BEGIN},
  {STR(">>"),Delimiter::LABEL_DELIMITER_END},
  {STR("/*"),Delimiter::MULTILINE_COMMENT_DELIMITER_BEGIN},
  {STR("*/"),Delimiter::MULTILINE_COMMENT_DELIMITER_END},
  {STR("*") ,Delimiter::MULTIPLICATION_OPERATOR},
  {STR("\""),Delimiter::QUOTED_IDENTIFIER_DELIMITER},
  {STR(".."),Delimiter::RANGE_OPERATOR},
  {STR("=") ,Delimiter::RELATIONAL_OPERATOR_EQUAL},
  {STR("<>"),Delimiter::RELATIONAL_OPERATOR_NOT_EQUAL},
  {STR("!="),Delimiter::RELATIONAL_OPERATOR_NOT_EQUAL},
  {STR("~="),Delimiter::RELATIONAL_OPERATOR_NOT_EQUAL},
  {STR("^="),Delimiter::RELATIONAL_OPERATOR_NOT_EQUAL},
  {STR("<") ,Delimiter::RELATIONAL_OPERATOR_LESS_THAN},
  {STR(">") ,Delimiter::RELATIONAL_OPERATOR_GREATER_THAN},
  {STR("<="),Delimiter::RELATIONAL_OPERATOR_LESS_THAN_OR_EQUAL},
  {STR(">="),Delimiter::RELATIONAL_OPERATOR_GREATER_THAN_OR_EQUAL},
  {STR("@") ,Delimiter::REMOTE_ACCESS_INDICATOR},
  {STR("--"),Delimiter::SINGLELINE_COMMENT_INDICATOR},
  {STR(";") ,Delimiter::STATEMENT_TERMINATOR},
  {STR("-") ,Delimiter::SUBTRACTION_OR_NEGATION_OPERATOR}
};

//the map value show if delimiter has secons part
const std::unordered_map<character,bool> DELEMITER_FIRST_PARTS = {
  {STR('+') ,false},
  {STR(':') ,true },
  {STR('=') ,true },
  {STR('\%'),false},
  {STR('\''),false},
  {STR('.') ,true },
  {STR('|') ,true },
  {STR('/') ,true },
  {STR('*') ,true },
  {STR('(') ,false},
  {STR(')') ,false},
  {STR(',') ,false},
  {STR('"') ,false},
  {STR('!') ,true },
  {STR('~') ,true },
  {STR('^') ,true },
  {STR('<') ,true },
  {STR('>') ,true },
  {STR('@') ,false},
  {STR('-') ,true },
  {STR(';') ,false}
};
