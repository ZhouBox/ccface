#ifndef OPENCVACTION_H
#define OPENCVACTION_H

#include <QObject>
#include <opencv/cv.h>
#include <opencv/highgui.h>

class OpenCVaction : public QObject
{
    Q_OBJECT
public:
    explicit OpenCVaction(QObject *parent = 0);
    virtual ~OpenCVaction();
    virtual void action(IplImage *imgin, IplImage *&imgout) = 0;
signals:

public slots:
};

#endif // OPENCVACTION_H
