#pragma once
#include "GameObject.h"
class CBullet :
	public CGameObject
{
public:
	explicit CBullet();
	virtual ~CBullet();

public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	void SetFlip(float Size) { m_fSize = Size; }
	void SetSpeed(float Speed) { m_fSpeed = Speed; }

	void MoveFrame(float fSpeed = 1.f);

	CGameObject* GetParent() { return m_ParentUnit; }

public:
	static CBullet* Create(D3DXVECTOR3 vCreatePos, D3DXVECTOR3 vDir, float Angle, float Size, float Speed);

private:
	float m_fSpeed;
	float m_fDeadTime;

	FRAME		m_tFrame;
	CGameObject* m_ParentUnit;


	// CGameObject을(를) 통해 상속됨
	virtual void LineRender() override;
};

