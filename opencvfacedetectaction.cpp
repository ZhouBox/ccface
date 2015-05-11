#include "opencvfacedetectaction.h"
#include <QDebug>
#include <QTime>

const char* cascade_name = "data/haarcascade_frontalface_alt.xml";
const int w = 240;
const int h = 300;

OpenCVfaceDetectAction::OpenCVfaceDetectAction(QObject *parent): OpenCVaction(parent),
    m_cascade(NULL),
    m_storage(NULL),
    m_scale(1.3),
    m_isTrain(false),
    m_isPredict(false),
    m_srcNum(2),
    m_label(0)
{
    m_cascade = (CvHaarClassifierCascade*)cvLoad(cascade_name, 0, 0, 0);
    if (!m_cascade) {
        qDebug() << "load Cascade fail!";
    }
    m_storage = cvCreateMemStorage(0);

    //m_isTrain = true;


}

OpenCVfaceDetectAction::~OpenCVfaceDetectAction()
{
    if (m_cascade) {
        cvReleaseHaarClassifierCascade(&m_cascade);
    }
    cvReleaseMemStorage(&m_storage);
}

void OpenCVfaceDetectAction::action(IplImage *imgin, IplImage *&imgout)
{
    imgout = cvCloneImage(imgin);
    if (!m_cascade) {
        qDebug() << "Cascade is invaild!";
        return;
    }
    static CvScalar color = CV_RGB(255, 255, 255);

    static int srcIndex = 0;

    //Image Preparation
    //
    IplImage* gray = cvCreateImage(cvGetSize(imgin), 8, 1);
    IplImage* small_img = cvCreateImage(cvSize(cvRound(imgin->width / m_scale),
                                               cvRound(imgin->height / m_scale)), 8, 1);

    cvCvtColor(imgin, gray, CV_BGR2GRAY);
    cvResize(gray, small_img, CV_INTER_LINEAR);

    cvEqualizeHist(small_img, small_img); //直方图均衡

    //Detect objects if any
    //
    cvClearMemStorage(m_storage);
    double t = (double)cvGetTickCount();
    CvSeq *objects = cvHaarDetectObjects(small_img,
                                         m_cascade,
                                         m_storage,
                                         1.1,
                                         2,
                                         0,
                                         cvSize(50,50));

    t = (double)cvGetTickCount() - t;
    qDebug() << "detection time =" << t / ((double)cvGetTickFrequency()*1000.) << "ms";

    //Loop through found objects and draw boxes around them
    for(int i=0; i < (objects ? objects->total : 0); ++i)
    {

        CvRect* r = (CvRect*)cvGetSeqElem(objects, i);
        cvRectangle(imgout, cvPoint(r->x * m_scale, r->y * m_scale),
                    cvPoint((r->x + r->width) * m_scale, (r->y + r->height) * m_scale), color, 2, CV_AA);

        if (m_isTrain) {
            if (objects->total != 1){
                break;
            }
            ++srcIndex;

            CvRect trect;
            trect.x = r->x * m_scale;
            trect.y = r->y * m_scale;
            trect.width = r->width * m_scale;
            trect.height = r->height * m_scale;

            cv::Mat t = getSubMat(gray, trect);
            m_mats.push_back(t);
            m_labels.push_back(m_label);
            if (srcIndex == m_srcNum) {
                emit train(m_mats, m_labels);
                srcIndex = 0;
                m_isTrain = false;
                m_mats.clear();
                m_labels.clear();
            }
        }

        if (m_isPredict) {
            if (objects->total != 1){
                break;
            }

            CvRect trect;
            trect.x = r->x * m_scale;
            trect.y = r->y * m_scale;
            trect.width = r->width * m_scale;
            trect.height = r->height * m_scale;

            cv::Mat t = getSubMat(gray, trect);
            emit predict(t);
            m_isPredict = false;
        }
    }

    cvReleaseImage(&gray);
    cvReleaseImage(&small_img);

    emit a();

}

cv::Mat OpenCVfaceDetectAction::norm_0_255(cv::InputArray _src) {
    cv::Mat src = _src.getMat();

    cv::Mat dst;
    switch(src.channels()) {
    case 1:
        cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}

cv::Mat OpenCVfaceDetectAction::getSubMat(IplImage *in, const CvRect &rect)
{
    IplImage *s = cvCreateImage(cvSize(w, h), in->depth, in->nChannels);
    cvSetImageROI(in, rect);
    cvResize(in, s);
    cvResetImageROI(in);
    return norm_0_255(cv::Mat(s, false));
}
