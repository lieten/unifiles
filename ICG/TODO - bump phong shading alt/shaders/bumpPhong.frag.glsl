#version 300 es
precision highp float;

//////////
//       This is the Fragment shader
/////////

uniform vec3 lightPosition ;

uniform vec3 Ia ;
uniform vec3 Id ;
uniform vec3 Is ;

uniform vec3 ka ;
uniform vec3 kd ;
uniform vec3 ks ;
uniform vec3 c ;
uniform float specExp ;

uniform mat4 fragmentViewMatrix ;

in vec4 vfVertexPositionCamSpace ;
in vec4 vfNormalCamSpace ;
in vec3 vfKd ;

//normal mapping variables
in vec4 vfTangent;
in vec4 vfBitangent;

uniform sampler2D map;
uniform sampler2D tex;
in vec2 vfTexCoord;


out vec4 fColor ;

void main()
{
    vec4 vertexPositionInCameraSpace = vfVertexPositionCamSpace ;
    vec4 lightPositionInCameraSpace = fragmentViewMatrix * vec4 ( lightPosition, 1.0 ) ;
    vec4 N = normalize ( vfNormalCamSpace ) ;
    
    vec3 nTan = texture(map, vfTexCoord).rgb;
    nTan = nTan * 2.0 - 1.0;
    vec3 n = normalize(vfNormalCamSpace.xyz);
    vec3 t = normalize(vfTangent.xyz);
    vec3 b = normalize(vfBitangent.xyz);
    mat3 tbnMat = mat3(t, b, n);
    /*
    tbnMat[0] = vec3(t.x, b.x, n.x);
    tbnMat[1] = vec3(t.y, b.y, n.y);
    tbnMat[2] = vec3(t.z, b.z, n.z);
    */
    vec3 nCam = tbnMat * nTan;
    N = vec4(nCam.xyz, 0.0);
       
    vec3 color = texture(tex, vfTexCoord).rgb    ;

    vec4 V = normalize ( - vec4 ( vertexPositionInCameraSpace.xyz, 0.0 ) ) ;
    vec4 L = normalize ( vec4 ( lightPositionInCameraSpace.xyz - vertexPositionInCameraSpace.xyz, 0.0 ) ) ;
    vec4 R = normalize ( reflect ( -L, N ) ) ;

    vec3 ambient = Ia * ka ;
    float d = distance ( vertexPositionInCameraSpace, lightPositionInCameraSpace );
    float fatt = 1.0 / ( c[0] + c[1]*d + c[2]*d*d) ;
    vec3 diffuse = Id * /*vfKd.rgb*/color * max ( 0.0, dot ( N, L ) ) ;
    vec3 specular = vec3 ( 0.0 ) ;
    if ( dot ( N, L ) > 0.0 )
    {
        specular = Is * ks * pow ( max ( 0.0, dot ( R, V ) ), specExp ) ;
    }
        
    fColor = vec4 ( ambient + fatt * ( diffuse + specular ), 1.0 )  ;
}

