#include "stdafx.h"
#include "cScene.h"


cScene::cScene()
{
}


cScene::~cScene()
{
}

void cScene::Setup()
{
	
}

void cScene::Destory()
{
}

void cScene::Update()
{
	if(m_pMap) m_pMap->Update();
	for each (cObject* pObj in m_vecObject)
	{
		pObj->Update();
	}
}

void cScene::Render()
{
	if(m_pMap) m_pMap->Render();
	for each (cObject* pObj in m_vecObject)
	{
		pObj->Render();
	}
}
