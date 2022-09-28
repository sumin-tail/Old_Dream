#include "iUtil.h"

#include "iStd.h"

void saveFile(const char* fileName, char* buf, int len)
{
	FILE* pf = fopen(fileName, "wb");

	fwrite(buf, sizeof(char), len, pf);

	fclose(pf);
}

char* loadFile(const char* fileName, int& len)
{
	FILE* pf = fopen(fileName, "rb");

	fseek(pf, 0, SEEK_END);
	len = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	char* str = new char[len + 1];
	fread(str, sizeof(char), len, pf);
	str[len] = 0;

	fclose(pf);

	return str;
}

//float _sin(float degree)
//{
//	return sinf(M_PI / 180 * degree);
//}
//
//float _cos(float degree)
//{
//	return sinf(M_PI / 180 * degree);
//}


