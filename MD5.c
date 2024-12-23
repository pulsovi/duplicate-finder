#include "MD5.h"

unsigned int leftRotate(unsigned int x, int y)
{
	return ((x << y) | (x >> (32 - y)));
}

void MD5_round(unsigned int* ABCD, unsigned int *bloc)
{
	int A = ABCD[0];
	int B = ABCD[1];
	int C = ABCD[2];
	int D = ABCD[3];

	// 1
	A = B + leftRotate(A + (D ^ (B & (C ^ D))) + bloc[0] + 0xd76aa478, 7);
	D = A + leftRotate(D + (C ^ (A & (B ^ C))) + bloc[1] + 0xe8c7b756, 12); 
	C = D + leftRotate(C + (B ^ (D & (A ^ B))) + bloc[2] + 0x242070db, 17);
	B = C + leftRotate(B + (A ^ (C & (D ^ A))) + bloc[3] + 0xc1bdceee, 22);
	A = B + leftRotate(A + (D ^ (B & (C ^ D))) + bloc[4] + 0xf57c0faf, 7);
	D = A + leftRotate(D + (C ^ (A & (B ^ C))) + bloc[5] + 0x4787c62a, 12);
	C = D + leftRotate(C + (B ^ (D & (A ^ B))) + bloc[6] + 0xa8304613, 17);
	B = C + leftRotate(B + (A ^ (C & (D ^ A))) + bloc[7] + 0xfd469501, 22);
	A = B + leftRotate(A + (D ^ (B & (C ^ D))) + bloc[8] + 0x698098d8, 7);
	D = A + leftRotate(D + (C ^ (A & (B ^ C))) + bloc[9] + 0x8b44f7af, 12);
	C = D + leftRotate(C + (B ^ (D & (A ^ B))) + bloc[10] + 0xffff5bb1, 17);
	B = C + leftRotate(B + (A ^ (C & (D ^ A))) + bloc[11] + 0x895cd7be, 22);
	A = B + leftRotate(A + (D ^ (B & (C ^ D))) + bloc[12] + 0x6b901122, 7);
	D = A + leftRotate(D + (C ^ (A & (B ^ C))) + bloc[13] + 0xfd987193, 12);
	C = D + leftRotate(C + (B ^ (D & (A ^ B))) + bloc[14] + 0xa679438e, 17);
	B = C + leftRotate(B + (A ^ (C & (D ^ A))) + bloc[15] + 0x49b40821, 22);
	// 2
	A = B + leftRotate(A + (C ^ (D & (B ^ C))) + bloc[1] + 0xf61e2562, 5);
	D = A + leftRotate(D + (B ^ (C & (A ^ B))) + bloc[6] + 0xc040b340, 9);
	C = D + leftRotate(C + (A ^ (B & (D ^ A))) + bloc[11] + 0x265e5a51, 14);
	B = C + leftRotate(B + (D ^ (A & (C ^ D))) + bloc[0] + 0xe9b6c7aa, 20);
	A = B + leftRotate(A + (C ^ (D & (B ^ C))) + bloc[5] + 0xd62f105d, 5);
	D = A + leftRotate(D + (B ^ (C & (A ^ B))) + bloc[10] + 0x02441453, 9);
	C = D + leftRotate(C + (A ^ (B & (D ^ A))) + bloc[15] + 0xd8a1e681, 14);
	B = C + leftRotate(B + (D ^ (A & (C ^ D))) + bloc[4] + 0xe7d3fbc8, 20);
	A = B + leftRotate(A + (C ^ (D & (B ^ C))) + bloc[9] + 0x21e1cde6, 5);
	D = A + leftRotate(D + (B ^ (C & (A ^ B))) + bloc[14] + 0xc33707d6, 9);
	C = D + leftRotate(C + (A ^ (B & (D ^ A))) + bloc[3] + 0xf4d50d87, 14);
	B = C + leftRotate(B + (D ^ (A & (C ^ D))) + bloc[8] + 0x455a14ed, 20);
	A = B + leftRotate(A + (C ^ (D & (B ^ C))) + bloc[13] + 0xa9e3e905, 5);
	D = A + leftRotate(D + (B ^ (C & (A ^ B))) + bloc[2] + 0xfcefa3f8, 9); 
	C = D + leftRotate(C + (A ^ (B & (D ^ A))) + bloc[7] + 0x676f02d9, 14);
	B = C + leftRotate(B + (D ^ (A & (C ^ D))) + bloc[12] + 0x8d2a4c8a, 20);
	// 3
	A = B + leftRotate(A + (B ^ C ^ D) + bloc[5] + 0xfffa3942, 4); 
	D = A + leftRotate(D + (A ^ B ^ C) + bloc[8] + 0x8771f681, 11); 
	C = D + leftRotate(C + (D ^ A ^ B) + bloc[11] + 0x6d9d6122, 16);
	B = C + leftRotate(B + (C ^ D ^ A) + bloc[14] + 0xfde5380c, 23);
	A = B + leftRotate(A + (B ^ C ^ D) + bloc[1] + 0xa4beea44, 4);
	D = A + leftRotate(D + (A ^ B ^ C) + bloc[4] + 0x4bdecfa9, 11);
	C = D + leftRotate(C + (D ^ A ^ B) + bloc[7] + 0xf6bb4b60, 16); 
	B = C + leftRotate(B + (C ^ D ^ A) + bloc[10] + 0xbebfbc70, 23);
	A = B + leftRotate(A + (B ^ C ^ D) + bloc[13] + 0x289b7ec6, 4); 
	D = A + leftRotate(D + (A ^ B ^ C) + bloc[0] + 0xeaa127fa, 11); 
	C = D + leftRotate(C + (D ^ A ^ B) + bloc[3] + 0xd4ef3085, 16); 
	B = C + leftRotate(B + (C ^ D ^ A) + bloc[6] + 0x04881d05, 23);
	A = B + leftRotate(A + (B ^ C ^ D) + bloc[9] + 0xd9d4d039, 4);
	D = A + leftRotate(D + (A ^ B ^ C) + bloc[12] + 0xe6db99e5, 11);
	C = D + leftRotate(C + (D ^ A ^ B) + bloc[15] + 0x1fa27cf8, 16);
	B = C + leftRotate(B + (C ^ D ^ A) + bloc[2] + 0xc4ac5665, 23);
	// 4
	A = B + leftRotate(A + (C ^ (B | (~D))) + bloc[0] + 0xf4292244, 6);
	D = A + leftRotate(D + (B ^ (A | (~C))) + bloc[7] + 0x432aff97, 10);
	C = D + leftRotate(C + (A ^ (D | (~B))) + bloc[14] + 0xab9423a7, 15);  
	B = C + leftRotate(B + (D ^ (C | (~A))) + bloc[5] + 0xfc93a039, 21);
	A = B + leftRotate(A + (C ^ (B | (~D))) + bloc[12] + 0x655b59c3, 6);  
	D = A + leftRotate(D + (B ^ (A | (~C))) + bloc[3] + 0x8f0ccc92, 10);  
	C = D + leftRotate(C + (A ^ (D | (~B))) + bloc[10] + 0xffeff47d, 15);  
	B = C + leftRotate(B + (D ^ (C | (~A))) + bloc[1] + 0x85845dd1, 21);
	A = B + leftRotate(A + (C ^ (B | (~D))) + bloc[8] + 0x6fa87e4f, 6);  
	D = A + leftRotate(D + (B ^ (A | (~C))) + bloc[15] + 0xfe2ce6e0, 10);
	C = D + leftRotate(C + (A ^ (D | (~B))) + bloc[6] + 0xa3014314, 15); 
	B = C + leftRotate(B + (D ^ (C | (~A))) + bloc[13] + 0x4e0811a1, 21);
	A = B + leftRotate(A + (C ^ (B | (~D))) + bloc[4] + 0xf7537e82, 6); 
	D = A + leftRotate(D + (B ^ (A | (~C))) + bloc[11] + 0xbd3af235, 10);
	C = D + leftRotate(C + (A ^ (D | (~B))) + bloc[2] + 0x2ad7d2bb, 15); 
	B = C + leftRotate(B + (D ^ (C | (~A))) + bloc[9] + 0xeb86d391, 21);

	ABCD[0] += A;
	ABCD[1] += B;
	ABCD[2] += C;
	ABCD[3] += D;
}

void MD5_parcour(FILE* message, fpos_t fileLength, hash_MD5* sum)
{
	static const hash_MD5 ini = {{0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10}};
	unsigned int* ABCD = (unsigned int*)sum;
	unsigned int bloc[16];

	*sum = ini;
	for(fpos_t i = fileLength / 64; i > 0; i--)
	{
		fread(bloc, 4, 16, message);
		MD5_round(ABCD, bloc);
	}

	{
		char *cbloc = (char*)bloc;
		int fin = fileLength % 64;
		fread(cbloc, 1, fin, message);
		cbloc[fin] = 0x80;
		fin++;

		if(fin > 56)
		{
			for(int i = fin; i < 64; i++)
				cbloc[i] = 0;
			MD5_round(ABCD, bloc);
			fin = 0;
		}

		while(fin < 56)
			cbloc[fin++] = 0;
		*(long long*)&bloc[14] = fileLength * 8;
		MD5_round(ABCD, bloc);
	}
}

int MD5_sum(const char* adresse, hash_MD5* sum)
{
	FILE* message = fopen(adresse, "rb");
	if(message == NULL)
	{
		fprintf(stderr, "l'ouverture du fichier a echoue\n\n");
		return 1;
	}

	fseek(message,0,SEEK_END);
	fpos_t fileLength;
	fgetpos(message, &fileLength);
	fseek(message,0,SEEK_SET);

	MD5_parcour(message, fileLength, sum);
	fclose(message);
	return 0;
}

/*int isMP3(const char* adresse)
{
	int i = 0;
	while(adresse[i++] != '\0');
	i -= 5;
	if(i > 0 && adresse[i] == '.' && adresse[i + 3] == '3'
	&& (adresse[i + 1] == 'M' || adresse[i + 1] == 'm')
	&& (adresse[i + 2] == 'P' || adresse[i + 2] == 'p'))
	{
		printf("%s\n",adresse);
		return 0;
	}
	return 1;
}//*/

int ID3_v1(FILE* fichier)
{
	char lu[3];
	fseek(fichier, -128, SEEK_END);
	fread(lu, 1, 3, fichier);
	if(lu[0] == 'T' && lu[1] == 'A' && lu[2] == 'G')
		return 128;
	return 0;
}

int ID3_v2(FILE* fichier)
{
	char D[6];
	fseek(fichier, 0, SEEK_SET);
	fread(D, 1, 6, fichier);
	if(D[0] != 'I' || D[1] != 'D' || D[2] != '3')
		return 0;
	fread(D, 1, 4, fichier);
	return D[3] + D[2] * 0x80 + D[1] * 0x4000 + D[0] * 0x200000 + 10;
}

int APE_Tag(FILE* fichier)
{
	int fin = ID3_v1(fichier);
	char lu[12];
	fseek(fichier, -fin - 32, SEEK_END);
	fread(lu, 1, 12, fichier);
	if(lu[0] != 'A' || lu[1] != 'P' || lu[2] != 'E' || lu[3] != 'T'
		|| lu[4] != 'A' || lu[5] != 'G' || lu[6] != 'E' || lu[7] != 'X')
		return fin;
	int longueur;
	fread(&longueur, 4, 1, fichier);
	return longueur + fin + 32;
}

int MP3_MD5_sum(const wchar_t* adresse, hash_MD5* sum)
{
	FILE* message = _wfopen(adresse, L"rb");
	if(message == NULL)
	{
		wprintf(L"\rl'ouverture du fichier \"%s\" a echou%c\n\n", adresse, 130);
		return 1;
	}

	fpos_t fileLength;
	{
		int debut = ID3_v2(message);
		int fin = APE_Tag(message);
		fseek(message, 0, SEEK_END);
		fgetpos(message, &fileLength);
		fileLength -= (debut + fin);
		fseek(message, debut, SEEK_SET);
	}

	MD5_parcour(message, fileLength, sum);
	fclose(message);
	return 0;
}