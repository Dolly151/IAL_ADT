/* c201.c **********************************************************************
** Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**                                              Úpravy: Kamil Jeřábek, září 2020
**                                                    Daniel Dolejška, září 2021
**                                                    Daniel Dolejška, září 2022
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu List.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ List:
**
**      List_Dispose ....... zrušení všech prvků seznamu,					*
**      List_Init .......... inicializace seznamu před prvním použitím,		*
**      List_InsertFirst ... vložení prvku na začátek seznamu,				*
**      List_First ......... nastavení aktivity na první prvek,				*
**      List_GetFirst ...... vrací hodnotu prvního prvku,					*
**      List_DeleteFirst ... zruší první prvek seznamu,						*
**      List_DeleteAfter ... ruší prvek za aktivním prvkem,					*
**      List_InsertAfter ... vloží nový prvek za aktivní prvek seznamu,		*
**      List_GetValue ...... vrací hodnotu aktivního prvku,					*
**      List_SetValue ...... přepíše obsah aktivního prvku novou hodnotou,	*
**      List_Next .......... posune aktivitu na další prvek seznamu,		*
**      List_IsActive ...... zjišťuje aktivitu seznamu.						*
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam předá
** někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**/

#include "c201.h"

#include <stdio.h>	// printf
#include <stdlib.h> // malloc, free

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě. Nastaví error_flag na logickou 1.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void List_Error(void)
{
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným
 * seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
 * že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu jednosměrně vázaného seznamu
 */
void List_Init(List *list)
{

	list->activeElement = NULL;
	list->firstElement = NULL;
	list->currentLength = 0;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam list do stavu, v jakém se nacházel
 * po inicializaci. Veškerá paměť používaná prvky seznamu list bude korektně
 * uvolněna voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 **/
void List_Dispose(List *list)
{
	ListElementPtr current = list->firstElement;
	ListElementPtr next;
	while (current != NULL)
	{
		next = current->nextElement;
		free(current);
		current = next;
	}

	list->activeElement = NULL;
	list->firstElement = NULL;
	list->currentLength = 0;
}

/**
 * Vloží prvek s hodnotou data na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci List_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void List_InsertFirst(List *list, int data)
{

	ListElementPtr newElement = (ListElementPtr)malloc(sizeof(struct ListElement)); // vytvoreni noveho prvku a alokace pameti
	if (newElement == NULL)
	{ // pri nezdareni alokace zavolana funkce List_Error dle zadani
		List_Error();
		return;
	}
	newElement->data = data;					  // vlozeni dat do prvku
	newElement->nextElement = list->firstElement; // novy prvek ukazuje na dosavadni prvni prvek
	list->firstElement = newElement;			  // novy prvek se stava prvnim prvkem
	list->currentLength++;
}

/**
 * Nastaví aktivitu seznamu list na jeho první prvek.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 */
void List_First(List *list)
{

	list->activeElement = list->firstElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci List_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void List_GetFirst(List *list, int *dataPtr)
{
	if (list->firstElement != NULL) // kontrola, zda seznam neni prazdny
	{
		*dataPtr = list->firstElement->data; // pokud seznam neni prazdny, do dataPtr ulozime hodnotu prvniho prvku
	}
	else
	{
		List_Error();
	}
}

/**
 * Zruší první prvek seznamu list a uvolní jím používanou paměť.
 * Pokud byl rušený prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 */
void List_DeleteFirst(List *list)
{
	ListElementPtr elementToDelete;
	if (list->firstElement != NULL) // list neni prazdny
	{
		elementToDelete = list->firstElement;				  // nastaveni ukazatele prvku, ktery chceme smazat, na prvni prvek
		list->firstElement = list->firstElement->nextElement; // propojeni ukazatele aktivniho prvku s prvkem nasledujicim po prvku mazanem
		if (list->activeElement == elementToDelete)			  // kontrola jestli byl ruseny prvek prvkem aktivnim
		{
			list->activeElement = NULL;
		}
		free(elementToDelete);
		list->currentLength--;
	}
}

/**
 * Zruší prvek seznamu list za aktivním prvkem a uvolní jím používanou paměť.
 * Pokud není seznam list aktivní nebo pokud je aktivní poslední prvek seznamu list,
 * nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 */
void List_DeleteAfter(List *list)
{
	ListElementPtr elementToDelete;
	if (list->activeElement != NULL)
	{ // seznam je aktivni
		if (list->activeElement->nextElement != NULL)
		{ // aktivni prvek neni posledni

			elementToDelete = list->activeElement->nextElement;				 // ulozeni ukazatele na prvek, ktery chceme smazat
			list->activeElement->nextElement = elementToDelete->nextElement; // propojeni ukazatele aktivniho prvku s prvkem nasledujicim po prvku mazanem
			free(elementToDelete);
			list->currentLength--;
		}
	}
}

/**
 * Vloží prvek s hodnotou data za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje!
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * zavolá funkci List_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void List_InsertAfter(List *list, int data)
{
	if (list->activeElement != NULL)
	{																					// overeni, ze jde o aktivni seznam
		ListElementPtr newElement = (ListElementPtr)malloc(sizeof(struct ListElement)); // alokace pameti

		if (newElement == NULL)
		{ // overeni uspesne alokace, pri neuspechu volame List_Error
			List_Error();
			return;
		}

		newElement->data = data;									// vkladame data do prvku
		newElement->nextElement = list->activeElement->nextElement; // novy prvek ukazuje tam, kam aktivni.
		list->activeElement->nextElement = newElement;				// aktivni prvek ted ukazuje na novy
		list->currentLength++;
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam není aktivní, zavolá funkci List_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void List_GetValue(List *list, int *dataPtr)
{
	if (List_IsActive(list))
	{
		*dataPtr = list->activeElement->data; // pokud seznam aktivni, do dataPtr ulozime hodnotu aktivniho prvku
	}
	else
	{
		List_Error();
		return;
	}
}

/**
 * Přepíše data aktivního prvku seznamu list hodnotou data.
 * Pokud seznam list není aktivní, nedělá nic!
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void List_SetValue(List *list, int data)
{
	if (list->activeElement != NULL)
	{
		list->activeElement->data = data; // pokud seznam aktivni, prepise se hodnota aktivniho prvku
	}
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
 * Pokud není předaný seznam list aktivní, nedělá funkce nic.
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 */
void List_Next(List *list)
{
	if (list->activeElement != NULL)
	{
		list->activeElement = list->activeElement->nextElement; // pokud seznam aktivni, aktivita se posune na nasledujici prvek
	}
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Tuto funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu jednosměrně vázaného seznamu
 */
int List_IsActive(List *list)
{

	return list->activeElement != NULL;
}

/* Konec c201.c */
