#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat3 transform;
uniform mat3 projection;

out vec3 ourColor;
out vec2 TexCoord;

void main() {
    
    vec3 fPos = vec3(aPos.x,aPos.y,1);

    mat3 mvp = transform * projection;

    fPos = fPos * mvp;

    gl_Position = vec4(fPos.x,fPos.y,fPos.z,1);
    ourColor = aColor;
    TexCoord = aTexCoord;
}