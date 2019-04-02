/*------------------------------------------------------------------------------------------------------------------
--	SOURCE FILE:		physical.cpp -			This file contains the functions that are implemented in the 1st 
--												layer, the physical layer, of the OSI Model. The function defined
--												here creates the connection with the serial port and initializes it.
--
--	PROGRAM:			asn1.exe
--
--	FUNCTIONS:			void open_port(LPCSTR comPort, HANDLE* hComm)
--						static DWORD WINAPI read_port(LPVOID threadParam)
--						void transmit_port(HANDLE hComm, LPCVOID lpBuffer)
--
--	DATE:				September 24th, 2018
--
--	DESIGNER:			Dasha Strigoun, A00983479
--
--	PROGRAMMER:			Dasha Strigoun, A00983479
--
----------------------------------------------------------------------------------------------------------------------*/

#define STRICT

#include <Windows.h>
#include <stdio.h>
#include "physical.h"
#include "application.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:			open_port()
--
-- DESCRIPTION:			Opens and initializes the serial port by using CreateFile() to set the HANDLE as a serial 
--						port.
--
-- PARAMETERS:			comPort -				type LPCSTR
--						hComm -					type HANDLE*
--
-- DATE:				September 24th, 2018
--
-- DESIGNER:			Dasha Strigoun, A00983479
--
-- PROGRAMMER:			Dasha Strigoun, A00983479
--
----------------------------------------------------------------------------------------------------------------------*/
void open_port(LPCSTR comPort, HANDLE* hComm, HWND hwnd, char buffer[]) {
	// Check for COM serial port
	if ((*hComm = CreateFile(comPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, NULL, NULL)) == INVALID_HANDLE_VALUE) 
	{
		MessageBox(NULL, "Error opening COM port", "", MB_OK);
	}

	// Initialize serial port
	DWORD bytes_buffer = 16;
	if (SetupComm(*hComm, bytes_buffer, bytes_buffer)) {
		create_read_thread(*hComm, hwnd, buffer);
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:			read_port()
--
-- DESCRIPTION:			Executes an infinite loop to read characters from the serial port and calls function to
--						display the character on the screen.
--
-- PARAMETERS:			threadParam -			type THREADPARAMETERS struct (defined in application.h)
--
-- DATE:				September 24th, 2018
--
-- DESIGNER:			Dasha Strigoun, A00983479
--
-- PROGRAMMER:			Dasha Strigoun, A00983479
--
----------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI read_port(LPVOID threadParam) {
	// Cast LPVOID struct to THREADPARAMETERS
	THREADPARAMETERS newParam = (THREADPARAMETERS)threadParam;

	HANDLE hComm = (HANDLE)newParam->hComm;
	HWND hwnd = (HWND)newParam->hwnd;
	char* buffer = (char*)newParam->buffer;
	DWORD bytesToRead = 0;

	while (true) {
		// Set up timeout for read function
			// without this I got an error where writing to other terminal
			// would only happen after reading a character
		COMMTIMEOUTS cto;
		GetCommTimeouts(hComm, &cto);
		cto.ReadIntervalTimeout = MAXDWORD;
		cto.ReadTotalTimeoutConstant = 0;
		cto.ReadTotalTimeoutMultiplier = 0;

		SetCommTimeouts(hComm, &cto);

		ReadFile(hComm, buffer, sizeof(buffer), &bytesToRead, NULL);

		// Recieved a character to display
		if (bytesToRead == 1) {
			display_char(hwnd, buffer);
		}
	}

	return 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:			transmit_port()
--
-- DESCRIPTION:			Sends the character to the serial port to display on other screen.
--
-- PARAMETERS:			hComm -					type HANDLE
--						lpBuffer -				type LPCVOID
--
-- DATE:				September 24th, 2018
--
-- DESIGNER:			Dasha Strigoun, A00983479
--
-- PROGRAMMER:			Dasha Strigoun, A00983479
--
----------------------------------------------------------------------------------------------------------------------*/
void transmit_port(HANDLE hComm, LPCVOID lpBuffer) {
	DWORD written = 0;

	WriteFile(hComm, lpBuffer, 1, &written, NULL);
}