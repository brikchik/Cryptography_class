// processes.cpp: defines entry point for app
//
#define _CRT_SECURE_NO_WARNINGS  //wcstombs requires this define to work
#include "windows.h"
#include <iostream>
#include <tlhelp32.h>
#include <algorithm> 
#include <iterator>
#include <string>
#include <map>
using namespace std;
map<DWORD,string> GetProcList()
{
	map<DWORD, string> bmap;
	HANDLE procsnap; //handle for snapshot
	procsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 proc;
	proc.dwSize = sizeof(PROCESSENTRY32);
	Process32First(procsnap, &proc); //first item
	char name[1024]; //large enough array for name, WCHAR used in Windows isn't the best solution
	wcstombs(name, proc.szExeFile, sizeof(proc.szExeFile)); //wchar conversion. requires define to get compiled
	bmap.insert(make_pair(proc.th32ProcessID, name)); //adding first name and id to map
	while (Process32Next(procsnap, &proc)) {
		wcstombs(name, proc.szExeFile, sizeof(proc.szExeFile)); //wchar conversion
		bmap.insert(make_pair(proc.th32ProcessID, name)); //adding process to the list
	} //cycle works till last element when no NEXT element exists
	CloseHandle(procsnap); //finishing work
	return bmap;
}
void compare(map<DWORD, string> p1)
{
	map<DWORD, string> p2;//last state(checked every time)
	map<DWORD, string> diff;//difference between previous and current processes
	p2 = GetProcList();
	if(p1 == p2)return; //exit if nothing changed
	set_difference(p2.begin(), p2.end(), p1.begin(), p1.end(),
		inserter(diff, diff.begin())); 
	//find started processes: if current state has more processes than the previous one [map2-map1]
	if (!diff.empty())
	{
		for (auto i : diff) //i=each diff pair. Means [for i in diff]
		{cout << "Process started: " << i.first << "\n" << "Name: " << i.second << endl;}
	}
	diff.clear(); //should be cleared before 2nd part
	set_difference(p1.begin(), p1.end(), p2.begin(), p2.end(),
		inserter(diff, diff.begin()));
	//find ended processes: if current state has less processes than the previous one [map1-map2]
	if (!diff.empty())
	{
		for (auto i : diff){cout << "Process finished: " << i.first << endl << "Name: " << i.second << endl << endl;}
	}
}
int main()
{
	map<DWORD, string> p1=GetProcList();// previous state
	while (true)
	{
		compare(p1);//checking if smth's changed
		p1=GetProcList(); //updating current state
		Sleep(50);//give user time for doing smth
	}
	return 0;
}