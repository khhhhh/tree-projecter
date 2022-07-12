#version 330

uniform vec4 leavesColor;
uniform sampler2D ColorTexture;

in vec2 uv;

out vec4 fragColor;

void main(void)
{
    vec4 MaterialDiffuse = texture( ColorTexture, uv );
    if(MaterialDiffuse.a < 0.1)
        discard;
    else if(leavesColor.a < 0.15)
        discard;

    fragColor= MaterialDiffuse * leavesColor;
}
