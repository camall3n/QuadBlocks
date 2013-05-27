#version 150

in vec3 position;
//in vec3 normal;

out vec4 fragDiffuseColor;

out vec3 fragCameraPosition;
out vec4 fragLightIntensity;
out vec4 fragAmbientIntensity;
out float fragLightAttenuation;

uniform vec4 diffuseColor;
uniform mat4 modelToWorldMatrix;

layout(std140) uniform TransformUniforms
{
    mat4 worldToClipMatrix;
//    mat4 cameraToClipMatrix;
};

layout(std140) uniform LightingUniforms
{
    vec3 worldLightPosition;
    vec4 lightIntensity;
    vec4 ambientIntensity;
    float lightAttenuation;
};

void main(){
    vec4 worldVertexPosition = modelToWorldMatrix * vec4(position, 1.0);
//    vec4 cameraVertexPosition = worldToCameraMatrix * worldVertexPosition;
    gl_Position = worldToClipMatrix * worldVertexPosition;
    
//    vec3 normCamSpace = normalize(normalModelToCameraMatrix * normal);
    
//    fragCameraPosition = vec3(cameraVertexPosition);
//    fragNormal = normCamSpace;
    fragDiffuseColor = diffuseColor;
}

