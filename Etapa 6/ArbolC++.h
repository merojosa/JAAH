#ifndef ArbolHMI_HD_P_HI_h
#define ArbolHMI_HD_P_HI_h
#include <iostream>
#include <list>
#include <vector>
using namespace std;
typedef int Etiqueta;


enum type {
    entero = 4,
    hilera = 1,
    caracter = 2,
    booleano = 3,
    unknown = 0
};

struct Caja{
    Etiqueta e;
	  string *tokenName;
    Caja* HMI; //Hijo más izquierdo
    Caja* HD; //Hermano derecho
    Caja* padre; //Padre
    Caja* HI; //Hermano Izquierdo
    int numHijos;
	list<string*>* params;

    Caja(Etiqueta etiq, string *token,Caja* HD, Caja* p){
        e = etiq;
		      tokenName = token;
        this->HMI = 0x0;
        this->HD = HD;
        this->padre = p;
        numHijos = 0;
    };
};

typedef Caja* Nodo;

struct Simbolos{
	string tokenName;
	bool inicializado;
	type tipo;
	Nodo* scope;
	Simbolos(){
		tokenName = "genericName";
		inicializado = false;
		tipo = unknown;
		this->scope = NULL;
	}
	Simbolos(string name, bool ini, type assignedType, Nodo* scopeParent){
		tokenName = name;
		inicializado = ini;
		tipo = assignedType;
		this->scope = scopeParent;
	}
};

const Nodo nodoNulo = 0x0;


class Arbol{
private:
    Nodo laRaiz;
    int cantNodos;
	vector<Simbolos>* symbolTable;
    void vaciarRec(Nodo n); //Vaciar a partir de este nodo
public:
    /*
     * EFE: Crea un Arbol Listo para Usar.
     * REQ: N/A
     * MOD: N/A
     */Arbol();

    /*
     * EFE: Destruye el arbol y lo deja inutilizable
     * REQ: Arbol inicializado.
     * MOD: Arbp;
     */virtual ~Arbol();

    /*
     * EFE: Elimina todos los elementos del arbol.
     * REQ: Arbol inicializado.
     * MOD: Arbol.
     */void vaciar();

    /*
     * EFE: Retorna verdadero si el arbol no tiene elementos, false en caso contrario.
     * REQ: Arbol inicializado.
     * MOD: N/A.
     */bool vacio();

    /*
     * EFE: Pone el primer elemento del arbol.
     * REQ: Arbol inicializado.
     * MOD: Arbol.
     */void ponerRaiz(Etiqueta, string*);

    /*
     * EFE: Agrega un nuevo nodo al arbol como hijo de n.
     * REQ: Arbol inicializado y no vacio, nodo valido en el arbol y pos entre uno y la cantidad de hijos del nodo + 1.
     * mas uno.
     * MOD: Arbol
     */void agregarHijo(Nodo n, Nodo nuevo);

    /*
     * EFE: Elimina un nodo hoja del arbol.
     * REQ: Arbol inicializado y nodo valido en el arbol.
     * MOD: Arbol.
     */void borrarHoja(Nodo n);

    /*
     * EFE: Modifica la etiqueta del nodo por la nueva etiqueta ingresada.
     * REQ: Arbol inicializado y nodo valido en el arbol.
     * MOD: N/A.
     */void modificarEtiqueta(Nodo n, Etiqueta etiq);

    /*
     * EFE: Retorna la raiz del arbol, si el arbol esta vacio retorna nodo nulo.
     * REQ: Arbol inicializado.
     * MOD: N/A.
     */Nodo raiz();

    /*
     * EFE: Retorna el padre del nodo.
     * REQ: Arbol inicializado y nodo valido en el arbol.
     * MOD: N/A.
     */Nodo padre(Nodo n);

    /*
     * EFE: Retorna el hijo mas izquierdo del nodo.
     * REQ: Arbol inicilizado y nodo valido en el arbol.
     * MOD: N/A.
     */Nodo hijoMasIzq(Nodo n);

    /*
     * EFE: Retorna el hermano derecho del nodo.
     * REQ: Arbol inicializado y nodo valido en el arbol.
     * MOD: N/A.
     */Nodo hermanoDer(Nodo n);

    /*
     * EFE: Retorna la etiqueta del nodo.
     * REQ: Arbol inicializado y nodo valido en el arbol.
     * MOD: N/A.
     */Etiqueta etiqueta(Nodo n);

	 //Retorna la lista de parámetros de n.
	 list<string*>* getParams(Nodo n);

	 //Retorna la tabla de símbolos.
	 vector<Simbolos>* getTable(){
		 return symbolTable;
	 }

    /*
     * EFE: Retorna la cantidad de nodos contenidos en el arbol.
     * REQ: Arbol inicializado.
     * MOD: N/A.
     */int numNodos();

    /*
     * EFE: Retorna la cantidad de hijos del nodo.
     * REQ: Arbol inicializado y nodo valido en el arbol.
     * MOD: N/A.
     */int numHijos(Nodo n);

    /*
     * EFE: Retorna un booleano, V si es hoja, F si no.
     * REQ: Arbol inicializado y nodo valido en el arbol.
     * MOD: N/A.
     */bool esHoja(Nodo n);

    void mergeLists(list<string*>*,list<string*>*);
};

Arbol::Arbol(){
    this->laRaiz = nodoNulo;
    this->cantNodos = 0;
}

Arbol::~Arbol(){
    this->vaciar();
    cantNodos = 0;
}

void Arbol::vaciar(){
    vaciarRec(this->laRaiz);
    cantNodos = 0;
}

void Arbol::vaciarRec(Nodo n){
    if (esHoja(n)){
        borrarHoja(n);
    }
    else{
        Nodo actual = n->HMI;
        for(int i=1; i<= n->numHijos; ++i){
            vaciarRec(actual);
            actual = n->HMI;
        }
        borrarHoja(n);
    }
}

bool Arbol::vacio(){
    return this->laRaiz == nodoNulo;
}

void Arbol::ponerRaiz(Etiqueta cnt, string* token){
    this->laRaiz = new Caja(cnt, token,nodoNulo, nodoNulo);
    cantNodos++;
}

void Arbol::agregarHijo(Nodo n, Nodo nuevo){
    Nodo nodoNuevo = nodoNulo;
    if(esHoja(n)){
        n->HMI = nuevo;
    } else{
		nodoNuevo = n->HMI;
		while(nodoNuevo->HD != nodoNulo){
			nodoNuevo = nodoNuevo->HD;
		}
		nodoNuevo->HD = nuevo;
		nuevo->HI = nodoNuevo;
		nuevo->padre = n;
		nuevo->HMI = NULL;
    ++n->numHijos;
    ++cantNodos;
	}
}

void Arbol::borrarHoja(Nodo n){
    if(esHoja(n)){
        if(n->padre->HMI == n){
            n->padre->HMI = n->HD;
        }
        --n->padre->numHijos;
        n->padre = nodoNulo;
        if(n->HD != nodoNulo){
            n->HD->HI = n->HI;
        }
        if(n->HI != nodoNulo){
            n->HI->HD = n->HD;
        }
        n->HD = nodoNulo;
        n->HI = nodoNulo;
        delete n;
    }
    --cantNodos;
}

void Arbol::modificarEtiqueta(Nodo n, Etiqueta etiq){
    n->e = etiq;
}

Nodo Arbol::raiz(){
    return this->laRaiz;
}

Nodo Arbol::padre(Nodo n){
    return n->padre;
}

Nodo Arbol::hijoMasIzq(Nodo n){
    return n->HMI;
}

Nodo Arbol::hermanoDer(Nodo n){
    return n->HD;
}

Etiqueta Arbol::etiqueta(Nodo n){
    return n->e;
}

int Arbol::numNodos(){
  this->cantNodos++;
    return this->cantNodos;
}

int Arbol::numHijos(Nodo n){
    return n->numHijos;
}

bool Arbol::esHoja(Nodo n){
    return n->HMI == nodoNulo;
}

list<string*>* Arbol::getParams(Nodo n){
	return n->params;
}

void Arbol::mergeLists(list<string*> *L1,list<string*> *L2){
		//Deja L2 en L1 y L2 queda vacía;
		for(list<string*>::iterator it = L2->begin(); it != L2->end();it++){
			L1->push_back(*it);
		}
}
#endif /* ArbolHMI_HD_P_HI_h */
