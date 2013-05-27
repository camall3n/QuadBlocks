#version 150

in vec4 fragDiffuseColor;
//in vec3 fragNormal;
//in vec3 fragCameraPosition;

out vec4 outputColor;

//uniform vec3 lightPos;
//uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;
//uniform float lightAttenuation;

void main()
{
//    vec3 lightDifference = lightPos - fragCameraPosition;
//    float lightDistance = length(lightDifference);
//    vec3 dirToLight = normalize(lightDifference);
    
//    vec4 attenIntensity = lightIntensity * (1 / ( 1.0 + lightAttenuation * lightDistance));
    
//    float cosAngIncidence = dot(normalize(fragNormal), dirToLight);
//	cosAngIncidence = clamp(cosAngIncidence, 0, 1);

//	outputColor = (fragDiffuseColor * attenIntensity * cosAngIncidence) + (fragDiffuseColor * ambientIntensity);
    outputColor = fragDiffuseColor;// * ambientIntensity;
}