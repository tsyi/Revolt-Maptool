#pragma once

#include <sstream>

// USES_CONVERSION ��ũ��
#include "atlconv.h"
#include "atlBase.h"
#pragma comment(lib, "atls.lib")
/*
ATL 3.0
USES_CONVERSION�� ����� �Լ��� ����Ǹ� �޸𸮰� �����ǹǷ� �����Ұ�.

��ũ��	����		���
A2CW	(LPCSTR)	(LPCWSTR)
A2W		(LPCSTR)	(LPWSTR)	(string->wstring)
W2CA	(LPCWSTR)	(LPCSTR)
W2A		(LPCWSTR)	(LPSTR)		(wstring->string)

T2COLE	(LPCTSTR)	(LPCOLESTR)
T2OLE	(LPCTSTR)	(LPOLESTR)
OLE2CT	(LPCOLESTR)	(LPCTSTR)
OLE2T	(LPCOLESTR)	(LPCSTR)

��)
void Func()
{
USES_CONVERSION;

TCHAR widechar[]=L"sample";
char ansichar[10];
ansichar = W2A(widechar);
}

ATL 7.0 ���ڿ� ��ȯ
CA2AEX		�� Ŭ������ ��� �� �� ���ڿ� ��ȯ ��ũ�ο��� CA2TEX �� CT2AEX, �� typedef CA2A.
CA2CAEX		�� Ŭ������ ��� �� �� ���ڿ� ��ȯ ��ũ�ο��� CA2CTEX �� CT2CAEX, �� typedef CA2CA.
CA2WEX		�� Ŭ������ ��� �� �� ���ڿ� ��ȯ ��ũ�ο��� CA2TEX, CA2CTEX, CT2WEX, �� CT2CWEX, �� typedef CA2W.
CW2AEX		�� Ŭ������ ��� �� �� ���ڿ� ��ȯ ��ũ�ο��� CT2AEX, CW2TEX, CW2CTEX, �� CT2CAEX, �� typedef CW2A.
CW2CWEX		�� Ŭ������ ��� �� �� ���ڿ� ��ȯ ��ũ�ο��� CW2CTEX �� CT2CWEX, �� typedef CW2CW.
CW2WEX		�� Ŭ������ ��� �� �� ���ڿ� ��ȯ ��ũ�ο��� CW2TEX �� CT2WEX, �� typedef CW2W.
(�� ���� EX�� �����ؼ� ��� ����)

CComBSTR		�� �� �� ���� Ŭ������ BSTRs.
_U_STRINGorID	�� �μ� ����� Ŭ���� �� ���ҽ� �̸��� �ֽ��ϴ� 
				(LPCTSTRs) �Ǵ� ���ҽ� Id (UINTs) ȣ���� ID�� ��� �Ͽ�
				���ڿ��� ��ȯ�� �ʿ� ���� �Լ��� ���� �ϴ� MAKEINTRESOURCE ��ũ��.
*/

class cStringUtil
{
public:
	cStringUtil();
	~cStringUtil();

public:
	//����->����
	static int ToInt(std::string text) { return atoi(text.c_str()); }
	static float ToFloat(std::string text) { return atof(text.c_str()); }
	static int ToInt(std::wstring text) { return _wtoi(text.c_str()); }
	static float ToFloat(std::wstring text) { return _wtof(text.c_str()); }

	//stringstream�� �̿�. ����->����
	static std::string ToString(int value) { std::stringstream ss; ss << value; return ss.str(); }
	static std::string ToString(float value) { std::stringstream ss; ss << value; return ss.str(); }
	static std::string ToString(CHAR text) { std::stringstream ss; ss << text; return ss.str(); }
	
	static std::wstring ToWString(int value) { std::wstringstream ss; ss << value; return ss.str(); }
	static std::wstring ToWString(float value) { std::wstringstream ss; ss << value; return ss.str(); }
	static std::wstring ToWString(WCHAR text) { std::wstringstream ss; ss << text; return ss.str(); }

	//ATL 3.0 ��ȯ ��ũ��  USES_CONVERSION ��� (���� ��ȯ) (loop ����)
	static std::string ToString_c(std::wstring text)
	{
		USES_CONVERSION; return W2A(text.c_str());
	}
	//ATL 3.0 ��ȯ ��ũ��  USES_CONVERSION ��� (���� ��ȯ) (loop ����)
	static std::wstring ToWString_c(std::string text)
	{
		USES_CONVERSION; return A2W(text.c_str());
	}
	
	//ATL 7.0 ��ȯ Ŭ����
	static std::string ToString(std::wstring text)
	{
		return ATL::CW2A(text.c_str());
	}
	//ATL 7.0 ��ȯ Ŭ����
	static std::wstring ToWString(std::string text)
	{
		return ATL::CA2W(text.c_str());
	}
	//�ڼ��� ����(ATL) : "https://technet.microsoft.com/ko-kr/library/87zae4a3(v=vs.110)"


private:
	void TestFunc()
	{
		
	}
};

