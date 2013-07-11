#ifndef OGLCONTEXT_H
#define OGLCONTEXT_H

#include <QGLWidget>
#include <QVector2D>
#include <QVector3D>
#include <QTimer>

class OGLContext : public QGLWidget
{
    Q_OBJECT

    QVector<QVector3D> vertices;
    QVector<QVector2D> texCoords;
    GLuint textures[4];

    GLfloat nScale;
    GLfloat ratio;

    GLfloat xRot;
    GLfloat yRot;
    GLfloat zRot;

    QPoint mousePosition;

    bool qRotate;

    QTimer *timer;

    void rotateBy(int xAngle, int yAngle, int zAngle);
    void makeObject();
private slots:
    void rotateOneStep();
protected:
    void initializeGL();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
public:
    OGLContext(QWidget* parent = 0);
    ~OGLContext();
};

#endif // OGLCONTEXT_H
