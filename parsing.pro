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
    ui/GLWidget.cpp \
    ui/MainWindow.cpp \
    parser/SceneParser.cpp \
    parser/ScenefileReader.cpp

HEADERS += \
    ui/GLWidget.h \
    ui/MainWindow.h \
    parser/SceneData.h \
    parser/SceneParser.h \
    parser/ScenefileReader.h

FORMS += \
    ui/MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += glm
DEPENDPATH += glm
DEFINES += _USE_MATH_DEFINES
DEFINES += TIXML_USE_STL
DEFINES += GLM_SWIZZLE GLM_FORCE_RADIANS
