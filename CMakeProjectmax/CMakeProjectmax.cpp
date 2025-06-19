// CMakeProjectmax.cpp: definiuje punkt wejścia dla aplikacji.
//

#include "CMakeProjectmax.h"

int main() {

    return 0;
}

const int FRAME_RATE = 30; // Frames per second
const int TIMER_ID = 1;
unsigned int g_frameCount = 0;
HWND hWnd;
POINT g_cursorPos;
bool g_keys[256] = { false };
float height = 0;
const float l1 = 500;
const float l2 = 600;
POINT base;
POINT knee;
float L;
float cosa;
float sina;
float sinb;
float sing;
float cosb;
float cosg;
float a;
float b;
float g;

VOID OnPaint(HDC hdc)
{

    // Get client area
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    int width = clientRect.right - clientRect.left;
    int height = clientRect.bottom - clientRect.top;

    // Double buffering setup
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmMem = CreateCompatibleBitmap(hdc, width, height);
    SelectObject(hdcMem, hbmMem);
    Graphics graphics(hdcMem);
    graphics.SetSmoothingMode(SmoothingModeHighQuality);
    graphics.Clear(Color(255, 255, 255));
    // Draw cursor position text
    SolidBrush textBrush(Color(255, 0, 0, 0));
    FontFamily fontFamily(L"Arial");
    Font font(&fontFamily, 16, FontStyleRegular, UnitPixel);

    // Format position string
    WCHAR posStr[64];
    swprintf_s(posStr, L"distance: (%d)", int(L));
    graphics.DrawString(posStr, -1, &font, PointF(10, 10), &textBrush);

    // Draw object controlled by arrow keys
    static POINT objPos = { 100, 100 };
   // if (g_keys[VK_LEFT]) objPos.x -= 2;
   // if (g_keys[VK_RIGHT]) objPos.x += 2;
   // if (g_keys[VK_UP]) objPos.y -= 2;
   // if (g_keys[VK_DOWN]) objPos.y += 2;

    // Draw movable object
    SolidBrush objBrush(Color(255, 255, 0, 0));
    Pen pen(Color(255, 0, 0, 255));
    graphics.FillEllipse(&objBrush, g_cursorPos.x-5 , g_cursorPos.y-5, 10, 10);
    graphics.DrawLine(&pen,int(base.x),int(base.y),int(knee.x),int(knee.y));
    graphics.DrawLine(&pen, int(knee.x), int(knee.y), g_cursorPos.x, g_cursorPos.y);
    // Draw to screen
    //graphics.DrawImage(&backBuffer, 0, 0, clientRect.right, clientRect.bottom);
    base = {0,clientRect.bottom};

    BitBlt(hdc, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);
    DeleteObject(hbmMem);
    DeleteDC(hdcMem);
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    
    MSG                 msg;
    WNDCLASS            wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
   
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = NULL;
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("drawing");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
        TEXT("drawing"), TEXT("ProjektTP4 Maksymilian Czarnecki 198406, Cezary Tralewski 197968"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);
    SetTimer(hWnd, TIMER_ID, 1000 / FRAME_RATE, NULL);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        OnPaint(hdc);
        EndPaint(hWnd, &ps);
        return 0;

    case WM_KEYDOWN:
        if (wParam < 256) // Only track standard VK_ codes
            g_keys[wParam] = true;
        return 0;

    case WM_KEYUP:
        if (wParam < 256)
            g_keys[wParam] = false;
        return 0;

    case WM_MOUSEMOVE:
    {
        // Get screen coordinates
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);

        // Convert to client coordinates (optional)
        POINT pt = { x, y };
        ClientToScreen(hWnd, &pt); 
        ScreenToClient(hWnd, &pt); 

        g_cursorPos = pt;
        L = sqrt(((base.x - g_cursorPos.x)* (base.x - g_cursorPos.x)) + ((base.y - g_cursorPos.y) * (base.y - g_cursorPos.y)));
        cosa = ((l1*l1)+(L*L)-(l2*l2)) / (2.0 * l1 * L);
        cosb = ((l1 * l1) + (l2 * l2) - (L * L)) / (2.0 * l1 * l2);
        a = acos(cosa);

        knee = { int(cos(a + g) * l1),base.y - int(sin(a+g)*l1) };


        // Force redraw to update display
        InvalidateRect(hWnd, NULL, TRUE);
        return 0;
    }
    case WM_TIMER:
        // Redraw on timer tick
        InvalidateRect(hWnd, NULL, FALSE);
        UpdateWindow(hWnd);
        g_frameCount++;
        return 0;

    case WM_DESTROY:
        KillTimer(hWnd, TIMER_ID);
        PostQuitMessage(0);
        return 0;

    case WM_ERASEBKGND:
        return 1;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}