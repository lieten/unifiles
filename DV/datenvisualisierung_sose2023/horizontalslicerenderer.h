#ifndef HORIZONTALSLICERENDERER_H
#define HORIZONTALSLICERENDERER_H
#include "horizontalslicetoimagemapper.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class HorizontalSliceRenderer
{
public:
    HorizontalSliceRenderer();
    void setMapper(HorizontalSliceToImageMapper* mapper);
    void drawImage(QMatrix4x4 mvpMatrix);
    void moveSlice(int steps);
    void changeWindComponent(int ic);

    HorizontalSliceToImageMapper mapper;

private:
    void initOpenGLShaders();
    void initGeometry();
    void updateSourceData();

    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vertexBuffer;
    QOpenGLVertexArrayObject vertexArrayObject;
    int currentz, zdimension, currentic;
};

#endif // HORIZONTALSLICERENDERER_H
