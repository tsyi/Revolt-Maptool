#pragma once

#include <fstream>
//#include <fstream>

#include <iostream>
#include <fstream>

class cPhysX
{
public:
	NxActor*  m_pAxtor;		//save/load (X)
	USERDATA* m_pUserData;	//save/load (X)

	cPhysX();
	~cPhysX();

	std::string actorName;						//save/load
	NxVec3 m_position;							//save/load
	NxVec3 m_sizeValue;							//save/load
	NxF32 m_matR[9] = { 1,0,0,0,1,0,0,0,1 };	//save/load


	bool m_IsTrigger = false;	//save/load
	bool m_isStatic = false;	//save/load
	bool m_isGravaty = true;	//save/load

	NxShapeType m_type;			//save/load


	void SetPosition(NxVec3 pos)
	{
		m_pAxtor->getGlobalPose().t = pos;
	}
	void SetPosition(D3DXVECTOR3 vec3)
	{
		SetPosition(NxVec3(vec3.x, vec3.y, vec3.z));
	}
	void SetRotation(D3DXMATRIX mat16)
	{
		NxF32 nxf[9] = { 1,0,0,0,1,0,0,0,1 };

		nxf[0] = mat16._11;
		nxf[1] = mat16._12;
		nxf[2] = mat16._13;
		nxf[3] = mat16._21;
		nxf[4] = mat16._22;
		nxf[5] = mat16._23;
		nxf[6] = mat16._31;
		nxf[7] = mat16._32;
		nxf[8] = mat16._33;

		SetRotation(nxf);
	}
	void SetRotation(NxF32* nxf32)
	{
		NxMat33 mat33;
		mat33.setColumnMajor(nxf32);
		SetRotation(mat33);
	}
	void SetRotation(NxMat33 mat33)
	{
		m_pAxtor->getGlobalPose().M = mat33;
	}

	NxVec3 GetPositionToNxVec3()
	{
		return m_pAxtor->getGlobalPose().t;
	}
	D3DXVECTOR3 GetPositionToD3DXVec3()
	{
		NxVec3 pos = m_pAxtor->getGlobalPose().t;
		return D3DXVECTOR3(pos.x, pos.y, pos.z);
	}
	NxMat33 GetRotationToNxMat33()
	{
		return m_pAxtor->getGlobalPose().M;
	}
	D3DXMATRIXA16 GetRotationToD3DXMat16()
	{
		D3DXMATRIXA16 mat16;
		D3DXMatrixIdentity(&mat16);

		NxF32 mat[9] = { 1,0,0,0,1,0,0,0,1 };
		m_pAxtor->getGlobalPose().M.getColumnMajor(mat);

		mat16._11 = mat[0];
		mat16._12 = mat[1];
		mat16._13 = mat[2];
		mat16._21 = mat[3];
		mat16._22 = mat[4];
		mat16._23 = mat[5];
		mat16._31 = mat[6];
		mat16._32 = mat[7];
		mat16._33 = mat[8];

		return mat16;
	}


#define TAB	'\t'
	void LoadPhysX(std::string fileName)
	{
		std::string fullpath = "Object/Scene/" + fileName + ".phy";

		std::ifstream LOAD(fullpath);

		if (LOAD.is_open())
		{
			while (!LOAD.eof())
			{
				CHAR text_[1024] = {};
				LOAD.getline(text_, 1024);
				std::string text = text_;

				std::vector<CHAR> Cut;
				Cut.push_back(' ');
				Cut.push_back(TAB);

				std::vector<std::string> Data;
				cStringUtil::Split(&Data, &Cut, &text);

				if (Data[0] == "");
			}
		}
	}

	void SavePhysX(std::string fileName)
	{
		std::string fullpath = "Object/Scene/" + fileName + ".phy";

		std::ofstream SAVE(fullpath);

		if (SAVE.is_open())
		{
			SAVE << fileName << std::endl;

			SAVE << "SHAPE_TYPE" << TAB << (int)m_type << std::endl;

			SAVE << "OPTION" << std::endl;
			SAVE << TAB << "IsTrigger" << TAB << m_IsTrigger << std::endl;
			SAVE << TAB << "isStatic"  << TAB << m_isStatic << std::endl;
			SAVE << TAB << "isGravaty" << TAB << m_isGravaty << std::endl;

			SAVE << "POS_XYZ" << TAB
				<< m_position.x << TAB 
				<< m_position.y << TAB 
				<< m_position.z
				<< std::endl;

			SAVE << "SIZE_XYZ" << TAB
				<< m_sizeValue.x << TAB
				<< m_sizeValue.y << TAB
				<< m_sizeValue.z
				<< std::endl;

			SAVE << "FXU32_[9]" << TAB
				<< m_matR[0] << TAB << m_matR[1] << TAB << m_matR[2]
				<< m_matR[3] << TAB << m_matR[4] << TAB << m_matR[5]
				<< m_matR[6] << TAB << m_matR[7] << TAB << m_matR[8]
				<< std::endl;


		}
	}

	NxActor* CreateActor(NxShapeType type, NxVec3 position, NxF32* mat, NxVec3 sizeValue, USERDATA* pUserData,
		bool IsTrigger = false, bool isStatic = false, bool isGravaty = true)
	{
		actorName =
			m_type = type;

		m_position = position;
		m_matR[0] = mat[0];
		m_matR[1] = mat[1];
		m_matR[2] = mat[2];
		m_matR[3] = mat[3];
		m_matR[4] = mat[4];
		m_matR[5] = mat[5];
		m_matR[6] = mat[6];
		m_matR[7] = mat[7];
		m_matR[8] = mat[8];


		bool isKinematic = false;
		// Our trigger is a cube
		NxBodyDesc triggerBody;
		triggerBody.setToDefault();

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
			//	desc.materialIndex - materialIndex;
			desc.radius = sizeValue.x;
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
			shapeDesc->shapeFlags = NX_TRIGGER_ENABLE;

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
		ActorDesc.globalPose.M.setColumnMajor(mat);

		ActorDesc.userData = (pUserData);

		NxActor* actor = MgrPhysXScene->createActor(ActorDesc);
		if (actor == NULL)
		{
			std::cout << "NULL";
		}
		return actor;
	}

};

