#include "stdafx.h"
#include "Boss_Bellial_RHand.h"
#include "MyLineMgr.h"
#include "Player.h"
#include "EffectAnimation.h"
#include "NormalEffect_Hit_R.h"
#include "Boss_Bellial_Laser_R.h"
CBoss_Bellial_RHand::CBoss_Bellial_RHand()
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


CBoss_Bellial_RHand::~CBoss_Bellial_RHand()
{
}

int CBoss_Bellial_RHand::Update()
{
	if (m_bDead)
		return DEAD_OBJ;

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

	if (GetAsyncKeyState('Z') & 0x8000)
	{
		return DEAD_OBJ;
	}

	return NO_EVENT;

}

void CBoss_Bellial_RHand::LateUpdate()
{
}

void CBoss_Bellial_RHand::Render()
{
	m_pTexInfo = m_pTextureMgr->GetTexInfo(
		L"Bellial",
		m_pFrameKey,
		int(m_tFrame.fCurFrame));
	NULL_CHECK(m_pTexInfo);
	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height* 0.5f;

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, m_iHitColor, m_iHitColor));
}

HRESULT CBoss_Bellial_RHand::Initialize(float fX, float fY)
{

	m_tInfo.vPos.x = fX;
	m_tInfo.vPos.y = fY;
	m_tInfo.vPos.z = 0.f;
	m_tInfo.iHp = 5;
	m_tInfo.iAtt = 1;
	m_iOption = 1;
	m_tInfo.vSize = { -4.f, 4.f, 0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	//m_tInfo.vCollisionSize = { 120.f , 90.f, 0.f };
	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 0.f;
	m_pFrameKey = L"HandIdle";



	m_fSpeed = 100 * m_pTimeMgr->GetDeltaTime();
	m_ObjType = MONSTER;


	m_iHitColor = 255;
	m_iCurHp = m_tInfo.iHp;

	return S_OK;
}



void CBoss_Bellial_RHand::MonsterWalk()
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
	//	if (m_tInfo.vPos.x < CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.x)
	//	{
	//		m_tInfo.vPos.x += m_fSpeed;
	//	}
	//	if (m_tInfo.vPos.x > CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.x)
	//	{
	//		m_tInfo.vPos.x -= m_fSpeed;
	//	}



	//}

	m_fDelayTimeTwo += CTimeMgr::GetInstance()->GetDeltaTime();


	m_fSpeed = 3.f;
	if (5.1f <= m_fDelayTimeTwo)
	{
		if (m_tInfo.vPos.y < CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.y)
		{
			m_tInfo.vPos.y += m_fSpeed;
		}
		if (m_tInfo.vPos.y > CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.y)
		{
			m_tInfo.vPos.y -= m_fSpeed;
		}
	}

	if (5000.f > fDist)
	{

		MoveFrame(0.f, 18.f, 1.f);
		if (6.5f <= m_fDelayTimeTwo)
		{
			m_eNextState = Attack;

			if (7.1f <= m_fDelayTimeTwo)
			{
				m_fDelayTimeTwo = 2.f;

				CSoundMgr::GetInstance()->StopSound(CSoundMgr::MONSTERLASER);
				CSoundMgr::GetInstance()->MyPlaySound(L"iceball.wav", CSoundMgr::MONSTERLASER);
				CEffectAnimation* pDash = CEffectAnimation::Create(
					L"Bellial", L"LaserHead", { 0.f, 6.f }, 1.f);

				// Ãß»óÃþ
				CNormalEffect_Hit_R* pEffect = CNormalEffect_Hit_R::Create(
					pDash, D3DXVECTOR3(m_tInfo.vPos.x - 55, m_tInfo.vPos.y + 25, 0.f));

				CObjectMgr::GetInstance()->AddObject(CGameObject::EFFECT, pEffect);

				CGameObject* pInstance = CBoss_Bellial_Laser_R::Create(D3DXVECTOR3(m_tInfo.vPos + m_tInfo.vDir), m_tInfo.vDir, m_fAngle, -(m_tInfo.vSize.x / fabsf(m_tInfo.vSize.x)), 3.f);
				CObjectMgr::GetInstance()->AddObject(CGameObject::BOSS_BELLIAL_LASER, pInstance);
			}
		}
		//else if (130.f <fDist && fDist < 300.f)
		//{
		//	if (m_tInfo.vPos.x > CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.x)
		//	{
		//		//m_tInfo.vPos.x -= m_fSpeed;
		//		m_eNextState = Crush;
		//		MoveFrame(0.f, 7.f, 2.f);
		//	}
		//	else
		//	{
		//		//m_tInfo.vPos.x += m_fSpeed;
		//		m_eNextState = Crush;
		//		MoveFrame(0.f, 7.f, 2.f);
		//	}
		//}
		else
		{
			m_eNextState = Idle;
			MoveFrame(0.f, 9.f, 2.f);

		}
	}
	//m_fJumpPower = 0.f;
	//m_tInfo.vPos.y -= m_fJumpPower * m_fJumpAccel - 9.8f * m_fJumpAccel * m_fJumpAccel * 0.5f;
	//m_fJumpAccel += 0.1f;
	//if (bIsColl && m_tInfo.vPos.y + 60 > fY)
	//{
	//	m_bIsFall = false;
	//	m_bIsJump = false;
	//	m_fJumpAccel = 0.f;
	//	m_tInfo.vPos.y = fY - 60;
	//}
}

void CBoss_Bellial_RHand::StateChange()
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case CBoss_Bellial_RHand::Idle:
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 9.f;
			m_pFrameKey = L"HandIdle";
			break;
		case CBoss_Bellial_RHand::Attack:
			m_tFrame.fCurFrame = 0.f;
			m_tFrame.fMaxFrame = 18.f;
			m_pFrameKey = L"HandAttack";
			break;
			//case CBoss_Bellial_RHand::Dead:
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

void CBoss_Bellial_RHand::MakeWorldMatrix()
{
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x - CScrollMgr::GetScroll(0),
		m_tInfo.vPos.y - CScrollMgr::GetScroll(1),
		0.f);

	m_tInfo.matWorld = matScale * matTrans;
}

HRESULT CBoss_Bellial_RHand::Initialize()
{
	return S_OK;
}

HRESULT CBoss_Bellial_RHand::LateInit()
{

	return S_OK;
}


void CBoss_Bellial_RHand::Release()
{
}

void CBoss_Bellial_RHand::MoveFrame(float fStartFrame, float fMaxFrame, float fSpeed)
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
			m_tFrame.fCurFrame = fStartFrame;
		}
		else
			m_tFrame.fCurFrame = fStartFrame;
	}
}

CBoss_Bellial_RHand * CBoss_Bellial_RHand::Create()
{
	return new CBoss_Bellial_RHand;
}

void CBoss_Bellial_RHand::LineRender()
{
}

void CBoss_Bellial_RHand::HitColorChange()
{
	if (m_iCurHp != m_tInfo.iHp)
	{
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



void CBoss_Bellial_RHand::SetHandAngle()
{
	//POINTa pt = {};
	//GetCursorPos(&pt);
	//ScreenToClient(g_hWnd, &pt);

	//float fMouseX = pt.x + CScrollMgr::GetScroll(0);
	//float fMouseY = pt.y + CScrollMgr::GetScroll(1);

	if (CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.x >= m_tInfo.vPos.x)
	{
		m_tInfo.vLook.x = 1.f;
		//m_tInfo.vSize.x = 3.f;
	}
	else
	{
		m_tInfo.vLook.x = -1.f;
		//m_tInfo.vSize.x = -3.f;
	}

	D3DXVECTOR3 vMouse(CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.x, CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.y, 0.f);

	vMouse -= m_tInfo.vPos;

	D3DXVec3Normalize(&vMouse, &vMouse);

	m_fAngle = acosf(D3DXVec3Dot(&vMouse, &m_tInfo.vLook));

	if (CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER)->Get_Info().vPos.y <= m_tInfo.vPos.y)
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