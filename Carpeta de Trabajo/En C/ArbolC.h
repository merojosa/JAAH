#ifndef ArbolC_H
#define ArbolC_H

#include "stdlib.h"     // malloc
#include "stdbool.h"    // bool
#include "lista_parametros.h"

// Nombres propios de algunos tipos de datos.
typedef int Etiqueta;
typedef struct Node Nodo_Arbol;
typedef struct Tree Arbol;

// Encabezados de todos los métodos.  Obligatorio en C.

Nodo_Arbol* constructor_nodo(char*, Nodo_Arbol* HD, Nodo_Arbol* padre, Nodo_Arbol* HI);
Nodo_Arbol* raiz(Arbol* arbol);
Nodo_Arbol* padre(Nodo_Arbol* padre);
Nodo_Arbol* hijoMasIzq(Nodo_Arbol* nodo);
Nodo_Arbol* hermanoDer(Nodo_Arbol* nodo);
void agregarHijo(Arbol* arbol, Nodo_Arbol* nodo, Nodo_Arbol*,Etiqueta etiq);

Arbol* constructor_arbol();
Arbol* arbol;

bool vacio(Arbol* arbol);
bool esHoja(Nodo_Arbol* nodo);

void borrarHoja(Arbol* arbol, Nodo_Arbol* nodo);
void modificarEtiqueta(Nodo_Arbol* nodo, Etiqueta etiq);
void destructor_arbol(Arbol* arbol);
void vaciar_arbol(Arbol* arbol, Nodo_Arbol* nodo);
void ponerRaiz(Arbol* arbol, Etiqueta etiq);

Etiqueta etiqueta(Nodo_Arbol* nodo);

int numNodos(Arbol* arbol);
int numHijos(Nodo_Arbol* nodo);

struct Node
{
    Etiqueta e;
	char* tokenName;
    Nodo_Arbol* HMI;      //Hijo más izquierdo
    Nodo_Arbol* HD;       //Hermano derecho
    Nodo_Arbol* padre;    //Padre
    Nodo_Arbol* HI;       //Hermano Izquierdo
	Lista_Punteros* params;
    int numHijos;
};

// Devuelve un puntero a un nodo en memoria dinámica.
Nodo_Arbol* constructor_nodo(char* token, Nodo_Arbol* HD, Nodo_Arbol* padre, Nodo_Arbol* HI )
{
    Nodo_Arbol* nodo = (Nodo_Arbol*) malloc(sizeof(Nodo_Arbol));  // Nodo_Arbol en memoria dinámica.
    nodo->e = numNodos(arbol);
	nodo->tokenName = token;
    nodo->HMI = 0x0;
    nodo->HD = HD;
    nodo->padre = padre;
    nodo->HI = HI;
    nodo->numHijos = 0;

    return nodo;
}

struct Tree
{
    Nodo_Arbol* raiz;      // ¿Puntero o no? AVERIGUARLO JEJE
    int cantNodos;
};

Arbol* constructor_arbol()
{
    arbol = (Arbol*) malloc(sizeof(Arbol));

    arbol->raiz = NULL;
    arbol->cantNodos = 0;

    return arbol;
}

void destructor_arbol( Arbol* arbol )
{
    vaciar_arbol( arbol, arbol->raiz );
    arbol->cantNodos = 0;
}

// Nota, originalmente le manda una copia.  Creo conveniente usar puntero.
void vaciar_arbol(Arbol* arbol, Nodo_Arbol* nodo)
{
    if (esHoja(nodo))
    {
        borrarHoja(arbol, nodo);
    }
    else
    {
        Nodo_Arbol* actual = nodo->HMI;
        for(int contador = 1; contador<= nodo->numHijos; ++contador)
        {
            vaciar_arbol(arbol, actual);
            actual = nodo->HMI;
        }
        borrarHoja(arbol, nodo);
    }
}

bool vacio( Arbol* arbol )
{
    return arbol->raiz == NULL;
}

void ponerRaiz(Arbol* arbol, Etiqueta etiq)
{
    arbol->raiz = constructor_nodo("root", NULL, NULL, NULL);
    arbol->cantNodos++;
}

void agregarHijo(Arbol* arbol, Nodo_Arbol* nodo, Nodo_Arbol* hijo, Etiqueta etiq)
{
    Nodo_Arbol* nuevoNodo = NULL;

    if(esHoja(nodo))
    {
        nuevoNodo = hijo;
        nodo->HMI = nuevoNodo;
    }
    else
    {
		Nodo_Arbol* actual = nodo->HD;
		while(actual->HD != NULL){
			actual = actual->HD;
		}
		actual->HD = hijo;
		hijo->HD = NULL;
    }
    ++nodo->numHijos;
    ++arbol->cantNodos;
}

void borrarHoja(Arbol* arbol, Nodo_Arbol* nodo)
{
    if(esHoja(nodo))
    {
        if(nodo->padre->HMI == nodo)
        {
            nodo->padre->HMI = nodo->HD;
        }
        --nodo->padre->numHijos;
        nodo->padre = NULL;

        if(nodo->HD != NULL)
        {
            nodo->HD->HI = nodo->HI;
        }
        if(nodo->HI != NULL)
        {
            nodo->HI->HD = nodo->HD;
        }

        nodo->HD = NULL;
        nodo->HI = NULL;
        free(nodo);
    }
    --arbol->cantNodos;
}

void modificarEtiqueta(Nodo_Arbol* nodo, Etiqueta etiq)
{
    nodo->e = etiq;
}

bool esHoja(Nodo_Arbol* nodo)
{
    return nodo->HMI == NULL;
}

Nodo_Arbol* raiz(Arbol* arbol)
{
    return arbol->raiz;
}

Nodo_Arbol* padre(Nodo_Arbol* padre)
{
    return padre->padre;
}

Nodo_Arbol* hijoMasIzq(Nodo_Arbol* nodo)
{
    return nodo->HMI;
}

Nodo_Arbol* hermanoDer(Nodo_Arbol* nodo)
{
    return nodo->HD;
}

Etiqueta etiqueta(Nodo_Arbol* nodo)
{
    return nodo->e;
}

int numNodos(Arbol* arbol)
{
    return arbol->cantNodos;
}

int numHijos(Nodo_Arbol* nodo)
{
    return nodo->numHijos;
}

#endif // ArbolC_H
