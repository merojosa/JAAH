#ifndef TABLA_SIMBOLOS_H
#define TABLA_SIMBOLOS_H

#define ID_MAXIMO 40    // Tamaño máximo de un identificador.
#define TAMANO_VALOR 100

typedef struct Node_T Nodo_Tabla;
typedef struct List Lista;

#include "string.h"
#include "stdlib.h"
#include "ArbolC.h"

/*Definicion del tipo*/
enum type {
    entero = 4,
    hilera = 1,
    caracter = 2,
    booleano = 3,
    unknown = 0
};
/*
 * Aquí estará un string identificador, un bool de si está inicializado, un int con el tipo,
 * un string(?) con el valor, un Nodo* donde se encuentra y un Nodo* al papá (acorde al profe).
 */
struct Node_T
{
    char id[ID_MAXIMO];
    bool inicializado;
    enum type tipo;             //0 si no se sabe el tipo, 1, si es bool, 2 si es string, etc...
    char valor[TAMANO_VALOR];  // Puede ser un puntero si se quiere dinámico.
    Nodo_Arbol* puntero_arbol;
    Nodo_Arbol* puntero_padre;
    Nodo_Tabla* siguiente;
};

struct List
{
    // Supongo que no es necesario un nodo anterior?
    Nodo_Tabla* primero;
};

Nodo_Tabla* constructor_nodo_tabla(
        char* id, bool inicializado, enum type tipo, char* valor, Nodo_Arbol* puntero_arbol, Nodo_Arbol* puntero_padre)
{
    Nodo_Tabla* nodo_tabla = (Nodo_Tabla*)malloc(sizeof(Nodo_Tabla));

    strcpy(nodo_tabla->id, id); // Copio el string del parámetro al nodo.
    nodo_tabla->inicializado = inicializado;
    nodo_tabla->tipo = tipo;
    strcpy(nodo_tabla->valor, valor);
    nodo_tabla->puntero_arbol = puntero_arbol;
    nodo_tabla->puntero_padre = puntero_padre;
    nodo_tabla->siguiente = NULL;

    return nodo_tabla;
}

Lista* constructor_lista()
{
    Lista* lista = (Lista*)malloc(sizeof(Lista));

    lista->primero = NULL;

    return lista;
}

// NOTA, agrega el nodo del parámetro.  Por lo que el que llama el método tiene que crear el nodo_tabla en heap.
void agregar_nodo_tabla( Lista* lista, Nodo_Tabla* nodo_tabla )
{
    if( lista->primero == NULL )             // Inicio nulo.
    {
        lista->primero = nodo_tabla;
    }
    else
    {
        Nodo_Tabla* temporal = lista->primero;
        while( temporal->siguiente != NULL )
        {
            temporal = temporal->siguiente;
        }

        temporal->siguiente = nodo_tabla;
    }
}

bool existe(Lista* lista,char* idToken){
  bool encontrado = false;
  for(Nodo_Tabla* temporal = lista->primero; temporal != NULL; temporal = temporal->siguiente){
    if(temporal->id == idToken){
      encontrado = true;
      temporal = NULL;
    }
  }
  return encontrado;
}

void destructor_lista(Lista* lista)
{
    Nodo_Tabla* temporal;

    if( lista->primero != NULL )
    {
        do
        {
            temporal = lista->primero;
            lista->primero = temporal->siguiente;   // Me muevo al siguiente.

            free(temporal);                     // Elimino el actual.

        } while( lista->primero != NULL );       // Si está en NULL, terminé.
    }

    temporal = NULL;

    free(lista);
}

#endif // TABLA_SIMBOLOS_H
