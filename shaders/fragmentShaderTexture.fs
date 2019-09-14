#version 430 core
out vec4 FragColor;

in vec3 ourColor2;
in vec2 texCoord;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;
uniform float mixVariable;
void main() {
    FragColor = mix(texture(ourTexture, texCoord),texture(ourTexture2,vec2(1.0 - texCoord.x, texCoord.y)), vec4(texture(ourTexture2, texCoord)).a);
}
