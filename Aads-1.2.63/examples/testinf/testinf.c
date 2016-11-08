#include	<stdio.h>
#include <tos.h>
#include	"inf.h"

int main(void)
{
	char	chaine[128];
	long	nombre;


	printf("\033Eprogramme de test de fichier INF evolue\n\n");

	printf("\nLecture du chapitre 1\n");
	GetProfileString("test.inf", "Chapitre1", "ValeurChaine1", chaine, "default");
	printf("ValeurChaine1 = <%s>\n", chaine);
	GetProfileString("test.inf", "Chapitre1", "ValeurChaine2", chaine, "default");
	printf("ValeurChaine2 = <%s>\n", chaine);
	Crawcin();


	printf("Creation de chapitre 1 et insertion entree\n");
	SetProfileString("test.inf", "Chapitre1", "ValeurChaine1", "ValeurInitiale");
	display();
	SetProfileString("test.inf", "Chapitre1", "ValeurChaine2", "ValeurInitiale");
	display();
	Crawcin();

	printf("Modification avec valeur chaine vide\n");
	SetProfileString("test.inf", "Chapitre1", "ValeurChaine1", "ValeurNew");
	display();
	SetProfileString("test.inf", "Chapitre1", "ValeurChaine2", "");
	display();
	Crawcin();


	SaveProfile();
	return 0;
}