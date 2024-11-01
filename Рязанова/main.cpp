#include <SFML/Graphics.hpp>
using namespace sf;

//пример для теста sfml
/*int main()
{

    RenderWindow window(VideoMode(400, 400), L"Platformer", Style::Default);

    window.setVerticalSyncEnabled(true);

    CircleShape shape(100.f, 3);
    shape.setPosition(100, 100);
    shape.setFillColor(Color::Magenta);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear(Color::Blue);
        window.draw(shape);
        window.display();
    }
    return 0;
}
*/

//#include <Windows.h>
//bool running = true;
//RECT rect;
//
//LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	LRESULT result = 0;
//	switch (uMsg)
//	{
//	case WM_CLOSE:
//	case WM_DESTROY:
//	{
//		running = false;
//		break;
//	}
//	case WM_CREATE:
//	case WM_SIZE:
//	{
//		GetClientRect(hwnd, &rect);
//		break;
//	}
//	/*case WM_KEYDOWN:
//	{
//		if (wParam == 32)
//		{
//			WinInit();
//		}
//	}*/
//	/*case WM_LBUTTONDOWN:
//	{
//
//	}*/
//	default:
//	{
//		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
//		break;
//	}
//	}
//	return result;
//}
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
//{
//	WNDCLASSA wlc = {};
//	//memset(&wlc, 0, sizeof(WNDCLASSA));
//	wlc.lpszClassName = "my Window";
//	wlc.lpfnWndProc = WndProc;
//	/*wlc.hCursor = LoadCursorA(NULL, (LPCSTR)IDC_CROSS);*/
//	RegisterClassA(&wlc);
//
//	HWND window = CreateWindowA("my Window", 0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
//	/*{
//		SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
//		MONITORINFO mi = { sizeof(mi) };
//		GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);
//		SetWindowPos(window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
//	}*/
//
//	HDC hdc = GetDC(window);
//
//	MSG msg;
//
//	while (running)
//	{
//		if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
//		{
//			if (msg.message == WM_QUIT)
//			{
//				break;
//			}
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//		/*else if (isGame)
//		{
//			WinMove();
//			WinShow(dc);
//			Sleep(5);
//		}*/
//	}
//
//	return 0;
//}
