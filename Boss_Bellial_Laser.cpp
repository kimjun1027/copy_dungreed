#include "stdafx.h"
#include "Boss_Bellial_Laser.h"

CBoss_Bellial_Laser::CBoss_Bellial_Laser()
{
	m_tCollisionRect = {};
}


CBoss_Bellial_Laser::~CBoss_Bellial_Laser()
{
}

int CBoss_Bellial_Laser::Update()
{
	if (m_fSpeed <= m_fDeadTime)
		return m_bDead;
	if (m_bDead)
		return DEAD_OBJ;


	/*CGameObject::*/UpdateCollRect();
	D3DXMATRIX matScale, matTrans, matRotZ;

	//m_tInfo.vPos.x = m_tInfo.vCollisionSize.x;
	//m_tInfo.vPos.y = m_tInfo.vCollisionSize.y;

	D3DXMatrixScaling(&matScale, 3.f, m_fSize * 30.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, m_fAngle + D3DXToRadian(90.f));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 95 - CScrollMgr::GetScroll(0), m_tInfo.vPos.y + 25 - CScrollMgr::GetScroll(1), 0.f);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	m_tInfo.vCollisionSize += m_tInfo.vPos + m_tInfo.vDir * m_pTimeMgr->GetDeltaTime() * (m_fSpeed * 1.f);

	MoveFrame(1);

	return NO_EVENT;
}

void CBoss_Bellial_Laser::LateUpdate()
{
	m_fDeadTime += m_pTimeMgr->GetDeltaTime();
}

void CBoss_Bellial_Laser::Render()
{
	m_pTexInfo = m_pTextureMgr->GetTexInfo(
		L"Bellial",
		L"LaserBody",
		m_tFrame.fCurFrame);
	NULL_CHECK(m_pTexInfo);

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height  * 0.5f;

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CBoss_Bellial_Laser::Initialize()
{

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 6.f;
	return S_OK;
}

void CBoss_Bellial_Laser::Release()
{
}

CBoss_Bellial_Laser * CBoss_Bellial_Laser::Create(D3DXVECTOR3 vCreatePos, D3DXVECTOR3 vDir, float Angle, float Size, float Speed)
{
	CBoss_Bellial_Laser* pInstance = new CBoss_Bellial_Laser;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetPos(vCreatePos);
	//pInstance->SetAngle(Angle);
	pInstance->SetDir(vDir);
	pInstance->SetFlip(Size);
	pInstance->SetSpeed(Speed);

	return pInstance;
}

void CBoss_Bellial_Laser::LineRender()
{
}


void CBoss_Bellial_Laser::MoveFrame(float fSpeed)
{
	m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime() * fSpeed;

	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
	{
		m_tFrame.fCurFrame = 0.f;
		m_bDead = true;
	}
}