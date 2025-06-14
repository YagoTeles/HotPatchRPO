#ifndef INTERFACE_H
#define INTERFACE_H

#include <windows.h>

// Interface
void RegistrarJanela(HINSTANCE hInstance);

// INI
void ObterCaminhoIni();
void CarregarINI(HWND hEdit);
void SalvarINI(HWND hEdit);

// Execução
void ExecutarConfirmar(HWND hwnd, HWND hEditDir, HWND hEditPtm, HWND hEditExe);

#endif
