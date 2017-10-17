#include "stdafx.h"
#include "cFontManager.h"


cFontManager::cFontManager()
{
}


cFontManager::~cFontManager()
{
}

void cFontManager::Setup()
{
	D3DXFONT_DESC fd; // 폰트의 성질
	eFontType fontType;
	ZeroMemory(&fd, sizeof(D3DXFONT_DESC));
	AddFontResourceA("font/umberto.ttf");


	fd.Height = 25;
	fd.Width = 12;
	fd.Weight = FW_BOLD;
	fd.Italic = false;
	fd.CharSet = DEFAULT_CHARSET;
	fd.OutputPrecision = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily = FF_DONTCARE;
	CreateFontDesc(E_DEFAULT, "굴림체", fd);

	fd.Height = 50;
	fd.Width = 20;
	fd.Weight = FW_LIGHT;
	fd.Italic = false;
	fd.CharSet = DEFAULT_CHARSET;
	fd.OutputPrecision = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily = FF_DONTCARE;

	CreateFontDesc(E_QUEST, "궁서체", fd);

	fd.Height = 20;
	fd.Width = 15;
	fd.Weight = FW_MEDIUM;
	fd.Italic = false;
	fd.CharSet = DEFAULT_CHARSET;
	fd.OutputPrecision = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily = FF_DONTCARE;
	//	AddFontResourceA("font/umberto.ttf");
	CreateFontDesc(E_TITLE, "umberto", fd);



	//	for each (auto font in m_mapFd)
	//	{
	//		D3DXCreateFontIndirect(GetDevice, &font.second, &m_mapFont[font.first]);
	//	}
}

void cFontManager::CreateFontDesc(eFontType type, std::string fontname, D3DXFONT_DESC fd)
{
	wcscpy_s(fd.FaceName, cStringUtil::ToWString(fontname).c_str());
	D3DXCreateFontIndirect(MgrD3DDevice, &fd, &m_mapFont[type]);
}

void cFontManager::CreateFontDesc(eFontType type, std::string fontname, int height, int width, int weight, bool italic,
	BYTE charSet, BYTE qutputPrecision, BYTE pitchAndFamily)
{
	D3DXFONT_DESC fd;
	fd.Height = height;
	fd.Width = width;
	fd.Weight = weight;
	fd.Italic = italic;
	fd.CharSet = charSet;
	fd.OutputPrecision = qutputPrecision;
	fd.PitchAndFamily = pitchAndFamily;

	CreateFontDesc(type, fontname, fd);
}

void cFontManager::RegisterFont(eFontType type)
{

}

LPD3DXFONT cFontManager::GetFont(eFontType fontType)
{
	if (m_mapFont.find(fontType) != m_mapFont.end())//있으면
	{
		return m_mapFont[fontType];
	}
	return NULL;
	//없으면...
//	D3DXCreateFontIndirect(GetDevice, &fd, &m_mapFont[fontType]);
//
//	return m_mapFont[fontType];
}

//void cFontManager::GetDesc(eFontType fontType, D3DXFONT_DESC* pDesc)
//{
//	if (GetFont(fontType))
//	{
//		GetFont(fontType)->GetDesc(pDesc);
//
//		LPD3DXFONT a;
//		
//	}
//}
//
//void cFontManager::SetDesc(eFontType fontType, D3DXFONT_DESC pDesc)
//{
//	if (GetFont(fontType))
//	{
//		D3DXCreateFontIndirect(GetDevice, &pDesc, &m_mapFont[fontType]);
//	}
//}

void cFontManager::Destory()
{
	for each (auto it in m_mapFont)
	{
		SAFE_RELEASE(it.second);
	}
	m_mapFont.clear();
}
