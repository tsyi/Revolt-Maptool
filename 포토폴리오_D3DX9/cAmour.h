#pragma once

enum E_AmourType { E_A_LIGHT, E_A_HEAVY };
struct AmourData
{
	int m_def;
	E_AmourType m_type;
	AmourData(E_AmourType type, int def)
	{
		m_type = type;
		m_def = def;
	}
};

class cAmour
{
protected:
	AmourData* m_pAmourData;
	int m_amourLevel;
public:
	cAmour();
	~cAmour();

	virtual void Setup();
	virtual int GetAmourValue();
	virtual E_AmourType GetAmourType();
};

