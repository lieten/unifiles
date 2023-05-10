#version 330
uniform mat4 mvpMatrix; //model-view-projection matrix
uniform int zoffset;

in vec4 vertexPosition;

smooth out vec2 texCoord;

void main()
{
    // Calculate vertex position in screen space.
    vec4 offsetPos = vertexPosition.xyzw;
    offsetPos.z += zoffset / 15.0;
    gl_Position = mvpMatrix * offsetPos;
    texCoord = vertexPosition.xy;
}
