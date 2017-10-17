#include "StdAfx.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"


cSkinnedMesh::cSkinnedMesh(std::string szFolder, std::string szFilename)
	: m_pRootFrame(NULL)
	, m_pAnimController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)

	, m_fBlendTime(0.3f)
	, m_fPassedBlendTime(0.0f)
	, m_isAnimBlend(false)
{
	cSkinnedMesh* pSkinnedMesh = g_pSkinnedMeshManager->GetSkinnedMesh(szFolder, szFilename);

	D3DXMatrixIdentity(&m_matWorldTM);
	m_pRootFrame = pSkinnedMesh->m_pRootFrame;
	m_dwWorkingPaletteSize = pSkinnedMesh->m_dwWorkingPaletteSize;
	m_pmWorkingPalette = pSkinnedMesh->m_pmWorkingPalette;
	m_pEffect = pSkinnedMesh->m_pEffect;

	/// >> : OBB
	{
		m_vMin = pSkinnedMesh->m_vMin;
		m_vMax = pSkinnedMesh->m_vMax;
	}

	if (pSkinnedMesh->m_pAnimController)
	{
		pSkinnedMesh->m_pAnimController->CloneAnimationController(
			pSkinnedMesh->m_pAnimController->GetMaxNumAnimationOutputs(),
			pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets(),
			pSkinnedMesh->m_pAnimController->GetMaxNumTracks(),
			pSkinnedMesh->m_pAnimController->GetMaxNumEvents(),
			&m_pAnimController);
	}
}

cSkinnedMesh::cSkinnedMesh()
	: m_pRootFrame(NULL)
	, m_pAnimController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)
	, m_vMin(0, 0, 0) /// >> : OBB
	, m_vMax(0, 0, 0) /// >> : OBB

	, m_fBlendTime(0.3f)
	, m_fPassedBlendTime(0.0f)
	, m_isAnimBlend(false)
{
}

cSkinnedMesh::~cSkinnedMesh(void)
{
	SAFE_RELEASE(m_pAnimController);
}

void cSkinnedMesh::Load(std::string szDirectory, std::string szFilename)
{
	m_pEffect = LoadEffect("MultiAnimation.hpp");

	int nPaletteSize = 0;
	m_pEffect->GetInt("MATRIX_PALETTE_SIZE", &nPaletteSize);

	cAllocateHierarchy ah;
	ah.SetDirectory(szDirectory);
	ah.SetDefaultPaletteSize(nPaletteSize);

	std::string sFullPath = szDirectory + szFilename;

	D3DXLoadMeshHierarchyFromXA(sFullPath.c_str(),
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		&ah,
		NULL,
		(LPD3DXFRAME*)&m_pRootFrame,
		&m_pAnimController);

	/// >> : OBB
	{
		m_vMin = ah.GetMin();
		m_vMax = ah.GetMax();
	}

	if (m_pmWorkingPalette)
		delete[] m_pmWorkingPalette;

	m_dwWorkingPaletteSize = ah.GetMaxPaletteSize();
	m_pmWorkingPalette = new D3DXMATRIX[m_dwWorkingPaletteSize];
	if (m_pmWorkingPalette == NULL)
	{
		m_dwWorkingPaletteSize = 0;
	}

	if (m_pRootFrame)
		SetupBoneMatrixPtrs(m_pRootFrame);
}

void cSkinnedMesh::UpdateAndRender()
{
	if (m_pAnimController)
	{
		m_pAnimController->AdvanceTime(MgrTime->GetElapsedTime(), NULL);
		if (m_isAnimBlend)
		{
			m_fPassedBlendTime += MgrTime->GetElapsedTime();
			if (m_fPassedBlendTime >= m_fBlendTime)
			{
				//BlendEnd
				m_isAnimBlend = false;
				m_pAnimController->SetTrackWeight(0, 1.0f);
				m_pAnimController->SetTrackEnable(1, false);
			}
			else
			{
				float fWeight = m_fPassedBlendTime / m_fBlendTime;
				m_pAnimController->SetTrackWeight(0, fWeight);
				m_pAnimController->SetTrackWeight(1, 1.0f - fWeight);
			}
		}
	}

	if (m_pRootFrame)
	{
		Update(m_pRootFrame, &m_matWorldTM);
		Render(m_pRootFrame);
	}
	else
	{
		Render_(NULL);
	}
}

void cSkinnedMesh::Render(ST_BONE* pBone)
{
	assert(pBone);

	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		if (pBoneMesh->pSkinInfo == NULL)
		{
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &pBone->CombinedTransformationMatrix);
			for (size_t i = 0; i < pBoneMesh->vecMaterial.size(); ++i)
			{
				g_pD3DDevice->SetTexture(0, pBoneMesh->vecTexture[i]);
				g_pD3DDevice->SetMaterial(&pBoneMesh->vecMaterial[i]);
				pBoneMesh->MeshData.pMesh->DrawSubset(i);
			}
		}
		else
		{
			LPD3DXBONECOMBINATION pBoneCombos =
				(LPD3DXBONECOMBINATION)(pBoneMesh->pBufBoneCombos->GetBufferPointer());

			D3DXMATRIXA16 matViewProj, matView, matProj;

			g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
			g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
			matViewProj = matView * matProj;

			D3DXMATRIXA16 mView, mInvView;
			g_pD3DDevice->GetTransform(D3DTS_VIEW, &mView);
			D3DXMatrixInverse(&mInvView, 0, &mView);
			D3DXVECTOR3 vEye = D3DXVECTOR3(0, 0, 0);
			D3DXVec3TransformCoord(&vEye, &vEye, &mInvView);

			for (DWORD dwAttrib = 0; dwAttrib < pBoneMesh->dwNumAttrGroups; ++dwAttrib)
			{
				for (DWORD dwPalEntry = 0; dwPalEntry < pBoneMesh->dwNumPaletteEntries; ++dwPalEntry)
				{
					DWORD dwMatrixIndex = pBoneCombos[dwAttrib].BoneId[dwPalEntry];
					if (dwMatrixIndex != UINT_MAX)
					{
						m_pmWorkingPalette[dwPalEntry] =
							pBoneMesh->pBoneOffsetMatrices[dwMatrixIndex] *
							(*pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
					}
				}

				m_pEffect->SetMatrixArray("amPalette",
					m_pmWorkingPalette,
					pBoneMesh->dwNumPaletteEntries);

				m_pEffect->SetMatrix("g_mViewProj", &matViewProj);
				m_pEffect->SetVector("vLightDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
				m_pEffect->SetVector("vWorldLightPos", &D3DXVECTOR4(500.0f, 500.0f, 500.0f, 1.0f));
				m_pEffect->SetVector("vWorldCameraPos", &D3DXVECTOR4(vEye, 1.0f));
				m_pEffect->SetVector("vMaterialAmbient", &D3DXVECTOR4(0.53f, 0.53f, 0.53f, 0.53f));
				m_pEffect->SetVector("vMaterialDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
				m_pEffect->SetTexture("g_txScene", pBoneMesh->vecTexture[pBoneCombos[dwAttrib].AttribId]);
				m_pEffect->SetInt("CurNumBones", pBoneMesh->dwMaxNumFaceInfls - 1);
				m_pEffect->SetTechnique("Skinning20");

				UINT uiPasses, uiPass;
				m_pEffect->Begin(&uiPasses, 0);
				for (uiPass = 0; uiPass < uiPasses; ++uiPass)
				{
					m_pEffect->BeginPass(uiPass);
					pBoneMesh->pWorkingMesh->DrawSubset(dwAttrib);
					m_pEffect->EndPass();
				}
				m_pEffect->End();
			}
		}
	}

	if (pBone->pFrameSibling)
	{
		Render((ST_BONE*)pBone->pFrameSibling);
	}

	if (pBone->pFrameFirstChild)
	{
		Render((ST_BONE*)pBone->pFrameFirstChild);
	}
}

void cSkinnedMesh::Render_(LPD3DXFRAME pFrame)
{
	if (pFrame == NULL)
		pFrame = m_pRootFrame;

	ST_BONE* pBone = (ST_BONE*)pFrame;

	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh =(ST_BONE_MESH*)pBone->pMeshContainer;
		if (pBoneMesh->MeshData.pMesh)
		{
			g_pD3DDevice->SetTransform(D3DTS_WORLD,	&pBone->CombinedTransformationMatrix);
			for (size_t i = 0;	i < pBoneMesh->vecMaterial.size();	++i)
			{
				g_pD3DDevice->SetTexture(0,	pBoneMesh->vecTexture[i]);g_pD3DDevice->SetMaterial(&pBoneMesh->vecMaterial[i]);
				pBoneMesh->MeshData.pMesh->DrawSubset(i);
			}
		}
	}

	if (pFrame->pFrameFirstChild)
	{
		Render_(pFrame->pFrameFirstChild);
	}
	if (pFrame->pFrameSibling)
	{
		Render_(pFrame->pFrameSibling);
	}
}

LPD3DXEFFECT cSkinnedMesh::LoadEffect(std::string szFilename)
{
	LPD3DXEFFECT pEffect = NULL;

	D3DXMACRO mac[2] =
	{
		{ "MATRIX_PALETTE_SIZE_DEFAULT", "35" },
		{ NULL,                          NULL }
	};

	D3DCAPS9 caps;
	D3DXMACRO *pmac = NULL;
	g_pD3DDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion > D3DVS_VERSION(1, 1))
		pmac = mac;

	DWORD dwShaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

	ID3DXBuffer* pBuffer = NULL;
	if (FAILED(D3DXCreateEffectFromFileA(g_pD3DDevice,
		szFilename.c_str(),
		pmac,
		NULL,
		dwShaderFlags,
		NULL,
		&pEffect,
		&pBuffer)))
	{
		if (pBuffer)
		{
			OutputDebugStringA((char*)pBuffer->GetBufferPointer());
			SAFE_RELEASE(pBuffer);
		}

		return NULL;
	}

	return pEffect;
}

void cSkinnedMesh::Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent)
{
	pCurrent->CombinedTransformationMatrix = pCurrent->TransformationMatrix;
	if (pmatParent)
	{
		pCurrent->CombinedTransformationMatrix =
			pCurrent->CombinedTransformationMatrix * (*pmatParent);
	}

	if (pCurrent->pFrameSibling)
	{
		Update((ST_BONE*)pCurrent->pFrameSibling, pmatParent);
	}

	if (pCurrent->pFrameFirstChild)
	{
		Update((ST_BONE*)pCurrent->pFrameFirstChild, &(pCurrent->CombinedTransformationMatrix));
	}
}

void cSkinnedMesh::SetupBoneMatrixPtrs(ST_BONE* pBone)
{
	assert(pBone);

	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				LPCSTR szBoneName = pSkinInfo->GetBoneName(i);
				if (szBoneName == NULL || strlen(szBoneName) == 0)
					continue;
				ST_BONE* pInfluence = (ST_BONE*)D3DXFrameFind(m_pRootFrame, szBoneName);
				pBoneMesh->ppBoneMatrixPtrs[i] = &(pInfluence->CombinedTransformationMatrix);
			}
		}
	}
	if (pBone->pFrameSibling)
	{
		SetupBoneMatrixPtrs((ST_BONE*)pBone->pFrameSibling);
	}

	if (pBone->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs((ST_BONE*)pBone->pFrameFirstChild);
	}
}

void cSkinnedMesh::SetAnimationIndex(int nIndex)
{
	if (!m_pAnimController)	return;
	LPD3DXANIMATIONSET pAnimSet = NULL;
	m_pAnimController->GetAnimationSet(nIndex, &pAnimSet);
	m_pAnimController->SetTrackAnimationSet(0, pAnimSet);
	SAFE_RELEASE(pAnimSet);
}

void cSkinnedMesh::SetAnimationIndexBlend(int nindex)
{
	if (!m_pAnimController)	return;

	m_pAnimController->ResetTime();
	m_isAnimBlend = true;
	m_fPassedBlendTime = 0.0f;
	int num = nindex % m_pAnimController->GetNumAnimationSets();

	LPD3DXANIMATIONSET pPrevAnimSet = NULL;
	LPD3DXANIMATIONSET pNextAnimSet = NULL;

	D3DXTRACK_DESC stTrackDesc;

	//prev
	m_pAnimController->GetTrackDesc(0, &stTrackDesc);
	m_pAnimController->GetTrackAnimationSet(0, &pPrevAnimSet);
	m_pAnimController->SetTrackAnimationSet(1, pPrevAnimSet);
	m_pAnimController->SetTrackDesc(1, &stTrackDesc);

	//next
	m_pAnimController->GetAnimationSet(num, &pNextAnimSet);
	m_pAnimController->SetTrackAnimationSet(0, pNextAnimSet);
	m_pAnimController->SetTrackPosition(0, 0.0f);

	//Blend
	m_pAnimController->SetTrackWeight(0, 0.0f);
	m_pAnimController->SetTrackWeight(1, 1.0f);

	SAFE_RELEASE(pPrevAnimSet);
	SAFE_RELEASE(pNextAnimSet);
}


void cSkinnedMesh::Destroy()
{
	cAllocateHierarchy ah;
	D3DXFrameDestroy((LPD3DXFRAME)m_pRootFrame, &ah);
	SAFE_DELETE_ARRAY(m_pmWorkingPalette);
	SAFE_RELEASE(m_pEffect);
}

void cSkinnedMesh::SetRandomTrackPosition()
{
	if (m_pAnimController)
		m_pAnimController->SetTrackPosition(0, (rand() % 100) / 10.0f);
}
