#include "main.h"

int WINAPI CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdParam, _In_ int nCmdShow)
{
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		0, 0, 1000, 700,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

float Gangle = 0;
DWORD WINAPI TimeLoop(LPVOID lpParameter)
{
	// The new thread will start here
	while (1) {
		savec = prec;
		prec = clock();
		stackTime += prec - savec;
		if (stackTime > 5) {
			float delta = stackTime / 1000.0f;
			gm.Update(delta);
			InvalidateRect(hWnd, 0, FALSE);
			stackTime = 0;
		}
	}
	return 0;
}

int mx = 0;
int my = 0;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memdc;
	static HBITMAP screenBit, oldScreen;
	PAINTSTRUCT ps;

	gm.Event(hWnd, iMessage, wParam, lParam);

	switch (iMessage) {
	case WM_CREATE:
	{
		GetClientRect(hWnd, &rt);
		whiteB = CreateSolidBrush(RGB(255, 255, 255));

		InitSprite();
		/*gm.AddObject((GameObject*)HeapDebugClass::HeapNew<PlayStation>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), false, 1));
		((PlayStation*)gm.GetGameObject(0))->LoadData("sampleTest.txt");*/
		gm.AddObject((GameObject*)HeapDebugClass::HeapNew<LogoPage>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1));
		//gm.AddObject((GameObject*)HeapDebugClass::HeapNew<EditStation>()->Init(shp::rect4f(rt.left, rt.top, rt.right, rt.bottom), 1));
		gm.Update(0);
		InitObjs();

		hTimeLoopThread = CreateThread(
			NULL,    // Thread attributes
			0,       // Stack size (0 = use default)
			TimeLoop, // Thread start address
			NULL,    // Parameter to pass to the thread
			0,       // Creation flags
			NULL);   // Thread id
		if (hTimeLoopThread == NULL)
		{
			// Thread creation failed.
			// More details can be retrieved by calling GetLastError()
			return 1;
		}

		//SetTimer(hWnd, 0, 20, NULL);
		break;
	}
	case WM_SIZE:
	{
		DeleteObject(hBit);
		hBit = NULL;
	}
	break;
	case WM_PAINT:
	{
		GetClientRect(hWnd, &rt);

		hdc = BeginPaint(hWnd, &ps);

		if (hBit == NULL) {
			hBit = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
		}
		HDC insDC = CreateCompatibleDC(hdc);
		HBITMAP OldBit = (HBITMAP)SelectObject(insDC, hBit);

		//여기에 추가..

		FillRect(insDC, &rt, whiteB);
		gm.Render(insDC);

		SelectObject(insDC, OldBit);
		DeleteDC(insDC);

		memdc = CreateCompatibleDC(hdc);
		OldBit = (HBITMAP)SelectObject(memdc, hBit);
		GetClientRect(hWnd, &rt);
		BitBlt(hdc, 0, 0, rt.right, rt.bottom, memdc, 0, 0, SRCCOPY);
		SelectObject(memdc, OldBit);
		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_LBUTTONDOWN:
		GameUI::LBtnPressed = true;
		break;
	case WM_LBUTTONUP:
		GameUI::LBtnPressed = false;
		break;
	case WM_RBUTTONDOWN:
		GameUI::RBtnPressed = true;
		break;
	case WM_RBUTTONUP:
		GameUI::RBtnPressed = false;
		break;
	case WM_MOUSEMOVE:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		GameUI::MousePos = shp::vec2f(mx, my);
		break;
	case WM_DESTROY:
		ClearSpriteData();
		if (hBit != NULL) {
			DeleteObject(hBit);
		}
		CloseHandle(hTimeLoopThread);

		gm.Clear();

		{
			TCHAR tstr[128] = {};
			wsprintf(tstr, L"Non Release Heap Num : %d", HeapDebugClass::GetPresentHeapDataCount());
			MessageBox(hWnd, tstr, L"해제되지 않은 힙 메모리 확인", MB_OK);
		}
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}