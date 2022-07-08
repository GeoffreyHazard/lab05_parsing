#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>

struct SceneMetaData;


class GLWidget : public QOpenGLWidget
{
public:
    GLWidget(QWidget *parent) : QOpenGLWidget(parent) { }

    void loadScene(const SceneMetaData &metaData);

protected:
    void initializeGL() override;
    void paintGL() override;
};

#endif // GLWIDGET_H
