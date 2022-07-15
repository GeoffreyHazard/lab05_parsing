#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>

struct RenderData;


class GLWidget : public QOpenGLWidget
{
public:
    GLWidget(QWidget *parent) : QOpenGLWidget(parent) { }

    void loadScene(const RenderData &renderData);

protected:
    void initializeGL() override;
    void paintGL() override;
};

#endif // GLWIDGET_H
