#pragma once

void open_port(LPCSTR comPort, HANDLE* hComm, HWND hwnd, char buffer[]);
DWORD WINAPI read_port(LPVOID threadParam);
void transmit_port(HANDLE hComm, LPCVOID lpBuffer);
