#ifndef HORIZONTALSLICETOIMAGEMAPPER_H
#define HORIZONTALSLICETOIMAGEMAPPER_H
#include "flowdatasource.h"
#include "qimage.h"


class HorizontalSliceToImageMapper
{
public:
    HorizontalSliceToImageMapper();
    HorizontalSliceToImageMapper(FlowDataSource* src);
    void setDataSource(FlowDataSource* src);
    QImage mapSliceToImage(int z, int ic);

private:
    FlowDataSource source;
};

#endif // HORIZONTALSLICETOIMAGEMAPPER_H
