#include "ActionManager.h"
#include "Enemys.h"
#include "Player.h"

Enemys* ActionManager::pEnemys = nullptr;
Player* ActionManager::pPlayer = nullptr;

void ActionManager::Init(Enemys *pEn, Player *pPl)
{
    pEnemys = pEn;
    pPlayer = pPl;
}

std::vector<AllResult> ActionManager::Action(XMINT2 Pos, int Damage, KnockBack KnB, Abnormality Abn, int AbnTurn)
{
    // ���ʂ܂Ƃ�
    std::vector<AllResult> Results;

    // �U���������Enemy�����݂��邩
    Enemy* MainEn = pEnemys->GetEnemy(Pos);
    // ���݂��Ȃ������Ƃ�
    if (MainEn == nullptr) {
        return Results;
    }
    /*--- ���݂����ꍇ ---*/

    //----- �U�����󂯂�Enemy�ւ̉e��
    AllResult MainEnemy;
    // Enemy�̃C���f�b�N�X
    MainEnemy.Index = pEnemys->GetEnemyIndex(Pos);
    // Enemy���󂯂�_���[�W
    MainEnemy.Result.m_Damage = Damage - MainEn->GetDef();
    // Enemy�̃m�b�N�o�b�N
    MainEnemy.Result.m_KnB = KnB;
    // Enemy�ɗ^�����Ԉُ�
    MainEnemy.Result.m_Abn = Abn;
    MainEnemy.Result.m_AbnTurn = AbnTurn;
    // ���U���g�Ɍ��ʂ�ǉ�
    Results.emplace_back(MainEnemy);

    // �m�b�N�o�b�N�̕���
    XMINT2 Knock = XMINT2(KnB.LEFT - KnB.RIGHT, KnB.UP - KnB.DOWN);
    // �m�b�N�o�b�N��܂ł̔���
    for (int i = 0; i < KnB.Power; i++) {
        XMINT2 ControlPos = Pos + Knock * i;
        // player�ɓ�������
        if (pPlayer->GetMapPos() == ControlPos) {
            AllResult HitEnemy;
            // player�̃C���f�b�N�X
            HitEnemy.Index = short(65535);
            // player�̃_���[�W
            HitEnemy.Result.m_Damage = Damage - pPlayer->GetDef();
            if (HitEnemy.Result.m_Damage < 0)  HitEnemy.Result.m_Damage = 0;
            // player�̓m�b�N�o�b�N���Ȃ�
            HitEnemy.Result.m_KnB = KnockBack();
            // player�ɂ͏�Ԉُ��^���Ȃ�
            HitEnemy.Result.m_Abn = Abnormality::None;
            HitEnemy.Result.m_AbnTurn = 0;
            // ���U���g�Ɍ��ʂ�ǉ�
            Results.emplace_back(HitEnemy);

            // �U�����󂯂��G�l�~�[�̍ŏI�I�ȃm�b�N�o�b�N��
            Results[0].Result.m_KnB.Power = i - 1;
            break;
        }
        // �Ԃ������炻�̏�Ńm�b�N�o�b�N�����߂�
        Enemy* en = pEnemys->GetEnemy(ControlPos);
        if (en != nullptr) {
            AllResult HitEnemy;
            // ��������Enemy�̃C���f�b�N�X
            HitEnemy.Index = short(pEnemys->GetEnemyIndex(ControlPos));
            // ��������Enemy�̃_���[�W
            HitEnemy.Result.m_Damage = Damage - en->GetDef();
            if (HitEnemy.Result.m_Damage < 0)  HitEnemy.Result.m_Damage = 0;
            // ���������G�l�~�[�̓m�b�N�o�b�N���Ȃ�
            HitEnemy.Result.m_KnB = KnockBack();
            // ���������G�l�~�[�ɂ͏�Ԉُ��^���Ȃ�
            HitEnemy.Result.m_Abn = Abnormality::None;
            HitEnemy.Result.m_AbnTurn = 0;
            // ���U���g�Ɍ��ʂ�ǉ�
            Results.emplace_back(HitEnemy);

            // �U�����󂯂��G�l�~�[�̍ŏI�I�ȃm�b�N�o�b�N��
            Results[0].Result.m_KnB.Power = i - 1;
            break;
        }
    }

    return Results;
}
