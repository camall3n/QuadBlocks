#version 150

in vec2 texPosition;

uniform sampler2D uiTexture;

out vec4 outputColor;

void main()
{
    outputColor = texture(uiTexture, texPosition);
}