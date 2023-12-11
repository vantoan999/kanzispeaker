uniform sampler2D Texture;
uniform lowp float BlendIntensity;
varying mediump vec2 vTexCoord;
uniform lowp vec4 TransparentColor;
uniform lowp float TransparentFactor;
void main()
{
    precision lowp float;

    vec4 color = texture2D(Texture, vTexCoord);
    gl_FragColor.rgb = color.rgb * BlendIntensity;
    gl_FragColor.a = BlendIntensity;
}
