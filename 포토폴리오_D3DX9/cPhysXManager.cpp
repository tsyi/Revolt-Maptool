#include "stdafx.h"
#include "cPhysXManager.h"
#include "DEBUG_RENDERER.h"


#include "NxCooking.h"


cPhysXManager::cPhysXManager()
{
}


cPhysXManager::~cPhysXManager()
{
}

BOOL cPhysXManager::InitNxPhysX(DEBUG_RENDERER** pDebugRenderer)
{
	NxSceneDesc sceneDesc;


	m_pNxPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);
	if ((m_pNxPhysicsSDK) == NULL)
	{
		return S_FALSE;
	}

	m_pNxPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.0f);
	if (*pDebugRenderer == NULL)
	{
		m_pNxPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 3);
		m_pNxPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
		m_pNxPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);
		(*pDebugRenderer) = new DEBUG_RENDERER;
	}

	sceneDesc.gravity = NxVec3(0.0f, -9.81f, 0.0f);

	NxHWVersion HWVersion = m_pNxPhysicsSDK->getHWVersion();
	if (HWVersion != NX_HW_VERSION_NONE)
		sceneDesc.simType = NX_SIMULATION_HW;

	m_pNxScene = m_pNxPhysicsSDK->createScene(sceneDesc);
	if (m_pNxScene == NULL)
	{
		return S_FALSE;
	}

	return S_OK;
}

void cPhysXManager::Destory()
{
	if (m_pNxPhysicsSDK != NULL)
	{
		if (m_pNxScene != NULL)
		{
			m_pNxPhysicsSDK->releaseScene(*m_pNxScene);
			m_pNxScene = NULL;
		}
		NxReleasePhysicsSDK(m_pNxPhysicsSDK);
		m_pNxPhysicsSDK = NULL;
	}
}

NxTriangleMeshShapeDesc cPhysXManager::CreateTringleMesh(ID3DXMesh* pMesh, D3DXMATRIXA16* matS)
{
	NxTriangleMeshShapeDesc meshShapeDesc;
	meshShapeDesc.setToDefault();

	NxTriangleMesh* pTriangleMesh;
	NxTriangleMeshDesc meshDesc;
	NxU32 flg = 0;

	if (pMesh)
	{
		//메쉬 정보 복사
		ID3DXMesh* pTempMesh;
		pMesh->CloneMeshFVF(pMesh->GetOptions(), D3DFVF_XYZ, MgrD3DDevice, &pTempMesh);

		//인덱스의 사이즈를 얻기
		LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
		pTempMesh->GetIndexBuffer(&pIndexBuffer);
		D3DINDEXBUFFER_DESC indexDesc;
		pIndexBuffer->GetDesc(&indexDesc);
		switch (indexDesc.Format)
		{
		case D3DFMT_INDEX16:
			flg |= NX_CF_16_BIT_INDICES;
		}
		SAFE_RELEASE(pIndexBuffer);

		meshDesc.flags = flg;

		meshDesc.numVertices = pTempMesh->GetNumVertices();
		meshDesc.pointStrideBytes = pTempMesh->GetNumBytesPerVertex();
		meshDesc.numTriangles = pTempMesh->GetNumFaces();

		if (flg & NX_CF_16_BIT_INDICES)
			meshDesc.triangleStrideBytes = 3 * sizeof(WORD);
		else
			meshDesc.triangleStrideBytes = 3 * sizeof(DWORD);

		D3DXVECTOR3* pVec = new D3DXVECTOR3[meshDesc.numVertices];

		LPBYTE pIndex = NULL;
		LPBYTE pBits = NULL;

		pTempMesh->LockVertexBuffer(D3DLOCK_READONLY, (PVOID*)&pBits);
		CopyMemory(pVec, pBits, meshDesc.pointStrideBytes * meshDesc.numVertices);
		if (matS)
		{
			for (int i = 0; i < meshDesc.numVertices; i++)
			{
				D3DXVec3TransformCoord(&(pVec[i]), &(pVec[i]), matS);
			}
		}
		meshDesc.points = pVec;
		pTempMesh->UnlockVertexBuffer();

		pIndex = new BYTE[meshDesc.triangleStrideBytes * meshDesc.numTriangles];

		pTempMesh->LockIndexBuffer(D3DLOCK_READONLY, (PVOID*)&pBits);
		CopyMemory(pIndex, pBits, meshDesc.triangleStrideBytes * meshDesc.numTriangles);
		meshDesc.triangles = pIndex;
		pTempMesh->UnlockIndexBuffer();

		//Cooking 초기화
		NxInitCooking();
		MemoryWriteBuffer buf;
		if (NxCookTriangleMesh(meshDesc, buf))
		{
			pTriangleMesh = MgrPhysXSDK->createTriangleMesh(MemoryReadBuffer(buf.data));
			if (pTriangleMesh)
			{
				meshShapeDesc.meshData = pTriangleMesh;
			}
		}
		NxCloseCooking();

		SAFE_RELEASE(pTempMesh);
		SAFE_DELETE_ARRAY(pVec);
		SAFE_DELETE_ARRAY(pIndex);
	}
	return meshShapeDesc;
}
