#version 300 es
precision highp float;

// ToDo 1.6.1 : Erstelle eine Uniformvariable fuer die Textur
uniform sampler2D map;

// ToDo 3.1 : Erstelle "in" fuer Texturekoordinaten
// vom Vertexshader
in vec2 vfTexCoord;

in vec4 vfColor;
out vec4 fColor;

void main()
{
    fColor = texture(map, vfTexCoord);

    // Todo 3.2
    // Benutze die Texturkoordinaten, um ein Texel aus der
    // Textur auszulesen, und setze fColor auf den
    // ausgelesenen Texelwert
}