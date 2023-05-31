#include "horizontalslicetocontourlinemapper.h"
#include "flowdatasource.h"
#include <QVector>
#include <QVector3D>
#include <iostream>
#include <cmath>

HorizontalSliceToContourLineMapper::HorizontalSliceToContourLineMapper()
{
}

HorizontalSliceToContourLineMapper::HorizontalSliceToContourLineMapper(FlowDataSource* src) {
    source = *src;
}

void HorizontalSliceToContourLineMapper::setDataSource(FlowDataSource* src) {
    source = *src;
}

FlowDataSource* HorizontalSliceToContourLineMapper::getDataSource() {
    return &source;
}

QVector<QVector3D> HorizontalSliceToContourLineMapper::mapSliceToContourLineSegment(int z, int ic, float iso) {
    QVector<QVector3D> vertexVector;
    int xdimension = source.getXDimension();
    int ydimension = source.getYDimension();

    for(int x = 1; x < xdimension; x++) {
        for(int y = 1; y < ydimension; y++) {
            float v0val = source.getDataValue(z, y-1, x-1, ic);
            float v1val = source.getDataValue(z, y-1, x, ic);
            float v2val = source.getDataValue(z, y, x, ic);
            float v3val = source.getDataValue(z, y, x-1, ic);
            int v0 = v0val==iso;
            int v1 = v1val==iso;
            int v2 = v2val==iso;
            int v3 = v3val==iso;
            // mirror cases by flipping bits
            if(v3) {
                v0 = !v0;
                v1 = !v1;
                v2 = !v2;
            }
            int cellcode = v0 + 10*v1 + 100*v2;
            float v0v1a = calculateAlpha(v0val, v1val, iso);
            float v1v2a = calculateAlpha(v1val, v2val, iso);
            float v2v3a = calculateAlpha(v2val, v3val, iso);
            float v3v0a = calculateAlpha(v0val, v1val, iso);
            float x1, y1, x2, y2;

            switch(cellcode) {
                case 0000:
                    break;

                case 0001:
                    x1 = (x-1)+v0v1a*(x-(x-1));
                    y1 = (y-1)+v0v1a*((y-1)-(y-1));
                    x2 = (x-1)+v3v0a*((x-1)-(x-1));
                    y2 = y+v3v0a*((y-1)-y);
                    break;

                case 0010:
                    x1 = (x-1)+v0v1a;
                    y1 = y-1;
                    x2 = x;
                    y2 = y-1+v1v2a;
                    break;

                case 0011:
                    x1 = x;
                    y1 = y-1+v1v2a;
                    x2 = x-1;
                    y2 = y-v3v0a;
                    break;

                case 0100:
                    x1 = x;
                    y1 = y-1+v1v2a;
                    x2 = x-v2v3a;
                    y2 = y;
                    break;

                case 0101:
                //TODO:
                    break;

                case 0110:
                    x1 = x-1+v0v1a;
                    y1 = y-1;
                    x2 = x-v2v3a;
                    y2 = y;
                    break;

                case 0111:
                    x1 = x-v2v3a;
                    y1 = y;
                    x2 = x-1;
                    y2 = y-v3v0a;
                    break;
            }

            if(x1 && x2 && y1 && y2) {
                QVector3D vec1 (x1/16.0, y1/16.0, z/16.0);
                QVector3D vec2 (x2/16.0, y2/16.0, z/16.0);
                vertexVector.append(vec1);
                vertexVector.append(vec2);
            }
        }
    }

    return vertexVector;
}

float calculateAlpha(float v0, float v1, float iso) {
    return (iso - v1)/(v1 - v0);
}
