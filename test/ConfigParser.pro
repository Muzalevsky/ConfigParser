TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        ../src/config_parser_api.cpp \
        ../src/filterparser.cpp

HEADERS += \
        ../src/config_parser_api.h \
        ../src/filterparser.h

INCLUDEPATH += \
        $$_PRO_FILE_PWD_/../src
