#pragma once
class CAstarMgr
{
	DECLARE_SINGLETON(CAstarMgr)

private:
	CAstarMgr();
	~CAstarMgr();

public:
	list<TILE*>& GetBestLst();

public:
	void StartAstar(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vGoal);

private:
	bool IsFindingPath(int iStartIndex, int iGoalIndex);	// 경로 탐색
	void MakeBestLst(int iStartIndex, int iGoalIndex);		// 경로 생성

private:
	int SelectIndex(const D3DXVECTOR3& vPos);
	bool IsPicking(const D3DXVECTOR3& vPos, int iIndex);

private:
	bool CheckOpenLst(int iIndex);
	bool CheckCloseLst(int iIndex);

private:
	list<int>	m_OpenLst;
	list<int>	m_CloseLst;
	list<TILE*>	m_BestLst;	// 생성된 경로

	int			m_iStartIndex;
};

