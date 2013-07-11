#include "OGLContext.h"

#include <QMouseEvent>
#include <QWheelEvent>

#ifdef _DEBUG
#include <QDebug>
#endif

OGLContext::OGLContext(QWidget* parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    // xRot: -167.684 yRot: 163.307
    xRot=-167.684f;
    yRot=163.307f;
    zRot=0.0f;

    nScale = 1;

    qRotate = true;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(rotateOneStep()));
    timer->start(20);
}

void OGLContext::makeObject()
{
    /* Bind Textures */
    for (int j=0; j < 4; ++j)
    {
        textures[j] = bindTexture
                (QPixmap(QString("://gfx/Textures/texture%1.png").arg(j + 1)), GL_TEXTURE_2D);
    }

    /* Global Variables */
    const float global_y = 1.8f;
    const float global_z = 1.5f;

    /* Block A */
    const float blockA_x = 0.5f;
    static const float blockA[6][4][3] =
    {
        {
            {+blockA_x, -global_y, -global_z},
            {-blockA_x, -global_y, -global_z},
            {-blockA_x, +global_y, -global_z},
            {+blockA_x, +global_y, -global_z}
        },
        {
            {+blockA_x, +global_y, -global_z},
            {-blockA_x, +global_y, -global_z},
            {-blockA_x, +global_y, +global_z},
            {+blockA_x, +global_y, +global_z}
        },
        {
            {+blockA_x, -global_y, +global_z},
            {+blockA_x, -global_y, -global_z},
            {+blockA_x, +global_y, -global_z},
            {+blockA_x, +global_y, +global_z}
        },
        {
            {-blockA_x, -global_y, -global_z},
            {-blockA_x, -global_y, +global_z},
            {-blockA_x, +global_y, +global_z},
            {-blockA_x, +global_y, -global_z}
        },
        {
            {+blockA_x, -global_y, +global_z},
            {-blockA_x, -global_y, +global_z},
            {-blockA_x, -global_y, -global_z},
            {+blockA_x, -global_y, -global_z}
        },
        {
            {-blockA_x, -global_y, +global_z},
            {+blockA_x, -global_y, +global_z},
            {+blockA_x, +global_y, +global_z},
            {-blockA_x, +global_y, +global_z}
        }
    };

    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            texCoords.append
                    (QVector2D(j == 0 || j == 3, j == 0 || j == 1));
            vertices.append
                    (QVector3D((2.0f * blockA[i][j][0]) - 6.5f, 2.0f * blockA[i][j][1], 2.0f * blockA[i][j][2]));

        }
    }
    /* End Block A */

    /* Block B */
    const float blockB_x = 1.7f;
    const float blockB_z = 1.2f;
    static const float blockB[6][4][3] =
    {
        {
            {+blockB_x, -global_y, -blockB_z},
            {-blockB_x, -global_y, -blockB_z},
            {-blockB_x, +global_y, -blockB_z},
            {+blockB_x, +global_y, -blockB_z}
        },
        {
            {+blockB_x, +global_y, -blockB_z},
            {-blockB_x, +global_y, -blockB_z},
            {-blockB_x, +global_y, +blockB_z},
            {+blockB_x, +global_y, +blockB_z}
        },
        {
            {+blockB_x, -global_y, +blockB_z},
            {+blockB_x, -global_y, -blockB_z},
            {+blockB_x, +global_y, -blockB_z},
            {+blockB_x, +global_y, +blockB_z}
        },
        {
            {-blockB_x, -global_y, -blockB_z},
            {-blockB_x, -global_y, +blockB_z},
            {-blockB_x, +global_y, +blockB_z},
            {-blockB_x, +global_y, -blockB_z}
        },
        {
            {+blockB_x, -global_y, +blockB_z},
            {-blockB_x, -global_y, +blockB_z},
            {-blockB_x, -global_y, -blockB_z},
            {+blockB_x, -global_y, -blockB_z}
        },
        {
            {-blockB_x, -global_y, +blockB_z},
            {+blockB_x, -global_y, +blockB_z},
            {+blockB_x, +global_y, +blockB_z},
            {-blockB_x, +global_y, +blockB_z}
        }
    };

    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            texCoords.append
                    (QVector2D(j == 0 || j == 3, j == 0 || j == 1));
            vertices.append
                    (QVector3D(2.0f * blockB[i][j][0] - 2.2f, 2.0f * blockB[i][j][1], 2.0f * blockB[i][j][2]));}
    }
    /* End Block B */

    /* Block C p1 */
    const float blockCp1_x = 0.6f;
    static const float blockCp1[6][4][3] =
    {
        {
            {+blockCp1_x, -global_y, -global_z},
            {-blockCp1_x, -global_y, -global_z},
            {-blockCp1_x, +global_y, -global_z},
            {+blockCp1_x, +global_y, -global_z}
        },
        {
            {+blockCp1_x, +global_y, -global_z},
            {-blockCp1_x, +global_y, -global_z},
            {-blockCp1_x, +global_y, +global_z},
            {+blockCp1_x, +global_y, +global_z}
        },
        {
            {+blockCp1_x, -global_y, +global_z},
            {+blockCp1_x, -global_y, -global_z},
            {+blockCp1_x, +global_y, -global_z},
            {+blockCp1_x, +global_y, +global_z}
        },
        {
            {-blockCp1_x, -global_y, -global_z},
            {-blockCp1_x, -global_y, +global_z},
            {-blockCp1_x, +global_y, +global_z},
            {-blockCp1_x, +global_y, -global_z}
        },
        {
            {+blockCp1_x, -global_y, +global_z},
            {-blockCp1_x, -global_y, +global_z},
            {-blockCp1_x, -global_y, -global_z},
            {+blockCp1_x, -global_y, -global_z}
        },
        {
            {-blockCp1_x, -global_y, +global_z},
            {+blockCp1_x, -global_y, +global_z},
            {+blockCp1_x, +global_y, +global_z},
            {-blockCp1_x, +global_y, +global_z}
        }
    };

    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            texCoords.append
                    (QVector2D(j == 0 || j == 3, j == 0 || j == 1));
            vertices.append
                    (QVector3D((2.0f * blockCp1[i][j][0]) + 2.4f, 2.0f * blockCp1[i][j][1], 2.0f * blockCp1[i][j][2]));
        }
    }
    /* End Block C p1 */

    /* Block C p2 */
    const float blockCp2_x = 0.2f;
    static const float blockCp2[5][4][3] =
    {
        {
            {+blockCp2_x, -global_y, 0.0f},
            {-1.0f, -global_y, - global_z},
            {-1.0f, +global_y, - global_z},
            {+blockCp2_x, +global_y, 0.0f}
        },
        {
            {+blockCp2_x, +global_y, 0.0f},
            {-1.0f, +global_y, - global_z},
            {-1.0f, +global_y, + global_z},
            {+blockCp2_x, +global_y, 0.0f}
        },
        {
            {-1.0f, -global_y, -global_z},
            {-1.0f, -global_y, + global_z},
            {-1.0f, +global_y, + global_z},
            {-1.0f, +global_y, - global_z}
        },
        {
            {+blockCp2_x, -global_y, 0.0f},
            {-1.0f, -global_y, + global_z},
            {-1.0f, -global_y, - global_z},
            {+blockCp2_x, -global_y, 0.0f}
        },
        {
            {-1.0f, -global_y, +global_z},
            {+blockCp2_x, -global_y, 0.0f},
            {+blockCp2_x, +global_y, 0.0f},
            {-1.0f, +global_y, +global_z}
        }
    };

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            texCoords.append
                    (QVector2D(j == 0 || j == 3, j == 0 || j == 1));
            vertices.append
                    (QVector3D((2.0f * blockCp2[i][j][0]) + 5.6f, 2.0f * blockCp2[i][j][1], 2.0f * blockCp2[i][j][2]));
        }
    }
    /* End Block C p1 */

    /* Roof block */
    const float roof_x = 3.5f;
    const float roof_y = 0.7f;
    const float roof_indent = 0.2f;
    static const float coords3[5][4][3] =
    {
        {
            {+roof_x, -roof_y, 0.0f},
            {-roof_x, -roof_y, 0.0f},
            {-roof_x, +roof_y, -(global_z + roof_indent)},
            {+roof_x, +roof_y, -(global_z + roof_indent)}
        },
        {
            {+roof_x, +roof_y, -(global_z + roof_indent)},
            {-roof_x, +roof_y, -(global_z + roof_indent)},
            {-roof_x, +roof_y, +(global_z + roof_indent)},
            {+roof_x, +roof_y, +(global_z + roof_indent)}
        },
        {
            {-roof_x, +roof_y, -(global_z + roof_indent)},
            {-roof_x, -roof_y, 0.0f},
            {-roof_x, -roof_y, 0.0f},
            {-roof_x, +roof_y, +(global_z + roof_indent)}
        },
        {
            {+roof_x, +roof_y, +(global_z + roof_indent)},
            {+roof_x, -roof_y, 0.0f},
            {+roof_x, -roof_y, 0.0f},
            {+roof_x, +roof_y, -(global_z + roof_indent)}
        },
        {
            {-roof_x, -roof_y, 0.0f},
            {+roof_x, -roof_y, 0.0f},
            {+roof_x, +roof_y, +(global_z + roof_indent)},
            {-roof_x, +roof_y, +(global_z + roof_indent)}
        }
    };

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (i == 2 || i == 3)
            {
                if (j == 0)
                {
                    texCoords.append(QVector2D(0, 0));
                }
                else if (j == 1 || j == 2)
                {
                    texCoords.append(QVector2D(0.5, 1));
                }
                else if (j == 3)
                {
                    texCoords.append(QVector2D(1, 0));
                }
            }
            else
            {
                texCoords.append(QVector2D(j == 0 || j == 3, j == 0 || j == 1));
            }
            vertices.append
                    (QVector3D((2.0f * coords3[i][j][0]) - 0.9f, (2.0f * coords3[i][j][1]) - 5.0f,
                    2.0f * coords3[i][j][2]));
        }
    }
    /* End Roof block */
}

void OGLContext::initializeGL()
{
    makeObject();

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
}

void OGLContext::paintGL()
{
    qglClearColor(Qt::white);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glScalef(nScale, nScale, nScale);
    glTranslatef(-1.0f / ratio, 0.0f, 0.0f);
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot, 0.0f, 0.0f, 1.0f);

    glVertexPointer(3, GL_FLOAT, 0, vertices.constData());
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords.constData());
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    for (int i = 0; i < 28; ++i)
    {
        if (i == 23 || i == 27)
        {
            glBindTexture(GL_TEXTURE_2D, textures[2]);
        }
        else if (i == 25 || i == 26)
        {
            glBindTexture(GL_TEXTURE_2D, textures[3]);
        }
        else if (i == 6 || i == 11)
        {
            glBindTexture(GL_TEXTURE_2D, textures[1]);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, textures[0]);
        }
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }
}

void OGLContext::resizeGL(int nWidth, int nHeight)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    ratio = (GLfloat)nHeight/(GLfloat)nWidth;

    if (nWidth >= nHeight)
    {
        glOrtho(-10.5f / ratio, +10.5f / ratio, -10.5f, +10.5f, -10.0f, +10.0f);
    }
    else
    {
        glOrtho(-10.5f, 10.5f, -10.5f * ratio, 10.5f * ratio, -10.0f, +10.0f);
    }

    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
    glMatrixMode(GL_MODELVIEW);
}

void OGLContext::mousePressEvent(QMouseEvent* event)
{
    mousePosition = event->pos();
}

void OGLContext::mouseMoveEvent(QMouseEvent* event)
{
    xRot += 180 * (GLfloat) (event->y() - mousePosition.y()) / height();
    yRot += 180 * (GLfloat) (event->x() - mousePosition.x()) / width();
#ifdef _DEBUG
    qDebug() << "xRot:" << xRot << "yRot:" << yRot;
#endif
    updateGL();

    mousePosition = event->pos();
}

void OGLContext::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;
    updateGL();
}

void OGLContext::rotateOneStep()
{
    if (qRotate)
    {
        rotateBy(1, 1, 1);
    }
}

void OGLContext::wheelEvent(QWheelEvent *event)
{
    if ((event->delta()) > 0 && nScale < 1.0f)
    {
        nScale = nScale * 1.05f;
#ifdef _DEBUG
        qDebug() << nScale;
#endif
    }
    else if ((event->delta()) < 0 && nScale > 0.40f)
    {
        nScale = nScale / 1.05f;
#ifdef _DEBUG
        qDebug() << nScale;
#endif
    }
    updateGL();
}

void OGLContext::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Down:
        xRot += 5.0f;
#ifdef _DEBUG
        qDebug() << xRot;
#endif
        break;
    case Qt::Key_Up:
        xRot -= 5.0f;
        break;
    case Qt::Key_Left:
        yRot += 5.0f;
        break;
    case Qt::Key_Right:
        yRot -= 5.0f;
        break;
    case Qt::Key_Space:
        (qRotate) ? qRotate = false : qRotate = true;
        break;
    default:
        break;
    }
    updateGL();
}

OGLContext::~OGLContext()
{
    /* Empty destructor */
}
