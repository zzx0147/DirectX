
cbuffer MatrixBuffer
{ 
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{ 
	float4 position : POSITION;
	float4 color : COLOR; 
};

struct PixelInputType 
{ 
	float4 position : SV_POSITION; 
	float4 color : COLOR; 
};

PixelInputType ColorVertexShader(VertexInputType input)
{ 
	PixelInputType output; // �ùٸ��� ��� ������ �ϱ� ���Ͽ� position ���͸� w���� �ִ� 4������ �ִ� ������ ����մϴ�.
	input.position.w = 1.0f; // ������ ��ġ�� ����, ��, �翵�� ������ ����մϴ�.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix); // �ȼ� ���̴����� ����ϱ� ���� �Է� ������ �����մϴ�.
	output.color = input.color; 
	return output; 
	}
