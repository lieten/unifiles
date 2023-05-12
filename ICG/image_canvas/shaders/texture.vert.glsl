#version 300 es
precision highp float;

in vec4 vPosition;
in vec4 vColor;

// ToDo 2.1 Erstelle Vertex Attribut fuer die
// Texturkoordinaten
in vec2 vTexCoord;

out vec4 vfColor;

// Todo 2.2 Uebergebe die Texturkoordiaten
// interpoliert an den Fragmentshader
out vec2 vfTexCoord;

void main()
{
    vfColor = vColor ;
    gl_Position = vPosition;
    vfTexCoord = vTexCoord;
}