#include<String>
#include<iostream>
#include<windows.h>
#define _INIPATH_ "C:\\Users\\Public\\Documents\\McportSetting.ini"
std::string UTF82Char(const char* szU8)
{
    std::string ans;
    int wcsLen = MultiByteToWideChar(CP_UTF8, NULL, szU8, (int)strlen(szU8), NULL, 0);
    wchar_t* wszString = new wchar_t[wcsLen + 1];
    MultiByteToWideChar(CP_UTF8, NULL, szU8, (int)strlen(szU8), wszString, wcsLen);
    wszString[wcsLen] = '\0';
    int len = WideCharToMultiByte(CP_ACP, 0, wszString, (int)wcslen(wszString), NULL, 0, NULL, NULL);
    char* c = new char[len + 1];
    WideCharToMultiByte(CP_ACP, 0, wszString, (int)wcslen(wszString), c, len, NULL, NULL);
    c[len] = '\0';
    delete[] wszString;
    ans = c;
    return ans;
}


char* wideCharToMultiByte(wchar_t* pWCStrKey)
{

    int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), NULL, 0, NULL, NULL);
    char* pCStrKey = new char[pSize + 1];

    WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
    pCStrKey[pSize] = '\0';
    return pCStrKey;
}

std::string Polarsetting()
{
		LPCTSTR lpFileName = TEXT(_INIPATH_);
		WCHAR content[100];
		DWORD status = ::GetPrivateProfileString(TEXT("Group1"), TEXT("setting"), TEXT(""), content, 100, lpFileName);
		std::cout << UTF82Char(wideCharToMultiByte(content)) << std::endl;
		return UTF82Char(wideCharToMultiByte(content)) + " ";
}