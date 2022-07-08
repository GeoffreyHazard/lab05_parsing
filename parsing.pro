QT += core gui
QT += xml
QT += opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    ui/glwidget.cpp \
    ui/mainwindow.cpp \
    parser/sceneparser.cpp \
    parser/scenefilereader.cpp

HEADERS += \
    ui/glwidget.h \
    ui/mainwindow.h \
    parser/scenedefinition.h \
    parser/sceneparser.h \
    parser/scenefilereader.h

FORMS += \
    ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += glm
DEPENDPATH += glm
DEFINES += _USE_MATH_DEFINES
DEFINES += TIXML_USE_STL
DEFINES += GLM_SWIZZLE GLM_FORCE_RADIANS
