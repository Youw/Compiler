TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    lexical/lexicalanalyzer.cpp \
    main.cpp \
    lexical/lexem.cpp

HEADERS += \
    lexical/lexicalanalyzer.h \
    lexical/lexem.h \
    lexical/lexeminfo.h \
    config.h

CONFIG += C++11

INCLUDEPATH += $$PWD
