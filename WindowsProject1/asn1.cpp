/*------------------------------------------------------------------------------------------------------------------
--	SOURCE FILE:		3980_asn1.cpp -			An application that acts as a minimal Windows terminal emulator. 
--												Monitors the keyboard input for characters to transmit to the serial
--												port, and displays characters received from the serial port. All 
--												functions of this application are split into the physical and 
--												application files from the OSI model.
--
--	PROGRAM:			asn1.exe
--
--	FUNCTIONS:			int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspszCmdParam, int nCmdShow)
--						LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
--
--
--	DATE:				September 24th, 2018
--
--	DESIGNER:			Dasha Strigoun, A00983479
--
--	PROGRAMMER:			Dasha Strigoun, A00983479
--
----------------------------------------------------------------------------------------------------------------------*/

#define STRICT

#pragma warning (disable: 4096)

#include <Windows.h>
#include <stdio.h>
#include "asn1.h"
#include "physical.h"
#include "application.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static TCHAR Name[] = TEXT("COMP 3980 - Assignment 1");

COMMCONFIG cc;
HANDLE hComm;
HWND hwnd;
LPCSTR comPort;

char buffer[1];

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspszCmdParam, int nCmdShow) {
	MSG Msg;
	WNDCLASSEX Wcl;

	// Define a Window class
	Wcl.cbSize = sizeof(WNDCLASSEX);
	Wcl.style = CS_HREDRAW | CS_VREDRAW;
	Wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	Wcl.hIconSm = NULL;
	Wcl.hCursor = LoadCursor(NULL, IDC_ARROW);

	Wcl.lpfnWndProc = WndProc;
	Wcl.hInstance = hInst;
	Wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	Wcl.lpszClassName = Name;

	Wcl.lpszMenuName = "MYMENU";
	Wcl.cbClsExtra = 0;
	Wcl.cbWndExtra = 0;

	// Register the class
	if (!RegisterClassEx(&Wcl))
		return 0;

	// Create the window
	hwnd = CreateWindow(Name, Name, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 400, NULL, NULL, hInst, NULL);

	// Display the window
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Create the message loop
	while (GetMessage(&Msg, NULL, 0, 0)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
		case WM_COMMAND:	// Process Menu item select
			switch (LOWORD(wParam)) {
				case IDM_COM1:	// Select COM1 properties
					CloseHandle(hComm);
					cc.dwSize = sizeof(COMMCONFIG);
					cc.wVersion = 0x100;

					// Displays window to alter port settings
					if (!CommConfigDialog("Com1", hwnd, &cc))
						break;

					SetCommConfig(hComm, &cc, cc.dwSize);
					break;

				case IDM_COM3:	// Select COM2 properties
					CloseHandle(hComm);
					cc.dwSize = sizeof(COMMCONFIG);
					cc.wVersion = 0x100;

					// Displays window to alter port settings
					if (!CommConfigDialog("Com3", hwnd, &cc))
						break;

					SetCommConfig(hComm, &cc, cc.dwSize);
					break;

				case IDM_CON_COM1:	// Select COM1 to connect
					comPort = "com1";
					open_port(comPort, &hComm, hwnd, buffer);
					break;

				case IDM_CON_COM3:	// Select COM3 to connect
					comPort = "com3";
					open_port(comPort, &hComm, hwnd, buffer);
					break;

				case IDM_CONNECT:	// Connect to output / input device
					//Start Read Thread 
					create_read_thread(hComm, hwnd, buffer); 


					//Start Write Thread


					break;

				case IDM_HELP:	// Select Help
					MessageBox(NULL, "Sorry, no help for you", "", MB_OK);
					break;
			}
			break;

		case WM_CHAR:	// Process keystroke
			switch (wParam) {
				case 0x1B: // Process an escape character
					TerminateThread(get_thread(), 0);
					CloseHandle(hComm);

					// Display window to alter port settings
					if (comPort != NULL) {
						cc.dwSize = sizeof(COMMCONFIG);
						cc.wVersion = 0x100;
						GetCommConfig(hComm, &cc, &cc.dwSize);

						// Displays window to alter port settings
						if (!CommConfigDialog(comPort, hwnd, &cc))
							break;
					}
					break;
				default: // Process a displayable character
					write_char(hComm, wParam);
			}
			break;

		case WM_DESTROY:  // Terminate program
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}