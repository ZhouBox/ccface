#include "opencvshowframe.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/contrib/contrib.hpp>
#include "opencvcapture.h"
#include <QSGTexture>
#include <QSGSimpleTextureNode>
#include <QQuickWindow>
#include "opencvaction.h"
#include "opencvcannyaction.h"
#include "opencvcommonaction.h"
#include "opencvfacedetectaction.h"
#include "opencvfacerecognizer.h"


OpenCVshowFrame::OpenCVshowFrame(QQuickItem *parent) : QQuickItem(parent),
    m_frameRate(33),
    m_run(false),
    m_capture(NULL)
{
    m_timer.setInterval(1000 / m_frameRate);
    connect(&m_timer, &QTimer::timeout, this, &OpenCVshowFrame::updateFrame);
    setFlag(QQuickItem::ItemHasContents);
    m_actions.push_back(new OpenCVcommonAction());
    //m_actions.push_back(new OpenCVcannyAction());
    /*OpenCVfaceDetectAction **/f = new OpenCVfaceDetectAction();
    m_actions.push_back(f);
    connect(f, &OpenCVfaceDetectAction::a, this, &OpenCVshowFrame::a);

    //using namespace cv;
    //Ptr<FaceRecognizer> r = createFisherFaceRecognizer();

    /*OpencvFaceRecognizer **/fr = new OpencvFaceRecognizer(this);
    fr->loadRecognizer();

    connect(f, &OpenCVfaceDetectAction::train, fr, &OpencvFaceRecognizer::train);
    connect(f, &OpenCVfaceDetectAction::predict, fr, &OpencvFaceRecognizer::predict);

    connect(fr, &OpencvFaceRecognizer::who, this, &OpenCVshowFrame::who);

}

OpenCVshowFrame::~OpenCVshowFrame()
{
    for (auto ite = m_actions.begin(); ite != m_actions.end(); ++ite) {
        (*ite)->deleteLater();
    }
    m_actions.clear();
}

int OpenCVshowFrame::frameRate() const
{
    return m_frameRate;
}

void OpenCVshowFrame::startPredict()
{
    f->startPredict();
}

void OpenCVshowFrame::setLabel(int i)
{
    f->setLabel(i);
}

void OpenCVshowFrame::startTrain()
{
    f->startTrain();
}

void OpenCVshowFrame::setFrameRate(int rate)
{
    if (rate <= 0) {
        rate = 33;
    }
    m_frameRate = rate;
    if (m_timer.isActive()) {
        m_timer.stop();
        m_timer.start(1000 / m_frameRate);
    } else {
        m_timer.setInterval(1000 / m_frameRate);
    }
}

bool OpenCVshowFrame::run() const
{
    return m_run;
}

void OpenCVshowFrame::setRun(bool r)
{
    m_run = r;
    if (m_run) {
        if (!m_timer.isActive())
            m_timer.start();
    } else {
        if (m_timer.isActive())
            m_timer.stop();
    }
}

QObject* OpenCVshowFrame::capture() const
{
    return m_capture;
}

void OpenCVshowFrame::fun()
{
    qDebug() << "fun";
}

void OpenCVshowFrame::setCapture(QObject *c)
{
    m_capture = c;
}

void OpenCVshowFrame::updateFrame()
{
    OpenCVcapture *cap = static_cast<OpenCVcapture*>(m_capture);
    if (cap->run()) {
        update();
    }
}


void OpenCVshowFrame::addAction(QObject *act)
{
    m_actions.push_back(act);
}

IplImage* OpenCVshowFrame::doActions(IplImage *img)
{
    IplImage *t = img;
    if (m_actions.empty()) {
    } else {
        QList<IplImage*> list;
        for (auto ite = m_actions.begin(); ite != m_actions.end(); ++ite) {
            OpenCVaction *act = static_cast<OpenCVaction*>(*ite);
            IplImage *out;
            act->action(t, out);
            t = out;
            list.push_back(out);
        }
        for (auto ite = list.begin(); ite != list.end(); ++ite) {
            if ((*ite) != t) {
                cvReleaseImage(&(*ite));
            }
        }
    }
    return t;
}


QImage::Format OpenCVshowFrame::format(int depth, int nChannels)
{
    QImage::Format re = QImage::Format_Invalid;
    do {
        if (depth == 8 && nChannels == 1) {
            re = QImage::Format_RGB888;
            break;
        }
        if (nChannels == 3) {
            re = QImage::Format_RGB888;
            break;
        }

    }while(0);

    return re;
}


QSGNode* OpenCVshowFrame::updatePaintNode(QSGNode *old, UpdatePaintNodeData *)
{
    QSGSimpleTextureNode *texture = static_cast<QSGSimpleTextureNode*>(old);
    if (texture == NULL) {
        texture = new QSGSimpleTextureNode();
    }
    QImage img;
    IplImage *iplImage = NULL;
    IplImage *out = NULL;
    if (m_capture) {
        iplImage = static_cast<OpenCVcapture*>(m_capture)->getFrame();
    }
    if (iplImage != NULL) {
        out = doActions(iplImage);
        uchar *imgData = (uchar *)out->imageData;
        //qDebug() << out->depth << out->nChannels;
        img = QImage(imgData, out->width, out->height, QImage::Format_RGB888);
    } else {
        img = QImage(boundingRect().size().toSize(), QImage::Format_RGB888);
    }
    QSGTexture *t = window()->createTextureFromImage(img.scaled(boundingRect().size().toSize()));
    if (t) {
        QSGTexture *tt = texture->texture();
        if (tt) {
            tt->deleteLater();
        }
        texture->setRect(boundingRect());
        texture->setTexture(t);
    }
    if (out) {
        cvReleaseImage(&out);
    }
    return texture;
}
