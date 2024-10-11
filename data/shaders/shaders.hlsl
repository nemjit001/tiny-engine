struct VSInput
{
    [[vk::location(0)]] float3 position : POSITION_0;
    [[vk::location(1)]] float2 texCoord : TEXCOORD_0;
};

struct PSInput
{
    float4 svPosition                   : SV_POSITION;
    [[vk::location(0)]] float2 texCoord : TEXCOORD_0;
};

PSInput VSMain(VSInput input)
{
    float4 position = float4(input.position, 1.);
    
    PSInput result;
    result.svPosition = position;
    result.texCoord = input.texCoord;
    
    return result;
}

float4 PSMain(PSInput input) : SV_TARGET_0
{
    return float4(input.texCoord, 0., 1.);
}
