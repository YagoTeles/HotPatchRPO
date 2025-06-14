
#include <windows.h>
#include <stdio.h>
#include <string.h>

char iniPath[MAX_PATH];

void ObterCaminhoIni() {
    GetModuleFileNameA(NULL, iniPath, MAX_PATH);
    char *lastSlash = strrchr(iniPath, '\\');
    if (lastSlash) *(lastSlash + 1) = '\0';
    strcat(iniPath, "aplica_patch.ini");
}

void CarregarINI(HWND hEditDir) {
    FILE *f = fopen(iniPath, "r");
    if (!f) return;
    char linha[512];
    while (fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\r\n")] = 0;
        if (strncmp(linha, "dir=", 4) == 0)
            SetWindowTextA(hEditDir, linha + 4);
    }
    fclose(f);
}

void SalvarINI(HWND hEditDir) {
    FILE *f = fopen(iniPath, "w");
    if (!f) return;
    char buffer[MAX_PATH];
    GetWindowTextA(hEditDir, buffer, MAX_PATH);
    fprintf(f, "dir=%s\n", buffer);
    fclose(f);
}
