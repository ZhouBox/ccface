#include "opencvcamera.h"
//#include <QSGSimpleTextureNode>
//#include <QQuickWindow>
//#include <QTimer>
#include <QDebug>


OpenCVcamera::OpenCVcamera(QObject *parent): OpenCVcapture(parent),
    m_cameraId(0),
    m_openedCameraId(-1),
    m_iplImage(NULL),
    m_cvCapture(NULL)
{

}



OpenCVcamera::~OpenCVcamera()
{
    if (m_cvCapture != NULL) {
        cvReleaseCapture(&m_cvCapture);
        m_cvCapture = NULL;
    }

    m_iplImage = NULL;

}


int OpenCVcamera::cameraId() const
{
    return m_cameraId;
}


void OpenCVcamera::setCameraId(int id)
{
    m_cameraId = id;
}




void OpenCVcamera::setRun(bool r)
{
    m_run = r;
    if (m_run) {
        if (m_cvCapture == NULL) {
            m_cvCapture = cvCreateCameraCapture(m_cameraId);
            m_openedCameraId = m_cameraId;
        } else if (m_cameraId != m_openedCameraId){
            cvReleaseCapture(&m_cvCapture);
            m_cvCapture = NULL;
            m_cvCapture = cvCreateCameraCapture(m_cameraId);
            m_openedCameraId = m_cameraId;
        }
    } else {
        if (m_cvCapture) {
            cvReleaseCapture(&m_cvCapture);
            m_cvCapture = NULL;
            //m_cameraId = 0;
            m_openedCameraId = -1;
        }
    }
}

IplImage* OpenCVcamera::getFrame()
{
    if (m_cvCapture) {
        m_iplImage = cvQueryFrame(m_cvCapture);
    } else {
        m_iplImage = NULL;
    }
    return m_iplImage;
}







