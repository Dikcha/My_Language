#pragma once

extern "C"
{
	char* _stdcall copystr(char*, char*);
	char* _stdcall sumstr(char*, char*);
	void _stdcall outstr(char*);
	void _stdcall outint(int);
	int _stdcall squareroot(int);
	int _stdcall square(int);
}