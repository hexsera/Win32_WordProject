// WindowsProject2listword.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WindowsProject2listword.h"

#define line_size_y_def 15

#define word_pos_x 50
#define word_pos_y 50
#define word_size_x 300
#define word_size_y 500

#define MAX_LOADSTRING 100


struct WORDFRAME;
struct WORDBOX;
struct TCHARLIST;
struct LINELIST;

struct WORDITEMMENU;
struct WORDITEMLIST;
enum WORDITEMTYPE;


struct FOCUS_WORDBOX;

struct MOUSE_FOCUS;

struct FONT_TYPE_BOOL;


struct WORDFRAME {
    WORDBOX* first_box;
    WORDITEMMENU* menu;
};

struct WORDBOX {
    WORDFRAME* parent;
    WORDBOX* next;
    WORDBOX* back;
    FOCUS_WORDBOX* focus;
    LINELIST* first_line;
    int pos_x;
    int pos_y;
    int size_x;
    int size_y;
};

struct TCHARLIST {
    LINELIST* parent;
    TCHAR str;
    LOGFONT* font;
    int str_size_x;
    int str_size_y;
    int start_y;
    TCHARLIST* next;
    TCHARLIST* back;
};

struct LINELIST {
    WORDBOX* parent;
    TCHARLIST* first_tchar;
    int line_size_x;
    int line_size_y;
    LINELIST* next;
    LINELIST* back;
};

struct WORDITEMMENU {
    WORDFRAME* parent;
    WORDITEMLIST* first_list;
    LOGFONT* font;
    int pos_x;
    int pos_y;
    int size_x;
    int size_y;
};

enum WORDITEMTYPE {
    WORDITEMTRPE_NULL = -1,
    WORDITEMTYPE_bold,
    WORDITEMTYPE_italic,
    WORDITEMTYPE_underline,
};

struct WORDITEMLIST {
    WORDITEMMENU* parent;
    WORDITEMLIST* next;
    WORDITEMLIST* back;
    WORDITEMTYPE type;
    bool toggle;
    int size_x;
    int size_y;
};



struct C_SIZE {
    int pos_x;
    int pos_y;
    int size_x;
    int size_y;
};

struct MOUSE_FOCUS {
    FOCUS_WORDBOX* wordbox;
};

struct FOCUS_WORDBOX {
    bool focus;
    LINELIST* linelist;
    TCHARLIST* tcharlist;
    int pos_x;
    int pos_y;
};

struct FONT_TYPE_BOOL {
    int font;
    int font_boolint;
};

WORDBOX* new_wordbox(WORDFRAME* frame, int x, int y, int size_x, int size_y);

LINELIST* new_linelist();
TCHARLIST* new_tcharlist(LOGFONT* font);
WORDITEMMENU* new_worditemmenu(WORDFRAME* frame, int pos_x, int pos_y, int size_x, int size_y);
WORDITEMLIST* new_worditem(WORDITEMMENU* menu, int size_x, int size_y, WORDITEMTYPE type);

LOGFONT* Font_defult();
LOGFONT* Font_copy(LOGFONT* font);

TCHARLIST* Delete_tchar(WORDBOX* box, TCHARLIST* tchar);
void Delete_hard_tchar(TCHARLIST* tchar);
LINELIST* Insert_line(WORDBOX* box, LINELIST* line, TCHARLIST* tchar);
LINELIST* Delete_line(WORDBOX* box, LINELIST* line, bool hard = false, bool move_caret = true);
void Delete_hard_linelist(LINELIST* line);
TCHARLIST* Insert_tchar(WORDITEMMENU* menu, LINELIST* line, TCHARLIST* tchar, WPARAM wParam);
void Print_tcharlist(HDC hdc, TCHARLIST* tcharlist, int start_x, int start_y);
void Print_linelist(HDC hdc, LINELIST* linelist, int start_x, int start_y);

bool Box_cast(int pos_x, int pos_y, int size_x, int size_y, int point_x, int point_y);
void Point_finder(WORDBOX* box, WORDITEMMENU* menu, int pos_x, int pos_y, C_SIZE* csize);

void Set_focusword(FOCUS_WORDBOX* focus, bool focus_bool, LINELIST* line, TCHARLIST* tchar);
void Set_focuswordpos(FOCUS_WORDBOX* focus, int x, int y);

void Set_wordmenufont(FOCUS_WORDBOX* focus, WORDITEMMENU* menu);
void Paint_worditemmenu(HDC hdc, WORDITEMMENU* menu);

void Font_finder(FONT_TYPE_BOOL* temp, WORDITEMTYPE type = WORDITEMTRPE_NULL, int bool_int = -1, int d_type = -1);
int Logfont_convert(LOGFONT* font, WORDITEMTYPE type);

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT2LISTWORD, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT2LISTWORD));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT2LISTWORD));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT2LISTWORD);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static WORDFRAME wordframe;
    static HDC hdc;
    static C_SIZE csz;
    int mouse_x = 0;
    int mouse_y = 0;

    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_CREATE:
    {
        wordframe.menu = new_worditemmenu(&wordframe, 0, 0, 100, 30);

        new_worditem(wordframe.menu, wordframe.menu->size_y, wordframe.menu->size_y, WORDITEMTYPE_bold);
        new_worditem(wordframe.menu, wordframe.menu->size_y, wordframe.menu->size_y, WORDITEMTYPE_italic);

        wordframe.first_box = new_wordbox(&wordframe, word_pos_x, word_pos_y, word_size_x, word_size_y);
        csz.pos_x = 0;
        csz.pos_y = 0;
        csz.size_x = 0;
        csz.size_y = 0;

        Insert_line(wordframe.first_box, NULL, NULL);

        Set_focusword(wordframe.first_box->focus, true, wordframe.first_box->first_line, NULL);
        Set_focuswordpos(wordframe.first_box->focus, 0, 0);

        CreateCaret(hWnd, NULL, 1, 15);
        ShowCaret(hWnd);
    }
    break;
    case WM_CHAR:
    {
        hdc = GetDC(hWnd);

        if (wParam == VK_RETURN)
        {
            LINELIST* templine = Insert_line(wordframe.first_box, wordframe.first_box->focus->linelist, wordframe.first_box->focus->tcharlist);
            Set_focusword(wordframe.first_box->focus, true, templine, NULL);
        }
        else if (wParam == VK_BACK)
        {
            if (wordframe.first_box->focus->tcharlist == NULL) //줄지우기
            {

                Delete_line(wordframe.first_box, wordframe.first_box->focus->linelist);

            }
            else //글자지우기
            {
                Delete_tchar(wordframe.first_box, wordframe.first_box->focus->tcharlist);
            }
        }
        else
        {
            TCHARLIST* temptchar = Insert_tchar(wordframe.menu, wordframe.first_box->focus->linelist, wordframe.first_box->focus->tcharlist, wParam);
            Set_focusword(wordframe.first_box->focus, true, temptchar->parent, temptchar);
        }

        InvalidateRgn(hWnd, NULL, TRUE);
        ReleaseDC(hWnd, hdc);
    }
    break;

    case WM_LBUTTONDOWN:
    {
        mouse_x = LOWORD(lParam);
        mouse_y = HIWORD(lParam);
        Point_finder(wordframe.first_box, wordframe.menu, mouse_x, mouse_y, &csz); // 클릭 찾기
        //Set_wordmenufont(wordframe.first_box->focus, wordframe.menu);

        InvalidateRgn(hWnd, NULL, TRUE);
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

        Paint_worditemmenu(hdc, wordframe.menu);

        Rectangle(hdc, wordframe.first_box->pos_x, wordframe.first_box->pos_y, wordframe.first_box->pos_x + wordframe.first_box->size_x, wordframe.first_box->pos_y + wordframe.first_box->size_y);
        if (wordframe.first_box->first_line != NULL)
        {
            Print_linelist(hdc, wordframe.first_box->first_line, wordframe.first_box->pos_x, wordframe.first_box->pos_y);
        }

        if (wordframe.first_box->focus->focus == true)
        {
            SetCaretPos(wordframe.first_box->focus->pos_x, wordframe.first_box->focus->pos_y);
        }

        Rectangle(hdc, csz.pos_x, csz.pos_y, csz.pos_x + csz.size_x, csz.pos_y + csz.size_y);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

WORDBOX* new_wordbox(WORDFRAME* frame, int x, int y, int size_x, int size_y)
{
    WORDBOX* tempbox = new WORDBOX;
    tempbox->parent = frame;
    tempbox->next = NULL;
    tempbox->back = NULL;
    tempbox->focus = new FOCUS_WORDBOX;
    tempbox->first_line = NULL;
    tempbox->pos_x = x;
    tempbox->pos_y = y;
    tempbox->size_x = size_x;
    tempbox->size_y = size_y;
    return tempbox;
}

LINELIST* new_linelist()
{
    LINELIST* temp_list = new LINELIST;
    temp_list->parent = NULL;
    temp_list->first_tchar = NULL;
    temp_list->line_size_x = 0;
    temp_list->line_size_y = 0;
    temp_list->back = NULL;
    temp_list->next = NULL;
    return temp_list;
}

TCHARLIST* new_tcharlist(LOGFONT* font)
{
    TCHARLIST* temp_list = new TCHARLIST;
    temp_list->parent = NULL;
    temp_list->str = NULL;
    temp_list->font = Font_copy(font);
    temp_list->str_size_x = 0;
    temp_list->str_size_y = 0;
    temp_list->start_y = 0;
    temp_list->next = NULL;
    temp_list->back = NULL;
    return temp_list;
}

WORDITEMMENU* new_worditemmenu(WORDFRAME* frame, int pos_x, int pos_y, int size_x, int size_y)
{
    WORDITEMMENU* menu = new WORDITEMMENU;
    menu->parent = frame;
    menu->font = Font_defult();

    menu->first_list = NULL;
    menu->pos_x = pos_x;
    menu->pos_y = pos_y;
    menu->size_x = size_x;
    menu->size_y = size_y;
    return menu;
}


LOGFONT* Font_defult()
{
    LOGFONT* lf = new LOGFONT;
    lf->lfHeight = 0;
    lf->lfWidth = 0;
    lf->lfEscapement = 0;
    lf->lfOrientation = 0;
    lf->lfWeight = 0;
    lf->lfItalic = FALSE;
    lf->lfUnderline = FALSE;
    lf->lfStrikeOut = FALSE;
    lf->lfCharSet = DEFAULT_CHARSET;
    lf->lfOutPrecision = OUT_DEFAULT_PRECIS;
    lf->lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf->lfQuality = DEFAULT_QUALITY;
    lf->lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    lf->lfFaceName[0] = NULL;
    return lf;
}


LOGFONT* Font_copy(LOGFONT* font)
{
    LOGFONT* lf = new LOGFONT;
    lf->lfHeight = font->lfHeight;
    lf->lfWidth = font->lfWidth;
    lf->lfEscapement = font->lfEscapement;
    lf->lfOrientation = font->lfOrientation;
    lf->lfWeight = font->lfWeight;
    lf->lfItalic = font->lfItalic;
    lf->lfUnderline = font->lfUnderline;
    lf->lfStrikeOut = font->lfStrikeOut;
    lf->lfCharSet = font->lfCharSet;
    lf->lfOutPrecision = font->lfOutPrecision;
    lf->lfClipPrecision = font->lfClipPrecision;
    lf->lfQuality = font->lfQuality;
    lf->lfPitchAndFamily = font->lfPitchAndFamily;
    wcscpy_s(lf->lfFaceName, font->lfFaceName);
    return lf;
}


WORDITEMLIST* new_worditem(WORDITEMMENU* menu, int size_x, int size_y, WORDITEMTYPE type)
{
    WORDITEMLIST* temp_list = new WORDITEMLIST;
    temp_list->parent = menu;

    if (menu->first_list != NULL)
    {
        WORDITEMLIST* temp_while_list = menu->first_list;
        while (temp_while_list->next != NULL)
        {
            temp_while_list = temp_while_list->next;
        }
        temp_list->back = temp_while_list;
        temp_while_list->next = temp_list;
    }
    else
    {
        menu->first_list = temp_list;
        temp_list->back = NULL;

    }

    temp_list->next = NULL;


    temp_list->size_x = size_x;
    temp_list->size_y = size_y;
    temp_list->toggle = false;
    temp_list->type = type;
    return temp_list;
}

TCHARLIST* Insert_tchar(WORDITEMMENU* menu, LINELIST* line, TCHARLIST* tchar, WPARAM wParam)
{
    TCHARLIST* now_char;
    TCHARLIST* temp_char;
    SIZE temp_size;

    TCHAR temp_size_char[2];

    if (tchar == NULL)
    {
        now_char = new_tcharlist(menu->font);
        if (line->first_tchar != NULL)
        {
            temp_char = line->first_tchar;
            temp_char->back = now_char;
            now_char->next = temp_char;
        }
        line->first_tchar = now_char;
    }
    else
    {
        now_char = new_tcharlist(menu->font);
        if (tchar->next != NULL)
        {
            temp_char = tchar->next;
            now_char->next = temp_char;
            temp_char->back = now_char;

        }

        tchar->next = now_char;
        now_char->back = tchar;
    }

    now_char->parent = line;
    now_char->str = wParam;

    return now_char;
}

TCHARLIST* Delete_tchar(WORDBOX* box, TCHARLIST* tchar)
{
    TCHARLIST* temptchar;
    TCHARLIST* returnchar;
    if (tchar->next != NULL)
    {
        temptchar = tchar->next;
        if (tchar->back != NULL) //a b c <- b
        {
            tchar->back->next = temptchar;
            temptchar->back = tchar->back;
            returnchar = tchar->back;
        }
        else // a b c <- a
        {
            temptchar->back = NULL;
            tchar->parent->first_tchar = temptchar;
            returnchar = NULL;
        }
    }
    else // a b c <- c
    {
        if (tchar->back != NULL) // a b c <- c
        {
            temptchar = tchar->back;
            temptchar->next = NULL;
            returnchar = tchar->back;
        }
        else // c <- c
        {
            tchar->parent->first_tchar = NULL;
            returnchar = NULL;
        }

    }

    Set_focusword(box->focus, true, tchar->parent, returnchar);

    Delete_hard_tchar(tchar);


    return returnchar;

}

void Delete_hard_tchar(TCHARLIST* tchar)
{
    //DeleteObject(tchar->font);
    if (tchar->font != NULL)
    {
        delete tchar->font; 
    }
    
    delete tchar;
}

void Print_tcharlist(HDC hdc, TCHARLIST* tcharlist, int start_x, int start_y)
{
    TCHARLIST* temp_tchar = tcharlist;
    TCHARLIST* before_tchar;
    TCHAR temp_char[2];
    SIZE size;
    int print_x = start_x;
    int print_y = start_y;
    int cal_y = 0;

    do
    {
        HFONT hfont = CreateFontIndirect(temp_tchar->font);
        HFONT holdfont = (HFONT)SelectObject(hdc, hfont);
        temp_char[0] = temp_tchar->str;
        temp_char[1] = NULL;
        GetTextExtentPoint32(hdc, temp_char, 1, &size);
        if (tcharlist->parent->parent->size_x < print_x + size.cx - start_x) //워드 밖으로 나가면 줄바꿈
        {
            temp_tchar->parent->line_size_y += cal_y;
            print_y += cal_y;
            print_x = start_x;
            cal_y = 0;
        }

        TextOut(hdc, print_x, print_y, temp_char, 1);
        SelectObject(hdc, holdfont);
        DeleteObject(hfont);
        temp_tchar->str_size_x = size.cx;
        temp_tchar->str_size_y = size.cy;
        temp_tchar->start_y = print_y;

        print_x += size.cx;

        if (cal_y < size.cy)
        {
            cal_y = size.cy;
        }

        if (tcharlist->parent->parent->focus->focus == true) //캐럿 위치 찾기
        {
            if (tcharlist->parent->parent->focus->tcharlist == temp_tchar)
            {
                Set_focuswordpos(tcharlist->parent->parent->focus, print_x, print_y);
            }
        }


        before_tchar = temp_tchar;
        temp_tchar = temp_tchar->next;

    } while (temp_tchar != NULL);
    temp_tchar = before_tchar;

    temp_tchar->parent->line_size_y += cal_y;

}

LINELIST* Insert_line(WORDBOX* box, LINELIST* line, TCHARLIST* tchar)
{
    LINELIST* now_line;
    LINELIST* temp_line;

    if (line == NULL)
    {
        now_line = new_linelist();
        box->first_line = now_line;
    }
    else
    {
        if (tchar != NULL)
        {
            now_line = new_linelist();
            if (line->next != NULL)
            {
                temp_line = line->next;
                now_line->next = temp_line;
                temp_line->back = now_line;
            }
            line->next = now_line;
            now_line->back = line;

            if (tchar->next != NULL)
            {
                TCHARLIST* temptchar = tchar->next;

                now_line->first_tchar = temptchar;
                tchar->next = NULL;
                temptchar->back = NULL;

                while (temptchar != NULL)
                {
                    temptchar->parent = now_line;
                    temptchar = temptchar->next;
                }
            }
        }
        else
        {
            now_line = new_linelist();
            if (line->next != NULL)
            {
                temp_line = line->next;
                now_line->next = temp_line;
                temp_line->back = now_line;
            }
            line->next = now_line;
            now_line->back = line;

            if (line->first_tchar != NULL)
            {
                TCHARLIST* temptchar = line->first_tchar;
                now_line->first_tchar = temptchar;

                while (temptchar != NULL)
                {
                    temptchar->parent = now_line;
                    temptchar = temptchar->next;
                }
                line->first_tchar = NULL;
            }
        }
    }

    now_line->parent = box;
    return now_line;

}

LINELIST* Delete_line(WORDBOX* box, LINELIST* line, bool hard, bool move_caret)
{
    LINELIST* return_line;
    TCHARLIST* return_tchar;
    LINELIST* templine;

    if (line->back == NULL)
    {
        return line;
    }
    else
    {
        if (line->next == NULL)
        {
            line->back->next = NULL;
        }
        else
        {
            line->back->next = line->next;
            line->next->back = line->back;
        }

        if (line->back->first_tchar != NULL)
        {
            TCHARLIST* tempchar;
            tempchar = line->back->first_tchar;
            while (tempchar->next != NULL)
            {
                tempchar = tempchar->next;
            }
            return_line = line->back;
            return_tchar = tempchar;
        }
        else
        {
            return_line = line->back;
            return_tchar = NULL;
        }

        if (line->first_tchar != NULL)
        {
            TCHARLIST* tempchar;
            tempchar = line->first_tchar;
            while (tempchar != NULL)
            {
                tempchar->parent = line->back;
                tempchar = tempchar->next;
            }

            if (line->back->first_tchar != NULL)
            {

                tempchar = line->back->first_tchar;
                while (tempchar->next != NULL)
                {
                    tempchar = tempchar->next;
                }

                line->first_tchar->back = tempchar;
                tempchar->next = line->first_tchar;

            }
            else
            {
                line->back->first_tchar = line->first_tchar;
            }
            return_line = line->back;
            return_tchar = tempchar;
        }

        if (hard == true)
        {
            Delete_hard_linelist(line);
        }
        else
        {
            delete line;
        }

        if (move_caret == true)
        {
            Set_focusword(box->focus, true, return_line, return_tchar);
        }



        return return_line;

    }
}

void Delete_hard_linelist(LINELIST* line)
{
    TCHARLIST* temp_char;
    TCHARLIST* del_char;
    if (line->first_tchar != NULL)
    {
        temp_char = line->first_tchar;
        while (temp_char != NULL)
        {
            del_char = temp_char;
            temp_char = temp_char->next;
            Delete_hard_tchar(del_char);
            //delete del_char;
        }
    }

    delete line;
}

void Print_linelist(HDC hdc, LINELIST* linelist, int start_x, int start_y)
{
    LINELIST* now_line = linelist;
    int print_y = start_y;
    while (now_line != NULL)
    {
        if (linelist->parent->focus->focus == true) //캐럿 위치 찾기
        {
            if (linelist->parent->focus->linelist == now_line && linelist->parent->focus->tcharlist == NULL)
            {
                Set_focuswordpos(linelist->parent->focus, start_x, print_y);
            }
        }


        if (now_line->first_tchar != NULL)
        {
            now_line->line_size_x = word_size_x;
            now_line->line_size_y = 0;
            Print_tcharlist(hdc, now_line->first_tchar, start_x, print_y);
        }
        else
        {
            now_line->line_size_x = word_size_x;
            now_line->line_size_y = line_size_y_def;
        }
        print_y += now_line->line_size_y;
        now_line = now_line->next;

    }

}

bool Box_cast(int pos_x, int pos_y, int size_x, int size_y, int point_x, int point_y)
{
    //box x좌표, box y좌표, box크기x, box크기y, 찾기x, 찾기y
    //box 영역안에 point가 있는지 확인하는 함수
    if (pos_x <= point_x && point_x < pos_x + size_x &&
        pos_y <= point_y && point_y < pos_y + size_y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Point_finder(WORDBOX* box, WORDITEMMENU* menu, int pos_x, int pos_y, C_SIZE* csize)
{
    /*
    box 개체, menu 개체, 위치x, 위치y, 디버그C_SIZE
    pos_x, pos_y 위치에 어떤 요소가 있는지 검색하는 함수
    */
    csize->pos_x = 0;
    csize->pos_y = 0;
    csize->size_x = 0;
    csize->size_y = 0;

    int now_x = 0;
    int now_y = 0;

    if (Box_cast(box->pos_x, box->pos_y, box->pos_x + box->size_x, box->pos_y + box->size_y, pos_x, pos_y)) //word box 영역 확인
    {
        now_x += box->pos_x;
        now_y += box->pos_y;

        LINELIST* now_list = box->first_line;
        while (now_list != NULL)
        {
            if (Box_cast(now_x, now_y, now_list->line_size_x, now_list->line_size_y, pos_x, pos_y) == true) //word box -> linelist 영역 확인
            {
                if (now_list->first_tchar == NULL)
                {
                    Set_focusword(box->focus, true, now_list, NULL);
                    return;
                }
                TCHARLIST* now_tchar = now_list->first_tchar;
                int tchar_list_start_x = now_x;
                int tchar_list_start_y = now_y;
                int tchar_now_y = now_tchar->start_y;

                while (now_tchar != NULL)
                {
                    if (Box_cast(now_x, now_y, now_tchar->str_size_x, now_tchar->str_size_y, pos_x, pos_y) == true) //word box -> linelist -> tcharlist 영역확인
                    {
                        if (now_x + (now_tchar->str_size_x / 2) >= pos_x) //글자 왼쪽,오른쪽 구분
                        {
                            if (now_tchar->back != NULL) //왼쪽 글자 반환
                            {
                                Set_focusword(box->focus, true, now_tchar->parent, now_tchar->back);
                            }
                            else //부모 문단 반환
                            {
                                Set_focusword(box->focus, true, now_tchar->parent, NULL);
                            }
                        }
                        else //글자 자신 반환
                        {
                            Set_focusword(box->focus, true, now_tchar->parent, now_tchar);
                        }
                        return;
                    }
                    now_x += now_tchar->str_size_x;
                    now_tchar = now_tchar->next;
                    if (now_tchar != NULL)
                    {
                        if (now_tchar->start_y != tchar_now_y)
                        {
                            now_x = tchar_list_start_x;
                            now_y = now_tchar->start_y;
                            tchar_now_y = now_tchar->start_y;
                        }
                    }
                }
                //word box -> linelist -> tcharlist 해당 없음 -> linelist 글자 없는 빈 영역
                now_tchar = now_list->first_tchar;
                while (now_tchar->next != NULL)
                {
                    now_tchar = now_tchar->next;
                }
                Set_focusword(box->focus, true, now_tchar->parent, now_tchar); 
                return;
            }
            now_y += now_list->line_size_y;
            now_list = now_list->next;
        }
    }
    else if (Box_cast(menu->pos_x, menu->pos_y, menu->size_x, menu->size_y, pos_x, pos_y) == true)
    {
        now_x += menu->pos_x;
        now_y += menu->pos_y;

        if (menu->first_list == NULL)
        {
            return;
        }
        WORDITEMLIST* tempitem = menu->first_list;
        while (tempitem != NULL)
        {
            if (Box_cast(now_x, now_y, now_x + tempitem->size_x, now_y + tempitem->size_y, pos_x, pos_y) == true)
            {
                csize->pos_x = now_x;
                csize->pos_y = now_y;
                csize->size_x = 10;
                csize->size_y = 10;
                if (tempitem->type == WORDITEMTYPE_bold)
                {
                    if (tempitem->toggle == true)
                    {
                        menu->font->lfWeight = FW_NORMAL;
                        tempitem->toggle = false;
                    }
                    else
                    {
                        menu->font->lfWeight = FW_BOLD;
                        tempitem->toggle = true;
                    }
                }
                else if (tempitem->type == WORDITEMTYPE_italic)
                {
                    if (tempitem->toggle == true)
                    {
                        menu->font->lfItalic = FALSE;
                        tempitem->toggle = false;
                    }
                    else
                    {
                        menu->font->lfItalic = TRUE;
                        tempitem->toggle = true;
                    }
                }
            }
            now_x += tempitem->size_x;
            tempitem = tempitem->next;
        }
    }
}



void Set_focusword(FOCUS_WORDBOX* focus, bool focus_bool, LINELIST* line, TCHARLIST* tchar)
{
    //현재 wordbox의 포커스, 라인, 문자
    focus->focus = focus_bool;
    focus->linelist = line;
    focus->tcharlist = tchar;

}

void Set_focuswordpos(FOCUS_WORDBOX* focus, int x, int y)
{
    //현재 wordbox의 focus 위치
    focus->pos_x = x;
    focus->pos_y = y;
}

void Paint_worditemmenu(HDC hdc, WORDITEMMENU* menu)
{
    Rectangle(hdc, menu->pos_x, menu->pos_y, menu->pos_x + menu->size_x, menu->pos_y + menu->size_y);
    if (menu->first_list != NULL)
    {
        WORDITEMLIST* temp_item = menu->first_list;
        int now_x = menu->pos_x;
        int now_y = menu->pos_y;
        while (temp_item != NULL)
        {

            if (temp_item->toggle == true)
            {
                HBRUSH h_brush = CreateSolidBrush(RGB(50, 50, 50));
                HBRUSH old_brush = (HBRUSH)SelectObject(hdc, h_brush);
                Rectangle(hdc, now_x, now_y, now_x + temp_item->size_x, now_y + temp_item->size_y);
                SelectObject(hdc, old_brush);
                DeleteObject(h_brush);
            }
            else
            {
                Rectangle(hdc, now_x, now_y, now_x + temp_item->size_x, now_y + temp_item->size_y);
            }



            now_x += temp_item->size_x;
            temp_item = temp_item->next;
        }

    }
}

void Set_wordmenufont(FOCUS_WORDBOX* focus, WORDITEMMENU* menu)
{
    if (focus->focus == true && focus->tcharlist != NULL)
    {
        if (menu->font != NULL)
        {
            delete menu->font;
        }
        
        menu->font = Font_copy(focus->tcharlist->font);

        if (menu->first_list != NULL)
        {
            FONT_TYPE_BOOL temp_font_type;
            WORDITEMLIST* temp_list = menu->first_list;
            while (temp_list)
            {
                int temp_int = Logfont_convert(menu->font, temp_list->type);
                Font_finder(&temp_font_type, WORDITEMTRPE_NULL, -1, temp_int);
                if (temp_font_type.font != -1)
                {
                    if (temp_font_type.font_boolint == 0)
                    {
                        temp_list->toggle = false;
                    }
                    else if (temp_font_type.font_boolint == 1)
                    {
                        temp_list->toggle = true;
                    }
                    else
                    {

                    }
                    
                    
                }

                temp_list = temp_list->next;
            }
        }

    }
}

void Font_finder(FONT_TYPE_BOOL* temp, WORDITEMTYPE type, int bool_int, int d_type)
{
    if ((type == WORDITEMTYPE_bold && bool_int == 0) || d_type == FW_NORMAL)
    {
        temp->font = FW_NORMAL;
        temp->font_boolint = 0;
    }
    else if ((type == WORDITEMTYPE_bold && bool_int == 1) || d_type == FW_BOLD)
    {
        temp->font = FW_BOLD;
        temp->font_boolint = 1;
    }
    else if (type == WORDITEMTYPE_italic)
    {
        if (bool_int == 0)
        {
            temp->font = FALSE;
            temp->font_boolint = 0;
        }
        else
        {
            temp->font = TRUE;
            temp->font_boolint = 1;
        }
    }
    else if (d_type == TRUE)
    {
        temp->font = TRUE;
        temp->font_boolint = 1;
    }
    else if (d_type == FALSE)
    {
        temp->font = FALSE;
        temp->font_boolint = 1;
    }
    else
    {
        temp->font = -1;
        temp->font_boolint = -1;
    }
    /*
    switch (type)
    {
    case WORDITEMTYPE_bold:
        if (bool_int == 0)
        {
            temp->font = FW_NORMAL;
            temp->font_boolint = 0;
        }
        else
        {
            temp->font = FW_BOLD;
            temp->font_boolint = 1;
        }
        break;
    case WORDITEMTYPE_italic:
        if (bool_int == 0)
        {
            temp->font = FALSE;
            temp->font_boolint = 0;
        }
        else
        {
            temp->font = TRUE;
            temp->font_boolint = 1;
        }
        break;

    default:
        temp->font = -1;
        temp->font_boolint = -1;
        break;
    }

    */
}

int Logfont_convert(LOGFONT* font, WORDITEMTYPE type)
{
    switch (type)
    {
    case WORDITEMTYPE_bold:
        return font->lfWeight;
    case WORDITEMTYPE_italic:
        return font->lfItalic;
    default:
        return -1;

    }
}