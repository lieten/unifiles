#version 330
uniform mat4 mvpMatrix; //model-view-projection matrix
uniform int zlayer; //current z layer
uniform int zdimension; //dimension of z axis

in vec4 vertexPosition;

void main()
{
    // Calculate vertex position in screen space.
    vec4 offsetPos = vertexPosition.xyzw;
    float zoffset = zlayer / (zdimension - 1.0);
    offsetPos.z += zoffset + 0.1;
    gl_Position = mvpMatrix * offsetPos;
}
