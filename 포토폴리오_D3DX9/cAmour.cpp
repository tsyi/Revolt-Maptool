#include "stdafx.h"
#include "cAmour.h"


cAmour::cAmour()
{
}


cAmour::~cAmour()
{
}

void cAmour::Setup()
{
}

int cAmour::GetAmourValue()
{
	return m_pAmourData->m_def;
}

E_AmourType cAmour::GetAmourType()
{
	return m_pAmourData->m_type;
}
