#version 330
uniform mat4 mvpMatrix; //model-view-projection matrix

in vec4 vertexPosition;

smooth out vec2 texCoord;

void main()
{
    // Calculate vertex position in screen space.
    gl_Position = mvpMatrix * vertexPosition;
    texCoord = vertexPosition.xy;
}
