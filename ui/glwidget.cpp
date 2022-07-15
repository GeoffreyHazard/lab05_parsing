#include "glwidget.h"
#include <iostream>
#include <QOpenGLFunctions>
#include "parser/sceneparser.h"


void GLWidget::initializeGL() {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glClearColor(1.0f, 0.5f, 0.0f, 1.0f);

    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_CULL_FACE);
    f->glCullFace(GL_BACK);
    f->glFrontFace(GL_CCW);
}

void GLWidget::paintGL() {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWidget::loadScene(const RenderData &renderData) {
    // TODO: The magic happens here lol
    std::cout << "GLWidget [loadScene] begin" << std::endl;

    std::cout << "GLWidget [loadScene] success" << std::endl;
}
