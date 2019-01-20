input float2 inPosition;
input float4 inColor;

output float4 outColor;

void main()
{
    oPosition = mul(float4(inPosition.xy, 0.0, 1.0), oViewProjection);
    outColor = inColor;
}
