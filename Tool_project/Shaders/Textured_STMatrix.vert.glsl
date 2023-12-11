attribute vec3 kzPosition;
attribute vec2 kzTextureCoordinate0;
uniform highp mat4 kzProjectionCameraWorldMatrix;

varying mediump vec2 vTexCoord;
uniform mediump vec2 Scale;
uniform mediump vec2 Offset;
void main()
{
    precision mediump float;
    
    vTexCoord = kzTextureCoordinate0*Scale + Offset;
    gl_Position = kzProjectionCameraWorldMatrix * vec4(kzPosition.xyz, 1.0);
}