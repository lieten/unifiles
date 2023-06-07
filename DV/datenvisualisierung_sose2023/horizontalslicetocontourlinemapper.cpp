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

    for(int xc = 1; xc < xdimension; xc++) {
        for(int yc = 1; yc < ydimension; yc++) {
            float v0val = source.getDataValue(z, yc-1, xc-1, ic);
            float v1val = source.getDataValue(z, yc-1, xc, ic);
            float v2val = source.getDataValue(z, yc, xc, ic);
            float v3val = source.getDataValue(z, yc, xc-1, ic);
            int v0 = v0val==iso; //checkIfInRange(v0val, iso, 0.0);
            int v1 = v1val==iso; //checkIfInRange(v1val, iso, 0.0);
            int v2 = v2val==iso; //checkIfInRange(v2val, iso, 0.0);
            int v3 = v3val==iso; //checkIfInRange(v3val, iso, 0.0);
            //std::cout << v0val << v1val << v2val << v3val<< "\n";
            //std::cout << "Treffer: " << v0 << v1 << v2 << v3 << "\n";
            // mirror cases by flipping bits
            if(v3) {
                v0 = invert(v0);
                v1 = invert(v1);
                v2 = invert(v2);
                //std::cout << "Treffer: " << v0 << v1 << v2 << v3 << "\n";
            }
            int cellcode = v0 + 10*v1 + 100*v2;
            float v0v1a = calculateAlpha(v0val, v1val, iso);
            float v1v2a = calculateAlpha(v1val, v2val, iso);
            float v2v3a = calculateAlpha(v2val, v3val, iso);
            float v3v0a = calculateAlpha(v0val, v1val, iso);
            float x1, y1, x2, y2, x3, y3, x4, y4, fcenter;
            float x = (float)xc;
            float y = (float)yc;
            if(cellcode > 0) {
                std::cout << cellcode << "\n";
            }
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
                    //cellcode = 0;
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

            if(cellcode != 0000) {
                x1 /= (float) xdimension;
                x2 /= (float) xdimension;
                y1 /= (float) ydimension;
                y2 /= (float) ydimension;
                float zval = (float)z / (float) zdimension;
                QVector3D vec1 (x1, y1, zval);
                QVector3D vec2 (x2, y2, zval);
                vertexVector.append(vec1);
                vertexVector.append(vec2);
                std::cout << "success " << x1 << " " << y1 << " " << x2 << " " << y2 << " " << zval << "\n";
                if(cellcode == 0101) {
                    x3 /= (float) xdimension;
                    x4 /= (float) xdimension;
                    y3 /= (float) ydimension;
                    y4 /= (float) ydimension;
                    QVector3D vec3 (x3, y3, zval);
                    QVector3D vec4 (x4, y4, zval);
                    vertexVector.append(vec3);
                    vertexVector.append(vec4);
                }
            }
        }
    }

    return vertexVector;
}

float calculateAlpha(float v0, float v1, float iso) {
    return (iso - v1)/(v1 - v0);
}

int checkIfInRange(float measure, float iso, float range) {
    if(measure < iso + range) {
        return 1;
    }
    if(measure > iso - range) {
        return 1;
    }
    return 0;
}

int invert(int x) {
    if (x != 0) {
        return 0;
    }
    return 1;
}
