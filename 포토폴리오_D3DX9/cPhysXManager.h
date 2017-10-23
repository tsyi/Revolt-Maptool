#pragma once
#include <NxPhysics.h>
#include <NxCooking.h>
#include "Stream.h"


#define MgrPhysX		cPhysXManager::GetInstance()
#define MgrPhysXScene	cPhysXManager::GetInstance()->GetPhysXScene()
#define MgrPhysXSDK		cPhysXManager::GetInstance()->GetPhysXSDK()
#define MgrPhysXData	cPhysXManager::GetInstance()->GetPhysXData()

class DEBUG_RENDERER;

struct PHYSXDATA
{
	int RaycastAllShapeHitCount;
	NxVec3	RaycastClosestShapePosition;
	PHYSXDATA() { Init(); }
	void Init()
	{
		RaycastAllShapeHitCount = 0;
		RaycastClosestShapePosition = NxVec3(0, 0, 0);
	}
};
struct USERDATA
{
	NxU32 ContactPairFlag;
	NX_BOOL RaycastClosestShape;
	NX_BOOL RaycastAllShape;
	NxVec3	RayHitPos;
	USERDATA() { Init(); }
	void Init()
	{
		ContactPairFlag = 0;
		RaycastClosestShape = NX_FALSE;
		RaycastAllShape = NX_FALSE;
		RayHitPos = NxVec3(0, 0, 0);
	}
};

struct KinematicActor {
	NxActor* actor;
	NxVec3 vel;
};
//NxArray<KinematicActor>	gKinematicActors;

class ContactCallBack : public NxUserContactReport
{
	//�浹Ȯ��
	void onContactNotify(NxContactPair& pair, NxU32 _event);
};
class TriggerCallback : public NxUserTriggerReport
{
	//�浹Ȯ��
	void onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status);
};

class RaycastCallBack : public NxUserRaycastReport
{
	//Ray Ȯ��
	virtual bool onHit(const NxRaycastHit& hit);
};

enum E_MATERIAL
{
	E_MATERIAL_DEFAULT = 0,
	E_MATERIAL_1,
	E_MATERIAL_2,
	E_MATERIAL_3,
	E_MATERIAL_4,
	E_MATERIAL_5,
};
enum eShapeTag
{
	E_SHAPE_NONE,
	E_SHAPE_BOX,
	E_SHAPE_SPHERE,
	E_SHAPE_PLANE,
	E_SHAPE_TRIANGLE,
};

class cPhysXManager
{
public:
	SINGLETONE(cPhysXManager);

	SYNTHESIZE(PHYSXDATA*, m_physXUserData, PhysXData);
private:

	NxPhysicsSDK*	m_pNxPhysicsSDK;
	NxScene*		m_pNxScene;

	std::map<E_MATERIAL, NxMaterial*> m_mapMaterial;
public:
	NxPhysicsSDK* GetPhysXSDK() { return m_pNxPhysicsSDK; }
	NxScene* GetPhysXScene() { return m_pNxScene; }
	BOOL InitNxPhysX(DEBUG_RENDERER** pDebugRenderer);

	void Update();
	void Destory();

	NxTriangleMeshShapeDesc CreateTringleMesh(ID3DXMesh* pMesh, D3DXMATRIXA16* matS = NULL);
	NxBoxShapeDesc CreateBoxShape(int materialIndex, NxVec3 boxSize);






	NxVec3 D3DVecToNxVec(D3DXVECTOR3& d3d)
	{
		NxVec3 vec;
		vec.x = d3d.x;
		vec.y = d3d.y;
		vec.z = d3d.z;
		return vec;
	}
	void D3DMatToNxMat(NxF32* nx, D3DMATRIX& dx)
	{
		nx[0] = dx._11;
		nx[1] = dx._12;
		nx[2] = dx._13;
		nx[3] = dx._21;
		nx[4] = dx._22;
		nx[5] = dx._23;
		nx[6] = dx._31;
		nx[7] = dx._32;
		nx[8] = dx._33;
	}

	void RaycastClosestShape(D3DXVECTOR3 start, D3DXVECTOR3 dir);
	void RaycastAllShapes(D3DXVECTOR3 start, D3DXVECTOR3 dir);


	//CreateActor Actor �� ����� �Ŵ����� ����� ��ȯ�Ѵ�.
	//1
	//NxShapeType
	//NX_SHAPE_SPHERE		:	NxVec3(radius, 0, 0);
	//NX_SHAPE_BOX			:	NxVec3(width/2, height/2, deep/2);
	//NX_SHAPE_CAPSULE		:	NxVec3(radius, height, 0);
	//NX_SHAPE_WHEEL		:	NxVec3(radius, 0, 0);
	//2
	//NxU32 : NxBodyFlag
	//0, NULL							:	���� ��ü
	//NX_BF_DISABLE_GRAVITY				:	�߷¹���ȭ
	//(default) NX_BF_VISUALIZATION		:	�ð�ȭ
	//NX_BF_KINEMATIC					:	�ٸ� ��ü�� ������ �浹�� ���� ����.					
	//NX_BF_ENERGY_SLEEP_TEST			:	������ ��� ���� �˰����� Ȱ��ȭ (default)
	//NX_BF_FILTER_SLEEP_VEL			:	�������� ������ ����
	//3
	//USERDATA*							:	���� ������(�浹üũ)
	//4	
	//NxVec3 (x, y, z);					:	type �� �ٸ� ������
	//5	
	//materialIndex						:	��������(�̸� �޴����� ��Ͽܾ��־�� �Ѵ�)
	//density							:	�е� ���� (ũ�Ⱑ Ŀ���� �翬�� �� ���Դ� �����Ѵ�.)
	//6	
	//trMatrix							:	������ Matrix���� ( = matR * matT) 
	NxActor* CreateActor(NxShapeType type, NxU32 shapeFlag, NxU32 bodyFlag, bool isStatic,
		USERDATA* pUserData, NxVec3 sizeValue, int materialIndex, float density
		, D3DXMATRIX trMatrix)
	{

		NxActorDesc actorDesc;	actorDesc.setToDefault();
		NxBodyDesc bodyDesc;	bodyDesc.setToDefault();
		NxShapeDesc* shapeDesc = NULL;

		switch (type)
		{
		case NX_SHAPE_PLANE: {
			break;
		}
		case NX_SHAPE_SPHERE: {
			NxSphereShapeDesc desc; desc.setToDefault();
			desc.materialIndex - materialIndex;
			desc.radius = sizeValue.x;
			break;
		}
		case NX_SHAPE_BOX: {
			NxBoxShapeDesc desc; desc.setToDefault();
			desc.materialIndex = materialIndex;
			desc.dimensions = sizeValue;
			desc.shapeFlags |= shapeFlag;
			shapeDesc = &desc;
			break;
		}
		case NX_SHAPE_CAPSULE: {
			NxCapsuleShapeDesc desc; desc.setToDefault();
			desc.materialIndex = materialIndex;
			desc.radius = sizeValue.x;
			desc.height = sizeValue.y;
			shapeDesc = &desc;
			break;
		}
		case NX_SHAPE_WHEEL: {
			NxWheelShapeDesc desc; desc.setToDefault();
			desc.materialIndex = materialIndex;
			desc.radius = 0;
			shapeDesc = &desc;
			break;
		}
		case NX_SHAPE_CONVEX: {
			break;
		}
		case NX_SHAPE_MESH: {
			break;
		}
		case NX_SHAPE_HEIGHTFIELD: {
			break;
		}
		case NX_SHAPE_RAW_MESH: {
			break;
		}
		case NX_SHAPE_COMPOUND: {
			break;
		}
		case NX_SHAPE_COUNT: {
			break;
		}
		case NX_SHAPE_FORCE_DWORD: {
			break;
		}
		default:
			break;
		}
		actorDesc.shapes.pushBack(shapeDesc);

		if (isStatic)
		{
			bodyDesc.flags |= bodyFlag;
			actorDesc.body = &bodyDesc;
			actorDesc.density = density;
		}
		else
		{
			actorDesc.body = NULL;
		}

		actorDesc.globalPose.t = NxVec3(trMatrix._41, trMatrix._42, trMatrix._43);
		NxF32 mtl[9];
		mtl[0] = trMatrix._11;
		mtl[1] = trMatrix._12;
		mtl[2] = trMatrix._13;
		mtl[3] = trMatrix._21;
		mtl[4] = trMatrix._22;
		mtl[5] = trMatrix._23;
		mtl[6] = trMatrix._31;
		mtl[7] = trMatrix._32;
		mtl[8] = trMatrix._33;
		actorDesc.globalPose.M.setColumnMajor(mtl);

		//		mtl = NULL;

		if (pUserData)
		{
			pUserData->ContactPairFlag = 0;
			pUserData->RaycastClosestShape = NX_FALSE;
			actorDesc.userData = pUserData;
		}

		return MgrPhysXScene->createActor(actorDesc);
	}


	//CreateActor
	//1
	//NxShapeType				sizeValue
	//NX_SHAPE_SPHERE		:	NxVec3(radius, 0, 0);
	//NX_SHAPE_BOX			:	NxVec3(width/2, height/2, deep/2);
	//NX_SHAPE_CAPSULE		:	NxVec3(radius, height, 0);
	//NX_SHAPE_WHEEL		:	NxVec3(radius, 0, 0);
	//2
	//position ��ġ , ȸ��
	//3
	//�浹 ũ��
	//4
	//���� ������
	//5 
	//Kinematic -> �ٸ� ��ü���� ���� ȿ���� ���� ����
	//IsTrigger -> Ʈ���� �浹�� �߻�
	//
	//        (T,T)    (T,F)    (F,T)    (F,F)
	// (F,F)   O/O      0/N      O/O      O/N
	// (F,T)    /        /        /        /
	// (T,F)    /        /        0        X
	// (T,T)    /        /        /        O
	//
	//
	NxActor* CreateActor(NxShapeType type, NxVec3 position, NxF32* mat, NxVec3 sizeValue, USERDATA* pUserData,
		bool IsTrigger = false, bool isStatic = false, bool isGravaty = true)
	{
		bool isKinematic = false;
		// Our trigger is a cube

		NxShapeDesc* shapeDesc = NULL;

		NxActorDesc ActorDesc;
		ActorDesc.setToDefault();

		switch (type)
		{
		case NX_SHAPE_PLANE: {
			break;
		}
		case NX_SHAPE_SPHERE: {
			NxSphereShapeDesc desc; desc.setToDefault();

			desc.setToDefault();
			desc.radius = sizeValue.x;
			desc.materialIndex = 0;
			shapeDesc = &desc;

			if (isKinematic)
			{
				NxSphereShapeDesc dummyShape;
				dummyShape.setToDefault();
				dummyShape.radius = sizeValue.x;
				ActorDesc.shapes.pushBack(&dummyShape);
			}
			break;
		}
		case NX_SHAPE_BOX: {
			NxBoxShapeDesc desc;
			desc.setToDefault();
			desc.dimensions.set(sizeValue);
			desc.materialIndex = 0;
			shapeDesc = &desc;

			if (isKinematic)
			{
				NxBoxShapeDesc dummyShape;
				dummyShape.setToDefault();
				dummyShape.dimensions.set(sizeValue);
				ActorDesc.shapes.pushBack(&dummyShape);
			}
			break;
		}
		case NX_SHAPE_CAPSULE: {
			NxCapsuleShapeDesc desc; desc.setToDefault();
			//	desc.materialIndex = materialIndex;
			desc.radius = sizeValue.x;
			desc.height = sizeValue.y;
			shapeDesc = &desc;
			break;
		}
		case NX_SHAPE_WHEEL: {
			NxWheelShapeDesc desc; desc.setToDefault();
			//	desc.materialIndex = materialIndex;
			desc.radius = 0;
			shapeDesc = &desc;
			break;
		}
		case NX_SHAPE_CONVEX: {
			break;
		}
		case NX_SHAPE_MESH: {
			break;
		}
		case NX_SHAPE_HEIGHTFIELD: {
			break;
		}
		case NX_SHAPE_RAW_MESH: {
			break;
		}
		case NX_SHAPE_COMPOUND: {
			break;
		}
		case NX_SHAPE_COUNT: {
			break;
		}
		case NX_SHAPE_FORCE_DWORD: {
			break;
		}
		default:break;
		}
		if (shapeDesc == NULL) return NULL;

		NxBodyDesc triggerBody;
		triggerBody.setToDefault();


		if (!isGravaty) triggerBody.flags |= NX_BF_DISABLE_GRAVITY;

		if (isKinematic&& IsTrigger)
		{
			shapeDesc->shapeFlags |= NX_TRIGGER_ENABLE;
			triggerBody.flags |= NX_BF_KINEMATIC;

			ActorDesc.body = &triggerBody;
			//	ActorDesc.body = NULL;
		}
		if (isKinematic && !IsTrigger)
		{
			triggerBody.flags |= NX_BF_KINEMATIC;

			ActorDesc.body = &triggerBody;
		}
		if (!isKinematic&& IsTrigger)
		{
			shapeDesc->shapeFlags |= NX_TRIGGER_ENABLE;

			//	ActorDesc.body = NULL;
		}
		if (!isKinematic && !IsTrigger)
		{
			ActorDesc.body = &triggerBody;
		}

		if (isStatic) ActorDesc.body = NULL;

		ActorDesc.density = 10.f;
		ActorDesc.shapes.pushBack(shapeDesc);
		ActorDesc.globalPose.t = position;

		if (mat == NULL)
		{
			NxF32 mat_[9] = { 1,0,0,0,1,0,0,0,1 };
			mat = mat_;
		}

		ActorDesc.globalPose.M.setColumnMajor(mat);

		if (pUserData != NULL) ActorDesc.userData = (pUserData);

		NxActor* actor = m_pNxScene->createActor(ActorDesc);
		if (actor == NULL)
		{
			std::cout << "NULL";
		}
		return actor;
	}
};