#include "stdafx.h"
#include "cObject.h"


cObject::cObject()
	: m_ulRefCount(1)
{
	// : add
	MgrObject->AddObject(this);
}


cObject::~cObject()
{
	// : remove 
	MgrObject->RemoveObject(this);
}

void cObject::AddRef()
{
	// : ref count ++
	++m_ulRefCount;
}

void cObject::Release()
{
	// : ref count -- 
	--m_ulRefCount;
	if (m_ulRefCount == 0)
		delete this;
}
