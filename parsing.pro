QT += core gui
QT += xml
QT += opengl openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    glwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    utils/CS123SceneLoader.cpp \
    utils/CS123XmlSceneParser.cpp

HEADERS += \
    glwidget.h \
    mainwindow.h \
    utils/CS123ISceneParser.h \
    utils/CS123SceneData.h \
    utils/CS123SceneLoader.h \
    utils/CS123XmlSceneParser.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += glm utils
DEPENDPATH += glm utils
DEFINES += _USE_MATH_DEFINES
DEFINES += TIXML_USE_STL
DEFINES += GLM_SWIZZLE GLM_FORCE_RADIANS
