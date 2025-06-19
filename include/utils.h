#ifndef UTILS_H
#define UTILS_H

#include <windows.h>

int ObterSecoesComSourcePath(const char *iniPath, HWND hCombo);
void ObterSourcePathDaSecao(const char *iniPath, const char *secao, char *dest);

#endif
