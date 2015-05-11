#ifndef OPENCVFACEDETECTACTION_H
#define OPENCVFACEDETECTACTION_H
#include "opencvaction.h"
#include <vector>

#include "typedef.h"

class OpenCVfaceDetectAction : public OpenCVaction
{
    Q_OBJECT
public:
    OpenCVfaceDetectAction(QObject *parent = 0);
    ~OpenCVfaceDetectAction();

    void action(IplImage *imgin, IplImage *&imgout);
    void startTrain() {
        m_isTrain = true;
    }

    void startPredict() {
        m_isPredict = true;
    }


    void setLabel(int i) {
        m_label = i;
    }



signals:
    void a();
    void train(const Mats &src, const Ints &labels);
    void predict(const cv::Mat& src);
private:
    CvHaarClassifierCascade* m_cascade;
    CvMemStorage* m_storage;
    double m_scale;
    bool m_isTrain;
    bool m_isPredict;
    int m_srcNum;
    int m_label;
    Mats m_mats;
    Ints m_labels;


    static cv::Mat getSubMat(IplImage *in, const CvRect &rect);
    static cv::Mat norm_0_255(cv::InputArray _src);

};

#endif // OPENCVFACEDETECTACTION_H
