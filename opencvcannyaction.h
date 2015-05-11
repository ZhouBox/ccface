#ifndef OPENCVCANNYACTION_H
#define OPENCVCANNYACTION_H

#include "opencvaction.h"


class OpenCVcannyAction : public OpenCVaction
{
    Q_OBJECT
public:
    OpenCVcannyAction(QObject *parent = 0);
    ~OpenCVcannyAction();

    void action(IplImage *imgin, IplImage *&imgout);

signals:

};

#endif // OPENCVCANNYACTION_H
