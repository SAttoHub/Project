#pragma once
#include "Framework.h"
#include "PostEffect.h"
#include "CommonTime.h"

#include "Gaussian.h"
#include "DepthOfField.h"
#include "Depth.h"
#include "Depth2.h"
#include "Bloom.h"
#include "ShadowMapLight.h"
#include "ShadowMapping.h"
#include "Vignette.h"
#include "OutLine.h"
//#include "FadeManger.h"

class Game : public Framework
{
private:
	// ���ԑ���
	std::unique_ptr<CommonTime> m_CommonTime = nullptr;
	// �K�E�X�ڂ����p
	std::unique_ptr<Gaussian> GaussianEffectX = nullptr;
	std::unique_ptr<Gaussian> GaussianEffectY = nullptr;
	std::unique_ptr<Gaussian> GaussianEffectX2 = nullptr;
	std::unique_ptr<Gaussian> GaussianEffectY2 = nullptr;
	std::unique_ptr<Gaussian> GaussianEffectX_b = nullptr;
	std::unique_ptr<Gaussian> GaussianEffectY_b = nullptr;
	std::unique_ptr<DepthOfField> DOF = nullptr;

	// �ŏI�`��p
	//std::unique_ptr<FadeBuffer> ResultScr = nullptr;

	bool DOFFlag = true;
	//�u���[��
	std::unique_ptr<Bloom> bloom = nullptr;
	bool BloomFlag = true;
	//�V���h�E
	std::unique_ptr<ShadowMapLight> Shadow_Map_Light = nullptr;
	std::unique_ptr<ShadowMapping> shadowMapping = nullptr;

	//�r�l�b�g
	std::unique_ptr<Vignette> vignette = nullptr;

	//�A�E�g���C��
	/*bool UseOutLine;
	OutLine *outLine = nullptr;*/
	enum PostEffectPerformance {
		PSf_High, //���t���[���S�čX�V����
		PSf_Normal //1�t�����Ƃɕ�����
	};
	bool HighPerformance = false;
	PostEffectPerformance PSf_Perf = PSf_Normal;
	int PSf_Counter = 0;

	float InterpSize = 380.0f;
	float Focus = 30.0f;
	float FocusSize = 20.0f;
	bool UseFlag = true;

	bool ShadowMapUse = true;

	float VignetteInfluence = 0.2f;
	bool UseVignette = true;
public:
	void Initialize() override;
	void Update();
	void Finalize() override;

	void Run(const int iCmdShow);

	GameManager *game = nullptr;

	bool PSf_isDraw(std::string RTName);
};

