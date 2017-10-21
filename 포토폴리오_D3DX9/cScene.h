#pragma once
class cObject;

class cScene
{
	cObject* m_pMap;
	std::vector<cObject*> m_vecObject;

public:
	cScene();
	~cScene();

	void Setup();
	void Destory();
	void Update();
	void Render();

	//TEST
//	void LoadMap();
//	void ChanageMap();

	void PushObject(cObject* obj)
	{
		m_vecObject.push_back(obj);
	}


};

