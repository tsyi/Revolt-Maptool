#pragma once

#define MgrObject	cObjectManager::GetInstance()

class cObject;

class cObjectManager
{
	// >> : 
	//public:
	//	cObjectManager();
	//	~cObjectManager();

private:
	SINGLETONE(cObjectManager);

private:
	std::set<cObject*>	m_setObject;

public:
	void AddObject(cObject* pObject)
	{
		// : add 
		m_setObject.insert(pObject);
	}

	void RemoveObject(cObject* pObject)
	{
		// : remove
		m_setObject.erase(pObject);
	}

	void Destroy()
	{
		// : clear
		m_setObject.clear();

	}
	// << : 
};

