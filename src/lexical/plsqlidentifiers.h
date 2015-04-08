#ifndef PLSQLIDENTIFIER
#define PLSQLIDENTIFIER

#include <config.h>

extern const size_t MAX_IDENTIFIER_SIZE;

bool isOrdinaryIdentifierCharacter(character c);
bool isKeyword(const string& word);
bool isReservedWord(const string& word);

#endif // PLSQLIDENTIFIER

