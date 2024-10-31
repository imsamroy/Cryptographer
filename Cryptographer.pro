QT += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += debug

QMAKE_CXXFLAGS += -fsanitize=address
QMAKE_LFLAGS += -fsanitize=address
LIBS += -static-libasan

QMAKE_LFLAGS += -Wl,--stack,2097152

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    githubapi.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    githubapi.h \
    mainwindow.h

RESOURCES += \
    resources.qrc
