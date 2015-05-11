#include "opencvfacerecognizer.h"
#include <QFile>

const char* file = "data/a.fr";

OpencvFaceRecognizer::OpencvFaceRecognizer(QObject *parent) : QObject(parent),
    m_type(FaceRecognizer_Invalid),
    m_hadLoad(false)
{

}

OpencvFaceRecognizer::~OpencvFaceRecognizer()
{
    if (!m_ptr.empty()) {
        m_ptr->save(file);
    }
}

void OpencvFaceRecognizer::loadRecognizer(FaceRecognizerType type)
{
    if (m_type == FaceRecognizer_Invalid && type != FaceRecognizer_Invalid) {
        switch (type) {
        case FaceRecognizer_Fisher:
            m_ptr = cv::createFisherFaceRecognizer();
            break;
        case FaceRecognizer_Eigen:
            m_ptr = cv::createEigenFaceRecognizer();
            break;
        case FaceRecognizer_LBPH:
            m_ptr = cv::createLBPHFaceRecognizer();
        default:
            break;
        }
    }
    QFile f(file);
    if (!m_ptr.empty() && f.exists()) {
        m_ptr->load(file);
        m_hadLoad = true;
    }
}

