#include "utils.h"
#include <stdio.h>
#include <string.h>

int ObterSecoesComSourcePath(const char *iniPath, HWND hCombo) {
    char buffer[4096];
    GetPrivateProfileSectionNamesA(buffer, sizeof(buffer), iniPath);

    int count = 0;
    char *p = buffer;
    while (*p) {
        char val[MAX_PATH] = {0};
        GetPrivateProfileStringA(p, "SourcePath", "", val, sizeof(val), iniPath);
        if (strlen(val) > 0) {
            SendMessageA(hCombo, CB_ADDSTRING, 0, (LPARAM)p);
            count++;
        }
        p += strlen(p) + 1;
    }
    return count;
}

void ObterSourcePathDaSecao(const char *iniPath, const char *secao, char *dest) {
    GetPrivateProfileStringA(secao, "SourcePath", "", dest, MAX_PATH, iniPath);
}
