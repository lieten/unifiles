#ifndef HORIZONTALSLICETOCONTOURLINEMAPPER_H
#define HORIZONTALSLICETOCONTOURLINEMAPPER_H
#include "flowdatasource.h"
#include "qimage.h"
#include <QVector>
#include <QVector3D>

class HorizontalSliceToContourLineMapper
{
public:
    HorizontalSliceToContourLineMapper();
    HorizontalSliceToContourLineMapper(FlowDataSource* src);
    void setDataSource(FlowDataSource* src);
    FlowDataSource* getDataSource();
    QVector<QVector3D> mapSliceToContourLineSegment(int z, int ic, float iso);
private:
    FlowDataSource source;
};

float calculateAlpha(float v0, float v1, float iso);

#endif // HORIZONTALSLICETOCONTOURLINEMAPPER_H
