#ifndef ArbolHMI_HD_P_HI_h
#define ArbolHMI_HD_P_HI_h
#include <iostream>
#include <list>
#include <stack>
using namespace std;
typedef int Etiqueta;


enum type {
    unknown = 0,
	t_void = 2,
    hilera = 1,
    booleano = 3,
    entero = 4
};

struct Simbolo;

struct Caja{
    Etiqueta e;
	  string *tokenName;
    int tokenValue;
    Caja* HMI; 		//Hijo más izquierdo
    Caja* HD; 		//Hermano derecho
    Caja* padre; 	//Padre
    Caja* HI; 		//Hermano Izquierdo
    Simbolo* where;
    int numHijos;
    bool addToTable;
	list<Caja*>* params;
  list<Caja*>* asignacion;
  list<Caja*>* comparacion;
  list<Caja*>* operaciones;
  list<Caja*>* array;
	type tipo;

    Caja(Etiqueta etiq, string *token,Caja* HD, Caja* p){
        e = etiq;
		    tokenName = token;
        this->HMI = NULL;
        this->HD = HD;
        this->padre = p;
        this->params = 0x0;
        this->asignacion = 0x0;
        this->comparacion = 0x0;
        this->operaciones = 0x0;
        addToTable = false;
        numHijos = 0;
		    this->HI = NULL;
    };
};

typedef Caja* Nodo;

struct Simbolo{
	string* tokenName;
	bool inicializado;
	Nodo where;
	type tipo;
	Nodo scope;
	Simbolo(){
		tokenName = new string("genericName");
		inicializado = false;
		tipo = unknown;
		where = 0x0;
		this->scope = NULL;
	}
	Simbolo(string* name, bool ini, type assignedType, Nodo   scopeParent){
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
	  list<Simbolo*>* symbolTable;
    stack<Simbolo*> pila;//Para agregar los símbolos
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

    void mergeLists(list<Nodo>*,list<Nodo>*);

    //Retorna la lista de parámetros de n.
    list<Nodo>* getParams(Nodo n);

    //Retorna la tabla de símbolos.
    list<Simbolo*>* getTable(){
      return symbolTable;
    }

    /*
    Busca si existe el simbolo que se quiere agregar en la tabla con el mismo
    nombre y scope.
    */
    bool findSymbol(Simbolo* simbolo){
      bool existence = false;
      for(list<Simbolo*>::iterator it = this->symbolTable->begin(); it != this->symbolTable->end(); it++){
        if(*simbolo->tokenName == *(*it)->tokenName && simbolo->scope ==  (*it)->scope){
          existence = true;
        }
      }
      return existence;
    }

    /*
    Busca si ya existe el símbolo con el mismo token en la tabla, y si eso es
    cierto, si el que está declarado en la tabla está en un nivel más abajo del
    que se quiere agregar, si ese es el caso, modifica el scope del que se quiere agregar.
    */
    bool insertSymbol(Simbolo* s){
      Nodo symbolScope = 0x0;
      Nodo iteratorScope = 0x0;
      for(list<Simbolo*>::iterator it = this->symbolTable->begin(); it != this->symbolTable->end(); it++){
        if(*s->tokenName == *(*it)->tokenName){
          iteratorScope = (*it)->scope;
          symbolScope = s->scope;
          while(iteratorScope != nodoNulo){
            if(iteratorScope == s->scope){
              (*it)->scope = s->scope;
              (*it)->where = s->where;
              s->where = (*it)->where;
              if((*it)->tipo == unknown){
                (*it)->tipo = s->tipo;
              }
              return true;
            }
            iteratorScope = iteratorScope->padre;
          }

          while(symbolScope != nodoNulo){
            if(s->scope == (*it)->scope){
              return true;
            }
            symbolScope = symbolScope->padre;
          }
        }
      }
      /*
      for(list<Simbolo*>::iterator it = this->symbolTable->begin(); it != this->symbolTable->end(); it++){
        if(*s->tokenName == *(*it)->tokenName){
              iteratorScope = (*it)->scope;
              symbolScope = s->scope;
              while(iteratorScope->padre != nodoNulo){
                if(iteratorScope == s->scope){
                  (*it)->scope = s->scope;
                  (*it)->where = s->where;
                  (*it)->where->where = (*it);
                  if((*it)->tipo == unknown){
                    (*it)->tipo = s->tipo;
                  }
                  return true;
                }
                iteratorScope = iteratorScope->padre;
                symbolScope = symbolScope->padre;
              }
        }
      }*/
      return false;
    }


    /*
    * EFE:Agrega a la tabla de simbolos los nodos marcados en la lista.
    * REQ: lista inicializada y nodo no nulo.
    * MOD: symbolTable.
    */
    void agregarNodosDeListaATabla(list<Nodo>* lista, Nodo scope){

        if(lista->size() != 0){
        for(list<Nodo>::iterator it = lista->begin(); it != lista->end();it++){
          (*it)->padre = scope;
          if((*it)->addToTable){
            Simbolo* symbol = new Simbolo();
            symbol->tokenName = (*it)->tokenName;
			      symbol->where = (*it);
            symbol->scope = scope;
            symbol->tipo = (*it)->tipo;
            pila.push(symbol);
          }
        }
      }
    }

    void agregarNodoATabla(Nodo agregado, Nodo scope){
      if(*agregado->tokenName == "*asignacion*"){
        agregarNodosDeListaATabla(agregado->params,scope);
      } else if(*scope->tokenName != "ret"){
        Simbolo* symbol = new Simbolo();
        symbol->tokenName = agregado->tokenName;
        symbol->scope = scope;
		    symbol->where = agregado;
        symbol->tipo = agregado->tipo;
        if(agregado->array != 0x0){
          agregarNodosDeListaATabla(agregado->array,scope);
          if(agregado->asignacion != 0x0)agregarNodosDeListaATabla(agregado->asignacion,scope);
        }
        pila.push(symbol);
      }
    }

    void fillTable(){
      while(!pila.empty()){
        Simbolo* symbol = pila.top();
        pila.pop();
        if(!this->findSymbol(symbol)){
          if(!insertSymbol(symbol)){
            symbolTable->push_back(symbol);
          }
        }
      }
    }

    void printTable(){
      cout<<"\n\n Tabla De Simbolos \n";
      for(list<Simbolo*>::iterator it = symbolTable->begin(); it != symbolTable->end();it++){
        cout<<*(*it)->tokenName<<" "<<(*it)->tipo<<"               "<<*(*it)->scope->tokenName<<" "<<(*it)->scope;
        cout<<"\n";
      }
    }

};

Arbol::Arbol(){
    this->laRaiz = nodoNulo;
    symbolTable = new list<Simbolo*>();
    this->cantNodos = 0;
}

Arbol::~Arbol(){
    this->vaciar();
    delete symbolTable;
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
    this->laRaiz->padre = nodoNulo;
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

list<Nodo>* Arbol::getParams(Nodo n){
	return n->params;
}

void Arbol::mergeLists(list<Nodo> *L1,list<Nodo> *L2){
		//Deja L2 en L1 y L2 queda vacía;
		for(list<Nodo>::iterator it = L2->begin(); it != L2->end();it++){
			L1->push_back(*it);
		}
}
#endif /* ArbolHMI_HD_P_HI_h */
