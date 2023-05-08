#version 300 es
precision highp float;

//////////
//       This is the Vertex shader
/////////

in vec4 vPosition;
in vec4 vColor;
in vec4 vNormal;

in vec2 vTexCoord;
out vec2 vfTexCoord;

in vec4 vTangent;
in vec4 vBitangent;

out vec4 vfTangent;
out vec4 vfBitangent;

//uniform sampler2D map;

out vec3 vfKd ;
out vec4 vfVertexPositionCamSpace ;
out vec4 vfNormalCamSpace ;

uniform mat4 projectionMatrix ;
// TODO 2.1: Erstelle Uniform-Variable für die Model Matrix
uniform mat4 modelMatrix ;
// TODO 2.1: Erstelle Uniform-Variable für die View-Matrix
uniform mat4 viewMatrix ;

void main()
{
    vec4 vertexPositionInCameraSpace = viewMatrix * modelMatrix * vPosition ;
    vfVertexPositionCamSpace = vertexPositionInCameraSpace ;

    mat4 normalMatrix = inverse ( transpose ( viewMatrix * modelMatrix ) ) ;
    
    vec4 N = normalMatrix * vec4 ( vNormal.xyz, 0.0 ) ;
    N.w = 0.0 ;
    N = normalize ( N ) ;
    vfNormalCamSpace = N ;

    vfTangent = normalMatrix * vTangent;
    vfTangent.w = 0.0;
    vfTangent = normalize(vfTangent);

    vfBitangent = normalMatrix * vBitangent;
    vfBitangent.w = 0.0;
    vfBitangent = normalize(vfBitangent);


    vfKd = vColor.rgb ;

    vfTexCoord = vTexCoord;

    // TODO 2.2: Transformiere Vertexposition mit Model und View Matrix
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vPosition ;
}