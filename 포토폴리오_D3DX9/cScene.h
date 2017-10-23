#pragma once
class cObject;
class cMap;

class cScene
{
	std::string m_sceenName;
	SYNTHESIZE(cObject*, m_pMap, Map);
	SYNTHESIZE(std::vector<cObject*>, m_vecObject, Objects);


	//임시변수;

	//const std::string BaseFolder = "Object/Scene";
	cObject* m_selectobj;

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

	//
//	void SaveScene();


	void OnChangePhysX(int eventID);

	void PushObject(cObject* obj)
	{
		m_vecObject.push_back(obj);
	}
	void OnChangeValue(int eventID, std::string eventKey);
	void OnChangeValueNx(int eventID, std::string eventKey);
	// -> 물리 내용 변경부 따로 만들기.

};

