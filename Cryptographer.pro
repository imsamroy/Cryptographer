QT += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += debug

# Only add ASan flags in debug mode
debug {
    QMAKE_CXXFLAGS += -fsanitize=address -g
    QMAKE_LFLAGS += -fsanitize=address
}

# Increase stack size if needed
QMAKE_LFLAGS += -Wl,--stack,2097152

# Source files
SOURCES += \
    githubapi.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    githubapi.h \
    mainwindow.h

RESOURCES += \
    resources.qrc
