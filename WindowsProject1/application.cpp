/*------------------------------------------------------------------------------------------------------------------
--	SOURCE FILE:		application.cpp -		This file contains the functions that are implemented in the 7th 
--												layer, the application layer, of the OSI Model. The functions 
--												defined here manage the reading and writing of the serial port. These
--												states are further broken down into reading/writing chararacters and 
--												displaying/transmitting characters.
--
--	PROGRAM:			asn1.exe
--
--	FUNCTIONS:			void create_read_thread(HANDLE hComm, HWND hwnd, char* buffer)
--						void display_char(HWND hwnd, char* buffer)
--						HANDLE get_thread()
--
--						void write_char(HANDLE hComm, WPARAM wParam)
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

#include <Windows.h>
#include <stdio.h>
#include "application.h"
#include "physical.h"

HANDLE hThrd;
static unsigned k = 0;

// READ FUNCTIONS
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:			create_read_thread()
--
-- DESCRIPTION:			Creates a thread to handle reading from the initialized serial port. 
--
-- PARAMETERS:			hComm -					type HANDLE
--						hwnd -					type HWND	
--						buffer -				type char*
--
-- DATE:				September 24th, 2018
-- 
-- DESIGNER:			Dasha Strigoun, A00983479
--
-- PROGRAMMER:			Dasha Strigoun, A00983479
--
----------------------------------------------------------------------------------------------------------------------*/
void create_read_thread(HANDLE hComm, HWND hwnd, char* buffer) {
	// Initialize struct defined in application.h to pass into CreateThread()
	THREADPARAMETERS threadParam = new parameters(hComm, hwnd, buffer);
	DWORD threadId;

	hThrd = CreateThread(NULL, 0, read_port, (LPVOID)threadParam, 0, &threadId);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:			display_char()
--
-- DESCRIPTION:			Displays the char to the screen and increments the x-position for the next character.
--
-- PARAMETERS:			hwnd -					type HWND
--						buffer -				type char*
--
-- DATE:				September 24th, 2018
--
-- DESIGNER:			Dasha Strigoun, A00983479
--
-- PROGRAMMER:			Dasha Strigoun, A00983479
--
----------------------------------------------------------------------------------------------------------------------*/
void display_char(HWND hwnd, char* buffer) {
	HDC hdc;
	char str[80] = "";

	// Write character to screen and increment x-position
	hdc = GetDC(hwnd);
	sprintf_s(str, "%c", (char)buffer[0]);
	TextOut(hdc, 13 * k, 0, str, strlen(str));
	k++;
	ReleaseDC(hwnd, hdc);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:			getThread()
--
-- DESCRIPTION:			Returns the thread handle initialized.
--
-- RETURN:				returns HANDLE
--
-- DATE:				September 24th, 2018
--
-- DESIGNER:			Dasha Strigoun, A00983479
--
-- PROGRAMMER:			Dasha Strigoun, A00983479
--
----------------------------------------------------------------------------------------------------------------------*/
HANDLE get_thread() {
	return &hThrd;
}



// WRITE FUNCTIONS
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:			write_char()
--
-- DESCRIPTION:			Converts the wParam passed in through a THREADPARAMETERS struct to a char*.
--
-- PARAMETERS:			hComm -					type HANDLE
--						wParam -				type WPARAM
--
-- DATE:				September 24th, 2018
--
-- DESIGNER:			Dasha Strigoun, A00983479
--
-- PROGRAMMER:			Dasha Strigoun, A00983479
--
----------------------------------------------------------------------------------------------------------------------*/
void write_char(HANDLE hComm, WPARAM wParam) {
	char str[2] = "";

	// Determine character to write
	sprintf_s(str, "%c", (char)wParam);

	transmit_port(hComm, (LPCVOID)str);
}
