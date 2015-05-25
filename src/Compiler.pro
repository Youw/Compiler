TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += qt

SOURCES += \
    lexical/lexicalanalyzer.cpp \
    main.cpp \
    lexical/lexem.cpp \
    lexical/plsqllexem.cpp \
    lexical/plsqllexemconstants.cpp \
    lexical/lexeminfo.cpp \
    syntax/syntax.cpp \
    syntax/rule.cpp \
    syntax/ruleentityterminal.cpp \
    syntax/ruleentitynonterminal.cpp \
    syntax/ruleentityextra.cpp

HEADERS += \
    lexical/lexicalanalyzer.h \
    lexical/lexem.h \
    lexical/lexeminfo.h \
    config.h \
    lexical/plsqllexem.h \
    lexical/plsqldelimeters.h \
    lexical/plsqlidentifiers.h \
    lexical/plsqlliteral.h \
    syntax/syntax.h \
    syntax/rule.h \
    syntax/ruleentity.h \
    syntax/ruleentityterminal.h \
    syntax/ruleentitynonterminal.h \
    syntax/ruleentityextra.h

CONFIG += C++11

INCLUDEPATH += $$PWD $$PWD/lexical $$PWD/syntax

RESOURCES += \
    syntax/synatx.qrc
