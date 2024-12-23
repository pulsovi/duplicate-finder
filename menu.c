#include "menu.h"

void middle(int width)
{
	printf(" \xBA");
	for(int i = 0; i < width; i++)
		printf(" ");
	printf("\xBA\n");
}

void before(int width)
{
	printf(" \xC9");
	for(int i = 0; i < width; i++)
		printf("\xCD");
	printf("\xBB\n");
	middle(width);
}

void after(int width)
{
	middle(width);
	printf(" \xC8");
	for(int i = 0; i < width; i++)
		printf("\xCD");
	printf("\xBC");
}

void clearEndLineIn()
{
	scanf_s("%*[^\n]");
	getchar();
}

int getLine(wchar_t *buf, int bufSize)
{	
	int ret = wscanf_s(L"%[^\n]", buf, bufSize);
	clearEndLineIn();
	return ret;
}