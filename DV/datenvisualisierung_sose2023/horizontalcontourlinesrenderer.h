#ifndef HORIZONTALCONTOURLINESRENDERER_H
#define HORIZONTALCONTOURLINESRENDERER_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "horizontalslicetocontourlinemapper.h"

class HorizontalContourLinesRenderer
{
public:
    HorizontalContourLinesRenderer();
    void setMapper(HorizontalSliceToContourLineMapper* mpr);
    void drawContourLines(QMatrix4x4 mvpMatrix);
    void moveSlice(int steps);
    void changeWindComponent(int ic);

    HorizontalSliceToContourLineMapper mapper;
    virtual ~HorizontalContourLinesRenderer();

private:
    void initOpenGLShaders();
    void initGeometry();
    void updateSourceData();

    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
    int currentz, zdimension, currentic;
};

#endif // HORIZONTALCONTOURLINESRENDERER_H
