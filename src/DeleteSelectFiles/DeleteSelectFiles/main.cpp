#include <windows.h>
#include <tchar.h>

BOOL ExecuteCommand(LPCTSTR lpApplication, LPTSTR lpCommandline, DWORD dwFlags = (0L), DWORD dwCreationFlags = (0L))
{
	BOOL bResult = FALSE;
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	si.dwFlags = dwFlags;

	// Start the child process. 
	if (::CreateProcess(
		lpApplication,  // No module name (use command line)
		lpCommandline,  // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		dwCreationFlags,// No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)            // Pointer to PROCESS_INFORMATION structure
		)
	{
		// Wait until child process exits.
		::WaitForSingleObject(pi.hProcess, INFINITE);

		// Close process and thread handles. 
		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);

		bResult = TRUE;
	}

	return bResult;
}

INT_PTR APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, INT nCmdShow)
{
	INT_PTR nResult = (0);
	
	_TCHAR tzSystemDirectory[MAX_PATH] = { 0 };
	_TCHAR tzWindowSystemCmd[MAX_PATH] = { 0 };
	_TCHAR tzDeleteSelectCmd[MAX_PATH] = { 0 };
	_TCHAR tzDeleteSelectExt[MAX_PATH] = { 0 };

	memset(tzSystemDirectory, 0, sizeof(tzSystemDirectory));
	::GetSystemDirectory(tzSystemDirectory, sizeof(tzSystemDirectory) / sizeof(*tzSystemDirectory));

	memset(tzWindowSystemCmd, 0, sizeof(tzWindowSystemCmd));
	::wsprintf(tzWindowSystemCmd, _T("%s\\cmd.exe"), tzSystemDirectory);

	memset(tzDeleteSelectExt, 0, sizeof(tzDeleteSelectExt));
	memset(tzDeleteSelectCmd, 0, sizeof(tzDeleteSelectCmd));
	::wsprintf(tzDeleteSelectExt, _T(" \"%s\""), _T("*.jpg"));
	::wsprintf(tzDeleteSelectCmd, _T(" /C DEL /S /Q /F %s"), tzDeleteSelectExt);

	if (ExecuteCommand(tzWindowSystemCmd, tzDeleteSelectCmd, SW_HIDE, CREATE_NO_WINDOW))
	{
		memset(tzDeleteSelectExt, 0, sizeof(tzDeleteSelectExt));
		memset(tzDeleteSelectCmd, 0, sizeof(tzDeleteSelectCmd));
		::wsprintf(tzDeleteSelectExt, _T(" \"%s\""), _T("*.png"));
		::wsprintf(tzDeleteSelectCmd, _T(" /C DEL /S /Q /F %s"), tzDeleteSelectExt);
		if (ExecuteCommand(tzWindowSystemCmd, tzDeleteSelectCmd, SW_HIDE, CREATE_NO_WINDOW))
		{
			::MessageBox(NULL, _T("清理完成!"), _T("提示信息"), MB_OK);
		}
	}

	return nResult;
}