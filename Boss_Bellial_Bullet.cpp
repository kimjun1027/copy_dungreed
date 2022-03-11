#include "stdafx.h"
#include "Boss_Bellial_Bullet.h"
#include "TerrainCollision.h"
CBoss_Bellial_Bullet::CBoss_Bellial_Bullet()
{
	m_tCollisionRect = {};
}


CBoss_Bellial_Bullet::~CBoss_Bellial_Bullet()
{
}

int CBoss_Bellial_Bullet::Update()
{
	int iIndex = dynamic_cast<CTerrainCollision*>(CObjectMgr::GetInstance()->Get_Object(CGameObject::TERRAIN_COLLISION))->SelectIndexCur(m_tInfo.vPos);

	if (iIndex != -1)
	{
		if (dynamic_cast<CTerrainCollision*> (CObjectMgr::GetInstance()->Get_Object(CGameObject::TERRAIN_COLLISION))->Get_Tile(iIndex)->byDrawID_Collision == 1)
			return DEAD_OBJ;
	}
	if (3.0f <= m_fDeadTime)
		return DEAD_OBJ;

	if (m_bDead)
		return DEAD_OBJ;
	//if (m_fSpeed <= m_fDeadTime)
	//	return DEAD_OBJ;



	/*CGameObject::*/UpdateCollRect();
	D3DXMATRIX matScale, matTrans, matRotZ;



	D3DXMatrixScaling(&matScale, 4.f, m_fSize * 4.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, m_fAngle + D3DXToRadian(90.f));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x +50.f - CScrollMgr::GetScroll(0), m_tInfo.vPos.y + 100.f - CScrollMgr::GetScroll(1), 0.f);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	//m_tInfo.vCollisionSize.x = m_tInfo.vPos.x - CScrollMgr::GetScroll(0);
	//m_tInfo.vCollisionSize.y = m_tInfo.vPos.y - CScrollMgr::GetScroll(1);

	m_tInfo.vPos.x += cosf(m_fAngle) * 10;
	m_tInfo.vPos.y -= sinf(m_fAngle) * 10;

	m_tInfo.vCollisionSize.x = m_tInfo.vPos.x;
	m_tInfo.vCollisionSize.y = m_tInfo.vPos.y;
	MoveFrame(0.5);

	return NO_EVENT;
}

void CBoss_Bellial_Bullet::LateUpdate()
{
	m_fDeadTime += m_pTimeMgr->GetDeltaTime();
}

void CBoss_Bellial_Bullet::Render()
{
	m_pTexInfo = m_pTextureMgr->GetTexInfo(
		L"Bellial",
		L"Bullet",
		m_tFrame.fCurFrame);
	NULL_CHECK(m_pTexInfo);

	float fCenterX = m_pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = m_pTexInfo->tImgInfo.Height  * 0.5f;

	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pGraphicDev->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CBoss_Bellial_Bullet::Initialize()
{

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fMaxFrame = 1.f;

	return S_OK;
}

void CBoss_Bellial_Bullet::Release()
{
}

//CBoss_Bellial_Bullet * CBoss_Bellial_Bullet::Create(D3DXVECTOR3 vCreatePos, D3DXVECTOR3 vDir, float Angle, float Size, float Speed)
//{
//	CBoss_Bellial_Bullet* pInstance = new CBoss_Bellial_Bullet;
//
//	if (FAILED(pInstance->Initialize()))
//	{
//		SafeDelete(pInstance);
//		return nullptr;
//	}
//	pInstance->SetPos(vCreatePos);
//	pInstance->SetAngle(Angle);
//	pInstance->SetDir(vDir);
//	pInstance->SetFlip(Size);
//	pInstance->SetSpeed(Speed);
//
//	return pInstance;
//}

CBoss_Bellial_Bullet * CBoss_Bellial_Bullet::Create2(D3DXVECTOR3 vCreatePos, float Angle, float Size, float Speed)
{
	CBoss_Bellial_Bullet* pInstance = new CBoss_Bellial_Bullet;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetPos(vCreatePos);
	pInstance->SetAngle(Angle);
	pInstance->SetFlip(Size);
	pInstance->SetSpeed(Speed);

	return pInstance;
}


void CBoss_Bellial_Bullet::LineRender()
{
}


void CBoss_Bellial_Bullet::MoveFrame(float fSpeed)
{
	m_tFrame.fCurFrame += m_tFrame.fMaxFrame * m_pTimeMgr->GetDeltaTime() * fSpeed;

	if (m_tFrame.fCurFrame > m_tFrame.fMaxFrame)
	{
		m_tFrame.fCurFrame = 0.f;
		m_bDead = true;
	}
}