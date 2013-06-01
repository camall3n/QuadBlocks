#version 150

in vec3 position;
in vec3 normal;
uniform vec4 diffuseColor;
uniform mat4 modelToWorldMatrix;


layout(std140) uniform TransformUniforms
{
    mat4 worldToClipMatrix;
};

layout(std140) uniform LightingUniforms
{
    vec3 worldLightPosition;
    vec3 lightColor;
    vec3 darkColor;
    float lightAttenuation;
};


//out vec4 gl_Position;// (built-in)
out vec3 fragClipPosition;
out vec3 fragNormal;
out vec4 fragDiffuseColor;

out vec3 fragClipLightPosition;
out vec3 fragLightColor;
out vec3 fragDarkColor;
//out float fragLightAttenuation;


void main(){
    vec4 worldVertexPosition = modelToWorldMatrix * vec4(position, 1.0);
    vec4 clipVertexPosition = worldToClipMatrix * worldVertexPosition;
    gl_Position = clipVertexPosition;
    fragClipPosition = clipVertexPosition.xyz;
    
    vec4 worldNormal = modelToWorldMatrix * vec4(normal, 0.0);
    vec4 clipNormal = worldToClipMatrix * worldNormal;
    fragNormal = normalize(clipNormal.xyz);
    
    fragDiffuseColor = diffuseColor;
    
    vec4 clipLightPosition = worldToClipMatrix * vec4(worldLightPosition, 1.0);
    fragClipLightPosition = clipLightPosition.xyz;
    fragLightColor = lightColor;
    fragDarkColor = darkColor;
//    fragLightAttenuation = lightAttenuation;
}

