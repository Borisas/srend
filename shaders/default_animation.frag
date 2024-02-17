#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec3 color;
uniform vec4 frame;

void main()
{

    // vec4 sampled = vec4(1.0, 1.0, 1.0, texture(ourTexture, TexCoord).r);
    // FragColor = vec4(color, 1.0) * sampled;

    vec2 finalUv = vec2(
        frame.x + (frame.z - frame.x) * TexCoord.x,
        frame.y + (frame.w - frame.y) * TexCoord.y
    );

    vec4 c = texture(ourTexture, finalUv);
    FragColor = c;
}