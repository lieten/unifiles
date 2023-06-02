#include "horizontalcontourlinesrenderer.h"
#include "horizontalslicetocontourlinemapper.h"
#include <iostream>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QOpenGLTexture>

HorizontalContourLinesRenderer::HorizontalContourLinesRenderer()
    : vertexBuffer(QOpenGLBuffer::VertexBuffer)
{
    initOpenGLShaders();
    initGeometry();
    currentz = 0;
    currentic = 0;
}

HorizontalContourLinesRenderer::~HorizontalContourLinesRenderer() {
    vertexBuffer.destroy();
}

void HorizontalContourLinesRenderer::setMapper(HorizontalSliceToContourLineMapper* mpr) {
    mapper = *mpr;
}

void HorizontalContourLinesRenderer::updateSourceData() {
    FlowDataSource* srcp = mapper.getDataSource();
    FlowDataSource src = *srcp;
    zdimension = src.getZDimension();
}

void HorizontalContourLinesRenderer::moveSlice(int steps) {
    updateSourceData();
    currentz = (currentz+steps) % zdimension;
    if(currentz < 0) {
        currentz = zdimension + currentz;
    }
}
void HorizontalContourLinesRenderer::changeWindComponent(int ic) {
    currentic = ic;
}
void HorizontalContourLinesRenderer::initOpenGLShaders(){
    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                               "lines_vshader.glsl"))
    {
        std::cout << "Vertex shader error:\n"
                  << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if (!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                               "lines_fshader.glsl"))
    {
        std::cout << "Fragment shader error:\n"
                  << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }

    if (!shaderProgram.link())
    {
        std::cout << "Shader link error:\n"
                  << shaderProgram.log().toStdString() << "\n" << std::flush;
        return;
    }
}

void HorizontalContourLinesRenderer::initGeometry() {
    //TODO: implement
    QVector<QVector3D> vertexVector = mapper.mapSliceToContourLineSegment(currentz, currentic, 0.0);
    QVector3D* vertexVectorData = vertexVector.data();
    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(vertexVectorData, vertexVector.length()*3*sizeof(float));
    vertexBuffer.release();

    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);
    if (vertexArrayObject.isCreated())
    {
        vertexBuffer.bind();
        shaderProgram.setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3, 3*sizeof(float));
        shaderProgram.enableAttributeArray("vertexPosition");
        vertexBuffer.release();
    }
}

void HorizontalContourLinesRenderer::drawContourLines(QMatrix4x4 mvpMatrix) {
    // Tell OpenGL to use the shader program of this class.
    shaderProgram.bind();

    // Probably update Geometry
    // initGeometry()

    // Bind the vertex array object that links to the bounding box vertices.
    vertexArrayObject.bind();

    // Set the model-view-projection matrix as a uniform value.
    shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);

    // Issue OpenGL draw commands.
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glLineWidth(2);
    f->glDrawArrays(GL_LINE_STRIP, 0, 16);

    // Release objects until next render cycle.
    vertexArrayObject.release();
    shaderProgram.release();
}
