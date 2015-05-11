TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    opencvcamera.cpp \
    opencvshowframe.cpp \
    opencvcapture.cpp \
    opencvaction.cpp \
    opencvcannyaction.cpp \
    opencvcommonaction.cpp \
    opencvfacedetectaction.cpp \
    opencvfacerecognizer.cpp

RESOURCES += qml.qrc

CONFIG += c++11



# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    opencvcamera.h \
    opencvshowframe.h \
    opencvcapture.h \
    opencvaction.h \
    opencvcannyaction.h \
    opencvcommonaction.h \
    opencvfacedetectaction.h \
    opencvfacerecognizer.h \
    typedef.h

INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2

LIBS += /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_core.so    \
        /usr/local/lib/libopencv_imgproc.so \
        /usr/local/lib/libopencv_objdetect.so \
        /usr/local/lib/libopencv_contrib.so
