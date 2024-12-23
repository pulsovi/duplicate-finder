#include "MP3List.h"

static int tempMD5 = 0;
static int tempUser = 0;
static fileCache* cache = NULL;
static size_t lengthCache = 0;

int cmpFolderData(const void* li1, const void* li2)
{
	fileData *a = (fileData*)li1, *b = (fileData*)li2;
	for(int i = 0; i < 16; i++)
		if(a->MD5.string[i] != b->MD5.string[i])
			return a->MD5.string[i] - b->MD5.string[i];
	return 0;
}

int cmpFileCache(const void* fil1, const void* fil2)
{
	wchar_t *a = ((fileCache*)fil1)->name.string, *b = ((fileCache*)fil2)->name.string;
	for(int i = 0; i < 260; i++)
		if(a[i] != b[i] || a[i] == L'\0')
			return a[i] - b[i];
	return 0;
}

int getList(folder** list)
{
	fileName adresse;
	int retVal;
	while(true)
	{
		printf(" ");
		tempUser -= clock();
		retVal = getLine(adresse.string, _MAX_PATH);
		tempUser += clock();

		if(!retVal)
		{
			printf("  Entr\x82\e invalide.\n");
			continue;
		}

		if(adresse.string[0] == '?')
			break;

		switch (_waccess_s(adresse.string, 00))
		{
		case 0:
			//tout va bien.
			break;
		case 2:
			printf("  Dossier ou chemin d'acc\x8As introuvable.\n");
			continue;
		case 13:
			printf("  Acc\x8As au dossier refus\x82.\n");
			continue;
		case 22:
			printf("  Entr\x82\e non valide.\n");
			continue;
		default:
			printf("  Erreur inconnue.\n ");
			continue;
		}
		folder* added = (folder*)malloc(sizeof(folder));
		added->chemin = adresse;
		added->next = *list;
		*list = added;
	}

	if(*list == NULL)
	{
		printf("\n Aucun dossier entr\x82.\n\n");
		return 1;
	}
	return 0;
}

void enumFiles(int* nbFiles)
{
	struct _wfinddata_t D;
	int hd = _wfindfirst(L"*.MP3",&D);
	if(hd == -1)
		return;
	int done = 0;
	while (!done)
	{
		(*nbFiles)++;
		printf("\r %d fichiers trouv\x82s.", *nbFiles);
		done = _wfindnext(hd, &D);
	}
	_findclose(hd);
}

void enumSubFolders(int* nbFiles)
{
	struct _wfinddata_t D;
	int hd = _wfindfirst(L"*",&D);
	if (hd == -1) 
		return;
	int done=0;
	while (!done) 
	{
		if (D.attrib & 0x10 && D.name[0] != L'.')
		{
			if(!_wchdir(D.name))
			{
				enumSubFolders(nbFiles);
				_wchdir(L"..");
			}
		}
		done = _wfindnext(hd,&D);
	}
	_findclose(hd);
	enumFiles(nbFiles);
}

int enumFolders(folder* list)
{
	printf("\n Parcours des dossiers:\n 0 fichiers trouv\x82s.");
	int nbFiles = 0;
	folder* curFolder = list;
	while (curFolder != NULL)
	{
		if(!_wchdir(curFolder->chemin.string))
			enumSubFolders(&nbFiles);
		curFolder = curFolder->next;
	}
	printf("\r %d fichiers trouv\x82s.\n\n", nbFiles);
	if(!nbFiles)
		printf(" Aucun fichier trouv\x82.\n\n");
	return nbFiles;
}

void readCache(wchar_t* ApFolder)
{
	wchar_t *curFolder = _wgetcwd(NULL, 0);
	_wchdir(ApFolder);

	printf(" Lecture du cache.");
	FILE* dat = NULL;
	if(errno_t retVal = fopen_s(&dat, "cache.dat", "rb"))
	{
		printf("\b : Erreur %d .\n\n", retVal);
		return;
	}

	long long length;
	_fseeki64(dat, 0, SEEK_END);
	fgetpos(dat, &length);
	lengthCache = (size_t)(length / sizeof(fileCache));

	if(lengthCache == 0)
	{
		printf("\n  Le cache est vide.\n\n");
		fclose(dat);
		return;
	}

	cache = (fileCache*)malloc(sizeof(fileCache) * lengthCache);
	if(cache == NULL)
	{
		printf("\b : Erreur.\n\n");
		fclose(dat);
		lengthCache = 0;
		return;
	}

	_fseeki64(dat, 0, SEEK_SET);
	fread_s(cache, sizeof(fileCache) * lengthCache, sizeof(fileCache), lengthCache, dat);
	printf("\b: 100%%.\n\n");

	_wchdir(curFolder);
	free(curFolder);
	fclose(dat);
}

fileCache* searchInCache(fileCache* wanted)
{
	if(cache == NULL || lengthCache == 0)
		return NULL;
	return (fileCache*)bsearch(wanted, cache, lengthCache, sizeof(fileCache), cmpFileCache);
}

int getMD5(fileCache* wanted, hash_MD5* buf)
{
	fileCache* found = searchInCache(wanted);
	int retVal = 0;
	if(found == NULL)
	{
		tempMD5 -= clock();
		retVal = MP3_MD5_sum(wanted->name.string, buf);
		tempMD5 += clock();
	} else
		*buf = found->MD5;
	return retVal;
}

int pause()//ok
{
	int lu = _getch();
	if(lu != 'p' && lu != 'P')
		return 0;
	tempUser -= clock();
	printf("\n\n Pause. Appuyer sur :\n R: pour reprendre.\n S: pour terminer.\n ");
	while(true)
	{
		lu = _getch();
		if(lu == 'r' || lu == 'R')
		{
			printf("Reprise.\n\n");
			tempUser += clock();
			return 0;
		}
		if(lu == 's' || lu == 'S')
		{
			printf("Parcours interrompu.");
			tempUser += clock();
			return 1;
		}
	}
}

int checkFiles(fileData* dataArray, int nbFiles, int* progress)
{
	//preparation des variables du dossier

	//adresse du dossier
	fileName pathName;
	_wgetcwd(pathName.string, _MAX_PATH);
	//filecache du fichier
	fileCache curFile;
	curFile.name = pathName;
	//adresse du fichier
	wchar_t *curAd = curFile.name.string;
	//caracteres consommés par l'adresse du dossier
	int lengthPath = wcsnlen_s(curAd, _MAX_PATH) - 1;
	if(curAd[lengthPath] != L'\\')
		curAd[++lengthPath] = L'\\';
	curAd += ++lengthPath;

	struct _wfinddata_t D;
	int hd = _wfindfirst(L"*.MP3",&D);
	if (hd == -1)
		return 0;
	int done=0;
	//parcours
	while (!done) 
	{
		//si depassement du tableau
		if(*progress >= nbFiles + 100)
		{
			_findclose(hd);
			return 1;
		}
		//si pause ou arret demande
		if(_kbhit() && pause())
		{
			_findclose(hd);
			return 2;
		}
		//traitement
		if(errno_t retVal = wcscpy_s(curAd, (_MAX_PATH) - lengthPath, D.name))
			wprintf_s(L"\n Erreur %d sur le nom du fichier \"%s\\%s\"\n", retVal, pathName.string, D.name);
		else if(!getMD5(&curFile, &dataArray[*progress].MD5))
		{
			dataArray[*progress].name = *(fileName*)D.name;
			dataArray[*progress].folder = pathName;
			dataArray[*progress].size = D.size;
			dataArray[*progress].time_create = D.time_create;
			dataArray[*progress].time_write = D.time_write;
			(*progress)++;
			printf("\r %d/%d fichiers trait\x82s: %d%%.", *progress, nbFiles, (100 * (*progress)) / nbFiles);
		}
		done = _wfindnext(hd,&D);
	}
	_findclose(hd);
	return 0;
}

int checkSubFolders(fileData* dataArray, int nbFiles, int* progress)
{
	struct _wfinddata_t D;
	int hd = _wfindfirst(L"*",&D);
	if (hd == -1)
		return 0;
	int done=0;
	while (!done) 
	{
		if (D.attrib & 0x10 && D.name[0] != L'.')
		{
			if(!_wchdir(D.name))
			{
				if(checkSubFolders(dataArray, nbFiles, progress))
				{
					_findclose(hd);
					return 1;
				}
				_wchdir(L"..");
			}
		}
		done = _wfindnext(hd,&D);
	}
	_findclose(hd);
	return checkFiles(dataArray, nbFiles, progress);
}

int checkFolders(folder* list, fileData* dataArray, int nbFiles)
{
	int progress = 0;
	folder* curFolder = list;
	while (curFolder != NULL)
	{
		if(!_wchdir(curFolder->chemin.string))
			if(checkSubFolders(dataArray, nbFiles, &progress))
				break;
		curFolder = curFolder->next;
	}
	printf("\n\n");
	return progress;
}

void freeListe(folder* list)
{
	folder* temp;
	while (list != NULL)
	{
		temp = list;
		list = list->next;
		//printf("free %p\n", temp);
		free(temp);
	}
}

FILE* openCSV(wchar_t* defaultFolder)
{
	_wchdir(defaultFolder);

	wchar_t adress[_MAX_PATH];
	FILE* csv = NULL;
	while (csv == NULL)
	{
		printf(" Entrez un nom pour le fichier de sortie:\n\n ");
		getLine(adress, _MAX_PATH);
		int lengthName = -1;
		while (adress[++lengthName] != '\0' && adress[lengthName] != '.');
		if(adress[lengthName] == '\0')
			wcscat_s(adress, _MAX_PATH, L".csv");
		if(errno_t retVal = _wfopen_s(&csv, adress, L"w,ccs=UTF-8"))
		{
			printf(" Erreur \x85 l'ouverture du fichier : ");
			switch (retVal)
			{
			case 13:
				printf("Acc\x8As refus\x82.\n");
				break;
			default:
				printf("Erreur n\xF8%d.\n", retVal);
				break;
			}
		}
	}
	printf("\n");
	fwprintf(csv, L"\"Checked\",\"Nom de fichier\",\"Dossier\",\"Taille\",\"Créé\",\"Modifié\",\"Type de fichier\",\"MD5\",\"Groupe\",\"Liens physiques\",\"Artiste\",\"Titre\",\"Album\",\"Longueur\",\"Débit binaire\",\"Fréquence d'échantillonnage\",\"Largeur d'image\",\"Hauteur d'image\"\n");

	return csv;
}

void writeCSV(fileData* dataArray, int nbFiles, wchar_t* apFolder)
{
	printf("\a G\x82n\x82ration du fichier de sortie.\n");
	qsort(dataArray, nbFiles, sizeof(fileData), cmpFolderData);
	FILE* csv = openCSV(apFolder);

	hash_MD5 cur = dataArray[0].MD5;
	int groupe = 1;
	struct tm instant;

	for(int i = 0; i < nbFiles; i++)
	{
		printf("\r \x90\criture du fichier: %d%%", i * 100 / nbFiles);
		for(int j = 0; j < 16; j++)
		{	
			if(cur.string[j] != dataArray[i].MD5.string[j])
			{
				groupe++;
				cur = dataArray[i].MD5;
				break;
			}
		}
		fwprintf(csv, L"0,\"%s\",\"%s\\\",", dataArray[i].name.string, dataArray[i].folder.string);
		localtime_s(&instant, &dataArray[i].time_create);
		fwprintf(csv, L"%d,\"%02d/%02d/%04d %02d:%02d:%02d\",", dataArray[i].size, instant.tm_mday, instant.tm_mon + 1, instant.tm_year + 1900, instant.tm_hour, instant.tm_min, instant.tm_sec);
		localtime_s(&instant, &dataArray[i].time_write);
		fwprintf(csv, L"\"%02d/%02d/%04d %02d:%02d:%02d\",\"VLC media file (.mp3)\",\"", instant.tm_mday, instant.tm_mon + 1, instant.tm_year + 1900, instant.tm_hour, instant.tm_min, instant.tm_sec);
		for(int j = 0; j < 16; j++)
			fwprintf(csv,L"%02X",dataArray[i].MD5.string[j]);
		fwprintf(csv, L"\",\"%d\",\"0\",\"\",\"\",\"\",0,\"\",\"\",\"\",\"\"\n",groupe);
	}
	printf("\r \x90\criture du fichier: 100%%\n\n");
	fclose(csv);
}

void fileDataToFileCache(fileData* from, fileCache* to)
{
	int end = swprintf(to->name.string, L"%s\\", from->folder.string);
	swprintf(&to->name.string[end], L"%s", from->name.string);
	to->MD5 = from->MD5;
}

void writeCache(fileData* dataArray, int nbFiles, wchar_t* defaultFolder)
{
	_wchdir(defaultFolder);

	printf(" \x90\criture du cache.");

	FILE* dat = NULL;
	if(errno_t retVal = fopen_s(&dat, "cache.dat", "wb"))
	{
		printf("\n  Erreur %d \x85 l'ouverture du fichier.\n\n", retVal);
		return;
	}

	fileCache* data = (fileCache*)calloc(nbFiles , sizeof(fileCache));
	if(data == NULL)
	{
		printf(" Echec 2.\n\n");
		return;
	}

	for(int i = 0; i < nbFiles; i++)
		fileDataToFileCache(&dataArray[i], &data[i]);
	qsort(data, nbFiles, sizeof(fileCache), cmpFileCache);

	int itemData = 0;
	size_t itemCache = 0;
	while (itemData < nbFiles && itemCache < lengthCache)
	{
		int comp = cmpFileCache(&data[itemData], &cache[itemCache]);
		if(comp <= 0)
		{
			fwrite(&data[itemData++], sizeof(fileCache), 1, dat);
			if(comp == 0)
				itemCache++;
		}
		else
			fwrite(&cache[itemCache++], sizeof(fileCache), 1, dat);

		printf("\r \x90\criture du cache: %3d%%.", (itemCache + itemData) * 100 / (nbFiles + lengthCache));
	}

	if(itemCache < lengthCache)
		fwrite(&cache[itemCache], sizeof(fileCache), lengthCache - itemCache, dat);
	else
		fwrite(&data[itemData], sizeof(fileCache), nbFiles - itemData, dat);

	printf("\r \x90\criture du cache: 100%%.\n\n");
	free(data);
	fclose(dat);
}

void dispClock()
{
	printf(" Temps hash        : %6.02lf%%, %ds.\n", (double)(tempMD5 * 100) / clock(), tempMD5 / CLOCKS_PER_SEC);
	printf(" Temps utilisateur : %6.02lf%%, %ds.\n", (double)(tempUser * 100) / clock(), tempUser / CLOCKS_PER_SEC);
	printf(" Autre temps       : %6.02lf%%, %ds.\n\n", (double)((clock() - tempMD5 - tempUser) * 100) / clock(), (clock() - tempMD5 - tempUser) / CLOCKS_PER_SEC);
}

int MP3_dupfind()
{
	wchar_t* curFolder = _wgetcwd(NULL, 260);
	logFolder(curFolder, DEFINE_ADRESS);
	folder* list = NULL;
	if(getList(&list))
		return 1;
	int nbFiles = enumFolders(list);
	if(!nbFiles)
		return 2;
	fileData* dataArray = (fileData*)calloc(nbFiles + 100, sizeof(fileData));
	readCache(curFolder);
	nbFiles = checkFolders(list, dataArray, nbFiles);
	dispClock();
	writeCSV(dataArray, nbFiles, curFolder);
	writeCache(dataArray, nbFiles, curFolder);

	freeListe(list);
	closeLog();
	free(curFolder);
	if(cache)
		free(cache);
	free(dataArray);
	return 0;
}