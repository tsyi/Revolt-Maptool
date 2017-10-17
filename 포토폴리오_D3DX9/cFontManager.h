#pragma once
#define MgrFont cFontManager::GetInstance()
enum eFontType {
	E_DEFAULT,
	E_QUEST,
	E_TITLE
	//...
};

class cFontManager
{
public:
	
private:
	SINGLETONE(cFontManager);

	std::map<eFontType, LPD3DXFONT> m_mapFont;
public:
	void Setup();
	void CreateFontDesc(eFontType type, std::string fontname, D3DXFONT_DESC fd);
	void CreateFontDesc(eFontType type, std::string fontname, int height, int width, int weight, bool italic,
		BYTE charSet, BYTE qutputPrecision, BYTE pitchAndFamily);
	void RegisterFont(eFontType type);

	LPD3DXFONT GetFont(eFontType fontType);

//	void GetDesc(eFontType fontType, D3DXFONT_DESC* pDesc);
//	void SetDesc(eFontType fontType, D3DXFONT_DESC pDesc);
	void Destory();
};

