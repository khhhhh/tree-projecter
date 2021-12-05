#version 330

uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ProjMat;
uniform vec3 LightPosition;

layout (location=0) in vec3 VertexPosition;
layout (location=2) in vec3 VertexColor;
layout (location=3) in vec3 VertexNormal;
layout (location=4) in vec2 VertexUV;

out vec3 eyePosition;
out vec3 normalVector;
out vec3 lightVector;
out float distance;
out float camDistance;
out vec2 uv;

void main(void)
{
    gl_Position = ProjMat * ViewMat * ModelMat * vec4(VertexPosition, 1);
    vec3 vpws = vec4(ModelMat * vec4(VertexPosition,1)).xyz;

    eyePosition = vec4(ViewMat * ModelMat * vec4(VertexPosition, 1)).xyz;

    normalVector = vec4(ViewMat * ModelMat * vec4(VertexNormal,0)).xyz;
    lightVector = vec4(ViewMat * vec4(LightPosition, 1)).xyz - eyePosition;

    distance = length(LightPosition - vpws);
    camDistance = length(eyePosition);

    uv = VertexUV;
}
