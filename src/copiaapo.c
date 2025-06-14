#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "interface.h"

void CopiarRecursivo(const char *origem, const char *destino) {
    char srcPath[1024], dstPath[1024];
    WIN32_FIND_DATAA ffd;

    snprintf(srcPath, sizeof(srcPath), "%s\\*", origem);
    HANDLE hFind = FindFirstFileA(srcPath, &ffd);
    if (hFind == INVALID_HANDLE_VALUE) return;

    CreateDirectoryA(destino, NULL);

    do {
        if (strcmp(ffd.cFileName, ".") == 0 || strcmp(ffd.cFileName, "..") == 0) continue;

        snprintf(srcPath, sizeof(srcPath), "%s\\%s", origem, ffd.cFileName);
        snprintf(dstPath, sizeof(dstPath), "%s\\%s", destino, ffd.cFileName);

        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            CopiarRecursivo(srcPath, dstPath);
        } else if (_stricmp(ffd.cFileName, "custom.rpo") == 0 || _stricmp(ffd.cFileName, "tttm120.rpo") == 0) {
            CopyFileA(srcPath, dstPath, FALSE);
        }
    } while (FindNextFileA(hFind, &ffd));

    FindClose(hFind);
}

void ExecutarConfirmar(HWND hwnd, HWND hEditDir, HWND hEditPtm, HWND hEditExe) {
    char dirOrigem[MAX_PATH], arquivoPtm[MAX_PATH], arquivoExe[MAX_PATH];
    GetWindowTextA(hEditDir, dirOrigem, MAX_PATH);
    GetWindowTextA(hEditPtm, arquivoPtm, MAX_PATH);
    GetWindowTextA(hEditExe, arquivoExe, MAX_PATH);

    if (strlen(dirOrigem) == 0 || strlen(arquivoPtm) == 0 || strlen(arquivoExe) == 0) {
        MessageBoxW(hwnd, L"Preencha todos os campos!", L"Erro", MB_OK | MB_ICONERROR);
        return;
    }

    DWORD attr = GetFileAttributesA(dirOrigem);
    if (attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY)) {
        MessageBoxW(hwnd, L"Diretório inválido.", L"Erro", MB_OK | MB_ICONERROR);
        return;
    }

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%y%m%d%H%M%S", tm_info);

    char parentDir[MAX_PATH];
    strcpy(parentDir, dirOrigem);
    char *lastSlash = strrchr(parentDir, '\\');
    if (lastSlash) *lastSlash = '\0';

    char nomeOriginal[MAX_PATH];
    strcpy(nomeOriginal, lastSlash + 1);

    char dirDestino[1024];
    snprintf(dirDestino, sizeof(dirDestino), "%s\\%s_%s", parentDir, nomeOriginal, timestamp);

    CopiarRecursivo(dirOrigem, dirDestino);

    char comando[2048];
    snprintf(comando, sizeof(comando), "\"%s\" \"%s\" \"%s\"", arquivoExe, arquivoPtm, dirDestino);
    WinExec(comando, SW_SHOW);

    MessageBoxW(hwnd, L"Diretório copiado e processo iniciado com sucesso.", L"Concluído", MB_OK | MB_ICONINFORMATION);
}
