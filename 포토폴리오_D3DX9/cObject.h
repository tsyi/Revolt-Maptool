#pragma once
#include "cTransform.h"
class cObject : public cTransform
{
public:
	cObject();
	//~cObject();
	// >> : 
protected:
	ULONG	m_ulRefCount;

public:
	virtual ~cObject(void);
	virtual void AddRef();
	virtual void Release();
	// << : 
};

