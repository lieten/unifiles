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
    int zdimension = source.getZDimension();

    for(int yc = 1; yc < xdimension; yc++) {
        for(int xc = 1; xc < ydimension; xc++) {
            float v0val = source.getDataValue(z, yc-1, xc-1, ic);
            float v1val = source.getDataValue(z, yc-1, xc, ic);
            float v2val = source.getDataValue(z, yc, xc, ic);
            float v3val = source.getDataValue(z, yc, xc-1, ic);
            int v0 = v0val>iso;
            int v1 = v1val>iso;
            int v2 = v2val>iso;
            int v3 = v3val>iso;
            // mirror cases by flipping bits
            if(v3) {
                v0 = invert(v0);
                v1 = invert(v1);
                v2 = invert(v2);
            }
            int cellcode = v0 + 2*v1 + 4*v2;
            float v0v1a = calculateAlpha(v0val, v1val, iso);
            float v1v2a = calculateAlpha(v1val, v2val, iso);
            float v2v3a = calculateAlpha(v2val, v3val, iso);
            float v3v0a = calculateAlpha(v3val, v0val, iso);
            float x1, y1, x2, y2, x3, y3, x4, y4, fcenter;
            float x = (float)xc;
            float y = (float)yc;

            switch(cellcode) {
                case 0:
                    break;

                case 1:
                    x1 = x-1+v0v1a;
                    y1 = y-1;
                    x2 = x-1;
                    y2 = y-v3v0a;
                    break;

                case 2:
                    x1 = (x-1)+v0v1a;
                    y1 = y-1;
                    x2 = x;
                    y2 = y-1+v1v2a;
                    break;

                case 3:
                    x1 = x;
                    y1 = y-1+v1v2a;
                    x2 = x-1;
                    y2 = y-v3v0a;
                    break;

                case 4:
                    x1 = x;
                    y1 = y-1+v1v2a;
                    x2 = x-v2v3a;
                    y2 = y;
                    break;

                case 5:
                    x1 = x-1+v0v1a;
                    y1 = y-1;
                    x2 = x;
                    y2 = y-1 + v1v2a;
                    x3 = x -v2v3a;
                    y3 = y;
                    x4 = x-1;
                    y4 = y-v3v0a;
                    fcenter = (v0val + v1val + v2val + v3val)/4.0;
                    break;

                case 6:
                    x1 = x-1+v0v1a;
                    y1 = y-1;
                    x2 = x-v2v3a;
                    y2 = y;
                    break;

                case 7:
                    x1 = x-v2v3a;
                    y1 = y;
                    x2 = x-1;
                    y2 = y-v3v0a;
                    break;

                default:
                    x1=0.0;
                    x2=0.0;
                    y1=0.0;
                    y2=0.0;
                break;
            }

            if(cellcode != 0) {
                x1 /= (float) xdimension-1;
                x2 /= (float) xdimension-1;
                y1 /= (float) ydimension-1;
                y2 /= (float) ydimension-1;

                float zval = (float)z / (float) zdimension;
                QVector3D vec1 (x1, y1, zval);
                QVector3D vec2 (x2, y2, zval);
                if(cellcode == 5) {
                    x3 /= (float) xdimension-1;
                    x4 /= (float) xdimension-1;
                    y3 /= (float) ydimension-1;
                    y4 /= (float) ydimension-1;
                    QVector3D vec3 (x3, y3, zval);
                    QVector3D vec4 (x4, y4, zval);
                    if(fcenter < iso) {
                        vertexVector.append(vec1);
                        vertexVector.append(vec2);
                        vertexVector.append(vec3);
                        vertexVector.append(vec4);
                    }
                    else {
                        vertexVector.append(vec4);
                        vertexVector.append(vec1);
                        vertexVector.append(vec2);
                        vertexVector.append(vec3);
                    }
                }
                else {
                    vertexVector.append(vec1);
                    vertexVector.append(vec2);
                }
            }
        }
    }
    return vertexVector;
}

float calculateAlpha(float v0, float v1, float iso) {
    if(v1 == v0) {
        return 0;
    }
    return (iso - v0)/(v1 - v0);
}

int invert(int x) {
    if (x != 0) {
        return 0;
    }
    return 1;
}
