#pragma once
#include "..\Engine\CommonMathIncludes.h"
#include "..\Engine\Primitive\PrimitiveFunc.h"
#include "..\TexManager.h"
#include "..\Engine\Objects\3DObject.h"
#include "..\DrawStrings.h"

#include "ActionManager.h"
#include "GameCamera.h"

//----------------------------------------------------------------
// ���A�N�V�����ꗗ
//----------------------------------------------------------------
enum Reactions {
	Reac_None,			// ���A�N�V�����Ȃ�
	Reac_Damage,		// �_���[�W
	Reac_Death,			// ���S
	Reac_Death_End		// ���S�I����̍폜�ҋ@
};

class Map;

//-------------------------------------------------------------------------------------------------------------
// �L�����N�^�[���class
//-------------------------------------------------------------------------------------------------------------
class Charactor
{
#pragma region �G�C���A�X

public:
	using XMINT2 = DirectX::XMINT2;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

#pragma endregion

#pragma region �R���X�g���N�^�E�f�X�g���N�^

//----------------------------------------------------------------
// �R���X�g���N�^�E�f�X�g���N�^
//----------------------------------------------------------------

public:
	/// <summary>
	/// ���R���X�g���N�^
	/// </summary>
	Charactor();

	/// <summary>
	/// ���f�X�g���N�^
	/// </summary>
	~Charactor();

#pragma endregion

#pragma region ���f���E���\�[�X�f�[�^

//----------------------------------------------------------------
// ���f���E���\�[�X�f�[�^
//----------------------------------------------------------------

protected:
	// �L�����N�^�[���f���|�C���^
	Object3d* m_pModel = nullptr;

	// �L�����N�^�[�e�N�X�`��
	int m_Image[8] = { 0 };

#pragma endregion

#pragma region �ʒu�E�}�b�v�֘A

//----------------------------------------------------------------
// �ʒu�E�}�b�v�֘A
//----------------------------------------------------------------

protected:
	// �}�b�v�N���X�̃|�C���^
	Map* m_pMap = nullptr;

	// ���݂̃}�b�v���W
	XMINT2 m_MapPos = XMINT2();

public:
	/// <summary>
	/// �}�b�v���W���㏑��
	/// </summary>
	/// <param name="_MapPos"> �}�b�v���W </param>
	void SetMapPos(XMINT2 _MapPos) { m_MapPos = _MapPos; }

	/// <summary>
	/// ���݂̃}�b�v���W���擾
	/// </summary>
	/// <returns></returns>
	XMINT2 GetMapPos() { return m_MapPos; }

	/// <summary>
	/// ���݂̃}�b�v���W�̃��[���h���W���擾
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 GetWorldPos();

	/// <summary>
	/// ���f���̌��݈ʒu���擾
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 GetModelPos() { return m_pModel->position; }

#pragma endregion

#pragma region �����֘A

//----------------------------------------------------------------
// �����֘A
//----------------------------------------------------------------

protected:
	/*------------------------------------------------
						BACK
			BACKRINGHT			BACKLEFT
	RINGHT									LEFT
			FRONTRINGHT			FRONTLEFT
						FRONT
	------------------------------------------------*/
	enum class Chara_Dir {
		FRONT,
		FRONTRINGHT,
		RINGHT,
		BACKRINGHT,
		BACK,
		BACKLEFT,
		LEFT,
		FRONTLEFT
	};

	// ���݌����Ă������
	Chara_Dir m_NowDir = Chara_Dir::FRONT;

	// �J�������猩���L�����̕���
	Chara_Dir m_DirFromCamera = Chara_Dir::FRONT;

public:
	/// <summary>
	/// ������ύX����
	/// </summary>
	/// <param name="_Dir"> ���� </param>
	void SetDir(Chara_Dir _Dir) { m_NowDir = _Dir; }

	/// <summary>
	/// �J�������猩���������X�V����
	/// </summary>
	virtual void SetDirFromCamera();

#pragma endregion

#pragma region �X�e�[�^�X�֘A

//----------------------------------------------------------------
// �X�e�[�^�X�֘A
//----------------------------------------------------------------

protected:
	// ����HP
	int m_HP = 1;
	// �ő�HP
	int m_MaxHP = 1;
	// �h���
	int m_Def = 0;
	// �X�^�~�i�H
	int m_Stamina = 5;
	// ���݂̏�Ԉُ�
	Abnormality m_Abn = Abnormality::None;

public:
	/// <summary>
	/// ���݂�HP���擾
	/// </summary>
	/// <returns></returns>
	int GetHP() { return m_HP; }

	/// <summary>
	/// ���݂̍ő�HP���擾
	/// </summary>
	/// <returns></returns>
	int GetMaxHP() { return m_MaxHP; }

	/// <summary>
	/// �h��͂��擾
	/// </summary>
	/// <returns></returns>
	int GetDef() { return m_Def; }

	/// <summary>
	/// ���݂̃X�^�~�i���擾
	/// </summary>
	/// <returns></returns>
	int GetStamina() { return m_Stamina; }

	/// <summary>
	/// ���݂̏�Ԉُ���擾
	/// </summary>
	/// <returns></returns>
	Abnormality GetAbn() { return m_Abn; }

	/// <summary>
	/// ��Ԉُ���Z�b�g
	/// </summary>
	/// <param name="_Abn">��Ԉُ�</param>
	void SetAbn(Abnormality _Abn) { m_Abn = _Abn; }

	/// <summary>
	/// _Valuw���̃_���[�W���󂯂�
	/// </summary>
	/// <param name="_Value">�_���[�W��</param>
	void Damage(int _Value) { m_HP -= _Value; };

	/// <summary>
	/// _Valuw���̃_���[�W���󂯂�
	/// </summary>
	/// <param name="_Value">�_���[�W��</param>
	void DamageShake(int _Value) { 
		m_HP -= _Value;
		GameCamera::Instance()->Shake(5, 14);
	};

	/// <summary>
	/// ���ݐ������Ă��邩
	/// </summary>
	/// <returns> true : ����   false : ���S</returns>
	bool isAlive() { return m_HP > 0 ? true : false; }

	/// <summary>
	/// �X�^�~�i���g�p����B�X�^�~�i������Ȃ��ꍇ�͎g�p���� false ��Ԃ�
	/// </summary>
	/// <param name="_Val">�g�p�����</param>
	/// <returns> true : �X�^�~�i������Ă���   false : �X�^�~�i������Ă��Ȃ�</returns>
	bool UseStamina(int _Val) {
		if (m_Stamina < _Val) {
			return false;
		}
		m_Stamina -= _Val;
		return true;
	}

#pragma endregion

#pragma region �҂����Ԋ֘A

//----------------------------------------------------------------
// �҂����Ԋ֘A
//----------------------------------------------------------------

protected:
	// �ړ����̃J�����҂�����
	static const int WAIT_TIMER_VALUE_MOVE = 5;

	// �J�����̃^�[�Q�b�g�������ɕύX���鎞�̃J�����҂�����
	static const int WAIT_TIMER_VALUE = 20;

	// �ҋ@�t���[����( 30 �Ȃ� 30 -= 1 ���Ă����A0�ɂȂ�܂őҋ@ )
	int m_WaitTimer = 0;

public:
	/// <summary>
	/// �҂����Ԃ��Z�b�g
	/// </summary>
	/// <param name="_Value">�ҋ@�t���[����</param>
	void Wait(int _Value) { m_WaitTimer = _Value; }

	/// <summary>
	/// �ҋ@���ԍX�V
	/// </summary>
	void WaitUpdate();

	/// <summary>
	/// ���ݑҋ@�������擾
	/// </summary>
	/// <returns> true : �ҋ@��  false : �ҋ@���ł͂Ȃ� </returns>
	bool isWait();

#pragma endregion

#pragma region ���A�N�V�����֘A

//----------------------------------------------------------------
// ���A�N�V�����֘A
//----------------------------------------------------------------

protected:
	// ���݂̃��A�N�V�������
	Reactions m_NowReaction = Reactions::Reac_None;
private:
	// �_���[�W���A�N�V�����œ�������
	static const float DAMAGE_REACTION_OFFSET_RANGE;

	// �_���[�W���A�N�V�����̑��t���[����
	static const int DAMAGE_REACTION_TIME;

	// [�ꎞ�ۑ��p]�@���ݎ��s���̃��A�N�V�����̑��t���[����
	int m_ReactionTimeMax = 0;

	// [�ꎞ�ۑ��p]�@���A�N�V�������������̍��W
	XMINT2 m_ReactionMakeDir = XMINT2();

	// [�ꎞ�ۑ��p]�@���A�N�V�������̕���
	XMINT2 m_ReactionDoDir = XMINT2();

	/// <summary>
	/// ���������Ƃ��鑤�̍��W���g���ă��A�N�V�����̕������擾
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 OffsetDirGet();

	/// <summary>
	/// �J�����̍��W���g���ă��A�N�V�����̕������擾
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 CameraOffsetDirGet();

protected:
	// [�ꎞ�ۑ��p] ���A�N�V�����̕���
	XMFLOAT3 m_ReactionOffset = XMFLOAT3();

	/// <summary>
	/// ���A�N�V������Ԃ̍X�V
	/// </summary>
	void ReactionUpdate();

private:
	/// <summary>
	/// �_���[�W���A�N�V�����X�V
	/// </summary>
	void DamageReactionUpdate();

	/// <summary>
	/// ���S���A�N�V�����X�V
	/// </summary>
	void DeathReactionUpdate();

public:
	/// <summary>
	/// �_���[�W���A�N�V������������
	/// </summary>
	/// <param name="_ReactionMakeDir">�U�����鑤�̃}�b�v���W</param>
	/// <param name="_ReactionDoDir">�U������鑤�̃}�b�v���W</param>
	void DamageReaction(XMINT2 _ReactionMakeDir, XMINT2 _ReactionDoDir);

	/// <summary>
	/// ���݂̃��A�N�V������Ԃ��擾
	/// </summary>
	/// <returns></returns>
	Reactions GetReaction() { return m_NowReaction; }

#pragma endregion

#pragma region ���̑��֐���

public:
	/// <summary>
	/// ���z�X�V�֐�
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �J�����̃^�[�Q�b�g�������ɕύX����
	/// </summary>
	/// <param name="isWait"> �J�����ړ������܂ő҂� </param>
	/// /// <param name="WaitTime"> �҂����� </param>
	void CameraTargetOnMe(bool _isWait, int _WaitTime = WAIT_TIMER_VALUE);

#pragma endregion

#pragma region �`��֐�

	/// <summary>
	/// �ʏ�`�扼�z�֐�
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// �V���h�E�}�b�v�p���C�g���_�`��
	/// </summary>
	void ShadowDraw();

	/// <summary>
	/// �[�x�`��
	/// </summary>
	void DepthDraw();

	/// <summary>
	/// DOF�p�[�x�`��
	/// </summary>
	void DOFDepthDraw();

#pragma endregion
};

