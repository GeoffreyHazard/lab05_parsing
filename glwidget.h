#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>

namespace CS123 {
    struct CS123SceneMetaData;
};


class GLWidget : public QOpenGLWidget
{
public:
    GLWidget(QWidget *parent) : QOpenGLWidget(parent) { }

    void loadScene(const CS123::CS123SceneMetaData &metaData);

protected:
    void initializeGL() override;
    void paintGL() override;
};

#endif // GLWIDGET_H
