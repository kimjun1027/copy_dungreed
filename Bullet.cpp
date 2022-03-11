#include "stdafx.h"
#include "Bullet.h"
#include "ScrollMgr.h"


CBullet::CBullet()
{
	m_tCollisionRect = {};
}


CBullet::~CBullet()
{
}

int CBullet::Update()
{
	if (m_bDead)
		return DEAD_OBJ;
	if (m_fSpeed <= m_fDeadTime)
		return DEAD_OBJ;
	/*CGameObject::*/UpdateCollRect();
	D3DXMATRIX matScale, matTrans, matRotZ;

	D3DXMatrixScaling(&matScale, 3.f, m_fSize * 6.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, m_fAngle + D3DXToRadian(90.f));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - CScrollMgr::GetScroll(0), m_tInfo.vPos.y - CScrollMgr::GetScroll(1), 0.f);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	m_tInfo.vPos += m_tInfo.vDir * m_pTimeMgr->GetDeltaTime() * (m_fSpeed * 500.f);

	MoveFrame(8);

	return NO_EVENT;
}

void CBullet::LateUpdate()
{
	m_fDeadTime += m_pTimeMgr->GetDeltaTime();
}

void CBullet::Render()
{
	m_pTexInfo = m_pTextureMgr->GetTexInfo(
		L"Effect",
		L"Bullet",
		m_tFrame.fCurFrame);
	NULL_CHECK(m_pTexInfo);

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height * 1;

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CBullet::Initialize()
{
	m_tInfo.vCollisionSize.x = 520.f;
	m_tInfo.vCollisionSize.y = 253.f;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 2.f;
	return S_OK;
}

void CBullet::Release()
{
}

CBullet * CBullet::Create(D3DXVECTOR3 vCreatePos, D3DXVECTOR3 vDir, float Angle, float Size, float Speed)
{
	CBullet* pInstance = new CBullet;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetPos(vCreatePos);
	pInstance->SetAngle(Angle);
	pInstance->SetDir(vDir);
	pInstance->SetFlip(Size);
	pInstance->SetSpeed(Speed);

	return pInstance;
}

void CBullet::LineRender()
{
}


void CBullet::MoveFrame(float fSpeed)
{
	m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime() * fSpeed;

	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
	{
		m_tFrame.fCurFrame = 0.f;
		m_bDead = true;
	}
}