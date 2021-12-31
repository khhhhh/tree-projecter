#version 330

//uniform vec3 MaterialAmbient;
//uniform vec3 MaterialDiffuse;
//uniform vec3 MaterialSpecular;
uniform vec4 leavesColor;
uniform float MaterialShiness;
uniform float LightPower;
uniform sampler2D ColorTexture;


in vec3 eyePosition;
in vec3 normalVector;
in vec3 lightVector;
uniform vec3 LightPosition;
in float distance;
in float camDistance;
in vec2 uv;

out vec4 fragColor;

vec3 applyFog(in vec3 color, in vec3 fogColor, in float distance) {
    float fogAmount = 1.0 - exp( -distance * 0.05);
    return mix( color, fogColor, fogAmount );
}

vec4 applyFog(in vec4 color, in vec4 fogColor, in float distance) {
    float fogAmount = 1.0 - exp( -distance * 0.05);
    return mix( color, fogColor, fogAmount );
}

void main(void)
{
    vec3 n = normalize(normalVector);
    vec3 l = normalize(lightVector);

    float iDiffuse = clamp(dot(n, l), 0, 1);

    vec3 e = normalize(-eyePosition);
    vec3 r = reflect(-l, n);

    float iSpecular = pow(clamp(dot(e, r), 0, 1), MaterialShiness);

    vec4 MaterialDiffuse = texture( ColorTexture, uv );
    if(MaterialDiffuse.a < 0.1)
        discard;
    if(leavesColor.a < 0.15)
        discard;

    //fragColor = MaterialDiffuse;
    fragColor= MaterialDiffuse * leavesColor;

    /*
    vec3 color = MaterialDiffuse;
    fragColor=vec4(color,1);

    vec4 MaterialDiffuse = texture( ColorTexture, uv ).rgba;
    vec4 MaterialAmbient = vec4(0.1,0.1,0.1,0.1) * MaterialDiffuse;
    vec4 MaterialSpecular = vec4(0.3,0.3,0.3,0.3);

    vec4 color = MaterialAmbient +
            LightPower * MaterialDiffuse * iDiffuse / (distance * distance) +
            LightPower * MaterialSpecular * iSpecular / (distance * distance);

    color = applyFog(color, vec4(0.5, 0.5, 0.5, 0.5), camDistance);
    fragColor=color;
    */
}
