#ifndef OGLCONTEXT_H
#define OGLCONTEXT_H

#include <QGLWidget>

class OGLContext : public QGLWidget
{
    Q_OBJECT
    GLfloat nScale;
    GLfloat ratio;

    GLfloat xRot1;
    GLfloat yRot1;
    GLfloat zRot1;

    GLfloat xRot2;
    GLfloat yRot2;
    GLfloat zRot2;
    QPoint mousePosition;
    GLuint textureID[3];
    void genTextures();
protected:
    void initializeGL();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

public:
    OGLContext(QWidget* parent = 0);
    ~OGLContext();
};

#endif // OGLCONTEXT_H
