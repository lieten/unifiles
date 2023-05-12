#version 300 es
in vec4 vPosition;
in vec4 vColor;
out vec4 vfColor;

void main()
{
    vfColor = vColor;
    gl_Position = vPosition;
}