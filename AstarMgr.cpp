#include "stdafx.h"
#include "AstarMgr.h"
#include "Terrain.h"

IMPLEMENT_SINGLETON(CAstarMgr)

CAstarMgr::CAstarMgr()
	: m_iStartIndex(0)
{
}


CAstarMgr::~CAstarMgr()
{
}

list<TILE*>& CAstarMgr::GetBestLst()
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_BestLst;
}

void CAstarMgr::StartAstar(const D3DXVECTOR3 & vStart, const D3DXVECTOR3 & vGoal)
{
	CGameObject* pTerrain = CObjectMgr::GetInstance()->Get_Object(CGameObject::TERRAIN);
	const vector<TILE*>& vecTile = dynamic_cast<CTerrain*>(pTerrain)->GetVecTile();

	// ������ ������� ���� ����.
	m_OpenLst.clear();
	m_CloseLst.clear();
	m_BestLst.clear();



	m_iStartIndex = SelectIndex(vStart);
	int iGoalIndex = SelectIndex(vGoal);

	if (0 > m_iStartIndex || 0 > iGoalIndex)
		return;

	if (m_iStartIndex == iGoalIndex)
		return;

	if (0 != vecTile[iGoalIndex]->byOption)
		return;

	if (true == IsFindingPath(m_iStartIndex, iGoalIndex))
		MakeBestLst(m_iStartIndex, iGoalIndex);
}

bool CAstarMgr::IsFindingPath(int iStartIndex, int iGoalIndex)
{
	CGameObject* pTerrain = CObjectMgr::GetInstance()->Get_Object(CGameObject::TERRAIN);
	const vector<TILE*>& vecTile = dynamic_cast<CTerrain*>(pTerrain)->GetVecTile();
	const vector<list<TILE*>>& vecGraph = dynamic_cast<CTerrain*>(pTerrain)->GetGraph();

	if(!m_OpenLst.empty())
		m_OpenLst.pop_front();

	m_CloseLst.push_back(iStartIndex);

	for (auto& pTile : vecGraph[iStartIndex])
	{
		if (iGoalIndex == pTile->iMyIndex)
		{
			pTile->iParentIndex = iStartIndex;
			return true; // ������ ã��. Ž�� ����.
		}

		if (CheckOpenLst(pTile->iMyIndex) && CheckCloseLst(pTile->iMyIndex))
		{
			pTile->iParentIndex = iStartIndex;
			m_OpenLst.push_back(pTile->iMyIndex);
		}
	}

	// ���̻� ��ΰ� ���ٴ� ��.
	if(m_OpenLst.empty())
		return false;

	// �޸���ƽ ��� ����.
	// �޸���ƽ = ��������� �Ÿ� + ���������� �Ÿ�
	int i��ŸƮ�ε��� = m_iStartIndex;

	m_OpenLst.sort([&vecTile, &i��ŸƮ�ε���, &iGoalIndex](int iPreIdx, int iNextIdx)
	{
		// iPreIdx�� �������� ��������� �Ÿ� + ���������� �Ÿ��� ���Ѵ�.
		D3DXVECTOR3 vDir1 = vecTile[i��ŸƮ�ε���]->vPos - vecTile[iPreIdx]->vPos;
		D3DXVECTOR3 vDir2 = vecTile[iGoalIndex]->vPos - vecTile[iPreIdx]->vPos;
		float fH1 = D3DXVec3Length(&vDir1) + D3DXVec3Length(&vDir2);

		// iNextIdx�� �������� ��������� �Ÿ� + ���������� �Ÿ��� ���Ѵ�.
		vDir1 = vecTile[i��ŸƮ�ε���]->vPos - vecTile[iNextIdx]->vPos;
		vDir2 = vecTile[iGoalIndex]->vPos - vecTile[iNextIdx]->vPos;
		float fH2 = D3DXVec3Length(&vDir1) + D3DXVec3Length(&vDir2);

		return fH1 < fH2;
	});

	// �ʺ�Ž���� Queue�� ������� ����.
	return IsFindingPath(m_OpenLst.front(), iGoalIndex);
}

void CAstarMgr::MakeBestLst(int iStartIndex, int iGoalIndex)
{
	CGameObject* pTerrain = CObjectMgr::GetInstance()->Get_Object(CGameObject::TERRAIN);
	const vector<TILE*>& vecTile = dynamic_cast<CTerrain*>(pTerrain)->GetVecTile();

	m_BestLst.push_front(vecTile[iGoalIndex]);

#ifdef _DEBUG
	vecTile[iGoalIndex]->byDrawID = 36;
#endif

	int iParentIndex = vecTile[iGoalIndex]->iParentIndex;

	while (true)
	{
		if (iStartIndex == iParentIndex)
			break;

		m_BestLst.push_front(vecTile[iParentIndex]);

#ifdef _DEBUG
		vecTile[iParentIndex]->byDrawID = 36;
#endif

		iParentIndex = vecTile[iParentIndex]->iParentIndex;
	}
}

int CAstarMgr::SelectIndex(const D3DXVECTOR3 & vPos)
{
	CGameObject* pTerrain = CObjectMgr::GetInstance()->Get_Object(CGameObject::TERRAIN);
	const vector<TILE*>& vecTile = dynamic_cast<CTerrain*>(pTerrain)->GetVecTile();

	for (size_t iIndex = 0; iIndex < vecTile.size(); ++iIndex)
	{
		if (IsPicking(vPos, iIndex))
			return iIndex;
	}

	return -1;
}

bool CAstarMgr::IsPicking(const D3DXVECTOR3 & vPos, int iIndex)
{
	CGameObject* pTerrain = CObjectMgr::GetInstance()->Get_Object(CGameObject::TERRAIN);
	const vector<TILE*>& vecTile = dynamic_cast<CTerrain*>(pTerrain)->GetVecTile();

	// �������� �� ������ 4���� �ð�������� ���Ѵ�.
	D3DXVECTOR3 vPoint[4] =
	{
		{ vecTile[iIndex]->vPos.x, vecTile[iIndex]->vPos.y + (TILECY * 0.5f), 0.f },
		{ vecTile[iIndex]->vPos.x + (TILECX * 0.5f), vecTile[iIndex]->vPos.y, 0.f },
		{ vecTile[iIndex]->vPos.x, vecTile[iIndex]->vPos.y - (TILECY * 0.5f), 0.f },
		{ vecTile[iIndex]->vPos.x - (TILECX * 0.5f), vecTile[iIndex]->vPos.y, 0.f },
	};

	// ������ �ð������ ���⺤�� 4���� ����.
	D3DXVECTOR3 vDir[4] =
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	// �� ���⺤�Ϳ��� �ٱ����� ���� �������͸� 4�� ���Ѵ�.
	D3DXVECTOR3 vNormal[4] =
	{
		{ -vDir[0].y, vDir[0].x, 0.f },
		{ -vDir[1].y, vDir[1].x, 0.f },
		{ -vDir[2].y, vDir[2].x, 0.f },
		{ -vDir[3].y, vDir[3].x, 0.f }
	};

	// �� �������� �������� ���콺���� ���⺤�͸� 4�� ���Ѵ�.
	D3DXVECTOR3 vMouseDir[4] =
	{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	// 4���� vMouseDir�� vNormal�� ���� ����� ��� ����(�а�)�� �� True
	for (int i = 0; i < 4; ++i)
	{
		if (0 < D3DXVec3Dot(&vMouseDir[i], &vNormal[i]))
			return false; // �ϳ��� ����� ������ false
	}

	return true;
}

bool CAstarMgr::CheckOpenLst(int iIndex)
{
	// find�Լ�: <algorithm>���� ����.
	auto iter_find = find(m_OpenLst.begin(), m_OpenLst.end(), iIndex);

	if (m_OpenLst.end() == iter_find)
		return true;

	return false;
}

bool CAstarMgr::CheckCloseLst(int iIndex)
{
	auto iter_find = find(m_CloseLst.begin(), m_CloseLst.end(), iIndex);

	if (m_CloseLst.end() == iter_find)
		return true;

	return false;
}
