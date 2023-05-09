#ifndef HORIZONTALSLICETOIMAGEMAPPER_H
#define HORIZONTALSLICETOIMAGEMAPPER_H
#include "flowdatasource.h"
#include "qimage.h"


class HorizontalSliceToImageMapper
{
public:
    HorizontalSliceToImageMapper();
    void setDataSource(FlowDataSource* source);
    QImage mapSliceToImage();
private:
    //FlowDataSource source;
};

#endif // HORIZONTALSLICETOIMAGEMAPPER_H
