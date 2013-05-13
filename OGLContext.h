#ifndef OGLCONTEXT_H
#define OGLCONTEXT_H

#include <QGLWidget>

#define N 48

struct P
{
    P(): z(0), vz(0) {}
    float x;
    float y;
    float z;
    float vz;
};

class OGLContext: public QGLWidget
{
    Q_OBJECT

    GLint xRot;
    GLint yRot;
    GLint zRot;

    GLfloat K;
    GLfloat DT;
    GLfloat nSca;

    QPoint lastPos;

    P p[N][N];

    void normalizeAngle(int *angle);

    GLfloat sqrtx(float x);

    void scale_plus();
    void scale_minus();
    void changeXRotPlus();
    void changeXRotMinus();
signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);
    virtual void keyPressEvent(QKeyEvent *keyEvent);
private slots:
    void updateScene();
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
public:
    OGLContext(QWidget *parent = 0);
    ~OGLContext();
};

#endif // OGLCONTEXT_H
