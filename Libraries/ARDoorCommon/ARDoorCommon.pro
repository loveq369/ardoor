#-------------------------------------------------
#
# Project created by QtCreator 2013-04-20T14:47:06
#
#-------------------------------------------------


QT       -= core gui
QT       += opengl

TARGET = ARDoorCommon
TEMPLATE = lib

include(../../_globals.pro)

SOURCES += \
    CameraCalibration.cpp \
    PoseEstimation.cpp \
    ImagePipeline.cpp \
    TestImageProcessor.cpp \
    RenderingContext.cpp \
    PatternExtractor.cpp \
    ImageUtils.cpp

HEADERS += \
    CameraCalibration.h \
    PoseEstimation.h \
    ImagePipeline.h \
    ImageProcessor.h \
    TestImageProcessor.h \
    RenderingContext.h \
    DebugHelper.h \
    PatternExtractor.h \
    Pattern.h \
    ImageUtils.h

unix:!macx {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}

macx {
    QMAKE_LFLAGS += -F/Library/Frameworks
    LIBS += -framework opencv2
}
