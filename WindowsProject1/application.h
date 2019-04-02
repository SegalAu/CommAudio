#pragma once

// Struct to pass into thread that executes readport()
typedef struct parameters {
	HANDLE hComm;
	char* buffer;
	HWND hwnd;
	parameters(HANDLE hComm, HWND hwnd, char* buffer) : hComm(hComm), hwnd(hwnd), buffer(buffer) {}
} *THREADPARAMETERS;

void create_read_thread(HANDLE hComm, HWND hwnd, char* buffer);
HANDLE get_thread();
void write_char(HANDLE hComm, WPARAM wParam);
void display_char(HWND hwnd, char* buffer);
