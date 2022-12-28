#include "ActionManager.h"
#include "Enemys.h"
#include "Player.h"
#include "Map.h"

Enemys* ActionManager::pEnemys = nullptr;
Player* ActionManager::pPlayer = nullptr;
Map* ActionManager::pMap = nullptr;

void ActionManager::Init(Enemys *pEn, Player *pPl, Map* pM)
{
    pEnemys = pEn;
    pPlayer = pPl;
    pMap = pM;
}

std::vector<AllResult> ActionManager::Action(XMINT2 Pos, int Damage, KnockBack KnB, Abnormality Abn, int AbnTurn, ACT_UNIT_TYPE AUT_Type)
{
    // 結果まとめ
    std::vector<AllResult> Results;

    // プレイヤーの行動なら
    if (AUT_Type == ACT_UNIT_TYPE::AUT_PL) {
        // 攻撃した先にEnemyが存在するか
        Enemy* MainEn = pEnemys->GetEnemy(Pos);
        // 存在しなかったとき
        if (MainEn == nullptr) {
            return Results;
        }
        /*--- 存在した場合 ---*/

        //----- 攻撃を受けたEnemyへの影響
        AllResult MainEnemy;
        // Enemyのインデックス
        MainEnemy.Index = pEnemys->GetEnemyIndex(Pos);
        // Enemyが受けるダメージ
        MainEnemy.Result.m_Damage = Damage - MainEn->GetDef();
        // Enemyのノックバック
        MainEnemy.Result.m_KnB = KnB;
        // Enemyに与える状態異常
        MainEnemy.Result.m_Abn = Abn;
        MainEnemy.Result.m_AbnTurn = AbnTurn;
        // リザルトに結果を追加
        Results.emplace_back(MainEnemy);

        // ノックバックの方向
        XMINT2 Knock = XMINT2(KnB.LEFT - KnB.RIGHT, KnB.UP - KnB.DOWN);
        // ノックバック先までの判定
        for (int i = 0; i < KnB.Power; i++) {
            XMINT2 ControlPos = Pos + Knock * i;

            // ぶつかったらその場でノックバックを辞める

            // 壁かマップ端に当たった
            if (pMap->InMap(ControlPos) == false || pMap->IsImmovableChip(ControlPos)) {
                // 攻撃を受けたエネミーの最終的なノックバック量
                Results[0].Result.m_KnB.Power = i - 1;
                break;
            }

            // playerに当たった
            if (pPlayer->GetMapPos() == ControlPos) {
                AllResult HitEnemy;
                // playerのインデックス
                HitEnemy.Index = short(65535);
                // playerのダメージ
                HitEnemy.Result.m_Damage = Damage - pPlayer->GetDef();
                if (HitEnemy.Result.m_Damage < 0)  HitEnemy.Result.m_Damage = 0;
                // playerはノックバックしない
                HitEnemy.Result.m_KnB = KnockBack();
                // playerには状態異常を与えない
                HitEnemy.Result.m_Abn = Abnormality::None;
                HitEnemy.Result.m_AbnTurn = 0;
                // リザルトに結果を追加
                Results.emplace_back(HitEnemy);

                // 攻撃を受けたエネミーの最終的なノックバック量
                Results[0].Result.m_KnB.Power = i - 1;
                break;
            }
            // 敵に当たった
            Enemy* en = pEnemys->GetEnemy(ControlPos);
            if (en != nullptr) {
                AllResult HitEnemy;
                // 当たったEnemyのインデックス
                HitEnemy.Index = short(pEnemys->GetEnemyIndex(ControlPos));
                // 当たったEnemyのダメージ
                HitEnemy.Result.m_Damage = Damage - en->GetDef();
                if (HitEnemy.Result.m_Damage < 0)  HitEnemy.Result.m_Damage = 0;
                // 当たったエネミーはノックバックしない
                HitEnemy.Result.m_KnB = KnockBack();
                // 当たったエネミーには状態異常を与えない
                HitEnemy.Result.m_Abn = Abnormality::None;
                HitEnemy.Result.m_AbnTurn = 0;
                // リザルトに結果を追加
                Results.emplace_back(HitEnemy);

                // 攻撃を受けたエネミーの最終的なノックバック量
                Results[0].Result.m_KnB.Power = i - 1;
                break;
            }
        }
    }
    // エネミーの行動なら
    else if (AUT_Type == ACT_UNIT_TYPE::AUT_EN) {

        //----- 攻撃を受けたPlayerへの影響
        AllResult MainEnemy;
        // Enemyのインデックス
        MainEnemy.Index = short(65535);
        // Enemyが受けるダメージ
        MainEnemy.Result.m_Damage = Damage - pPlayer->GetDef();
        // Enemyのノックバック
        MainEnemy.Result.m_KnB = KnB;
        // Enemyに与える状態異常
        MainEnemy.Result.m_Abn = Abn;
        MainEnemy.Result.m_AbnTurn = AbnTurn;
        // リザルトに結果を追加
        Results.emplace_back(MainEnemy);

        // ノックバックの方向
        XMINT2 Knock = XMINT2(KnB.LEFT - KnB.RIGHT, KnB.UP - KnB.DOWN);
        // ノックバック先までの判定
        for (int i = 0; i < KnB.Power; i++) {
            XMINT2 ControlPos = Pos + Knock * i;

            // ぶつかったらその場でノックバックを辞める
             // 壁かマップ端に当たった
            if (pMap->InMap(ControlPos) == false || pMap->IsImmovableChip(ControlPos)) {
                // 攻撃を受けたエネミーの最終的なノックバック量
                Results[0].Result.m_KnB.Power = i - 1;
                break;
            }
            // 敵に当たった
            Enemy* en = pEnemys->GetEnemy(ControlPos);
            if (en != nullptr) {
                AllResult HitEnemy;
                // 当たったEnemyのインデックス
                HitEnemy.Index = short(pEnemys->GetEnemyIndex(ControlPos));
                // 当たったEnemyのダメージ
                HitEnemy.Result.m_Damage = Damage - en->GetDef();
                if (HitEnemy.Result.m_Damage < 0)  HitEnemy.Result.m_Damage = 0;
                // 当たったエネミーはノックバックしない
                HitEnemy.Result.m_KnB = KnockBack();
                // 当たったエネミーには状態異常を与えない
                HitEnemy.Result.m_Abn = Abnormality::None;
                HitEnemy.Result.m_AbnTurn = 0;
                // リザルトに結果を追加
                Results.emplace_back(HitEnemy);

                // 攻撃を受けたエネミーの最終的なノックバック量
                Results[0].Result.m_KnB.Power = i - 1;
                break;
            }
        }
    }

    return Results;
}
