#-------------------------------------------------
#
# Project created by QtCreator 2013-04-27T17:56:27
#
#-------------------------------------------------

QT += core gui sql opengl svg

TARGET = SensorMonitor
TEMPLATE = app

#-------------------------------------------------
# For static build (need static Qt librares)
# Warning: please use qt_ru.qm coincident with the Qt version
#-------------------------------------------------

CONFIG += static
static {
    win32-g++: QMAKE_LFLAGS += -static -static-libgcc -static-libstdc++
}

# For Debug
QMAKE_CXXFLAGS_DEBUG += -D_DEBUG

# For using QWT
!static {
    QT_VER = $${QT_VERSION}
} else {
    QT_VER = $${QT_VERSION}-static
}

QWT_VER = 6.0.2

win32-g++ {
    QWT_PATH = C:/Qt/Qt$${QT_VER}/qwt-$${QWT_VER}
    QWT_INC_PATH = $${QWT_PATH}/include
    CONFIG(debug,debug|release) {
        DEBUG_SFX = d
    } else {
        DEBUG_SFX =
    }
    QWT_LIB = qwt$${DEBUG_SFX}$${VER_SFX}
}

linux-g++ {
    QWT_PATH = /usr/local/qwt-$${QWT_VER}
    QWT_INC_PATH = $${QWT_PATH}/include
    CONFIG(debug,debug|release) {
        DEBUG_SFX = d
    } else {
        DEBUG_SFX =
    }
    QWT_LIB = qwt$${DEBUG_SFX}$${VER_SFX}
}

INCLUDEPATH += $${QWT_INC_PATH}
LIBS += -L$${QWT_PATH}/lib -l$${QWT_LIB}

# For icon
win32-g++:RC_FILE = SensorMonitor.rc

# Files
SOURCES += main.cpp\
    Generators.cpp \
    LineChart.cpp \
    LineChartWidget.cpp \
    MainWindow.cpp \
    OGLContext.cpp \
    OGLContextWidget.cpp \
    SchemeSensors.cpp \
    LevelsWidget.cpp \
    LevelOneChart.cpp \
    DataBaseEngine.cpp \
    LevelOneMuChart.cpp \
    LevelOneDecWidget.cpp \
    LevelTwoDecWidget.cpp \
    LevelTwoChart.cpp \
    LevelTwoMuChart.cpp \
    NumSensorsDialog.cpp \
    HelpBrowser.cpp

HEADERS  += \
    Generators.h \
    LineChart.h \
    LineChartWidget.h \
    LoadData.h \
    MainWindow.h \
    OGLContext.h \
    OGLContextWidget.h \
    SchemeSensors.h \
    LevelsWidget.h \
    LevelOneChart.h \
    DataBaseEngine.h \
    LevelOneMuChart.h \
    LevelOneDecWidget.h \
    LevelTwoDecWidget.h \
    LevelTwoChart.h \
    LevelTwoMuChart.h \
    NumSensorsDialog.h \
    HelpBrowser.h

RESOURCES += \
    SensorMonitor.qrc

TRANSLATIONS += \
    sensormonitor_ru.ts \
    sensormonitor_en.ts

OTHER_FILES += \
    SensorMonitor.rc \
    CHANGELOG.txt \
    TODO.txt
