#include "stdafx.h"
#include "cPhysX.h"


cPhysX::cPhysX()
{
	m_pActor = NULL;
	m_pUserData = new USERDATA;

	actorName = "";
	m_position = NxVec3(0, 0, 0);
	m_sizeValue = NxVec3(1, 0, 0);

	m_type = NX_SHAPE_SPHERE;
}


cPhysX::~cPhysX()
{
}
