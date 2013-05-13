#include "oglcontext.h"

#include <QMouseEvent>
#include <QTimer>

#ifdef _DEBUG
#include <QDebug>
#endif

#include <cmath>

OGLContext::OGLContext(QWidget *parent)
    : QGLWidget(parent)
{
    /* For KeyEvent! */
    setFocusPolicy(Qt::StrongFocus);

    K = 40;
    DT = 0.03;
    nSca = 1;

    /* Fix View */
    xRot = 5000;

    for (int x = 0; x < N; ++x)
    {
        for (int y = 0; y < N; ++y)
        {
            p[x][y].x = x * 480 / N;
            p[x][y].y = y * 480 / N;
        }
    }
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateScene()));
    timer->start();
}

void OGLContext::setXRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != xRot)
    {
        /* Limiter */
        if (angle > 3800)
        {
            xRot = angle;
        }
        emit xRotationChanged(angle);
        updateGL();
    }
}

void OGLContext::setYRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != yRot)
    {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void OGLContext::setZRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != zRot)
    {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void OGLContext::normalizeAngle(int *angle)
{
    while (*angle < 0)
    {
        *angle += 360 * 16;
    }
    while (*angle > 360 * 16)
    {
        *angle -= 360 * 16;
    }
}

GLfloat OGLContext::sqrtx(float x)
{
    return x * x;
}

void OGLContext::initializeGL()
{
    glEnable(GL_NORMALIZE);
    glClearColor(0, 0, 0, 1.0);
}

void OGLContext::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    /* Zooming */
    glScalef(1, nSca, 1);

    /* Rotating Up/Down */
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    //    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    //    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);

    //qDebug() << xRot / 16.0;

    for (int x = 0; x < N; ++x)
    {
        glBegin(GL_LINE_STRIP);
        for (int y = 0; y < N; ++y)
        {
            glVertex3f(p[x][y].x, p[x][y].y, p[x][y].z * 10);
        }
        glEnd();
    }
    for (int y = 0; y < N; ++y)
    {
        glBegin(GL_LINE_STRIP);
        for (int x = 0; x < N; ++x)
        {
            glVertex3f(p[x][y].x, p[x][y].y, p[x][y].z * 10);
        }
        glEnd();
    }
    glPopMatrix();

    /* Please use Pop/Push Matrix and not swapBuffers */
    // swapBuffers();
}

void OGLContext::updateScene()
{
    const int dx[] = {-1, 0, 1, 0};
    const int dy[] = {0, 1, 0, -1};
    if (rand() % 500 == 0)
        p[rand() % (N - 2) + 1][rand() % (N - 2) + 1].vz += 150;
    for (int y = 1; y < N - 1; ++y)
        for (int x = 1; x < N - 1; ++x)
        {
            P &p0 = p[x][y];
            for (int i = 0; i < 4; ++i)
            {
                P &p1 = p[x + dx[i]][y + dy[i]];
                const float d = sqrt(sqrtx(p0.x - p1.x) + sqrtx(p0.y - p1.y) + sqrtx(p0.z - p1.z));
                p0.vz += K * (p1.z - p0.z) / d * DT;
                p0.vz *= 0.999;
            }

        }
    for (int y = 1; y < N - 1; ++y)
        for (int x = 1; x < N - 1; ++x)
        {
            P &p0 = p[x][y];
            p0.z += p0.vz * DT;
        }
    updateGL();
}

void OGLContext::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-100, 100, -100, 100, 100, 2000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-240, -240, -200);

    /* Moved to PaintGL */
    // glRotatef(-30, 1, 0, 0);
}

void OGLContext::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void OGLContext::wheelEvent(QWheelEvent *event)
{
    if ((event->delta()) > 0)
    {
        scale_plus();
    }
    else if ((event->delta()) < 0)
    {
        scale_minus();
    }
    updateGL();
}

void OGLContext::scale_plus()
{
    nSca = nSca*1.1;
#ifdef _DEBUG
    qDebug() << "Pressed +";
#endif
}

void OGLContext::scale_minus()
{
    nSca = nSca/1.1;
#ifdef _DEBUG
    qDebug() << "Pressed -";
#endif
}

void OGLContext::changeXRotPlus()
{
    xRot += 20;
    if (xRot > 5750)
    {
        xRot = 5750;
    }
}

void OGLContext::changeXRotMinus()
{
    xRot -= 20;
    if (xRot < 3800)
    {
        xRot = 3800;
    }
}

void OGLContext::keyPressEvent(QKeyEvent * keyEvent)
{
    switch (keyEvent->key())
    {
    case Qt::Key_Plus:
        scale_plus();
        break;

    case Qt::Key_Minus:
        scale_minus();
        break;

    case Qt::Key_Up:
        changeXRotPlus();
        break;

    case Qt::Key_Down:
        changeXRotMinus();
        break;
    }
    updateGL();
}

void OGLContext::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    }
    else if (event->buttons() & Qt::RightButton)
    {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }

    lastPos = event->pos();
}

OGLContext::~OGLContext()
{
    /* Empty Destructor */
}
