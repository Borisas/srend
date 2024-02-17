#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec3 color;

void main()
{    
    vec2 tc = vec2(TexCoord[0], 1-TexCoord[1]);//flip
    // vec2 tc = vec2(TexCoord[0], TexCoord[1]);
    float tAlpha = texture(ourTexture,tc).r;
    FragColor = vec4(color,tAlpha);
}