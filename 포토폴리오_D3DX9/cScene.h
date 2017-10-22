#pragma once
class cObject;
class cMap;

class cScene
{
	std::string m_sceenName;
	SYNTHESIZE(cObject*, m_pMap, Map);
	SYNTHESIZE(std::vector<cObject*>, m_vecObject, Objcets);


	//임시변수;
	cObject* m_selectobj;

	//const std::string BaseFolder = "Object/Scene";

public:
	cScene();
	~cScene();

	void Setup();
	void Destory();
	void Update();
	void LastUpdate();
	void Render();

	void LoadScene(std::string FileName);
	void SaveScene(std::string FileName);

	void PushObject(cObject* obj)
	{
		m_vecObject.push_back(obj);
	}
	void OnChangeValue(int eventID);
};

