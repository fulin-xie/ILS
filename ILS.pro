TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    data.cpp \
    ILS.cpp \
    dataInitialise.cpp \
    initialSolution.cpp \
    localSearch.cpp \
    perturbation.cpp

HEADERS += \
    data.h \
    ILS.h \
    dataInitialise.h \
    initialSolution.h \
    localSearch.h \
    perturbation.h

