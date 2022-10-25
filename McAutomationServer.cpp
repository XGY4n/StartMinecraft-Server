#include <iostream>
#include <string>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Iphlpapi.h>
#include <thread>
#include <mutex>
#include <type_traits>
#include <memory>
#include <conio.h>
#include <Windows.h>
#include <processthreadsapi.h>
#include <tlhelp32.h>
#include<conio.h>
#include <shellapi.h>
#include<sstream>
#include<fstream>
#pragma comment(lib,"Iphlpapi.lib")  
#pragma comment(lib, "ws2_32.lib")  

#define _TXTPATH_ "C:\\Users\\Public\\Documents\\Mcport.txt"

std::string cpolarStr = "cpolar tcp -remote-addr=2.tcp.cpolar.cn:12011 ";

typedef enum {
    GET_JAVA_PORT_ERROR,
    GET_JAVA_PID_ERROR,
    START_SEVER_SUCCESS
}Server_stat;


std::string Ipv4Str()
{
    PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
    //得到结构体大小,用于GetAdaptersInfo参数
    unsigned long stSize = sizeof(IP_ADAPTER_INFO);
    //调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
    int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
    DWORD netCardNum = 0;
    GetNumberOfInterfaces(&netCardNum);
    std::string user_ip;
    int IPnumPerNetCard = 0;
    if (ERROR_BUFFER_OVERFLOW == nRel)
    {
        delete pIpAdapterInfo;
        pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
        nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
    }
    if (ERROR_SUCCESS == nRel)
    {
        IPnumPerNetCard = 0;
        IP_ADDR_STRING* pIpAddrString = &(pIpAdapterInfo->IpAddressList);
        do
        {
            user_ip = pIpAddrString->IpAddress.String;
            pIpAddrString = pIpAddrString->Next;
        } while (pIpAddrString);

    }

    if (pIpAdapterInfo)
    {
        delete pIpAdapterInfo;
    }
    return user_ip;
}
std::wstring string2wstring(std::string str)
{
    std::wstring result;
    //获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
    TCHAR* buffer = new TCHAR[len + 1];
    //多字节编码转换成宽字节编码  
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
    buffer[len] = '\0';             //添加字符串结尾  
    //删除缓冲区并返回值  
    result.append(buffer);
    delete[] buffer;
    return result;
}

DWORD Get_Java_Pid(std::string name)
{
    DWORD pid = 0;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (std::wstring(entry.szExeFile).compare(string2wstring(name))==0)
            {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
                pid = GetProcessId(hProcess);
                CloseHandle(hProcess);
            }
        }
    }
    CloseHandle(snapshot);
    return pid;
}

std::string Get_PidPort(std::string JAVA_pid)
{
    std::string temp_PidPort;
    std::fstream f;
    JAVA_pid = "netstat -ano|findstr " + JAVA_pid + " | findstr \"LISTENING\"> C:\\Users\\Public\\Documents\\McPort.txt";
    system(JAVA_pid.c_str());
    f.open(_TXTPATH_);
    std::string buf;
    bool isfind = false;
    while (f >> buf && !isfind)
    {
        std::cout << buf << std::endl;
        int index = buf.find(':') ;
        if (index == std::string::npos)
            continue;
        for (int i = index ; i < buf.size(); i++)
        {
                temp_PidPort = temp_PidPort + buf[i ];
                isfind = true;
        }
    }
    return temp_PidPort;
}



Server_stat McSeverMian()
{
    int JAVA_pid;
    std::string PidPort;
    std::string Ipv4 = Ipv4Str();
    
    JAVA_pid = Get_Java_Pid("javaw.exe");
    std::cout << JAVA_pid;
    if (JAVA_pid == 0)
        return GET_JAVA_PID_ERROR;

    PidPort = Get_PidPort(std::to_string(JAVA_pid));
    if (PidPort.size() == 0)
        return  GET_JAVA_PORT_ERROR;

    cpolarStr = cpolarStr + Ipv4 +  PidPort;
    std::cout << cpolarStr;
    //getchar();
    system(cpolarStr.c_str());
    return  START_SEVER_SUCCESS;
   // WinExec(cpolarStr.c_str(), SW_SHOWNORMAL);

}


void SeverStatCheck(Server_stat stat)
{
    if (stat == GET_JAVA_PID_ERROR)
        std::cout << "JAVA_PID Get error PRESS ANY KEY TO RESCAN" << std::endl;
    else if (stat == GET_JAVA_PORT_ERROR)
        std::cout << "Minecraft Port maby shut down PRESS ANY KEY TO RESCAN" << std::endl;
    else if (stat == GET_JAVA_PORT_ERROR)
        std::cout << "Successful start Server " << std::endl;
}


int main()
{
    while (1)
    {
        Server_stat stat = McSeverMian();
        SeverStatCheck(stat);
        getchar();
        system("cls");
    }
    getchar();
    return 0;
}


