#pragma once
class cObject;

class cScene
{
	cObject* m_pMap;
	std::vector<cObject*> m_vecLights;

public:
	cScene();
	~cScene();

	void Update()
	{

	}
	HRESULT LoadScene(std::string FileName);
	HRESULT SaveScene(std::string FileName);

};

