#include <windows.h>// классические программы
#include <stdlib.h>
#include <string.h>
#include <tchar.h>//юникод добавление

static TCHAR szWindowClass[] = _T("Базовое приложение");

static TCHAR szTitle[] = _T("АПП");

HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)//отправная точка всего приложения
{
    WNDCLASSEX wcex;//основные сведения об окне

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))//осуществляется регистрация WINDCLASSEX 
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

  
    hInst = hInstance;


    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        300, 100,
        NULL,
        NULL,
        hInstance,
        NULL
    );//возвращает дескриптор hwnd в окно (дескриптор похож на указатель)

    /*if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }*/


    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);//делает видимым окно 

    // обрабатывает сообщения отправленные в окно
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)//код для обработки сообщений получ. в прил.
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("текст сообщения");

    switch (message)//с его помощью обрабатывется сообщение 
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);//размещение и логика кнопок и др. элементов

  
        TextOut(hdc,
            5, 5,
            greeting, _tcslen(greeting));
       
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY://отправляется при первом создании окна 
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);//при закрытии окна
        break;
    }

    return 0;
}