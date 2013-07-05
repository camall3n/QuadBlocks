#version 150

in vec3 position;

//out vec4 gl_Position;// (built-in)
out vec2 texPosition;

void main(){
    gl_Position = vec4(position, 1);
    texPosition = position.xy;
}
