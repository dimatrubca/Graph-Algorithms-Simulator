QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
RC_FILE = myapp.rc

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    about.cpp \
    abstractalgorithm.cpp \
    bfs.cpp \
    dfs.cpp \
    dijkstra.cpp \
    edge.cpp \
    edgeform.cpp \
    eulercycle.cpp \
    graph.cpp \
    hamiltoniancycle.cpp \
    kruskal.cpp \
    main.cpp \
    mainwindow.cpp \
    maxflow.cpp \
    node.cpp \
    scene.cpp \
    sleeper.cpp

HEADERS += \
    about.h \
    abstractalgorithm.h \
    bfs.h \
    dfs.h \
    dijkstra.h \
    edge.h \
    edgeform.h \
    eulercycle.h \
    graph.h \
    hamiltoniancycle.h \
    kruskal.h \
    mainwindow.h \
    maxflow.h \
    node.h \
    scene.h \
    sleeper.h

FORMS += \
    about.ui \
    edgeform.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    myres.qrc
