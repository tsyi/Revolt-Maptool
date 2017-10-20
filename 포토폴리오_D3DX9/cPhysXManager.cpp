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

void cPhysXManager::Update()
{
//
//	//	MgrPhysXScene->simulate(MgrTime->GetElapsedTime());	//프레임 지정
//	//	MgrPhysXScene->flushStream();
//	//	MgrPhysXScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
//
//
//	NxU32 ContactPairFlag = 0;
//	NxU32 actorCount = MgrPhysXScene->getNbActors();
//	for (NxU32 i = 0; i < actorCount; i++)
//	{
//		NxActor* pActor = MgrPhysXScene->getActors()[i];
//
//		pActor->setName((char*)this);
//		
//		if (pActor == NULL) continue;
//		if (!ContactPairFlag)
//		{
//			USERDATA* UserData = (USERDATA*)pActor->userData;
//
//			if (UserData)
//			{
//				ContactPairFlag = UserData->ContactPairFlag;
//				//	pActor->userData = 0;
//			}
//			//std::cout << ContactPairFlag << std::endl;
//		}
//
//		if (pActor->getName() != NULL)
//		{
//			if (pActor->getName() == TestName)
//			{
//				D3DXMatrixIdentity(&matMeshTest);
//
//				matMeshTest._41 = pActor->getGlobalPose().t.x;
//				matMeshTest._42 = pActor->getGlobalPose().t.y;
//				matMeshTest._43 = pActor->getGlobalPose().t.z;
//
//				NxF32 mtl[3 * 3];
//				pActor->getGlobalPose().M.getColumnMajor(mtl);
//				matMeshTest._11 = mtl[0];
//				matMeshTest._12 = mtl[1];
//				matMeshTest._13 = mtl[2];
//				matMeshTest._21 = mtl[3];
//				matMeshTest._22 = mtl[4];
//				matMeshTest._23 = mtl[5];
//				matMeshTest._31 = mtl[6];
//				matMeshTest._32 = mtl[7];
//				matMeshTest._33 = mtl[8];
//			}
//			if (pActor->getName() == TeapotName)
//			{
//				D3DXMATRIXA16 matWorld;
//				D3DXMatrixIdentity(&matWorld);
//
//				matWorld._41 = pActor->getGlobalPose().t.x;
//				matWorld._42 = pActor->getGlobalPose().t.y;
//				matWorld._43 = pActor->getGlobalPose().t.z;
//
//				NxF32 mtl[3 * 3];
//				pActor->getGlobalPose().M.getColumnMajor(mtl);
//				matWorld._11 = mtl[0];
//				matWorld._12 = mtl[1];
//				matWorld._13 = mtl[2];
//				matWorld._21 = mtl[3];
//				matWorld._22 = mtl[4];
//				matWorld._23 = mtl[5];
//				matWorld._31 = mtl[6];
//				matWorld._32 = mtl[7];
//				matWorld._33 = mtl[8];
//
//				TeapotTr.SetQuaternion(matWorld);
//				TeapotTr.SetPosition(matWorld);
//			}
//			if (pActor->getName() == strCarName)
//			{
//
//				D3DXMATRIXA16 matWorld;
//				D3DXMatrixIdentity(&matWorld);
//
//				matWorld._41 = pActor->getGlobalPose().t.x;
//				matWorld._42 = pActor->getGlobalPose().t.y;
//				matWorld._43 = pActor->getGlobalPose().t.z;
//
//				NxF32 mtl[3 * 3];
//				pActor->getGlobalPose().M.getColumnMajor(mtl);
//				matWorld._11 = mtl[0];
//				matWorld._12 = mtl[1];
//				matWorld._13 = mtl[2];
//				matWorld._21 = mtl[3];
//				matWorld._22 = mtl[4];
//				matWorld._23 = mtl[5];
//				matWorld._31 = mtl[6];
//				matWorld._32 = mtl[7];
//				matWorld._33 = mtl[8];
//
//
//				carTr.SetQuaternion(matWorld);
//				carTr.SetPosition(matWorld);
//			}
//		}
//	}
}
