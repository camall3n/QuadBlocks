#version 150

in vec3 fragClipPosition;
in vec3 fragNormal;
in vec4 fragDiffuseColor;

in vec3 fragClipLightPosition;
in vec3 fragLightColor;
in vec3 fragDarkColor;
//in float fragLightAttenuation;

out vec4 outputColor;

void main()
{
    vec3 lightDifference = fragClipLightPosition - fragClipPosition;
    float lightDistance = length(lightDifference);
    vec3 dirToLight = normalize(lightDifference);
    
//    vec4 attenIntensity = fragLightColor * (1 / ( 1.0 + fragLightAttenuation * lightDistance));
    
    float cosAngIncidence = clamp( dot(normalize(fragNormal), dirToLight), 0, 1 );
//	cosAngIncidence = cosAngIncidence, 0, 1);
    
    float alpha = fragDiffuseColor.a;
    vec3 highlightColor = mix( fragDiffuseColor.xyz, fragLightColor, 0.5);
//    vec4 highlightColor = mix( fragDiffuseColor, vec4(1,1,1,alpha), 0.5);

    vec3 shadowColor = mix( fragDiffuseColor.xyz, fragDarkColor, 0.5);
//    vec4 shadowColor = mix( fragDiffuseColor, vec4(0,0,0,alpha), 0.5);

    vec4 mixedColor = vec4(mix(shadowColor, highlightColor, cosAngIncidence), alpha);
//    vec4 mixedColor = mix(shadowColor, highlightColor, cosAngIncidence);
    
    outputColor = mixedColor;
//    outputColor = vec4(fragDarkColor, 1.0);
//    outputColor = fragDiffuseColor;
    

//    outputColor = mix(shadowColor, highlightColor, cosAngIncidence);
    
//	outputColor = (fragDiffuseColor * attenIntensity * cosAngIncidence) + (fragDiffuseColor * fragAmbientIntensity);
//    outputColor = fragDiffuseColor;
}