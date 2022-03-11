#pragma once
#include "GameObject.h"
class CBoss_Bellial_LHand :
	public CGameObject
{
public:
	explicit CBoss_Bellial_LHand();
	virtual ~CBoss_Bellial_LHand();
	enum STATE { Crush, Idle, CrushFX, Attack };


public:
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
public:
	bool Get_OP() { return m_bIsOP; }

public:
	HRESULT Initialize(float fX, float fY);
public:
	void MonsterWalk();
	void StateChange();
	void MakeWorldMatrix();
	void Set_State(STATE eState) { m_eNextState = eState; }
	void SetHurtEnd(bool IsState) { m_bIsHurt = IsState; }
	void SetNoneHit(bool IsCoolTime) { m_bIsOP = IsCoolTime; }


private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit() override;
	virtual void Release() override;
	void SetHandAngle();



private:
	void MoveFrame(float fStartFrame, float fMaxFrame, float fSpeed = 1.f);
	void CBoss_Bellial_LHand::HitColorChange();

public:
	static CBoss_Bellial_LHand* Create();
private:

	FRAME			m_tFrame;
	TCHAR*			m_pFrameKey = L"";

	STATE			m_eCurState;
	STATE			m_eNextState;

	bool			m_bIsJump = false;
	bool			m_bIsFall = false;
	bool			m_bIsHurt = false;
	bool			m_bIsThrow = false;
	bool			m_bIsOP = false;

	float			m_fJumpPower = 0.f;
	float			m_fJumpAccel = 0.f;
	float			m_fSpeed;
	float			m_fLineLX;
	float			m_fLineRX;
	float			m_fCurTime;
	float			m_fDelayTime;
	float			m_fJumpTime;
	float			m_fThrowTime;
	float			m_fWalkTime;

	float			m_fTempX;
	float			m_fTempY;
	int				m_iHitColor;
	float			m_fDelayTimeTwo;

	int				m_iCurHp;


	// CGameObject을(를) 통해 상속됨
	virtual void LineRender() override;

};

