TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    lexical/lexicalanalyzer.cpp \
    main.cpp

HEADERS += \
    lexical/lexicalanalyzer.h

CONFIG += C++11

INCLUDEPATH += $$PWD
