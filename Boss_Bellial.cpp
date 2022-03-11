#include "stdafx.h"
#include "Boss_Bellial.h"
#include "MyLineMgr.h"
#include "Player.h"
#include "M_Bullet_Arch.h"
#include "Boss_Bellial_Bullet.h"
#include "Boss_Bellial_Sword.h"
#include "Bullet.h"
#include "EffectAnimation.h"
#include "NormalEffect_Hit_R.h"
#include "Boss_Bellial_Laser_R.h"
#include "Boss_Bellial_LHand.h"
#include "Boss_Bellial_RHand.h"

CBoss_Bellial::CBoss_Bellial()
	:m_eCurState(Idle)
	, m_eNextState(Idle)
	, m_fSpeed(0.f)
	, m_fLineLX(0.f)
	, m_fLineRX(0.f)
	, m_fTempX(0.f)
	, m_fTempY(0.f)
	, m_fCurTime(0.f)
	, m_fDelayTime(0.f)
	, m_fJumpTime(0.f)
{
	ZeroMemory(&m_tFrame, sizeof(FRAME));
}


CBoss_Bellial::~CBoss_Bellial()
{
}

int CBoss_Bellial::Update()
{
	if (m_bDead)
	{
		return DEAD_OBJ;
	}

	CGameObject::LateInit();
	SetHandAngle();
	MonsterWalk();
	/*CGameObject::*/UpdateCollRect();

	MakeWorldMatrix();
	StateChange();
	HitColorChange();

	//if (m_tInfo.vPos.x > CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.x)
	//{
	//	m_tInfo.vSize.x = -3.f;
	//}
	//else
	//{
	//	m_tInfo.vSize.x = 3.f;
	//}
	return NO_EVENT;

}

void CBoss_Bellial::LateUpdate()
{

}

void CBoss_Bellial::Render()
{
	m_pTexInfo_one = m_pTextureMgr->GetTexInfo(
		L"Bellial",
		L"Back",
		int(m_tFrame.fCurFrame));
	NULL_CHECK(m_pTexInfo_one);
	float fCenterX = m_pTexInfo_one->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo_one->tImgInfo.Height* 0.5f;

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(m_pTexInfo_one->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX - 7, fCenterY -25, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, m_iHitColor, m_iHitColor));



	m_pTexInfo = m_pTextureMgr->GetTexInfo(
		L"Bellial",
		m_pFrameKey,
		int(m_tFrame.fCurFrame));
	NULL_CHECK(m_pTexInfo);
	fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	fCenterY = m_pTexInfo->tImgInfo.Height* 0.5f;

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, m_iHitColor, m_iHitColor));



}

HRESULT CBoss_Bellial::Initialize(float fX, float fY)
{

	m_tInfo.vPos.x = fX;
	m_tInfo.vPos.y = fY;
	m_tInfo.vPos.z = 0.f;
	m_tInfo.iHp = 100;
	m_tInfo.iAtt = 1;
	m_iOption = 1;
	m_tInfo.vSize = { 4.f, 4.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	m_tInfo.vCollisionSize = { 120.f , 90.f, 0.f };
	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 10.f;
	m_pFrameKey = L"Idle";


	m_fSpeed = 100 * m_pTimeMgr->GetDeltaTime();
	m_ObjType = MONSTER;
	m_fCircleAngle = 0.f;

	m_iHitColor = 255;
	m_iCurHp = m_tInfo.iHp;
	iBack = 0;
	return S_OK;
}



void CBoss_Bellial::MonsterWalk()
{
	//float fY = 0.f;
	//bool bIsColl = CMyLineMgr::GetInstance()->LineCollision(m_tInfo.vPos.x, m_tInfo.vPos.y, &fY, &m_fLineLX, &m_fLineRX);


	float fWidth = CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.x - m_tInfo.vPos.x;
	float fHeight = CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.y - m_tInfo.vPos.y;
	float fDist = sqrtf(fWidth * fWidth + fHeight * fHeight);

	//if (fDist < 150.f || fDist > 350.f)
	//{
	//	m_fSpeed = 0.f;
	//}
	//if (fDist < 550.f)
	//{
	//	m_fSpeed = 3.f;
	//	if (m_tInfo.vPos.x < CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.x)
	//	{
	//		m_tInfo.vPos.x += m_fSpeed;
	//	}
	//	if (m_tInfo.vPos.x > CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.x)
	//	{
	//		m_tInfo.vPos.x -= m_fSpeed;
	//	}
	//	if (m_tInfo.vPos.y < CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.y)
	//	{
	//		m_tInfo.vPos.y += m_fSpeed;
	//	}
	//	if (m_tInfo.vPos.y > CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.y)
	//	{
	//		m_tInfo.vPos.y -= m_fSpeed;
	//	}
	//}

	m_AttackTime += CTimeMgr::GetInstance()->GetDeltaTime();
	if (m_AttackTime >= 7.8f)
	{
	
		m_fDelayTimeThree += CTimeMgr::GetInstance()->GetDeltaTime();


		if (m_fDelayTimeThree >= 2.5f)
		{
			m_fSwordTime1 += CTimeMgr::GetInstance()->GetDeltaTime();
			m_fSwordTime2 += CTimeMgr::GetInstance()->GetDeltaTime();
			m_fSwordTime3 += CTimeMgr::GetInstance()->GetDeltaTime();
			m_fSwordTime4 += CTimeMgr::GetInstance()->GetDeltaTime();
			m_fSwordTime5 += CTimeMgr::GetInstance()->GetDeltaTime();
			m_fSwordTime6 += CTimeMgr::GetInstance()->GetDeltaTime();
			if (m_fSwordTime1 >= 3.5f)
			{
				CSoundMgr::GetInstance()->StopSound(CSoundMgr::SWORDSPWAN);
				CSoundMgr::GetInstance()->MyPlaySound(L"SpawnMonster.wav", CSoundMgr::SWORDSPWAN);
				CGameObject* pSword1 = CCBoss_Bellial_Sword::Create(D3DXVECTOR3(m_tInfo.vPos.x - 250.f, m_tInfo.vPos.y - 200.f, 0), m_tInfo.vDir, m_fAngle, -(m_tInfo.vSize.x / fabsf(m_tInfo.vSize.x)), 5.f);
				CObjectMgr::GetInstance()->AddObject(CGameObject::M_BULLET, pSword1);
		
				m_fSwordTime1 = 0.f;
			}
			if (m_fSwordTime2 >= 3.7f)
			{
				CSoundMgr::GetInstance()->StopSound(CSoundMgr::SWORDSPWAN);
				CSoundMgr::GetInstance()->MyPlaySound(L"SpawnMonster.wav", CSoundMgr::SWORDSPWAN);
				CGameObject* pSword2 = CCBoss_Bellial_Sword::Create(D3DXVECTOR3(m_tInfo.vPos.x - 150.f, m_tInfo.vPos.y - 200.f, 0), m_tInfo.vDir, m_fAngle, -(m_tInfo.vSize.x / fabsf(m_tInfo.vSize.x)), 5.f);
				CObjectMgr::GetInstance()->AddObject(CGameObject::M_BULLET, pSword2);

				m_fSwordTime2 = 0.f;
			}
			if (m_fSwordTime3 >= 3.8f)
			{
				CSoundMgr::GetInstance()->StopSound(CSoundMgr::SWORDSPWAN);
				CSoundMgr::GetInstance()->MyPlaySound(L"SpawnMonster.wav", CSoundMgr::SWORDSPWAN);
				CGameObject* pSword3 = CCBoss_Bellial_Sword::Create(D3DXVECTOR3(m_tInfo.vPos.x - 50.f, m_tInfo.vPos.y - 200.f, 0), m_tInfo.vDir, m_fAngle, -(m_tInfo.vSize.x / fabsf(m_tInfo.vSize.x)), 5.f);
				CObjectMgr::GetInstance()->AddObject(CGameObject::M_BULLET, pSword3);

				m_fSwordTime3 = 0.f;
			}
			if (m_fSwordTime4 >= 3.9f)
			{
				CSoundMgr::GetInstance()->StopSound(CSoundMgr::SWORDSPWAN);
				CSoundMgr::GetInstance()->MyPlaySound(L"SpawnMonster.wav", CSoundMgr::SWORDSPWAN);
				CGameObject* pSword4 = CCBoss_Bellial_Sword::Create(D3DXVECTOR3(m_tInfo.vPos.x + 50.f, m_tInfo.vPos.y - 200.f, 0), m_tInfo.vDir, m_fAngle, -(m_tInfo.vSize.x / fabsf(m_tInfo.vSize.x)), 5.f);
				CObjectMgr::GetInstance()->AddObject(CGameObject::M_BULLET, pSword4);

				m_fSwordTime4 = 0.f;
			}
			if (m_fSwordTime5 >= 4.0f)
			{
				CSoundMgr::GetInstance()->StopSound(CSoundMgr::SWORDSPWAN);
				CSoundMgr::GetInstance()->MyPlaySound(L"SpawnMonster.wav", CSoundMgr::SWORDSPWAN);
				CGameObject* pSword5 = CCBoss_Bellial_Sword::Create(D3DXVECTOR3(m_tInfo.vPos.x + 150.f, m_tInfo.vPos.y - 200.f, 0), m_tInfo.vDir, m_fAngle, -(m_tInfo.vSize.x / fabsf(m_tInfo.vSize.x)), 5.f);
				CObjectMgr::GetInstance()->AddObject(CGameObject::M_BULLET, pSword5);

				m_fSwordTime5 = 0.f;
			}
			if (m_fSwordTime6 >= 4.1f)
			{
				CSoundMgr::GetInstance()->StopSound(CSoundMgr::SWORDSPWAN);
				CSoundMgr::GetInstance()->MyPlaySound(L"SpawnMonster.wav", CSoundMgr::SWORDSPWAN);
				CGameObject* pSword6 = CCBoss_Bellial_Sword::Create(D3DXVECTOR3(m_tInfo.vPos.x + 250.f, m_tInfo.vPos.y - 200.f, 0), m_tInfo.vDir, m_fAngle, -(m_tInfo.vSize.x / fabsf(m_tInfo.vSize.x)), 5.f);
				CObjectMgr::GetInstance()->AddObject(CGameObject::M_BULLET, pSword6);

				m_fSwordTime6 = 0.f;
				m_fDelayTimeThree = 0.f;
				m_AttackTime = 0;
				m_AttackDelay += CTimeMgr::GetInstance()->GetDeltaTime();



			}
		}

		if (m_AttackDelay <= 5.5f)
		{
			m_fFaceTime += CTimeMgr::GetInstance()->GetDeltaTime();
			m_fFaceOpenTime += CTimeMgr::GetInstance()->GetDeltaTime();

			if (m_fFaceTime <= 0.5f)
			{
				m_eNextState = Attack;
				MoveFrame(0, 5.f, 2.f);
			}
			if (m_fFaceOpenTime >= 0.5)
			{
				m_eNextState = AttackEnd;
				MoveFrame(6, 9.f, 2.f);
			}
			m_fDelayTimeTwo += CTimeMgr::GetInstance()->GetDeltaTime();


			if (m_fDelayTimeTwo >= 0.15f)
			{
				AttackCount = 1;
				m_fDelayTimeTwo = 0.f;
				CSoundMgr::GetInstance()->StopSound(CSoundMgr::MONSTERLASER);
				CSoundMgr::GetInstance()->MyPlaySound(L"water_spell_impact_hit_01.wav", CSoundMgr::MONSTERLASER);
				CGameObject* pInstance = CBoss_Bellial_Bullet::Create2(D3DXVECTOR3(m_tInfo.vPos), D3DXToRadian(m_fCircleAngle), -(m_tInfo.vSize.x / fabsf(m_tInfo.vSize.x)), 2.f);
				CObjectMgr::GetInstance()->AddObject(CGameObject::M_BULLET, pInstance);
				CGameObject* pInstance1 = CBoss_Bellial_Bullet::Create2(D3DXVECTOR3(m_tInfo.vPos), D3DXToRadian(m_fCircleAngle + 90.f), -(m_tInfo.vSize.x / fabsf(m_tInfo.vSize.x)), 2.f);
				CObjectMgr::GetInstance()->AddObject(CGameObject::M_BULLET, pInstance1);
				CGameObject* pInstance2 = CBoss_Bellial_Bullet::Create2(D3DXVECTOR3(m_tInfo.vPos), D3DXToRadian(m_fCircleAngle + 180.f), -(m_tInfo.vSize.x / fabsf(m_tInfo.vSize.x)), 2.f);
				CObjectMgr::GetInstance()->AddObject(CGameObject::M_BULLET, pInstance2);
				CGameObject* pInstance3 = CBoss_Bellial_Bullet::Create2(D3DXVECTOR3(m_tInfo.vPos), D3DXToRadian(m_fCircleAngle + 270.f), -(m_tInfo.vSize.x / fabsf(m_tInfo.vSize.x)), 2.f);
				CObjectMgr::GetInstance()->AddObject(CGameObject::M_BULLET, pInstance3);

				m_fCircleAngle += 15.f;

				if (m_fCircleAngle >= 360.f)
					m_fCircleAngle = 0;
			}
		}

		
	}
	else
	{
		m_eNextState = Idle;
		MoveFrame(0.f, 9.f, 2.f);

	}
}

void CBoss_Bellial::StateChange()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case CBoss_Bellial::Idle:
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 9.f;
			m_pFrameKey = L"Idle";
			break;
		case CBoss_Bellial::Attack:
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 5.f;
			m_pFrameKey = L"Attack";
			break;
		case CBoss_Bellial::AttackEnd:
			m_tFrame.fCurFrame = 6.f;
			m_tFrame.fMaxFrame = 9.f;
			m_pFrameKey = L"Attack";
			break;
		//case CBoss_Bellial::Dead:
		//	m_tFrame.fCurFrame = 0.f;
		//	m_tFrame.fMaxFrame = 9.f;
		//	m_pFrameKey = L"Dead";
		//	break;


		default:
			break;
		}
		m_eCurState = m_eNextState;
	}
}

void CBoss_Bellial::MakeWorldMatrix()
{
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x - CScrollMgr::GetScroll(0),
		m_tInfo.vPos.y - CScrollMgr::GetScroll(1),
		0.f);

	m_tInfo.matWorld = matScale * matTrans;
}

HRESULT CBoss_Bellial::Initialize()
{
	return S_OK;

}

HRESULT CBoss_Bellial::LateInit()
{

	return S_OK;
}


void CBoss_Bellial::Release()
{
}

void CBoss_Bellial::MoveFrame(float fStartFrame, float fMaxFrame, float fSpeed)
{
	m_tFrame.fMaxFrame = fMaxFrame;
	float EndFrame = fMaxFrame - fStartFrame;
	if (m_tFrame.fCurFrame < m_tFrame.fMaxFrame)
		m_tFrame.fCurFrame += EndFrame * m_pTimeMgr->GetDeltaTime() * fSpeed;



	if (m_tFrame.fCurFrame > fMaxFrame)
	{
		if (m_eCurState == Attack)
		{
			m_eNextState = Idle;
			m_tFrame.fCurFrame = fMaxFrame;
		}
		else
			m_tFrame.fCurFrame = fStartFrame;
	}
}


CBoss_Bellial * CBoss_Bellial::Create()
{
	return new CBoss_Bellial;
}

void CBoss_Bellial::LineRender()
{
}

void CBoss_Bellial::HitColorChange()
{
	if (m_iCurHp != m_tInfo.iHp)
	{
		CSoundMgr::GetInstance()->StopSound(CSoundMgr::MONSTERHIT);
		CSoundMgr::GetInstance()->MyPlaySound(L"Hit_Monster.wav", CSoundMgr::MONSTERHIT);
		m_iHitColor = 0;
	}
	//m_fDelayTime = 0.f;
	if (0.4f <= m_fDelayTime)
	{
		m_iHitColor = 255;
		m_fDelayTime = 0.f;
	}
	m_iCurHp = m_tInfo.iHp;
	m_fDelayTime += CTimeMgr::GetInstance()->GetDeltaTime();

}



void CBoss_Bellial::SetHandAngle()
{
	////POINTa pt = {};
	////GetCursorPos(&pt);
	////ScreenToClient(g_hWnd, &pt);

	////float fMouseX = pt.x + CScrollMgr::GetScroll(0);
	////float fMouseY = pt.y + CScrollMgr::GetScroll(1);

	//if (CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.x >= m_tInfo.vPos.x)
	//{
	//	m_tInfo.vLook.x = 1.f;
	//	//m_tInfo.vSize.x = 3.f;
	//}
	//else
	//{
	//	m_tInfo.vLook.x = -1.f;
	//	//m_tInfo.vSize.x = -3.f;
	//}

	//D3DXVECTOR3 vPlayer(CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.x, CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.y, 0);

	//vPlayer -= m_tInfo.vPos;

	//D3DXVec3Normalize(&vPlayer, &vPlayer);

	//m_fAngle = acosf(D3DXVec3Dot(&vPlayer, &m_tInfo.vLook));

	//if (CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.y <= m_tInfo.vPos.y)
	//{
	//	if (m_tInfo.vSize.x >= 0.f)
	//		m_fAngle *= -1.f;
	//}
	//else
	//{
	//	if (m_tInfo.vSize.x < 0.f)
	//		m_fAngle *= -1.f;
	//}



	/*POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	float fMouseX = pt.x + CScrollMgr::GetScroll(0);
	float fMouseY = pt.y + CScrollMgr::GetScroll(1);

	if (fMouseX >= m_tInfo.vPos.x)
	{
		m_tInfo.vLook.x = 1.f;
		m_tInfo.vSize.x = 3.f;
	}
	else
	{
		m_tInfo.vLook.x = -1.f;
		m_tInfo.vSize.x = -3.f;
	}

	D3DXVECTOR3 vMouse(fMouseX, fMouseY, 0.f);

	vMouse -= m_tInfo.vPos;

	D3DXVec3Normalize(&vMouse, &vMouse);

	m_fAngle = acosf(D3DXVec3Dot(&vMouse, &m_tInfo.vLook));

	if (fMouseY <= m_tInfo.vPos.y)
	{
		if (m_tInfo.vSize.x >= 0.f)
			m_fAngle *= -1.f;
	}


	else
	{
		if (m_tInfo.vSize.x < 0.f)
			m_fAngle *= -1.f;
	}*/
}