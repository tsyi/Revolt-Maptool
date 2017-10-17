#pragma once

struct ST_BONE;

class cSkinnedMesh
{
	friend class cSkinnedMeshManager;

private:
	ST_BONE*					m_pRootFrame;
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	LPD3DXEFFECT				m_pEffect;

	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	D3DXMATRIXA16				m_matWorldTM;

	/// >> : OBB -
	SYNTHESIZE(D3DXVECTOR3, m_vMin, Min);
	SYNTHESIZE(D3DXVECTOR3, m_vMax, Max);
protected:
	float	m_fBlendTime;
	float	m_fPassedBlendTime;
	bool	m_isAnimBlend;
public:
	cSkinnedMesh(std::string szFolder, std::string szFilename);
	~cSkinnedMesh(void);

	void UpdateAndRender();
	void SetAnimationIndex(int nIndex);
	void SetAnimationIndexBlend(int nindex);

	void SetRandomTrackPosition();
	void SetTransform(D3DXMATRIXA16* pmat){m_matWorldTM = *pmat;}

private:
	cSkinnedMesh();
	void Load(std::string szFolder, std::string szFilename);
	LPD3DXEFFECT LoadEffect(std::string szFilename);
	void Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent);
	void Render(ST_BONE* pBone = NULL);
	void Render_(LPD3DXFRAME pFrame);
	void SetupBoneMatrixPtrs(ST_BONE* pBone);
	void Destroy();



};

