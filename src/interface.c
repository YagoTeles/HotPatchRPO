#include "interface.h"
#include <commdlg.h>
#include <shlobj.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define ID_EDIT_DIR      1001
#define ID_BTN_DIR       2001
#define ID_EDIT_PTM      1002
#define ID_BTN_PTM       2002
#define ID_EDIT_EXE      1003
#define ID_BTN_EXE       2003
#define ID_BTN_CONFIRMAR 3001

HFONT fontePadrao;
HWND hEditDir, hEditPtm, hEditExe;

extern void ObterCaminhoIni();
extern void CarregarINI(HWND hEdit);
extern void SalvarINI(HWND hEdit);
extern void ExecutarConfirmar(HWND hwnd, HWND hEditDir, HWND hEditPtm, HWND hEditExe);

void SelecionarPasta(HWND hwnd, HWND hEdit, LPCWSTR titulo) {
    BROWSEINFOW bi = {0};
    bi.hwndOwner = hwnd;
    bi.lpszTitle = titulo;
    LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);
    if (pidl) {
        wchar_t path[MAX_PATH];
        SHGetPathFromIDListW(pidl, path);
        SetWindowTextW(hEdit, path);
    }
}

void SelecionarArquivo(HWND hwnd, HWND hEdit) {
    wchar_t file[MAX_PATH] = {0};
    OPENFILENAMEW ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = L"Todos os arquivos\0*.*\0";
    ofn.lpstrFile = file;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn))
        SetWindowTextW(hEdit, file);
}

HWND CriarEdit(HWND hwndPai, int id, int x, int y, int largura, int altura) {
    HWND h = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                           x, y, largura, altura, hwndPai, (HMENU)(intptr_t)id, NULL, NULL);
    SendMessageW(h, WM_SETFONT, (WPARAM)fontePadrao, TRUE);
    return h;
}

HWND CriarBotao(HWND hwndPai, LPCWSTR texto, int id, int x, int y, int largura, int altura) {
    HWND h = CreateWindowW(L"button", texto, WS_VISIBLE | WS_CHILD,
                           x, y, largura, altura, hwndPai, (HMENU)(intptr_t)id, NULL, NULL);
    SendMessageW(h, WM_SETFONT, (WPARAM)fontePadrao, TRUE);
    return h;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            ObterCaminhoIni();
            fontePadrao = CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                      ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                      DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Times New Roman");

            CreateWindowW(L"static", L"Selecione a pasta APO:", WS_VISIBLE | WS_CHILD, 20, 20, 400, 20, hwnd, NULL, NULL, NULL);
            hEditDir = CriarEdit(hwnd, ID_EDIT_DIR, 20, 45, 450, 22);
            CriarBotao(hwnd, L"Browse", ID_BTN_DIR, 480, 45, 80, 22);

            CreateWindowW(L"static", L"Arquivo .PTM:", WS_VISIBLE | WS_CHILD, 20, 80, 400, 20, hwnd, NULL, NULL, NULL);
            hEditPtm = CriarEdit(hwnd, ID_EDIT_PTM, 20, 105, 450, 22);
            CriarBotao(hwnd, L"Browse", ID_BTN_PTM, 480, 105, 80, 22);

            CreateWindowW(L"static", L"Arquivo .EXE:", WS_VISIBLE | WS_CHILD, 20, 140, 400, 20, hwnd, NULL, NULL, NULL);
            hEditExe = CriarEdit(hwnd, ID_EDIT_EXE, 20, 165, 450, 22);
            CriarBotao(hwnd, L"Browse", ID_BTN_EXE, 480, 165, 80, 22);

            CriarBotao(hwnd, L"Confirmar", ID_BTN_CONFIRMAR, 480, 300, 80, 25);

            CarregarINI(hEditDir);
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_BTN_DIR:
                    SelecionarPasta(hwnd, hEditDir, L"Escolha o diretório");
                    break;
                case ID_BTN_PTM:
                    SelecionarArquivo(hwnd, hEditPtm);
                    break;
                case ID_BTN_EXE:
                    SelecionarArquivo(hwnd, hEditExe);
                    break;
                case ID_BTN_CONFIRMAR:
                    SalvarINI(hEditDir);
                    ExecutarConfirmar(hwnd, hEditDir, hEditPtm, hEditExe);
                    break;
            }
            break;

        case WM_DESTROY:
            DeleteObject(fontePadrao);
            PostQuitMessage(0);
            break;

        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            SetBkMode(hdcStatic, TRANSPARENT);
            return (LRESULT)GetStockObject(NULL_BRUSH);
        }
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void RegistrarJanela(HINSTANCE hInstance) {
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"AppWin";
    RegisterClassW(&wc);
}
