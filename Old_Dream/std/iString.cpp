#include "iString.h"

#include "iStd.h"

iString::iString()
{
	str = new char[defaultLength];
	strLength = defaultLength;
}

iString::iString(const char* szFormat, ...)
{
	str = new char[defaultLength];
	strLength = defaultLength;

	char szText[512];
	va_start_end(szFormat, szText);
	copy(szText);
	printf("iString\n");
}

iString::iString(const iString& s)
{
	str = new char[defaultLength];
	strLength = defaultLength;

	copy(s.str);
	printf("iString\n");
}

iString::~iString()
{
	printf("~iString\n");
	delete str;
}

iString& iString::operator = (const char* s)
{
	copy(s);
	return *this;
}
iString& iString::operator = (const iString& s)
{
	copy(s.str);
	return *this;
}

iString& iString::operator += (const char* s)
{
	cat(s);
	return *this;
}
iString& iString::operator += (const iString& s)
{
	cat(s.str);
	return *this;
}

static char* string = NULL;
int stringLength = 0;
const char* iString::operator + (const char* s)
{
	int len0 = strlen(str);
	int len1 = strlen(s);
	if (string == NULL)
	{
		string = new char[len0 + len1 + 1];
		stringLength = len0 + len1 + 1;
	}
	else
	{
		if (stringLength < len0 + len1 + 1)
		{
			delete string;
			string = new char[len0 + len1 + 1];
			stringLength = len0 + len1 + 1;
		}
	}
	char* r = string;
	strcpy(r, str);
	strcpy(&r[len0], s);
	return r;
}
const char* iString::operator + (const iString& _s)
{
	const char* s = _s.str;// #issue 코드 정리 요망

	int len0 = strlen(str);
	int len1 = strlen(s);
	if (string == NULL)
	{
		string = new char[len0 + len1 + 1];
		stringLength = len0 + len1 + 1;
	}
	else
	{
		if (stringLength < len0 + len1 + 1)
		{
			delete string;
			string = new char[len0 + len1 + 1];
			stringLength = len0 + len1 + 1;
		}
	}
	char* r = string;
	strcpy(r, str);
	strcpy(&r[len0], s);
	return r;
}

void iString::copy(const char* s)
{
	int len = strlen(s);
	if (strLength < len + 1)
	{
		delete str;
		str = new char[len + 1];
		strLength = len + 1;
	}
	strcpy(str, s);
}

void iString::cat(const char* s)
{
	int len0 = strlen(str);
	int len1 = strlen(s);
	if (strLength < len0 + len1 + 1)
	{
		char* newStr = new char[len0 + len1 + 1];
		strcpy(newStr, str);
		delete str;
		str = newStr;
		strLength = len0 + len1 + 1;
	}
	strcpy(&str[len0], s);
}

char** iString::getStringLine(const char* str, int& lineNum, int d)
{
	lineNum = 1;
	for (int i = 0; str[i]; i++)
	{
		if (str[i] == d)
			lineNum++;
	}
	char** line = new char* [lineNum];
	int i, off = 0, n = 0;
	for (i = 0; str[i]; i++)
	{
		if (str[i] == d)
		{
			int len = i - off;
			char* s = new char[len + 1];
			memcpy(s, &str[off], len);
			s[len] = 0;
			
			line[n] = s; n++;

			off = i + 1;
		}
	}
	
	int len = strlen(str) - off;
	char* s = new char[len + 1];
	memcpy(s, &str[off], len);
	s[len] = 0;
	line[n] = s;

	return line;
}

void iString::freeStringLine(char** line, int lineNum)
{
	for (int i = 0; i < lineNum; i++)
		delete line[i];
	delete line;
}