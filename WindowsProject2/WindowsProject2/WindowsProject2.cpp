#include <iostream>
#include <windows.h>
#include <tchar.h>

void StartApplicationAsUser(const TCHAR* applicationPath)
{
    const int UNLEN = 1;
    // Получаем имя пользователя
    wchar_t userName[UNLEN + 1];
    DWORD userNameSize = UNLEN + 1;
    GetUserName(userName, &userNameSize);

    // Запускаем клиентское приложение от имени пользователя
    ShellExecute(NULL, NULL, applicationPath, NULL, NULL, SW_SHOWDEFAULT);
}

void WINAPI ServiceMain(DWORD argc, LPTSTR* argv);

void WINAPI ServiceCtrlHandler(DWORD ctrl);

SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE serviceStatusHandle;

void StartService()
{
    serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    serviceStatus.dwCurrentState = SERVICE_RUNNING;
    serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    serviceStatus.dwWin32ExitCode = NO_ERROR;
    serviceStatus.dwServiceSpecificExitCode = 0;
    serviceStatus.dwCheckPoint = 0;
    serviceStatus.dwWaitHint = 0;

    serviceStatusHandle = RegisterServiceCtrlHandler(L"UserApplicationService", ServiceCtrlHandler);
    SetServiceStatus(serviceStatusHandle, &serviceStatus);

    // Запускаем клиентское приложение от имени пользователя
    StartApplicationAsUser(L"C:\\Path\\To\\Your\\Application.exe");
}

void StopService()
{
    serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
    SetServiceStatus(serviceStatusHandle, &serviceStatus);

    // Добавить код остановки сервиса

    serviceStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(serviceStatusHandle, &serviceStatus);
}

void WINAPI ServiceMain(DWORD argc, LPTSTR* argv)
{
    serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    serviceStatus.dwCurrentState = SERVICE_START_PENDING;
    serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    serviceStatus.dwWin32ExitCode = NO_ERROR;
    serviceStatus.dwServiceSpecificExitCode = 0;
    serviceStatus.dwCheckPoint = 0;
    serviceStatus.dwWaitHint = 0;

    serviceStatusHandle = RegisterServiceCtrlHandler(L"UserApplicationService", ServiceCtrlHandler);
    SetServiceStatus(serviceStatusHandle, &serviceStatus);

    StartService();
}

void WINAPI ServiceCtrlHandler(DWORD ctrl)
{
    switch (ctrl)
    {
    case SERVICE_CONTROL_STOP:
        StopService();
        break;
    }
}

int _tmain(int argc, TCHAR* argv[])
{
    SERVICE_TABLE_ENTRY DispatchTable[] =
    {
        { L"UserApplicationService", ServiceMain },
        { NULL, NULL }
    };

    if (!StartServiceCtrlDispatcher(DispatchTable))
    {
        return GetLastError();
    }

    return 0;
}
