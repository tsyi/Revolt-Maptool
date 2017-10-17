#pragma once

class cGrid
{
private:

	int							m_nNumLine;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;

public:
	cGrid(void);
	~cGrid(void);

	void Setup(int nNumHalfTile = 15, int fInterval = 1);
	void Render();
};

