#include "horizontalslicerenderer.h"
#include "horizontalslicetoimagemapper.h"
#include <iostream>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QOpenGLTexture>

HorizontalSliceRenderer::HorizontalSliceRenderer()
    : vertexBuffer(QOpenGLBuffer::VertexBuffer)
{
    initOpenGLShaders();
    initGeometry();
    currentz = 0;
    currentic = 0;
}

void HorizontalSliceRenderer::setMapper(HorizontalSliceToImageMapper* mpr)
{
    mapper = *mpr;
}

void HorizontalSliceRenderer::moveSlice(int steps) {
    currentz = (currentz+steps) % 16;
    if(currentz < 0) {
        currentz = 16 + currentz;
    }
}

void HorizontalSliceRenderer::changeWindComponent(int ic) {
    currentic = ic;
}

void HorizontalSliceRenderer::initOpenGLShaders()
{
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

void HorizontalSliceRenderer::initGeometry() {
    const unsigned int numVertices = 4;
    float unitCubeVertices[numVertices][3] = {
        {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0},
};

    // Create vertex buffer and upload vertex data to buffer.
    vertexBuffer.create(); // make sure to destroy in destructor!
    vertexBuffer.bind();
    vertexBuffer.allocate(unitCubeVertices, numVertices * 3 * sizeof(float));
    vertexBuffer.release();

    // Store the information OpenGL needs for rendering the vertex buffer
    // in a "vertex array object". This can easily be bound to the OpenGL
    // pipeline during rendering.
    QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject);
    if (vertexArrayObject.isCreated())
    {
        vertexBuffer.bind();
        shaderProgram.setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3, 3*sizeof(float));
        shaderProgram.enableAttributeArray("vertexPosition");
        vertexBuffer.release();
    }
}

void HorizontalSliceRenderer::drawImage(QMatrix4x4 mvpMatrix) {
    QImage image = mapper.mapSliceToImage(currentz, currentic);
    QOpenGLTexture texture(QOpenGLTexture::Target2D);
    texture.create();
    texture.setWrapMode(QOpenGLTexture::ClampToEdge);
    texture.setData(image);

    // Tell OpenGL to use the shader program of this class.
    shaderProgram.bind();

    // Bind the vertex array object that links to the bounding box vertices.
    vertexArrayObject.bind();

    // Set the model-view-projection matrix as a uniform value.
    shaderProgram.setUniformValue("mvpMatrix", mvpMatrix);

    const int textureUnit = 0;
    texture.bind(textureUnit);
    shaderProgram.setUniformValue("colorMappingTexture", textureUnit);


    // Issue OpenGL draw commands.
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glLineWidth(2);
    f->glDrawArrays(GL_TRIANGLE_STRIP, 0, 16);

    // Release objects until next render cycle.
    vertexArrayObject.release();
    shaderProgram.release();
}
