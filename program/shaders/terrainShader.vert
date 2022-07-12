#version 330

uniform mat4 ModelMat;
/*
uniform mat4 ViewMat;
uniform mat4 ProjMat;
*/
uniform mat4 ProjViewMat;

layout (location=0) in vec3 VertexPosition;
layout (location=2) in vec3 VertexColor;
layout (location=4) in vec2 VertexUV;

out vec3 eyePosition;
out vec3 color;
out float distance;
out float camDistance;
out vec2 uv;

void main(void)
{
    gl_Position = ProjViewMat * ModelMat * vec4(VertexPosition, 1);
    vec3 vpws = vec4(ModelMat * vec4(VertexPosition,1)).xyz;

    color = VertexColor;

    distance = length(vpws);
    camDistance = length(eyePosition);

    uv = VertexUV;
}
