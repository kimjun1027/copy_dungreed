#pragma once
#include "Effect.h"

class CBuffEffect :
	public CEffect
{
private:
	explicit CBuffEffect();
	explicit CBuffEffect(CEffectBridge* pBridge);

public:
	virtual ~CBuffEffect();

public:
	// CEffect��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void LineRender() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit() override;
	virtual void Release() override;

public:
	static CBuffEffect* Create(CEffectBridge* pBridge);

private:
	float	m_fAngleZ;
};

