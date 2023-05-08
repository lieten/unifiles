#version 300 es
in vec4 vPosition;
in vec4 vColor;
uniform mat4 Ia;
uniform vec4 Id;
uniform vec4 Is;
uniform vec4 ka;
uniform vec4 kd;
uniform vec4 ks;
out vec4 vfColor;
const float c1 = 1.0;
const float c2 = 0.0005;
const float c3 = 0.000003;

// TODO 2.1: Erstelle Uniform-Variable für die Model Matrix
uniform mat4 modelMatrix;
// TODO 2.1: Erstelle Uniform-Variable für die View-Matrix
uniform mat4 viewMatrix;

void main()
{
    const mat4 projectionMatrix = mat4(
        1.2071068286895752, 0, 0, 0, 
        0, 2.4142136573791504, 0, 0, 
        0, 0, -1.0100502967834473, -1, 
        0, 0, -1.0050251483917236, 0);
              
    vfColor = vColor;

    // TODO 2.2: Transformiere Vertexposition mit Model und View Matrix
    //vPosition = modelMatrix * vPosition;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vPosition;
}