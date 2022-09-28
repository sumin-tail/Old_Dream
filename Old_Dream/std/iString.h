#pragma once

class iString
{
public:
	iString();
	iString(const char* szFormat, ...);
	iString(const iString& s);
	virtual ~iString();

	iString& operator = (const char* s);
	iString& operator = (const iString& s);

	iString& operator += (const char* s);
	iString& operator += (const iString& s);

	const char* operator + (const char* s);
	const char* operator + (const iString& s);

	void copy(const char* s);
	void cat(const char* s);

	static char** getStringLine(const char* str, int& lineNum, int d='\n');
	static void freeStringLine(char** line, int lineNum);

public:
	char* str;
	int strLength;
};

#define defaultLength 32