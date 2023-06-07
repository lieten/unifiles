#include "horizontalslicetoimagemapper.h"
#include "flowdatasource.h"
#include <QImage>
#include <iostream>
#include <cmath>

//#define INTENSITYFACTOR 3

HorizontalSliceToImageMapper::HorizontalSliceToImageMapper() {}

HorizontalSliceToImageMapper::HorizontalSliceToImageMapper(FlowDataSource* src) {
    source = *src;
}

void HorizontalSliceToImageMapper::setDataSource(FlowDataSource* src)
{
    source = *src;
}

FlowDataSource* HorizontalSliceToImageMapper::getDataSource() {
    return &source;
}

QImage HorizontalSliceToImageMapper::mapSliceToImage(int z, int ic) {
    //QImage image("uhhlogo.png");
    /*
    int * dp;
    dp = source.getDimensions();
    int xs = *dp;
    int ys = *(dp+1);
    //int zs = *(dp+2);
    */

    int xs = source.getXDimension(); //16
    int ys = source.getYDimension(); //16
    //int zs = source.getZDimension(); //16

    QImage image = QImage(xs, ys, QImage::Format_RGB16);
    QColor colour;
    float val;
    for(int y = 0; y < ys; y++) {
        for(int x = 0; x < xs; x++) {
            if(ic == 3) { //Berechne Vektorbetrag
                float xcomp = source.getDataValue(z, y, x, 0);
                float ycomp = source.getDataValue(z, y, x, 1);
                float zcomp = source.getDataValue(z, y, x, 2);
                val = sqrt(xcomp * xcomp + ycomp * ycomp + zcomp * zcomp);
            }
            else { //Nimm die jeweilige Komponente (0-2)
                val = source.getDataValue(z, y, x, ic);
            }
            int colourVal = abs(val * 255);
            //coord = QPoint(x, y);
            if(val < 0) {
                colour = QColor(0, 0, colourVal);
            }
            else if(val > 0) {
                colour = QColor(colourVal, 0, 0);
            }
            else {
                colour = QColor(0, 0, 0);
            }

            //colour = QColor(100, 100, 100);
            image.setPixelColor(x, y, colour);
        }
    }

    return image;
}
