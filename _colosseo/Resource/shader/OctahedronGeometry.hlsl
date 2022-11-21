#include "Octahedron.hlsli"

//’¸“_
static const float3 vertexs[24] =
{
float3(0.0, 0.0, -0.5),
float3(0.0, -0.5, 0.0),
float3(-0.5, 0.0, 0.0),
float3(0.0, 0.0, -0.5),
float3(0.5, 0.0, 0.0),
float3(0.0, -0.5, 0.0),
float3(0.5, 0.0, 0.0),
float3(0.0, 0.0, 0.5),
float3(0.0, -0.5, 0.0),
float3(0.0, 0.0, 0.5),
float3(-0.5, 0.0, 0.0),
float3(0.0, -0.5, 0.0),
float3(0.0, 0.0, -0.5),
float3(-0.5, 0.0, 0.0),
float3(0.0, 0.5, 0.0),
float3(0.0, 0.0, -0.5),
float3(0.0, 0.5, 0.0),
float3(0.5, 0.0, 0.0),
float3(0.5, 0.0, 0.0),
float3(0.0, 0.5, 0.0),
float3(0.0, 0.0, 0.5),
float3(0.0, 0.0, 0.5),
float3(0.0, 0.5, 0.0),
float3(-0.5, 0.0, 0.0)
};

//–@ü
static const float3 normals[8] =
{
float3(-0.57735,-0.57735,-0.57735),
float3(0.57735,-0.57735,-0.57735),
float3(0.57735,-0.57735,0.57735),
float3(-0.57735,-0.57735,0.57735),
float3(-0.57735,0.57735,-0.57735),
float3(0.57735,0.57735,-0.57735),
float3(0.57735,0.57735,0.57735),
float3(-0.57735,0.57735,0.57735),
};

[maxvertexcount(24)]
void main(
    point VSOutput input[1],
    inout TriangleStream< GSOutput > output
)
{
    GSOutput element;
   const float Rad = input[0].Radius * 2;

   int j = 0;
   int k = 0;
    [unroll]
    for (j = 0; j < 8; j++) 
    {
        [unroll]
        for (k = 0; k < 3; k++)
        {
            const int vid = j * 3 + k;
            element.svpos = float4(vertexs[vid].x * Rad, vertexs[vid].y * Rad, vertexs[vid].z * Rad, 0.0);
           // element.svpos = float4(vertexs[vid], 1.0);
            element.svpos = element.svpos + input[0].pos;
            element.normal = normals[j];

            element.color = input[0].color;
            element.svpos = mul(mat, element.svpos);
            element.Lighting = input[0].Lighting;

            output.Append(element);
        }
        output.RestartStrip();
    }
}