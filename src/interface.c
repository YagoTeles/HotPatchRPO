#include "interface.h"
#include "utils.h"
#include "copiaapo.h"
#include <windows.h>
#include <commdlg.h>
#include <shlwapi.h>
#include <stdio.h>

#define ID_EDIT_EXE     1000
#define ID_BTN_EXE      1001
#define ID_COMBO_SECAO  1002
#define ID_EDIT_PTM     1003
#define ID_BTN_PTM      1004
#define ID_BTN_CONFIRM  1005

HWND hComboSecao, hEditExe, hEditPtm;
HFONT fontePadrao;
char iniPath[MAX_PATH];
char secaoSelecionada[128] = {0};

void SelecionarArquivo(HWND hwnd, HWND hEdit, LPCWSTR filtro) {
    wchar_t file[MAX_PATH] = {0};
    OPENFILENAMEW ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = filtro;
    ofn.lpstrFile = file;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;
    if (GetOpenFileNameW(&ofn)) {
        SetWindowTextW(hEdit, file);
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static char caminhoExe[MAX_PATH];

    switch (msg) {
        case WM_CREATE: {
            fontePadrao = CreateFontW(
                14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Verdana"
            );

            // Label Appserver
            HWND lblExe = CreateWindowW(L"static", L"Selecionar appserver.exe:", WS_VISIBLE | WS_CHILD,
                                        20, 20, 400, 20, hwnd, NULL, NULL, NULL);
            SendMessageW(lblExe, WM_SETFONT, (WPARAM)fontePadrao, TRUE);

            hEditExe = CreateWindowW(L"edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
                                     20, 45, 400, 22, hwnd, (HMENU)ID_EDIT_EXE, NULL, NULL);
            SendMessageW(hEditExe, WM_SETFONT, (WPARAM)fontePadrao, TRUE);

            HWND btnExe = CreateWindowW(L"button", L"[...]", WS_VISIBLE | WS_CHILD,
                                        430, 45, 30, 22, hwnd, (HMENU)ID_BTN_EXE, NULL, NULL);
            SendMessageW(btnExe, WM_SETFONT, (WPARAM)fontePadrao, TRUE);

            // Label Ambiente
            HWND lblAmb = CreateWindowW(L"static", L"Ambiente:", WS_VISIBLE | WS_CHILD,
                                        20, 80, 400, 20, hwnd, NULL, NULL, NULL);
            SendMessageW(lblAmb, WM_SETFONT, (WPARAM)fontePadrao, TRUE);

            hComboSecao = CreateWindowW(L"combobox", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,
                            20, 105, 400, 200, hwnd, (HMENU)ID_COMBO_SECAO, NULL, NULL);
            SendMessageW(hComboSecao, WM_SETFONT, (WPARAM)fontePadrao, TRUE);

            // Label .ptm
            HWND lblPtm = CreateWindowW(L"static", L"Arquivo .ptm:", WS_VISIBLE | WS_CHILD,
                                        20, 140, 400, 20, hwnd, NULL, NULL, NULL);
            SendMessageW(lblPtm, WM_SETFONT, (WPARAM)fontePadrao, TRUE);

            hEditPtm = CreateWindowW(L"edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER,
                                     20, 165, 400, 22, hwnd, (HMENU)ID_EDIT_PTM, NULL, NULL);
            SendMessageW(hEditPtm, WM_SETFONT, (WPARAM)fontePadrao, TRUE);

            HWND btnPtm = CreateWindowW(L"button", L"[...]", WS_VISIBLE | WS_CHILD,
                                        430, 165, 30, 22, hwnd, (HMENU)ID_BTN_PTM, NULL, NULL);
            SendMessageW(btnPtm, WM_SETFONT, (WPARAM)fontePadrao, TRUE);

            // Botão confirmar
            HWND btnConfirm = CreateWindowW(L"button", L"Confirmar", WS_VISIBLE | WS_CHILD,
                                            380, 210, 80, 25, hwnd, (HMENU)ID_BTN_CONFIRM, NULL, NULL);
            SendMessageW(btnConfirm, WM_SETFONT, (WPARAM)fontePadrao, TRUE);
            break;
        }

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case ID_BTN_EXE: {
                    wchar_t file[MAX_PATH] = {0};
                    OPENFILENAMEW ofn = {0};
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hwnd;
                    ofn.lpstrFilter = L"Executável\0*.exe\0Todos\0*.*\0";
                    ofn.lpstrFile = file;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.Flags = OFN_FILEMUSTEXIST;
                    if (GetOpenFileNameW(&ofn)) {
                        SetWindowTextW(hEditExe, file);
                        wcstombs(caminhoExe, file, MAX_PATH);
                        strcpy(iniPath, caminhoExe);
                        char *ponto = strrchr(iniPath, '.');
                        if (ponto) strcpy(ponto, ".ini");
                        SendMessageA(hComboSecao, CB_RESETCONTENT, 0, 0);
                        ObterSecoesComSourcePath(iniPath, hComboSecao);
                    }
                    break;
                }

                case ID_BTN_PTM:
                    SelecionarArquivo(hwnd, hEditPtm, L"Patch Protheus (*.ptm)\0*.ptm\0");
                    break;

                case ID_COMBO_SECAO:
                    if (HIWORD(wParam) == CBN_SELCHANGE) {
                        int sel = SendMessageA(hComboSecao, CB_GETCURSEL, 0, 0);
                        SendMessageA(hComboSecao, CB_GETLBTEXT, sel, (LPARAM)secaoSelecionada);
                    }
                    break;

                case ID_BTN_CONFIRM: {
                    char sourcePath[MAX_PATH] = {0};
                    if (strlen(secaoSelecionada) == 0 || strlen(iniPath) == 0) {
                        MessageBoxW(hwnd, L"Selecione o executável e o ambiente.", L"Erro", MB_OK | MB_ICONERROR);
                        break;
                    }
                    ObterSourcePathDaSecao(iniPath, secaoSelecionada, sourcePath);
                    ExecutarConfirmar(hwnd, sourcePath, hEditPtm);
                    break;
                }
            }
            break;
        }

        case WM_CTLCOLORSTATIC: {
            HDC hdc = (HDC)wParam;
            SetBkMode(hdc, TRANSPARENT);
            return (LRESULT)GetStockObject(NULL_BRUSH);
        }

        case WM_DESTROY:
            DeleteObject(fontePadrao);
            PostQuitMessage(0);
            break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void RegistrarJanela(HINSTANCE hInstance) {
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"HotPatchRPO";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassW(&wc);
}
