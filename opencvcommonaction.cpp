#include "opencvcommonaction.h"

OpenCVcommonAction::OpenCVcommonAction()
{

}

OpenCVcommonAction::~OpenCVcommonAction()
{

}

void OpenCVcommonAction::action(IplImage *imgin, IplImage *&imgout)
{
    imgout = cvCreateImage(cvGetSize(imgin), imgin->depth, imgin->nChannels);
    cvCvtColor(imgin, imgout, CV_BGR2RGB);
}

