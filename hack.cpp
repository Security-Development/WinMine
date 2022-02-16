#include <stdio.h>
#include <windows.h>
#include <tlHelp32.h>
#include <string.h>

int GetProcessByPid(char *ProcessName) {
	int buff;
	char ProcessByName[32] = { 0x00, };
	DWORD pid;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	
	sprintf(ProcessByName, "%s.exe", ProcessName);
	
	Process32First(hSnap, &pe);
	
	do {
		if( !strcmp(ProcessByName, pe.szExeFile) )
			buff = pe.th32ProcessID;
	}while(Process32Next(hSnap, &pe));
	
	CloseHandle(hSnap);
	
	return buff;
}

HANDLE GetProcessByHandle(DWORD ProcessPid) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessPid);
	return hProcess;
}

void hack(DWORD pid) {
	LPVOID address = (LPVOID)0x01001F0F;
	BYTE bOne[5] = {0xE8, 0x6C, 0x10, 0x00, 0x00};// call 01002f80
	BYTE bTwo[2] = {0x6A, 0x0A};// push 0Ah

	WriteProcessMemory(GetProcessByHandle(pid), address, bOne, sizeof(bOne), NULL);

	address = (LPVOID)0x01001F07;
	WriteProcessMemory(GetProcessByHandle(pid), address, bTwo, sizeof(bTwo), NULL);
}

int main() {
	char pName[32] = "winmine_(ÇÑ±Û)";
	DWORD pid = GetProcessByPid(pName);
	printf("Process Name : %s\n", pName);
	printf("PID : %d\n", pid);
	
	if( GetProcessByHandle(pid) != NULL ) {
		int select = MessageBox(NULL, "Get Process Handle!", "Info", MB_OK);
		if( select == 1 ) {
			select = MessageBox(NULL, "Do you want to run a Hack?", "Info", MB_YESNO);
			if( select == IDYES ) {
				MessageBox(NULL, "You have successfully run the hack.", "Info", MB_OK);
				hack(pid);
			}
		}
		
	} else{
		MessageBox(NULL, "Can not Get Process Handle!", "Info", MB_OK);
	} 
	
	return 0;

}
