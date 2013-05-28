#version 150

in vec3 fragClipPosition;
in vec3 fragNormal;
in vec4 fragDiffuseColor;

in vec3 fragClipLightPosition;
//in vec4 fragLightIntensity;
//in vec4 fragAmbientIntensity;
//in float fragLightAttenuation;

out vec4 outputColor;

void main()
{
    vec3 lightDifference = fragClipLightPosition - fragClipPosition;
    float lightDistance = length(lightDifference);
    vec3 dirToLight = normalize(lightDifference);
    
//    vec4 attenIntensity = fragLightIntensity * (1 / ( 1.0 + fragLightAttenuation * lightDistance));
    
    float cosAngIncidence = clamp( dot(normalize(fragNormal), dirToLight), 0, 1 );
//	cosAngIncidence = cosAngIncidence, 0, 1);
    
    float alpha = fragDiffuseColor.w;
    vec4 highlightColor = mix( fragDiffuseColor, vec4(1,1,1,alpha), 0.5);
    vec4 shadowColor = mix( fragDiffuseColor, vec4(0,0,0,alpha), 0.5);
//    vec4 shadowColor = mix( fragDiffuseColor, fragAmbientIntensity, 0.5);

    outputColor = mix(shadowColor, highlightColor, cosAngIncidence);
//	outputColor = (fragDiffuseColor * attenIntensity * cosAngIncidence) + (fragDiffuseColor * fragAmbientIntensity);
//    outputColor = fragDiffuseColor;
}