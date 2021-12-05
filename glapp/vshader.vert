#version 330

uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ProjMat;
uniform vec3 LightPosition;
uniform float LightPower;


uniform vec3 MaterialAmbient;
uniform vec3 MaterialDiffuse;
uniform vec3 MaterialSpecular;
uniform float MaterialShiness;

layout (location=0) in vec3 VertexPosition;
layout (location=2) in vec3 VertexColor;
layout (location=3) in vec3 VertexNormal;

out vec3 color;

void main(void)
{


    gl_Position = ProjMat * ViewMat * ModelMat * vec4(VertexPosition, 1);
    vec3 vpws = vec4(ModelMat * vec4(VertexPosition,1)).xyz;

    vec3 eyePosition = vec4(ViewMat * ModelMat * vec4(VertexPosition, 1)).xyz;

    vec3 n = vec4(ViewMat * ModelMat * vec4(VertexNormal,0)).xyz;
    vec3 l = vec4(LightPosition, 0).xyz - eyePosition;

    float distance = length(LightPosition - vpws);

    n = normalize(n);
    l = normalize(l);

    float iDiffuse = clamp(dot(n, l), 0, 1);

//
    vec3 r = reflect(-l, n);
    vec3 e = normalize(-eyePosition);

    float iSpecular = pow(clamp(dot(e, r), 0, 1), MaterialShiness);

    color = MaterialAmbient +
            LightPower * MaterialDiffuse * iDiffuse / (distance * distance)+
            LightPower * MaterialSpecular * iSpecular / (distance * distance);


}
