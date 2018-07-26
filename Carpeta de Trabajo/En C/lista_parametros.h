#ifndef LISTA_PARAMETROS_H
#define LISTA_PARAMETROS_H

#define ID_MAXIMO 40
#define token_MAXIMO 20    // Tamaño máximo de un identificador.

typedef struct Node_L Nodo_Lista;
typedef struct List2 Lista_Punteros;

#include "string.h"
#include "stdlib.h"

/*
 * Aquí estará un string con el token, un int con el numero.
 */
struct Node_L
{
    char token[ID_MAXIMO];
    int* numero;
    Nodo_Lista* siguiente;
};

struct List2
{
    Nodo_Lista* primero;
};

Nodo_Lista* constructor_nodo_lista(char* token, int* num)
{
    Nodo_Lista* nodo_lista = (Nodo_Lista*)malloc(sizeof(Nodo_Lista));

    strcpy(nodo_lista->token, token); // Copio el string del parámetro al nodo.
    nodo_lista->numero = num;

    return nodo_lista;
}

Lista_Punteros* constructor_lista_p()
{
    Lista_Punteros* lista_p = (Lista_Punteros*)malloc(sizeof(Lista_Punteros));

    lista_p->primero = NULL;

    return lista_p;
}

// NOTA, agrega el nodo del parámetro.  Por lo que el que llama el método tiene que crear el nodo_tabla en heap.
void agregar_nodo_lista_p(Lista_Punteros* lista_p, Nodo_Lista* nodo_lista)
{
    Nodo_Lista* temporal;
    if( lista_p->primero == NULL )             // Inicio nulo.
    {
        lista_p->primero = nodo_lista;
    }
    else
    {
        temporal = lista_p->primero;
        while( temporal->siguiente != NULL )
        {
            temporal = temporal->siguiente;
        }

        temporal->siguiente = nodo_lista;
    }
}

void destructor_lista_p(Lista_Punteros* lista_p)
{
    Nodo_Lista* temporal;

    if( lista_p->primero != NULL )
    {
        do
        {
            temporal = lista_p->primero;
            lista_p->primero = lista_p->primero->siguiente;   // Me muevo al siguiente.

            free(temporal);                     // Elimino el actual.

        } while( lista_p->primero != NULL );       // Si está en NULL, terminé.
    }

    temporal = NULL;

    free(lista_p);
}

#endif // LISTA_PARAMETROS_H
