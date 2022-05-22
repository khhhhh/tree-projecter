#version 330

uniform mat4 ModelMat;
uniform mat4 ProjViewMat;

layout (location=0) in vec3 VertexPosition;
layout (location=4) in vec2 VertexUV;

out vec3 eyePosition;
out float distance;
out float camDistance;
out vec2 uv;

void main(void)
{
    gl_Position = ProjViewMat * ModelMat * vec4(VertexPosition, 1);
    vec3 vpws = vec4(ModelMat * vec4(VertexPosition,1)).xyz;

    distance = length(vpws);
    camDistance = length(eyePosition);

    uv = VertexUV;
}
