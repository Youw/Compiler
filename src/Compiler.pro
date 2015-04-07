TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    lexical/lexicalanalyzer.cpp \
    main.cpp \
    lexical/lexem.cpp \
    lexical/plsqllexem.cpp

HEADERS += \
    lexical/lexicalanalyzer.h \
    lexical/lexem.h \
    lexical/lexeminfo.h \
    config.h \
    lexical/plsqllexem.h \
    lexical/plsqldelimeters.h

CONFIG += C++11

INCLUDEPATH += $$PWD
