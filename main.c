#include <stdio.h>
#include <conio.h>
#include "MP3List.h"
#include "menu.h"
#include "MD5_MP3.h"

int main()
{
	before(45);
	printf(" \xBA  Entrer le chemin des dossiers \x85 analyser   \xBA\n");
	middle(45);
	printf(" \xBA      \x85 raison d'un dossier par ligne,       \xBA\n");
	middle(45);
	printf(" \xBA   puis taper le caract\x8Are '?' et ENTREE.    \xBA\n");
	after(45);
	printf("\n\n");

	MP3_dupfind();
	printf("\a");
	
	before(45);
	printf(" \xBA             Programme termin\x82.              \xBA\n");
	middle(45);
	printf(" \xBA     Appuyer sur une touche pour fermer.     \xBA\n");
	after(45);
	printf("\n ");
	_getch();
	return 0;
}