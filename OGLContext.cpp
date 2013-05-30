#include "OGLContext.h"

#include <QMouseEvent>
#include <QWheelEvent>

OGLContext::OGLContext(QWidget* parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot1=-159.726f; yRot1=102.339f; zRot1=0.0f;
    xRot2=-90.0f; yRot2=0.0f; zRot2=0.0f;

    nScale = 1;
}

void OGLContext::initializeGL()
{
    qglClearColor(Qt::white);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    genTextures();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void OGLContext::resizeGL(int nWidth, int nHeight)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    ratio = (GLfloat)nHeight/(GLfloat)nWidth;

    if (nWidth >= nHeight)
    {
        glOrtho(-10.0 / ratio, 10.0 / ratio, -10.0, 10.0, -10.0, 10.0);
    }
    else
    {
        glOrtho(-10.0, 10.0, -10.0 * ratio, 10.0 * ratio, -10.0, 10.0);
    }

    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
}

void OGLContext::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(nScale, nScale, nScale);
    glTranslatef(-1.0f/ratio, 0.0f, 0.0f);
    glRotatef(xRot1, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot1, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot1, 0.0f, 0.0f, 1.0f);

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textureID[0]);

    glBegin(GL_QUADS);

    /********** Block A ***********/
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.7f, -3.0f, -4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2.7f, -3.0f, -4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2.7f, 3.0f, -4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.7f, 3.0f, -4.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.7f, -3.0f, -6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.7f, 3.0f, -6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2.7f, 3.0f, -6.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(2.7f, -3.0f, -6.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.7f, 3.0f, -6.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.7f, 3.0f, -4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2.7f, 3.0f, -4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2.7f, 3.0f, -6.0f);

    glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.7f, -3.0f, -6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2.7f, -3.0f, -6.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(2.7f, -3.0f, -4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.7f, -3.0f, -4.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f(2.7f, -3.0f, -6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2.7f, 3.0f, -6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2.7f, 3.0f, -4.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(2.7f, -3.0f, -4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.7f, -3.0f, -6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.7f, -3.0f, -4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.7f, 3.0f, -4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.7f, 3.0f, -6.0f);

    /********** Block B ***********/
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, -3.0f, 2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2.0, -3.0f, 2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2.0, 3.0f, 2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.0, 3.0f, 2.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.0, -3.0f, -4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.0, 3.0f, -4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2.0, 3.0f, -4.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(2.0, -3.0f, -4.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.0, 3.0f, -4.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0, 3.0f, 2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2.0, 3.0f, 2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2.0, 3.0f, -4.0f);

    glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.0, -3.0f, -4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2.0, -3.0f, -4.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(2.0, -3.0f, 2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.0, -3.0f, 2.0f);

    /********** Block C1 ***********/
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, -3.0f, 4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f, -3.0f, 4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0f, 3.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0f, 3.0f, 4.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f(-3.0f, -3.0f, 2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-3.0f, 3.0f, 2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0f, 3.0f, 2.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, -3.0f, 2.0f);

    glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0f, 3.0f, 2.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, 3.0f, 4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f, 3.0f, 4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0f, 3.0f, 2.0f);

    glTexCoord2f(1.0f, 1.0f); glVertex3f(-3.0f, -3.0f, 2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0f, -3.0f, 2.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, -3.0f, 4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-3.0f, -3.0f, 4.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f, -3.0f, 2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0f, 3.0f, 2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0f, 3.0f, 4.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, -3.0f, 4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, -3.0f, 2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-3.0f, -3.0f, 4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-3.0f, 3.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0f, 3.0f, 2.0f);

    /********** Block C2 ***********/
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, -3.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, -3.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 3.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-3.0f, 3.0f, 4.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, -3.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, -3.0f, 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 3.0f, 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f, 3.0f, 4.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f(-3.0f, -3.0f, 4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-3.0f, 3.0f, 4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0f, 3.0f, 4.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, -3.0f, 4.0f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f(-3.0f, -3.0f, -6.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0f, -3.0f, -6.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(3.0f, -3.0f, 6.2f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, -3.0f, 6.2f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, -3.0f, 4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f, -3.0f, 4.0f);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, -3.0f, 6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, 3.0f, 4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f, 3.0f, 4.0f);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, 3.0f, 6.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, -3.0f, -6.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0f, -3.0f, -6.2f);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, -5.0f, -6.2f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, -3.0f, 6.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0f, -3.0f, 6.2f);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, -5.0f, 6.2f);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textureID[1]);

    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2.0, -3.0f, -4.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2.0, 3.0f, -4.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2.0, 3.0f, 2.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(2.0, -3.0f, 2.0f);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0, -3.0f, -4.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.0, -3.0f, 2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.0, 3.0f, 2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.0, 3.0f, -4.0f);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textureID[2]);

    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-3.0f, -3.0f, -6.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, -5.0f, -6.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, -5.0f, 6.2f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, -3.0f, 6.2f);

    glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0f, -3.0f, -6.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, -5.0f, -6.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, -5.0f, 6.2f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, -3.0f, 6.2f);
    glEnd();

    glPopMatrix();
}

void OGLContext::mousePressEvent(QMouseEvent* event)
{
    mousePosition = event->pos();
}

void OGLContext::genTextures()
{
    for (int i = 0; i < 3; ++i)
    {
        textureID[i]=bindTexture(QPixmap(QString("://gfx/Textures/texture%1.jpg").arg(i + 1)), GL_TEXTURE_2D);
    }
}

void OGLContext::mouseMoveEvent(QMouseEvent* event)
{
    xRot1 += 180 * (GLfloat) (event->y() - mousePosition.y()) / height();
    yRot1 += 180 * (GLfloat) (event->x() -mousePosition.x()) / width();

    updateGL();

    mousePosition = event->pos();
}

void OGLContext::wheelEvent(QWheelEvent *event)
{
    if ((event->delta()) > 0 && nScale < 1.61051)
    {
        nScale = nScale * 1.1;
    }
    else if ((event->delta()) < 0 && nScale > 0.101526)
    {
        nScale = nScale / 1.1;
    }
    updateGL();
}

OGLContext::~OGLContext()
{
    /* Empty destructor */
}
