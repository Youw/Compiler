TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    lexical/lexicalanalyzer.cpp \
    main.cpp \
    lexical/lexem.cpp \
    lexical/plsqllexem.cpp \
    lexical/plsqllexemconstants.cpp \
    lexical/lexeminfo.cpp

HEADERS += \
    lexical/lexicalanalyzer.h \
    lexical/lexem.h \
    lexical/lexeminfo.h \
    config.h \
    lexical/plsqllexem.h \
    lexical/plsqldelimeters.h \
    lexical/plsqlidentifiers.h \
    lexical/plsqlliteral.h

CONFIG += C++11

INCLUDEPATH += $$PWD
