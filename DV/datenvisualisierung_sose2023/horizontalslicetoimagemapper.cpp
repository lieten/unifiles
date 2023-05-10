#include "horizontalslicetoimagemapper.h"
#include "flowdatasource.h"
#include <QImage>
#include <iostream>

HorizontalSliceToImageMapper::HorizontalSliceToImageMapper() {}

HorizontalSliceToImageMapper::HorizontalSliceToImageMapper(FlowDataSource* src) {
    source = *src;
}

void HorizontalSliceToImageMapper::setDataSource(FlowDataSource* src)
{
    source = *src;
}

QImage HorizontalSliceToImageMapper::mapSliceToImage(int z, int ic) {
    QImage image("uhhlogo.png");
    /*
    int * dp;
    dp = source.getDimensions();
    int xs = *dp;
    int ys = *(dp+1);
    //int zs = *(dp+2);
    */
    int xs = 16;
    int ys = 16;
    QColor colour;
    image = QImage(xs, ys, QImage::Format_RGB16);

    float val;
    for(int y = 0; y < ys; y++) {
        for(int x = 0; x < xs; x++) {
            val = source.getDataValue(z, y, x, ic);
            int colourVal = abs(val * 255)*3;
            //coord = QPoint(x, y);
            if(val < 0) {
                colour = QColor(0, 0, colourVal);
            }
            else {
                colour = QColor(colourVal, 0, 0);
            }

            //colour = QColor(100, 100, 100);
            image.setPixelColor(x, y, colour);
        }
    }

    return image;
}
