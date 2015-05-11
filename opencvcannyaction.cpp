#include "opencvcannyaction.h"

OpenCVcannyAction::OpenCVcannyAction(QObject *parent): OpenCVaction(parent)
{

}

OpenCVcannyAction::~OpenCVcannyAction()
{

}

void OpenCVcannyAction::action(IplImage *imgin, IplImage *&imgout)
{
    IplImage* pIplImageCanny = cvCreateImage(cvGetSize(imgin), imgin->depth, 1);
    cvCvtColor(imgin, pIplImageCanny, CV_RGB2GRAY);
    IplImage *imgCannyout = cvCreateImage(cvGetSize(imgin), imgin->depth, 1);
    cvCanny(pIplImageCanny, imgCannyout, 15, 145, 3);
    cvReleaseImage(&pIplImageCanny);
    imgout = cvCreateImage(cvGetSize(imgin), imgin->depth, 3);
    cvCvtColor(imgCannyout, imgout, CV_GRAY2RGB);//qt not suppert Format_Indexed8. so do this.
    cvReleaseImage(&imgCannyout);

}

