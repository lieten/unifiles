#include "horizontalslicetoimagemapper.h"
#include "flowdatasource.h"
#include <QImage>

HorizontalSliceToImageMapper::HorizontalSliceToImageMapper() {
}

void HorizontalSliceToImageMapper::setDataSource(FlowDataSource* src)
{
    FlowDataSource source = *src;
}

QImage HorizontalSliceToImageMapper::mapSliceToImage() {
    QImage image;
    image.load("uhhlogo.png");
    return image;
}
