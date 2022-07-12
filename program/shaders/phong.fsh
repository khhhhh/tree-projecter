#version 330

uniform sampler2D ColorTexture;

in vec2 uv;

out vec4 fragColor;

void main(void)
{
    vec4 MaterialDiffuse = texture( ColorTexture, uv );

    fragColor= MaterialDiffuse;
}
