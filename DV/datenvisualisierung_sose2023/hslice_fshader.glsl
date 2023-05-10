#version 330
layout(location = 0) out vec4 fragColor;

uniform sampler2D colorMappingTexture;
smooth in vec2 texCoord;

void main()
{
    // Set the color "red", RGB = (1, 0, 0) to the fragment.
    //fragColor = vec4(1, 0, 0, 1);
    fragColor = texture(colorMappingTexture, texCoord);
}

