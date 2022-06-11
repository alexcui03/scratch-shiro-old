#version 400 core
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D aTexture;

void main() {
    FragColor = texture(aTexture, texCoord);
    //FragColor = vec4(texCoord, 1.0, 1.0);
}

