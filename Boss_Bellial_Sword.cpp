#include "stdafx.h"
#include "Boss_Bellial_Sword.h"
#include "TerrainCollision.h"
#include "EffectAnimation.h"
#include "NormalEffect_Hit_R.h"
#include "Boss_Bellial_Laser_R.h"

CCBoss_Bellial_Sword::CCBoss_Bellial_Sword()
{
	m_tCollisionRect = {};
	m_fPlayerX = CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.x;
	m_fPlayerY = CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.y;


}


CCBoss_Bellial_Sword::~CCBoss_Bellial_Sword()
{
}

int CCBoss_Bellial_Sword::Update()
{
	if (m_bDead)
		return DEAD_OBJ;
	//if (m_fSpeed <= m_fDeadTime)
	//	return DEAD_OBJ;
	int iIndex = dynamic_cast<CTerrainCollision*>(CObjectMgr::GetInstance()->Get_Object(CGameObject::TERRAIN_COLLISION))->SelectIndexCur(m_tInfo.vPos);

	//if (iIndex != -1)
	//{
	//	if (dynamic_cast<CTerrainCollision*> (CObjectMgr::GetInstance()->Get_Object(CGameObject::TERRAIN_COLLISION))->Get_Tile(iIndex)->byDrawID_Collision == 1)
	//	{
	//		//return DEAD_OBJ;
	//	}
	//}

	if (10.0f <= m_fDeadTime)
		return DEAD_OBJ;


	/*CGameObject::*/UpdateCollRect();
	D3DXMATRIX matScale, matTrans, matRotZ;
	m_fSwordTime += CTimeMgr::GetInstance()->GetDeltaTime();
	
	if (!(m_bCheck))
	{
		m_fPlayerX = CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.x;
		m_fPlayerY = CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.y;
		D3DXVECTOR3 vPlayerPos = { m_fPlayerX, m_fPlayerY, 0.f };

		m_tInfo.vDir = m_tInfo.vPos - vPlayerPos;

		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

		if (m_fPlayerX >= m_tInfo.vPos.x)
		{
			m_tInfo.vLook.x = 1.f;
			m_tInfo.vSize.x = 4.f;
		}
		else
		{
			m_tInfo.vLook.x = -1.f;
			m_tInfo.vSize.x = -4.f;
		}

		D3DXVECTOR3 vPlayer(m_fPlayerX, m_fPlayerY, 0);

		vPlayer -= m_tInfo.vPos;

		D3DXVec3Normalize(&vPlayer, &vPlayer);

		m_fAngle = acosf(D3DXVec3Dot(&vPlayer, &m_tInfo.vLook));

		if (m_fPlayerY <= m_tInfo.vPos.y)
		{
			if (m_tInfo.vSize.x >= 0.f)
				m_fAngle *= -1.f;
		}
		else
		{
			if (m_tInfo.vSize.x < 0.f)
				m_fAngle *= -1.f;
		}
	}


	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.x, 0.f);
	D3DXMatrixRotationZ(&matRotZ, m_fAngle + D3DXToRadian(90.f));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - CScrollMgr::GetScroll(0), m_tInfo.vPos.y - CScrollMgr::GetScroll(1), 0.f);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	//m_tInfo.vCollisionSize.x = m_tInfo.vPos.x - CScrollMgr::GetScroll(0);
	//m_tInfo.vCollisionSize.y = m_tInfo.vPos.y - CScrollMgr::GetScroll(1);
	
	//float fWidth = CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.x - m_tInfo.vPos.x;
	//float fHeight = CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.y - m_tInfo.vPos.y;
	//float fDist = sqrtf(fWidth * fWidth + fHeight * fHeight);
	
	
	

	if (m_iEffectCount == 0)
	{
		CEffectAnimation* pDash = CEffectAnimation::Create(
			L"Bellial", L"BellialSwordCahrge", { 0.f, 8.f }, 2.f);

		// Ãß»óÃþ
		CNormalEffect_Hit_R* pEffect = CNormalEffect_Hit_R::Create(
			pDash, D3DXVECTOR3(m_tInfo.vPos.x -= m_tInfo.vDir.x * m_pTimeMgr->GetDeltaTime() * m_fSpeed, m_tInfo.vPos.y -= m_tInfo.vDir.y * m_pTimeMgr->GetDeltaTime() * m_fSpeed, 0.f));

		CObjectMgr::GetInstance()->AddObject(CGameObject::EFFECT, pEffect);
		m_iEffectCount = 1;
	}

	MoveFrame(0.5);
	m_fSpeed = 1500.f;
	if (m_fSwordTime >= 3.f)
	{
		//CGameObject* pInstance = CBoss_Bellial_Laser_R::Create(D3DXVECTOR3(m_tInfo.vPos + m_tInfo.vDir), m_tInfo.vDir, m_fAngle, -(m_tInfo.vSize.x / fabsf(m_tInfo.vSize.x)), 3.f);
		//CObjectMgr::GetInstance()->AddObject(CGameObject::BOSS_BELLIAL_LASER, pInstance);

		m_bCheck = true;
		if (!(dynamic_cast<CTerrainCollision*> (CObjectMgr::GetInstance()->Get_Object(CGameObject::TERRAIN_COLLISION))->Get_Tile(iIndex)->byDrawID_Collision == 1))
		{
			iCount = 1;

			m_tInfo.vPos -= m_tInfo.vDir * m_pTimeMgr->GetDeltaTime() * m_fSpeed;
		}
		else
		{
			iCount = 0;

			if (m_iDeadCount == 0)
			{
				CEffectAnimation* pDash = CEffectAnimation::Create(
					L"Bellial", L"BellialSwordDead", { 0.f, 5.f }, 1.f);

				// Ãß»óÃþ
				CNormalEffect_Hit_R* pEffect = CNormalEffect_Hit_R::Create(
					pDash, D3DXVECTOR3(m_tInfo.vPos.x -= m_tInfo.vDir.x * m_pTimeMgr->GetDeltaTime() * m_fSpeed, m_tInfo.vPos.y += m_tInfo.vDir.y * m_pTimeMgr->GetDeltaTime() * m_fSpeed - 35.f, 0.f));

				CObjectMgr::GetInstance()->AddObject(CGameObject::EFFECT, pEffect);
				m_iDeadCount = 1;
			}

		}
		//m_fSwordTime = 0;
	}


	return NO_EVENT;
}

void CCBoss_Bellial_Sword::LateUpdate()
{
	m_fDeadTime += m_pTimeMgr->GetDeltaTime();
}

void CCBoss_Bellial_Sword::Render()
{
	m_pTexInfo = m_pTextureMgr->GetTexInfo(
		L"Bellial",
		L"BellialSword",
		iCount);
	NULL_CHECK(m_pTexInfo);

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height  * 0.3f;

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CCBoss_Bellial_Sword::Initialize()
{
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 0.f;
	m_bCheck = false;
	iCount = 0;
	m_iDeadCount = 0;
	m_iEffectCount = 0;
	return S_OK;
}

void CCBoss_Bellial_Sword::Release()
{
}

CCBoss_Bellial_Sword * CCBoss_Bellial_Sword::Create(D3DXVECTOR3 vCreatePos, D3DXVECTOR3 vDir, float Angle, float Size, float Speed)
{
	CCBoss_Bellial_Sword* pInstance = new CCBoss_Bellial_Sword;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetPos(vCreatePos);
	//pInstance->SetAngle(Angle);
	//pInstance->SetDir(vDir);
	pInstance->SetFlip(Size);
	pInstance->SetSpeed(Speed);

	return pInstance;
}

//CCBoss_Bellial_Sword * CCBoss_Bellial_Sword::Create2(D3DXVECTOR3 vCreatePos, float Angle, float Size, float Speed)
//{
//	CCBoss_Bellial_Sword* pInstance = new CCBoss_Bellial_Sword;
//
//	if (FAILED(pInstance->Initialize()))
//	{
//		SafeDelete(pInstance);
//		return nullptr;
//	}
//	pInstance->SetPos(vCreatePos);
//	pInstance->SetAngle(Angle);
//	pInstance->SetFlip(Size);
//	pInstance->SetSpeed(Speed);
//
//	return pInstance;
//}


void CCBoss_Bellial_Sword::LineRender()
{
}


void CCBoss_Bellial_Sword::MoveFrame(float fSpeed)
{
	m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime() * fSpeed;

	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
	{
		m_tFrame.fCurFrame = 0.f;
		m_bDead = true;
	}
}
