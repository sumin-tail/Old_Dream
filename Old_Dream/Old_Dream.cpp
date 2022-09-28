#include "Old_Dream.h"

#include "iWindow.h"
#include "GameManager.h"

static int win_border_width, win_border_height;

HWND hWnd;
HDC hDC;

bool runApp = false;
DWORD prevTime;
bool* keys;

LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    ULONG_PTR token = iGraphics::startGdiplus();

    WCHAR szTitle[20] = L"Title Name";
    WCHAR szWindowClass[20] = L"Class Name";

    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = wndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OLDDREAM));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_PS);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wcex);

    int mW = GetSystemMetrics(SM_CXSCREEN);
    int mH = GetSystemMetrics(SM_CYSCREEN);
    float wndWidth = mW * 2 / 3;
    float wndHeight = mH * 2 / 3;

    hWnd = CreateWindowW(szWindowClass, szTitle,
        WS_OVERLAPPEDWINDOW,
        wndWidth / 4, wndHeight / 4,// x, y
        wndWidth, wndHeight,// w, h
        nullptr, nullptr, hInstance, nullptr);

    hDC = GetDC(hWnd);
    loadOpenGL(hDC);

    int wH = wndHeight + win_border_height;
    int wW = wndHeight * devSize.width / devSize.height + win_border_width;

    MoveWindow(hWnd, (mW - wW) / 2, (mH - wH) / 2, wW, wH, TRUE);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    prevTime = GetTickCount();
    keys = new bool[128];
    memset(keys, false, sizeof(bool) * 128);
    srand(time(NULL));

    GameManager::instance()->loadGame();
    loadUI(); 
    loadCursor(); 

    runApp = true;
    MSG msg;
    while (runApp)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            DWORD now = GetTickCount();
            float delta = (now - prevTime) / 1000.f;
            prevTime = now;

            void virtualDrawGame(float dt);
            drawOpenGL(delta, virtualDrawGame);
        }
    }

    GameManager::instance()->freeGame();
    freeCursor();

    freeOpenGL();
    ReleaseDC(hWnd, hDC);
    DestroyWindow(hWnd);

    delete keys;
    iGraphics::stopGdiplus(token);
    return (int)msg.wParam;
}

void virtualDrawGame(float dt)
{
    GameManager::instance()->drawGame(dt);
}

iPoint convertCoordinate(float x, float y)
{
    RECT rt;
    GetClientRect(hWnd, &rt);
    x -= rt.left;
    y -= rt.top;

    return iPointMake(
        (x - viewport.origin.x) / viewport.size.width * devSize.width,
        (y - viewport.origin.y) / viewport.size.height * devSize.height);
}

void enforceResolution(int edge, RECT& rect, int win_border_width, int win_border_height);

bool gamePause = false;

LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        //case WM_CHAR:
    case WM_SYSKEYDOWN:
        return 0;// break;

    case WM_KEYDOWN:
        if (gamePause)
        {
            // pop key....
            if (wParam == 27) //esc만 예외처리
            {
                setKeyboard(true, wParam);
                keys[wParam] = true;
            }

            break;
        }
        if (keys[wParam] == false)
        {
            setKeyboard(true, wParam);
            keys[wParam] = true;
        }

        if (SceneMgt::instance()->gs == GameSceneTitle)
        {
            SceneMgt::instance()->set(GameSceneMenu, new MainMenu);
        }
        break;
    case WM_KEYUP:
        setKeyboard(false, wParam);
        keys[wParam] = false;
        break;

    case WM_LBUTTONDOWN:
        GameManager::instance()->keyGame(iKeyStateBegan, convertCoordinate(LOWORD(lParam), HIWORD(lParam)));
        break;
    case WM_MOUSEMOVE:
    {
        iPoint c = convertCoordinate(LOWORD(lParam), HIWORD(lParam));
        setCursor(c);
        GameManager::instance()->keyGame(iKeyStateMoved, c);
        break;
    }
    case WM_LBUTTONUP:
        GameManager::instance()->keyGame(iKeyStateEnded, convertCoordinate(LOWORD(lParam), HIWORD(lParam)));
        break;

    case WM_MOUSEWHEEL:
    {
        int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        switch (zDelta) {
        case 120:
            printf("+1\n");
            break;
        case -120:
            printf("-1\n");
            break;
        }
        break;
    }

    case WM_SETCURSOR:
        if (updateCursor(LOWORD(lParam) == HTCLIENT))
            return true;
        break;

    case WM_CREATE:
    {
#if 1
        if (hWnd == NULL)
            hWnd = hwnd;
#endif
        RECT rt0, rt1;
        GetWindowRect(hWnd, &rt0);
        GetClientRect(hWnd, &rt1);
        win_border_width = (rt0.right - rt0.left) - (rt1.right - rt1.left);
        win_border_height = (rt0.bottom - rt0.top) - (rt1.bottom - rt1.top);
        break;
    }

    case WM_GETMINMAXINFO:
    {
        float height = GetSystemMetrics(SM_CYSCREEN) / 3;
        int width = height * devSize.width / devSize.height;

        MINMAXINFO* info = (MINMAXINFO*)lParam;
        info->ptMinTrackSize.x = width + win_border_width;
        info->ptMinTrackSize.y = height + win_border_height;
        break;
    }

    case WM_MOVE:
    case WM_SIZING:
    {
        RECT& rect = *reinterpret_cast<LPRECT>(lParam);
        enforceResolution(int(wParam), rect, win_border_width, win_border_height);
        break;
    }
    case WM_SIZE:
        resizeOpenGL(LOWORD(lParam), HIWORD(lParam));
        if (runApp)
            drawOpenGL(0.0f, virtualDrawGame);
        break;

    case WM_CLOSE:
    {
#if 0
        int result = MessageBox(NULL, L"종료하시겠습니까?", L"Exit", MB_YESNO);
        if (result == IDYES)
        {
            runApp = false;
        }
        else
        {
            return 0;
        }
#else
        extern iPopup* popQuit;
        if (popQuit->bShow == true)
            return 0;

        gamePause = true;
        void showPopQuit(bool show);
        showPopQuit(true);
        return 0;
#endif
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;// break;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

void enforceResolution(int edge, RECT& rect, int win_border_width, int win_border_height)
{
    switch (edge)
    {
    case WMSZ_BOTTOM:
    case WMSZ_TOP:
    {
        int h = (rect.bottom - rect.top) - win_border_height;
        int w = h * devSize.width / devSize.height + win_border_width;
        rect.left = (rect.left + rect.right) / 2 - w / 2;
        rect.right = rect.left + w;
        break;
    }
    case WMSZ_BOTTOMLEFT:
    {
        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;
        if (w * devSize.height > devSize.width * h)
        {
            w = h * devSize.width / devSize.height + win_border_width;
            rect.left = rect.right - w;
        }
        else
        {
            h = w * devSize.height / devSize.width + win_border_height;
            rect.bottom = rect.top + h;
        }
        break;
    }
    case WMSZ_BOTTOMRIGHT:
    {
        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;
        if (w * devSize.height > devSize.width * h)
        {
            // w : h = devSize.width : devSize.height
            w = h * devSize.width / devSize.height + win_border_width;
            rect.right = rect.left + w;
        }
        else
        {
            h = w * devSize.height / devSize.width + win_border_height;
            rect.bottom = rect.top + h;
        }
        break;
    }
    case WMSZ_LEFT:
    case WMSZ_RIGHT:
    {
        int w = (rect.right - rect.left) - win_border_width;
        int h = w * devSize.height / devSize.width + win_border_height;
        rect.top = (rect.top + rect.bottom) / 2 - h / 2;
        rect.bottom = rect.top + h;
        break;
    }
    case WMSZ_TOPLEFT:
    {
        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;
        //if( w/h > devSize.width/devSize.height )
        if (w * devSize.height > devSize.width * h)
        {
            // w : h = devSize.width : devSize.height
            w = h * devSize.width / devSize.height + win_border_width;
            rect.left = rect.right - w;
        }
        else
        {
            // w : h = devSize.width : devSize.height
            h = w * devSize.height / devSize.width + win_border_height;
            rect.top = rect.bottom - h;
        }
        break;
    }
    case WMSZ_TOPRIGHT:
    {
        int w = (rect.right - rect.left) - win_border_width;
        int h = (rect.bottom - rect.top) - win_border_height;
        if (w * devSize.height > devSize.width * h)
        {
            w = h * devSize.width / devSize.height + win_border_width;
            rect.right = rect.left + w;
        }
        else
        {
            h = w * devSize.height / devSize.width + win_border_height;
            rect.top = rect.bottom - h;
        }
        break;
    }
    }
}

HGLRC hRC;
void loadOpenGL(void* hdc)
{
    //
    // setup OpenGL
    //
    PIXELFORMATDESCRIPTOR pfd;
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelformat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pixelformat, &pfd);

    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    glewExperimental = true;
    GLenum err = glewInit();
    if (GLEW_OK == err)
    {
        printf("GLEW %s GL %s\n",
            glewGetString(GLEW_VERSION), glGetString(GL_VERSION));
    }
    else
    {
        printf("Error: % s\n", glewGetErrorString(err));
    }

    //
    // initialize OpenGL
    //

    // Drawing Region
    devSize = iSizeMake(DEV_WIDTH, DEV_HEIGHT);
    //viewport;// resizeOpenGL

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, devSize.width, devSize.height, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    setGlBlendFunc(iBlendFuncAlpha);

    // Settings of Texture
    setTexture(LINEAR, CLAMP);

    // Back or Front Buffer(Frame Buffer Object)
    fbo = new iFBO(devSize.width, devSize.height);
}

void freeOpenGL()
{
    // setup OpenGL
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);

    delete fbo;
}

struct iZoom
{
    iPoint c;
    float scale, scaleS, scaleE;
    float zoomDt, _zoomDt;
    float delayDt, _delayDt;

    bool order;

    iZoom()
    {
        c = iPointZero;
        scale = 1.0f;
        scaleS = 1.0f;
        scaleE = 1.0f;
        zoomDt = 0.0f;
        _zoomDt = 0.0f;
        _delayDt = 0.0f;
        delayDt = 0.0f;
        order = false;
    }

    void set(iPoint c, float s, float zDt, float dDt)
    {
        this->c = c;
        scaleS = this->scale;
        scaleE = s;
        _zoomDt = zDt;
        zoomDt = 0.000001f;
        _delayDt = dDt;
        delayDt = 0.0f;

        order = true;
    }

    float update(float dt)
    {
        if (zoomDt == 0.0f)
            return 1.0f;

        // 줌인중..
        if (zoomDt < _zoomDt)
        {
            zoomDt += dt;
            if (zoomDt > _zoomDt)
            {
                zoomDt = _zoomDt;
                if (order == false)
                {
                    zoomDt = 0.0f;// stop
                    scale = scaleS;
                    return 1.0f;
                }
            }

            if (order)
                scale = linear(zoomDt / _zoomDt, scaleS, scaleE);
            else
                scale = linear(zoomDt / _zoomDt, scaleE, scaleS);
        }
        // 줌이 된 상태
        else if (delayDt < _delayDt)
        {
            scale = scaleE;

            delayDt += dt;
            if (delayDt > _delayDt)
            {
                delayDt = _delayDt;

                zoomDt = 0.000001f;
                order = false;
            }
        }
        return scale;
    }
};
static iZoom zoom;

void setZoom(iPoint c, float scale, float zoomDt, float delayDt)
{
    zoom.set(c, scale, zoomDt, delayDt);
}

void drawOpenGL(float dt, MethodDraw m)
{
    setGlBlendFunc(iBlendFuncAlpha);
    fbo->bind();
    fbo->clear(0, 0, 0, 0);
    m(dt);// drawGame
    updateKeyboard();
    drawCursor(dt);
    fbo->unbind();

    setGlBlendFunc(iBlendFuncPremultipliedAlpha);
    fbo->clear(0, 0, 0, 0);
    setRGBA(1, 1, 1, 1);
    Texture* t = fbo->tex;
    float s = zoom.update(dt);
    if (s == 1.0f)
    {
        drawImage(t, 0, 0, 1, 1,
            TOP | LEFT, 0, 0, t->width, t->height, 2, 0, REVERSE_HEIGHT);
    }
    else
    {
        iPoint p = zoom.c * (1.0f - s);
        drawImage(t, p.x, p.y, s, s,
            TOP | LEFT, 0, 0, t->width, t->height, 2, 0, REVERSE_HEIGHT);
    }

    SwapBuffers(hDC);
}

void resizeOpenGL(int width, int height)
{
    iSize wndSize = iSizeMake(width, height);
    printf("size (%d, %d)\n", width, height);

    float r0 = wndSize.width / devSize.width;
    float r1 = wndSize.height / devSize.height;
    if (r0 < r1)// 세로가 긴 화면
    {
        viewport.origin.x = 0;
        viewport.size.width = wndSize.width;

        float r = wndSize.width / devSize.width;
        viewport.size.height = devSize.height * r;
        viewport.origin.y = (wndSize.height - viewport.size.height) / 2;
    }
    else if (r0 > r1)// 가로가 긴 화면
    {
        viewport.origin.y = 0;
        viewport.size.height = wndSize.height;

        float r = wndSize.height / devSize.height;
        viewport.size.width = devSize.width * r;
        viewport.origin.x = (wndSize.width - viewport.size.width) / 2;
    }
    else// if (r0 == r1)// 정비율
    {
        viewport.origin = iPointZero;
        viewport.size = wndSize;
    }
    glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);
}

static iImage* imgCursor;
iPoint positionCursor;
bool bCursor;
void loadCursor()
{
    iImage* img = new iImage();

    Texture* tex = createImage("assets/cursor.png");
    img->addObject(tex);
    freeImage(tex);

    img->position = iPointMake(0, 0);
    img->scale = 1.0f;
    imgCursor = img;
}
void freeCursor()
{
    delete imgCursor;
}
void setCursor(iPoint position)
{
    positionCursor = position;
}
bool updateCursor(bool inClient)
{
    if (bCursor == inClient)
        return false;
    bCursor = inClient;

    if (bCursor)
    {
        while (1)
        {
            int n = ShowCursor(FALSE);
            if (n < 0) break;
        }
    }
    else
    {
        while (1)
        {
            int n = ShowCursor(TRUE);
            if (n > -1) break;

        }
    }
    return true;
}
void drawCursor(float dt)
{
    if (bCursor)
        imgCursor->paint(dt, positionCursor);
}
