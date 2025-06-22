/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
** uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void)
{
    printf("*ERROR* The program has performed an illegal operation.\n");
    error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init(DLList *list)
{
    list->activeElement = NULL;
    list->firstElement = NULL;
    list->lastElement = NULL;
    list->currentLength = 0;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose(DLList *list)
{
    DLLElementPtr current = list->firstElement;
    DLLElementPtr next;
    while (current != NULL) //dokud nedojdeme na konec seznamu, pokracuj
    {
        next = current->nextElement;    //do next nahrajeme dalsi prvek
        free(current);
        current = next; //do current ulozime nasledujici prvek
    }
    //seznam vratime do stavu po inicializaci
    list->activeElement = NULL;
    list->firstElement = NULL;
    list->lastElement = NULL;
    list->currentLength = 0;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst(DLList *list, long data)
{
    DLLElementPtr newElement = (DLLElementPtr)malloc(sizeof(struct DLLElement));    //alokujeme misto pro newElement
    if (newElement == NULL) //pokud nebude alokace uspesna vrat DLL_Error()
    {
        DLL_Error();
        return;
    }

    // Nastavení dat a ukazatelů nového prvku
    newElement->data = data;
    newElement->previousElement = NULL;
    newElement->nextElement = list->firstElement;

    if (list->firstElement == NULL)
    {
        // Pokud je seznam prazdny, newElement je jediny prvek seznamu -> tudiz i posledni prvek
        list->lastElement = newElement;
    }
    else
    {
        // puvodnimu prvnimu prvku nastavujeme zpetny ukazatel na vkladany prvek
        list->firstElement->previousElement = newElement;
    }

    // novym prvkem se stava prvek vkladany
    list->firstElement = newElement;
    list->currentLength++;
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast(DLList *list, long data) //princip stejny jako u InsertFirst, jen menime opacne ukazatele
{
    DLLElementPtr newElement = (DLLElementPtr)malloc(sizeof(struct DLLElement));
    if (newElement == NULL)
    {
        DLL_Error();
        return;
    }

    newElement->data = data;
    newElement->previousElement = list->lastElement;
    newElement->nextElement = NULL;

    if (list->lastElement == NULL)
    {
        
        list->firstElement = newElement;
    }
    else
    {
        
        list->lastElement->nextElement = newElement;
    }

    list->lastElement = newElement;
    list->currentLength++;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First(DLList *list)
{
    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last(DLList *list)
{
    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst(DLList *list, long *dataPtr)
{
    if (list->firstElement != NULL) // kontrola, zda seznam není prázdný
    {
        *dataPtr = list->firstElement->data; // pokud seznam není prázdný, do dataPtr uložíme hodnotu prvního prvku
    }
    else
    {
        DLL_Error(); // pokud je seznam prázdný, zavoláme DLL_Error()
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast(DLList *list, long *dataPtr)
{
    if (list->lastElement != NULL) // kontrola, zda seznam není prázdný
    {
        *dataPtr = list->lastElement->data; // pokud seznam není prázdný, do dataPtr uložíme hodnotu posledního prvku
    }
    else
    {
        DLL_Error(); // pokud je seznam prázdný, zavoláme DLL_Error()
    }
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst(DLList *list)
{
    if (list->firstElement != NULL) //kontrola, jestli neni seznam prazdny
    {
        DLLElementPtr elementToDelete;
        elementToDelete =  list->firstElement;  //vytvoreni a ukazatele na prvek ktery budeme rusit a nastaveni jej na prvni prvek
        list->firstElement = list->firstElement->nextElement;   //posunuti ukazatele na prvni prvek na nasledujici 

        if (list->firstElement != NULL)
        {
            list->firstElement->previousElement = NULL; 
        }
        else
        {
            // Pokud byl smazán poslední prvek,lastElement zresetujeme
            list->lastElement = NULL;
        }

        if (list->activeElement == elementToDelete) //pokud sme smazali aktivni prvek, seznam prestava byt aktivni
        {
            list->activeElement = NULL;
        }
        free(elementToDelete);
        list->currentLength--;
    }
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast(DLList *list) //stejny princip jako DeleteFirst, jen mazeme posledni prvek
{
    if (list->lastElement != NULL)
    {
        DLLElementPtr elementToDelete = list->lastElement;
        list->lastElement = list->lastElement->previousElement;

        if (list->lastElement != NULL)
        {
            list->lastElement->nextElement = NULL;
        }
        else
        {
            // Pokud byl smazán první prvek, reset firstElement
            list->firstElement = NULL;
        }

        if (list->activeElement == elementToDelete)
        {
            list->activeElement = NULL;
        }
        free(elementToDelete);
        list->currentLength--;
    }
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter(DLList *list)
{
    if (list->activeElement != NULL && list->activeElement->nextElement != NULL)    //kontrola jestli je seznam aktivni a nema jen jeden prvek
    {
        DLLElementPtr elementToDelete = list->activeElement->nextElement; // Element k odstranění
        list->activeElement->nextElement = elementToDelete->nextElement; // Nastaveni ukazatele na další prvek

        if (elementToDelete->nextElement != NULL) // Kontrola, zda existuje dalsi prvek
        {
            elementToDelete->nextElement->previousElement = list->activeElement; // Nastaveni predchoziho ukazatele
        }
        else
        {
            list->lastElement = list->activeElement; // nastaveni ukazatele activeElement na posledni prvek, pokud byl posledni prvek smazan
        }

        free(elementToDelete); 
        list->currentLength--; 
    }
}



/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore(DLList *list) //stejny princip jako u DeleteFirst, jen otacime ukazatele u elementToDelete a activeElement
{
    if (list->activeElement != NULL && list->activeElement->previousElement != NULL)
    {
        DLLElementPtr elementToDelete = list->activeElement->previousElement; 
        list->activeElement->previousElement = elementToDelete->previousElement; 

        if (elementToDelete->previousElement != NULL) 
        {
            elementToDelete->previousElement->nextElement = list->activeElement; 
        }
        else
        {
            list->firstElement = list->activeElement; 
        }

        free(elementToDelete); 
        list->currentLength--; 
    }
}



/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter(DLList *list, long data)
{
    if (list->activeElement != NULL) // overujeme, ze je seznam aktivni
    {
        DLLElementPtr newElement = (DLLElementPtr)malloc(sizeof(struct DLLElement)); //alokace pameti pro newElement
        if (newElement == NULL) // pokud se alokace nezdarila, vratime DLL_Error()
        {
            DLL_Error();
            return;
        }

        newElement->data = data; // vkladani dat do noveho prvku
        newElement->nextElement = list->activeElement->nextElement; // nastaveni predniho ukazatele, na prvek za activeElement
        newElement->previousElement = list->activeElement; // nastaveni zpetneho ukazatele na activeElement

        if (list->activeElement == list->lastElement)
        {
            list->lastElement = newElement; // pokud byl activeElement poslednim prvkem seznamu, nastavujeme lastElement na vkladany prvek
        }
        else
        {
            list->activeElement->nextElement->previousElement = newElement; // jinak nastavujeme zpetny ukazatel prvku za activeElement na vkladany prvek
        }

        list->activeElement->nextElement = newElement;  //nastaveni predniho ukazatele activeElement na vkladany prvek
        list->currentLength++; 
    }
}


/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore(DLList *list, long data)  //stejny princip jako u InsertAfter, jen otacime ukazatele u vkladaneho prvku a na konci nastavujeme zpetny ukazatel ActiveElement misto predniho
{
    if (list->activeElement != NULL) 
    {
        DLLElementPtr newElement = (DLLElementPtr)malloc(sizeof(struct DLLElement)); 
        if (newElement == NULL) 
        {
            DLL_Error();
            return;
        }

        newElement->data = data; 
        newElement->previousElement = list->activeElement->previousElement; 
        newElement->nextElement = list->activeElement; 

        if (list->activeElement == list->firstElement)
        {
            list->firstElement = newElement; 
        }
        else
        {
            list->activeElement->previousElement->nextElement = newElement; 
        }

        list->activeElement->previousElement = newElement; 
        list->currentLength++; 
    }
}


/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue(DLList *list, long *dataPtr)
{
    if (list->activeElement != NULL) // kontrola aktivity seznamu
    {
        *dataPtr = list->activeElement->data; // pokud je seznam aktivni, do dataPtr ukladame hodnotu aktivniho prvku
    }
    else
    {
        DLL_Error(); // pokud neni aktivni vracime Dll_Error
    }
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue(DLList *list, long data)
{
    if (list->activeElement != NULL)
    {
        list->activeElement->data = data; // pokud je seznam aktivni, prepise se hodnota aktivniho prvku na hodnotu v data
    }
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next(DLList *list)
{
    if (list->activeElement != NULL)
    {
        list->activeElement = list->activeElement->nextElement;
    }
}

/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous(DLList *list)
{
    if (list->activeElement != NULL)
    {
        list->activeElement = list->activeElement->previousElement;
    }
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
bool DLL_IsActive(DLList *list)
{
    return (list->activeElement != NULL);
}

/* Konec c206.c */
