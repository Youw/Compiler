#ifndef PLSQLIDENTIFIER
#define PLSQLIDENTIFIER

#include <config.h>

extern const size_t MAX_IDENTIFIER_SIZE;

bool isOrdinaryIdentifierCharacter(character c);
bool isKeyword(const string& identifier);
bool isReservedWord(const string& identifier);

#endif // PLSQLIDENTIFIER

