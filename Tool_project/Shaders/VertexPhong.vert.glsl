#version 100

#if KANZI_SHADER_MORPH_TARGET_COUNT <= 1
attribute vec3 kzPosition;
attribute vec3 kzNormal;
#endif

uniform highp mat4 kzProjectionCameraWorldMatrix;
uniform highp mat4 kzWorldMatrix;
uniform highp mat4 kzNormalMatrix;
uniform highp vec4 kzViewPosition;

#if KANZI_SHADER_MORPH_TARGET_COUNT > 1
attribute vec3 kzMorphTarget0Position;
attribute vec3 kzMorphTarget1Position;
#if KANZI_SHADER_MORPH_TARGET_COUNT > 2
attribute vec3 kzMorphTarget2Position;
#if KANZI_SHADER_MORPH_TARGET_COUNT > 3
attribute vec3 kzMorphTarget3Position;
#if KANZI_SHADER_MORPH_TARGET_COUNT > 4
attribute vec3 kzMorphTarget4Position;
#if KANZI_SHADER_MORPH_TARGET_COUNT > 5
attribute vec3 kzMorphTarget5Position;
#if KANZI_SHADER_MORPH_TARGET_COUNT > 6
attribute vec3 kzMorphTarget6Position;
#if KANZI_SHADER_MORPH_TARGET_COUNT > 7
attribute vec3 kzMorphTarget7Position;
#if KANZI_SHADER_MORPH_TARGET_COUNT > 8
attribute vec3 kzMorphTarget8Position;
#endif
#endif
#endif
#endif
#endif
#endif
#endif

attribute vec3 kzMorphTarget0Normal;
#if KANZI_SHADER_USE_MORPH_TARGET_NORMALS
attribute vec3 kzMorphTarget1Normal;
#if KANZI_SHADER_MORPH_TARGET_COUNT > 2
attribute vec3 kzMorphTarget2Normal;
#if KANZI_SHADER_MORPH_TARGET_COUNT > 3
attribute vec3 kzMorphTarget3Normal;
#if KANZI_SHADER_MORPH_TARGET_COUNT > 4
attribute vec3 kzMorphTarget4Normal;
#if KANZI_SHADER_MORPH_TARGET_COUNT > 5
attribute vec3 kzMorphTarget5Normal;
#if KANZI_SHADER_MORPH_TARGET_COUNT > 6
attribute vec3 kzMorphTarget6Normal;
#if KANZI_SHADER_MORPH_TARGET_COUNT > 7
attribute vec3 kzMorphTarget7Normal;
#if KANZI_SHADER_MORPH_TARGET_COUNT > 8
attribute vec3 kzMorphTarget8Normal;
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif // KANZI_SHADER_USE_MORPH_TARGET_NORMALS

uniform mediump float kzMorphWeights[KANZI_SHADER_MORPH_TARGET_COUNT];
#endif // KANZI_SHADER_MORPH_TARGET_COUNT

#if KANZI_SHADER_USE_BASECOLOR_TEXTURE
attribute vec2 kzTextureCoordinate0;
varying mediump vec2 vTexCoord;
uniform mediump vec2 TextureOffset;
uniform mediump vec2 TextureTiling;
#endif

#if KANZI_SHADER_SKINNING_BONE_COUNT 
attribute vec4 kzWeight;
attribute vec4 kzMatrixIndices;
uniform highp vec4 kzMatrixPalette[KANZI_SHADER_SKINNING_BONE_COUNT*3];
#endif

#if KANZI_SHADER_NUM_DIRECTIONAL_LIGHTS
uniform mediump vec3 DirectionalLightDirection[KANZI_SHADER_NUM_DIRECTIONAL_LIGHTS];
uniform lowp    vec4 DirectionalLightColor     [KANZI_SHADER_NUM_DIRECTIONAL_LIGHTS];
#endif

#if KANZI_SHADER_NUM_POINT_LIGHTS
uniform lowp vec4 PointLightColor[KANZI_SHADER_NUM_POINT_LIGHTS];
uniform mediump vec3 PointLightAttenuation[KANZI_SHADER_NUM_POINT_LIGHTS];
uniform mediump vec3 PointLightPosition[KANZI_SHADER_NUM_POINT_LIGHTS];
#endif

#if KANZI_SHADER_NUM_SPOT_LIGHTS
uniform mediump vec3  SpotLightPosition[KANZI_SHADER_NUM_SPOT_LIGHTS];
uniform mediump vec4  SpotLightColor         [KANZI_SHADER_NUM_SPOT_LIGHTS];
uniform mediump vec3  SpotLightDirection     [KANZI_SHADER_NUM_SPOT_LIGHTS];
uniform mediump vec3  SpotLightConeParameters[KANZI_SHADER_NUM_SPOT_LIGHTS];
uniform mediump vec3  SpotLightAttenuation   [KANZI_SHADER_NUM_SPOT_LIGHTS];
#endif

uniform lowp float BlendIntensity;
uniform lowp vec4 Emissive;
uniform lowp vec4 Ambient;
uniform lowp vec4 Diffuse;
uniform lowp vec4 SpecularColor;
uniform mediump float SpecularExponent;

#if KANZI_SHADER_NUM_SPOT_LIGHTS || KANZI_SHADER_NUM_POINT_LIGHTS || KANZI_SHADER_NUM_DIRECTIONAL_LIGHTS
varying lowp vec3 vAmbDif;
varying lowp vec3 vSpec;
#endif

#if KANZI_SHADER_USE_REFLECTION_CUBE
varying mediump vec3 vViewDirection;
varying lowp vec3 vNormal;
#endif

struct PositionData
{
    vec3 position;
    vec3 normal;
};

#if KANZI_SHADER_MORPH_TARGET_COUNT > 1
PositionData calculateMorphTargetPositionData()
{
    PositionData posData;

    float baseMorphWeight = 1.0 - kzMorphWeights[1]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 2
                     - kzMorphWeights[2]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 3
                     - kzMorphWeights[3]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 4
                     - kzMorphWeights[4]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 5
                     - kzMorphWeights[5]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 6
                     - kzMorphWeights[6]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 7
                     - kzMorphWeights[7]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 8
                     - kzMorphWeights[8]
#endif
#endif
#endif
#endif
#endif
#endif
#endif
    ;
    
    posData.position = kzMorphTarget0Position * baseMorphWeight
                     + kzMorphTarget1Position * kzMorphWeights[1]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 2
                     + kzMorphTarget2Position * kzMorphWeights[2]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 3
                     + kzMorphTarget3Position * kzMorphWeights[3]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 4
                     + kzMorphTarget4Position * kzMorphWeights[4]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 5
                     + kzMorphTarget5Position * kzMorphWeights[5]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 6
                     + kzMorphTarget6Position * kzMorphWeights[6]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 7
                     + kzMorphTarget7Position * kzMorphWeights[7]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 8
                     + kzMorphTarget8Position * kzMorphWeights[8]
#endif
#endif
#endif
#endif
#endif
#endif
#endif
    ;

    posData.normal = kzMorphTarget0Normal
#if KANZI_SHADER_USE_MORPH_TARGET_NORMALS
                   * baseMorphWeight
                   + kzMorphTarget1Normal * kzMorphWeights[1]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 2
                   + kzMorphTarget2Normal * kzMorphWeights[2]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 3
                   + kzMorphTarget3Normal * kzMorphWeights[3]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 4
                   + kzMorphTarget4Normal * kzMorphWeights[4]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 5
                   + kzMorphTarget5Normal * kzMorphWeights[5]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 6
                   + kzMorphTarget6Normal * kzMorphWeights[6]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 7
                   + kzMorphTarget7Normal * kzMorphWeights[7]
#if KANZI_SHADER_MORPH_TARGET_COUNT > 8
                   + kzMorphTarget8Normal * kzMorphWeights[8]
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif // KANZI_SHADER_USE_MORPH_TARGET_NORMALS
    ;

    return posData;
}
#endif // KANZI_SHADER_MORPH_TARGET_COUNT

#if KANZI_SHADER_MORPH_TARGET_COUNT <= 1
PositionData calculateSimplePositionData()
{
    PositionData posData;
    posData.position = kzPosition.xyz;
    posData.normal = kzNormal.xyz;
    return posData;
}
#endif

#if KANZI_SHADER_SKINNING_BONE_COUNT
PositionData calculateSkinningPositionData(PositionData posData)
{
    mat4 localToSkinMatrix;
    int i1 = 3 * int(kzMatrixIndices.x);
    int i2 = 3 * int(kzMatrixIndices.y);
    int i3 = 3 * int(kzMatrixIndices.z);
    int i4 = 3 * int(kzMatrixIndices.w);
    vec4 b1 = kzWeight.x * kzMatrixPalette[i1]
            + kzWeight.y * kzMatrixPalette[i2]
            + kzWeight.z * kzMatrixPalette[i3]
            + kzWeight.w * kzMatrixPalette[i4];
    vec4 b2 = kzWeight.x * kzMatrixPalette[i1 + 1]
            + kzWeight.y * kzMatrixPalette[i2 + 1]
            + kzWeight.z * kzMatrixPalette[i3 + 1]
            + kzWeight.w * kzMatrixPalette[i4 + 1];
    vec4 b3 = kzWeight.x * kzMatrixPalette[i1 + 2]
            + kzWeight.y * kzMatrixPalette[i2 + 2]
            + kzWeight.z * kzMatrixPalette[i3 + 2]
            + kzWeight.w * kzMatrixPalette[i4 + 2];
   
    localToSkinMatrix[0] = vec4(b1.xyz, 0.0);
    localToSkinMatrix[1] = vec4(b2.xyz, 0.0);
    localToSkinMatrix[2] = vec4(b3.xyz, 0.0);
    localToSkinMatrix[3] = vec4(b1.w, b2.w, b3.w, 1.0);
    mat4 localToWorldMatrix = kzWorldMatrix * localToSkinMatrix;
      
    posData.position = (localToSkinMatrix * vec4(posData.position.xyz, 1.0)).xyz;
    posData.normal = mat3(localToSkinMatrix) * posData.normal;
    
    return posData;
}
#endif

void main()
{
    precision mediump float;
    
#if KANZI_SHADER_MORPH_TARGET_COUNT > 1
    PositionData posData = calculateMorphTargetPositionData();
#else
    PositionData posData = calculateSimplePositionData();
#endif

#if KANZI_SHADER_SKINNING_BONE_COUNT
    posData = calculateSkinningPositionData(posData);
#endif

    posData.normal = normalize(posData.normal);
    vec4 positionWorld = kzWorldMatrix * vec4(posData.position, 1.0);
    
    mediump vec3 viewDirection = positionWorld.xyz * kzViewPosition.w - kzViewPosition.xyz;
    lowp vec3 normal = normalize(mat3(kzNormalMatrix) * posData.normal);
    gl_Position = kzProjectionCameraWorldMatrix * vec4(posData.position, 1.0);
    
    vec3 V = normalize(viewDirection);
    
#if KANZI_SHADER_NUM_DIRECTIONAL_LIGHTS || KANZI_SHADER_NUM_POINT_LIGHTS || KANZI_SHADER_NUM_SPOT_LIGHTS
    vec3 L = vec3(1.0, 0.0, 0.0);
    vec3 H = vec3(1.0, 0.0, 0.0);
    float LdotN, NdotH;
    float specular;
    vec3 c;
    float d, attenuation;
    vAmbDif = Ambient.rgb;
    vSpec = vec3(0.0);    
    vec3 pointLightDirection;  
    vec3 spotLightDirection;
#endif   

#if KANZI_SHADER_NUM_DIRECTIONAL_LIGHTS
    for (int i = 0; i < KANZI_SHADER_NUM_DIRECTIONAL_LIGHTS; ++i)
    {
        if(length(DirectionalLightDirection[i])> 0.01)
        {
            L = normalize(-DirectionalLightDirection[i]);
        }
        H = normalize(-V + L);
        LdotN = max(0.0, dot(L, normal));
        NdotH = max(0.0, dot(normal, H));        
        specular = pow(NdotH, SpecularExponent);
        vAmbDif += (LdotN * Diffuse.rgb) * DirectionalLightColor[i].rgb;
        vSpec += SpecularColor.rgb * specular * DirectionalLightColor[i].rgb;        
    }
#endif
    
#if KANZI_SHADER_NUM_POINT_LIGHTS
    for (int i = 0; i < KANZI_SHADER_NUM_POINT_LIGHTS; ++i)
    {
        pointLightDirection = positionWorld.xyz - PointLightPosition[i];
        L = normalize(-pointLightDirection);
        H = normalize(-V + L);
        LdotN = max(0.0, dot(L, normal));
        NdotH = max(0.0, dot(normal, H));
        specular = pow(NdotH, SpecularExponent);
        c = PointLightAttenuation[i];
        d = length(pointLightDirection);
        attenuation = 1.0 / max(0.001, (c.x + c.y * d + c.z * d * d));        
        vAmbDif += (LdotN * Diffuse.rgb) * attenuation * PointLightColor[i].rgb;
        vSpec +=  SpecularColor.rgb * specular * attenuation * PointLightColor[i].rgb;
        
    }
#endif

#if KANZI_SHADER_NUM_SPOT_LIGHTS
    for (int i = 0; i < KANZI_SHADER_NUM_SPOT_LIGHTS; ++i)
    {
        spotLightDirection = positionWorld.xyz - SpotLightPosition[i];
        L = normalize(-spotLightDirection);
        LdotN = dot(L, normal);
        
        if(LdotN > 0.0)
        {
            float cosDirection = dot(L, -SpotLightDirection[i]);
            float cosOuter = SpotLightConeParameters[i].x;
            float t = cosDirection - cosOuter;
            if (t > 0.0)
            {
                vec3 H = normalize(-V + L);
                float NdotH = max(0.0, dot(normal, H));
                float specular = pow(NdotH, SpecularExponent);
                vec3  c = SpotLightAttenuation[i];
                float d = length(spotLightDirection);
                float denom = (0.01 + c.x + c.y * d + c.z * d * d) * SpotLightConeParameters[i].z;
                float attenuation = min(t / denom, 1.0);
                vAmbDif += (LdotN * Diffuse.rgb) * attenuation * SpotLightColor[i].rgb;
                vSpec += SpecularColor.rgb * specular * attenuation * SpotLightColor[i].rgb;
            }
        }        
    }    
#endif

#if KANZI_SHADER_NUM_SPOT_LIGHTS || KANZI_SHADER_NUM_POINT_LIGHTS || KANZI_SHADER_NUM_DIRECTIONAL_LIGHTS
    vSpec += Emissive.rgb;
#endif
    
#if KANZI_SHADER_USE_BASECOLOR_TEXTURE
    vTexCoord = kzTextureCoordinate0 * TextureTiling + TextureOffset;
#endif

#if KANZI_SHADER_USE_REFLECTION_CUBE
   vViewDirection = viewDirection;
   vNormal = normal;
#endif
}