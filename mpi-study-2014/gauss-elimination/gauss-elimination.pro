TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp

LIBS += -L"C:/Program Files (x86)/Microsoft SDKs/MPI/Lib/x86/"\
    -lmsmpi

INCLUDEPATH += "C:\Program Files (x86)\Microsoft SDKs\MPI\Include"
