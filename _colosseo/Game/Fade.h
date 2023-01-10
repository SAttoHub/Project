#pragma once
#include "../Singleton.h"
#include "../Engine/CommonMathIncludes.h"

class Fade : public Singleton<Fade>
{
	friend class Singleton<Fade>;
private:
	using XMINT2 = DirectX::XMINT2;
	using XMINT4 = DirectX::XMINT4;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

	int NormalFade;
public:
	Fade();
	~Fade();
	void LoadFadeResouces();
	void DrawNormalFade(int Flame, int MaxFlame);
	void DrawNormalFadeIn(int Flame, int MaxFlame);
};

