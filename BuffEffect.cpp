#include "stdafx.h"
#include "BuffEffect.h"
#include "EffectBridge.h"
#include "Player.h"

CBuffEffect::CBuffEffect()
	: m_fAngleZ(0.f)
{
}

CBuffEffect::CBuffEffect(CEffectBridge* pBridge)
	: CEffect(pBridge),
	m_fAngleZ(0.f)
	
{
}

CBuffEffect::~CBuffEffect()
{
	Release();
}

int CBuffEffect::Update()
{
	CGameObject::LateInit();

	D3DXMATRIX matScale, matRotZ, matTrans, matRevZ;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fAngleZ));
	D3DXMatrixRotationZ(&matRevZ, -D3DXToRadian(m_fAngleZ));

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(
		CObjectMgr::GetInstance()->Get_Object(CGameObject::PLAYER));

	m_tInfo.matWorld = matScale * matRotZ * matTrans * matRevZ * pPlayer->GetInfo().matWorld;

	m_fAngleZ += 90.f * m_pTimeMgr->GetDeltaTime();

	return m_pBridge->Update();
}

void CBuffEffect::LateUpdate()
{
}

void CBuffEffect::Render()
{
	m_pGraphicDev->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pBridge->Render();
}

HRESULT CBuffEffect::Initialize()
{
	// 공전하는 물체는 부모를 기준으로 좌표 계산.
	m_tInfo.vPos = { 300.f, 0.f, 0.f };
	m_tInfo.vSize = { 1.f, 1.f, 0.f };
	return S_OK;
}

HRESULT CBuffEffect::LateInit()
{
	return S_OK;
}

void CBuffEffect::Release()
{
}

CBuffEffect* CBuffEffect::Create(CEffectBridge * pBridge)
{
	CBuffEffect* pInstance = new CBuffEffect(pBridge);

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	
	return pInstance;
}

void CBuffEffect::LineRender()
{
}