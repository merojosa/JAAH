%{
#include <stdio.h>
#include <string>
#include <stack>

#include "ArbolC++.h"
using namespace std;
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern "C" char* yytext;
void yyerror(const char *s);
Arbol arbol;
std::stack<Caja*> pila;
int cuenta = 0;
bool parametros = false;
bool operaciones = false;
bool comparaciones = false;
bool semanticERROR = false;

/*______________________________________________________________________________

INICIO DE LOS METODOS DE IMPRESION.

________________________________________________________________________________
*/
void printList(list<Nodo>* lst){
	for(list<Nodo>::iterator it = lst->begin(); it != lst->end(); it++){
		if(*(*it)->tokenName == "*NULL*"){
			cout<<(*it)->tokenValue;
		} else {
			cout<<*(*it)->tokenName;
		}
		it++;
		if(parametros && it != lst->end()){
			cout<<", ";
		}
		it--;
	}
}


void imprimir_tabs(int cantidad_tabs)
{
	for(int contador = 0; contador < cantidad_tabs; ++contador)
	{
		cout << '\t';
	}
}

/**
* EFE: Lista las etiquetas del arbol haciendo un recorrido en pre-orden.
* REQ: Arbol inicializado.
* MOD: N/A.
*/
void listPreORecur(Arbol& arbol, Nodo nodo, int cantidad_tabs)
{
	imprimir_tabs(cantidad_tabs);\
	if(cantidad_tabs == 0){
		cout << *nodo->tokenName << endl;
	} else {
		if(arbol.esHoja(nodo)){
			cout<<"Instruccion ";
		} else {
			if(cantidad_tabs == 1){
				cout << "Metodo ";
			} else {
				cout<<"Bloque ";
			}
		}
		if(*nodo->tokenName != "*asignacion*" && *nodo->tokenName != "*NULL*"){
			cout << *nodo->tokenName << " ";
		} else if(*nodo->tokenName == "*NULL*"){
			cout << nodo->tokenValue << " ";
		}
	}				// Imprimo instrucción.
	if(nodo->params != 0x0){
		parametros = true;
		if(nodo->params->size() != 0){
			cout<<" Parametros: ";
			printList(nodo->params);
		}
		parametros = false;
	}

	if(nodo->array != 0x0){
		if(nodo->array->size() != 0){
			cout<<" Tamaño Arreglo: ";
			printList(nodo->array);
		}
		parametros = false;
	}

	if(nodo->asignacion != 0x0){
		if(nodo->asignacion->size() != 0){
			cout<<" Asignacion: ";
			printList(nodo->asignacion);
		}
	}
	if(nodo->comparacion != 0x0){
		if(nodo->comparacion->size() != 0){
			cout<<" Comparacion: ";
			printList(nodo->comparacion);
		}
	}
	if(nodo->operaciones != 0x0){
		if(nodo->operaciones->size() != 0){
			cout<<" Operaciones: ";
			printList(nodo->operaciones);
		}
	}
	cout<<"\n";
	Nodo hijo = arbol.hijoMasIzq(nodo);
	while (hijo != nodoNulo) {
		listPreORecur(arbol, hijo, cantidad_tabs+1);
		hijo = arbol.hermanoDer(hijo);
	}
}

// El llamado que se hace en el main.
void listPreO(Arbol& arbol)
{
	cout<<"\n";
	if (!arbol.vacio())
	{
		listPreORecur(arbol, arbol.raiz(), 0);
	}
}

void printTree(){
	listPreO(arbol);
	//listPostO(arbol);
	arbol.printTable();
}
/*______________________________________________________________________________

FIN DE LOS METODOS DE IMPRESION
________________________________________________________________________________
*/

/*______________________________________________________________________________

INICIO DE LOS METODOS DE BUSQUEDA DE ANALISIS
________________________________________________________________________________
*/

bool caracterEspecial(string* tokenName){
		if(*tokenName == "+" || *tokenName == "-" || *tokenName == "/" || *tokenName == "*" || *tokenName == "%" ||
			 *tokenName == "==" || *tokenName == ">" || *tokenName == "<" || *tokenName == "<=" || *tokenName == ">=" ||
			 *tokenName == "||" || *tokenName == "&&" || *tokenName == "=" || *tokenName == "*NULL*" || (*tokenName)[0] == '\"'
		 || *tokenName == "verdadero" || *tokenName == "falso" || *tokenName == "nulo"){
			 return true;
		 }
		return false;
}

/*Obtiene el tipo de retorno de un método*/
Nodo retType(Nodo n){
	Nodo tmp = n;
	bool rst = true;
	Nodo found;
	while(tmp->padre != nodoNulo){
		if(*tmp->padre->tokenName == "Programa:"){
			return tmp;
		}
		tmp = tmp->padre;
	}
}

bool esParametro(Nodo n, list<Caja*>* lst){
		for(list<Caja*>::iterator it = lst->begin(); it != lst->end(); it++){
			if(*n->tokenName == *(*it)->tokenName){
				return true;
			}
		}
		return false;
}

bool nodoEnRango(Nodo nodo, Simbolo* symbol){
	/*=============================================================================================================*/
	//return true;
	bool rst = false;
	if(symbol == nodo->where){//Para metodos.
		rst = true;
	} else {
		if(symbol->scope->params != 0x0){
			if(*symbol->scope->tokenName == "para"){
				if(*(*symbol->scope->asignacion->begin())->tokenName == *nodo->tokenName){
						rst = true;
				}
			} else {
				rst = esParametro(nodo,symbol->scope->params);
			}
		}
		Nodo tmpNode = nodo->HI;
		Nodo tmpParent = nodo->padre;

		while(tmpParent != nodoNulo && !rst){
			while(tmpNode != nodoNulo && !rst){
				if(*tmpNode->tokenName == *nodo->tokenName){
					rst = true;
				}
				if(*tmpNode->tokenName == "*asignacion*" || *tmpNode->tokenName == "para"){
					if(*(*tmpNode->asignacion->begin())->tokenName == *symbol->tokenName){
						rst = true;
					} else if (tmpParent->params!=0x0){
						rst = esParametro(nodo, tmpParent->params);
					}
				} else if(tmpParent->params != 0x0 && !rst){
						rst = esParametro(nodo, tmpParent->params);
				}
				tmpNode = tmpNode->HI;
			}
			tmpNode = tmpParent;
			tmpParent = tmpParent->padre;
		}
	}

	if(rst && nodo->where == 0x0){
		nodo->where = symbol;
	}
	return rst;
}

/*Verifica que el token este en la tabla de simbolos, si lo esta, verifica que el encontrado este en el
rango correcto.*/
bool nodoEnTabla(Nodo nodo){
	bool encontrado = false;
	for(list<Simbolo*>::iterator it = arbol.getTable()->begin(); it != arbol.getTable()->end() && !encontrado; it++){
		if(*(*it)->tokenName == *nodo->tokenName){
			nodo->where = (*it);
			encontrado = nodoEnRango(nodo, (*it));
			if(operaciones && encontrado){
				if((*it)->tipo != entero && (*it)->tipo != unknown){
					cout<<"Error: "<<*(*it)->tokenName<<" no es tipo entero, por lo que no se puede usar en esta operacion.\n";
				}
			}
		}
	}

	if(!encontrado){
		cout<<"Token: "<< *nodo->tokenName<<" no ha sido declarado en este scope.\n";
		semanticERROR = true;
	}
	return encontrado;
}

//CREO QUE FUNCA PERO DEBO ARREGLAR LOS PARAMETROS.
void validezDeComparaciones(list<Caja*>* lst){
	list<Caja*>::iterator it1, it2, it3, pivot;
	int cnt1, cnt3;
	bool comparisonError = false;
	type tipo1;
	type tipo3;
	cnt1 = 0;
	cnt3 = 0;
	it1 = lst->begin();
	it2 = it1;//It2 will be the compration operator
	while(*(*it2)->tokenName != "==" && *(*it2)->tokenName != "<=" && *(*it2)->tokenName != ">=" &&
				*(*it2)->tokenName != "<" && *(*it2)->tokenName != ">=" && it2 !=lst->end()){
		it2++;
		cnt1++;
	}
	if(it2 != lst->end()){
		it3 = it2;
		it3++;
	}
		pivot = it3;
	do{
		while(*(*pivot)->tokenName != "||" && *(*pivot)->tokenName != "&&" && pivot != lst->end()){
			++pivot;
			cnt3++;
			if(pivot == lst->end()){//to get out of this without an arror.
				break;
			}
		}
		//==========================================================================================================
		//comparisons go here
		if(*(*it1)->tokenName == "*NULL*"){//if it1 is a number.
			if(*(*it3)->tokenName != "*NULL*"){
				if((*it3)->where != 0x0){//if it3 is a valid symbol.
					if((*it3)->where->tipo != unknown && (*it3)->where->tipo != entero){
						comparisonError = true;
					}
				}
			}
		} else {//if it1 has a symbol.
			if(*(*it3)->tokenName != "*NULL*"){//comparison between two variables
				if((*it3)->where != 0x0 && (*it3)->where != 0x0){//if both symbols are valid.
					if((*it3)->where->tipo != (*it1)->where->tipo){
						if((*it3)->where->tipo != unknown || (*it3)->where->tipo != unknown){//we can compare unknown with anything.
							comparisonError = true;
						}
					}
				}
			} else {//it1 contains a number
				if((*it1)->where != 0x0){//if it3 is a valid symbol.
					if((*it1)->where->tipo != unknown && (*it1)->where->tipo != entero){
						comparisonError = true;
					}
				}
			}
		}
		if(comparisonError){
			cout<<"No se pueden comparar ";
			if(*(*it1)->tokenName == "*NULL*"){
				cout<< (*it1)->tokenValue;
			} else {
				cout<< (*(*it1)->tokenName);
			}
			cout<<" y ";
			if(*(*it3)->tokenName == "*NULL*"){
				cout<< (*it3)->tokenValue;
			} else {
				cout<< *(*it3)->tokenName;
			}
			cout<<" ya que son distintos tipos.\n";
			semanticERROR = true;
		}
		comparisonError = false;
		//==========================================================================================================
		cnt3 = 0;
		cnt1 = 0;
		if(pivot != lst->end()){//There are || or &&.
			it1 = pivot;
			it1++;
			it2 = it1;
			while(*(*it2)->tokenName != "==" && *(*it2)->tokenName != "<=" && *(*it2)->tokenName != ">=" &&
						*(*it2)->tokenName != "<" && *(*it2)->tokenName != ">=" && it2 !=lst->end()){
				it2++;
				cnt1++;
			}
				if(it2 != lst->end()){
				it3 = it2;
				it3++;
			}
			pivot = it3;
		} else {
			break;
		}
	} while(pivot != lst->end());
}

//******************************************************************************************

//******************************************************************************************

void buscarEnTablaListasDeNodos(Nodo nodo){
	if(nodo->params != 0x0){
	if(nodo->params->size() != 0){
		for(list<Nodo>::iterator it = nodo->params->begin(); it != nodo->params->end(); it++){
				if(!caracterEspecial((*it)->tokenName)){
					nodoEnTabla(*it);
				}
			}
		}
	}

	if(nodo->comparacion != 0x0){
		if(nodo->comparacion->size() != 0){
			list<Nodo>::iterator it = nodo->comparacion->begin();
				while(it != nodo->comparacion->end()){
					if(!caracterEspecial((*it)->tokenName)){
						nodoEnTabla(*it);
					}
					it++;
				}
				validezDeComparaciones(nodo->comparacion);
			}
		}

		if(nodo->operaciones != 0x0){
			if(nodo->operaciones->size() != 0){
				list<Nodo>::iterator it = nodo->operaciones->begin();
					while(it != nodo->operaciones->end()){
						if(!caracterEspecial((*it)->tokenName)){
							operaciones = true;
							nodoEnTabla(*it);
							operaciones = false;
						}
						it++;
					}
				}
		}

		if(nodo->asignacion != 0x0){
			if(nodo->asignacion->size() != 0){
				list<Nodo>::iterator it = nodo->asignacion->begin();
					it++;
					it++;
					while(it != nodo->asignacion->end()){
						if(!caracterEspecial((*it)->tokenName)){
							nodoEnTabla(*it);
						}
						it++;
					}
				}
			}

			if(nodo->array != 0x0){
				if(nodo->array->size() != 0){
					list<Nodo>::iterator it = nodo->array->begin();
						it++;
						it++;
						while(it != nodo->array->end()){
							if(!caracterEspecial((*it)->tokenName)){
								operaciones = true;
								nodoEnTabla(*it);
								operaciones = false;
							}
							it++;
						}
					}
				}

}

void printType(type tipo){
		switch(tipo){
			case hilera: cout<<"hilera ";
										break;
			case entero: cout<<"entero ";
										break;
			case booleano: cout<<"booleano ";
										break;
		}
}

void semanticAnalisisPreORecur(Arbol& arbol, Nodo nodo, int nivel)
{
	if(nivel > 0){
		if(*nodo->tokenName != "*NULL*"){
			if(*nodo->tokenName != "si" && *nodo->tokenName != "sino" &&
					*nodo->tokenName != "entrada" && *nodo->tokenName != "salida" &&
					*nodo->tokenName != "para" && *nodo->tokenName != "mientras" &&
					*nodo->tokenName != "*asignacion*" && *nodo->tokenName != "ret")
					{
						nodoEnTabla(nodo);
					}

						if(nodo->params != 0x0){
							if(nodo->params->size() != 0){
								for(list<Nodo>::iterator it = nodo->params->begin(); it != nodo->params->end(); it++){
										if(!caracterEspecial((*it)->tokenName)){
											nodoEnTabla(*it);
											buscarEnTablaListasDeNodos(*it);
										}
									}
								}
							}

							if(nodo->comparacion != 0x0){
								if(nodo->comparacion->size() != 0){
									list<Nodo>::iterator it = nodo->comparacion->begin();
										while(it != nodo->comparacion->end()){
											if(!caracterEspecial((*it)->tokenName)){
												nodoEnTabla(*it);
												buscarEnTablaListasDeNodos(*it);
											}
											it++;
										}
										validezDeComparaciones(nodo->comparacion);
									}
								}

								if(nodo->operaciones != 0x0){
									if(nodo->operaciones->size() != 0){
										list<Nodo>::iterator it = nodo->operaciones->begin();
											while(it != nodo->operaciones->end()){
												if(!caracterEspecial((*it)->tokenName)){
													operaciones = true;
													nodoEnTabla(*it);
												  operaciones = false;
													buscarEnTablaListasDeNodos(*it);
												}
												it++;
											}
										}
								}

								if(nodo->asignacion != 0x0){
									if(nodo->asignacion->size() != 0){
										list<Nodo>::iterator it = nodo->asignacion->begin();
											it++;
											it++;
											while(it != nodo->asignacion->end()){
												if(!caracterEspecial((*it)->tokenName)){
													nodoEnTabla(*it);
													buscarEnTablaListasDeNodos(*it);
												}
												it++;
											}
										}
									}

									if(nodo->array != 0x0){
										if(nodo->array->size() != 0){
											list<Nodo>::iterator it = nodo->array->begin();
												it++;
												it++;
												while(it != nodo->array->end()){
													if(!caracterEspecial((*it)->tokenName)){
														operaciones = true;
														nodoEnTabla(*it);
														operaciones = false;
														buscarEnTablaListasDeNodos(*it);
													}
													it++;
												}
											}
										}

										//Se verifica que el valor de retorno sea real.
										if(*nodo->tokenName == "ret"){
												Nodo padre = retType(nodo);
												if(padre->where->tipo == unknown){//Actualiza el tipo del método.
													if(nodo->tipo != unknown){
														padre->tipo = nodo->tipo;
														padre->where->tipo = nodo->tipo;
													} else {
														padre->tipo = (*nodo->params->begin())->where->tipo;
														padre->where->tipo = (*nodo->params->begin())->where->tipo;
													}
												}
											if(nodo->tipo == unknown){//When a variable is being returned.
												if((*nodo->params->begin())->where != 0x0){//If the symbol exists in the table.
													if(padre->tipo != (*nodo->params->begin())->where->tipo && padre->tipo != unknown && (*nodo->params->begin())->where->tipo != unknown){
															cout<<"Retorno inadecuado para el método "<<*padre->tokenName<<"\n"<<
															"Retorno esperado: ";
															printType(padre->tipo);
															cout<<"\n";
															cout<<"Retorno obtenido: ";
															cout<<" en "<<*(*nodo->params->begin())->tokenName;
															cout<<"\n";
														}
													}
												} else {
													if(padre->tipo != nodo->tipo && nodo->tipo != unknown){
														cout<<"Retorno inadecuado para el método "<<*padre->tokenName<<"\n"<<
														"Retorno esperado: ";
														printType(padre->tipo);
														cout<<"\n";
														cout<<"Retorno obtenido: ";
														printType(nodo->tipo);
														cout<<" en "<<*nodo->tokenName;
														cout<<"\n";
													}
												}
										}
			}
		}
	Nodo hijo = arbol.hijoMasIzq(nodo);
	if( hijo != nodoNulo ){
		semanticAnalisisPreORecur(arbol, hijo,nivel+1);
	}

	Nodo hermano_derecho = arbol.hermanoDer(nodo);
	if( hermano_derecho != nodoNulo )				// Verifico si tiene hermano.
	{
		semanticAnalisisPreORecur(arbol, hermano_derecho, nivel);
	}
	// Si no tiene hermano, termino (sea nivel o método).
}

// El llamado que se hace en el main.
void semanticAnalisisPreO(Arbol& arbol)
{
	cout<<"\n";
	if (!arbol.vacio())
	{
		semanticAnalisisPreORecur(arbol, arbol.raiz(), 0);
	}
}
/*______________________________________________________________________________

FIN DE LOS METODOS DE BUSQUEDA DE EXISTENCIA EN LA TABLA Y ALCANCE
________________________________________________________________________________
*/

%}
%error-verbose

%code requires{
	#include <list>
	#include <string>
	using namespace std;
}
%union {
	string* hilera;
	int intVal;
	struct Caja* nodo;
	struct Simbolos* simbolo;
	std::list<Caja*>* params;
}

%token <hilera> HILERA "Hilera"
%token <hilera> FOR "para"
%token <hilera> WHILE "mientras"
%token <hilera> IF "si"
%token <hilera> ELSE "sino"
%token <hilera> COMPARACION "comparador"
%token <hilera> AND_OR "&& o ||"
%token <hilera> BOOL "verdadero o falso"
%token <intVal> NUM "numero"
%token <hilera> INI "ini"
%token <hilera> FIN "fin"
%token <hilera> RET "ret"
%token <hilera> MAS "+"
%token <hilera> MENOS "-"
%token <hilera> POR "*"
%token <hilera> ENTRE "/"
%token <hilera> MOD "%"
%token <hilera> ENTRADA "entrada"
%token <hilera> SALIDA "salida"
%token <hilera> ID "identificador"
%token <hilera> ERROR
%token <hilera> EJECUCION "principal"
%token <hilera> PARD ")"
%token <hilera> CORD "]"
%token <hilera> PARI "("
%token <hilera> CORI "["
%token <hilera> IGUAL "="
%token <hilera> DOSP ":"
%token <hilera> COM ","
%token <hilera> PUNTO "."
%token <hilera> POINTER "->"
%token <hilera> PYC ";"
%token <hilera> LINK "enlazador de archivos"
%token <hilera> JAAH "ruta del archivo"


%type <nodo> declarations
%type <nodo> assign
%type <nodo> rule_IF
%type <params> comparaciones;
%type <nodo> instrucciones;
%type <nodo> tipos
%type <params> operaciones
%type <nodo> rule_for
%type <nodo> rule_While
%type <nodo> rule_ELSE
%type <nodo> ret
%type <nodo> method_call
%type <nodo> method
%type <params> method_arguments
%type <params> concat_IDs_operations
%type <nodo> operando
%type <nodo> arreglo
%type <hilera> OPERACIONES
%type <params> comparacionesbooleanas
%type <params> tamano_arreglo
%type <nodo> inicio
%type <nodo> principal

%%
//This is where the fun begins.

super:
	inicio
	{
	  string* root = new std::string("Programa:");
		arbol.ponerRaiz(cuenta,root);
		cuenta++;
		Nodo raiz = arbol.raiz();

		arbol.agregarHijo(arbol.raiz(),$1);
		for(Nodo tmp = $1; tmp != nodoNulo; tmp = tmp->HD)
		{
			tmp->padre = arbol.raiz();
			Nodo tmp2 = tmp->HD;
			if(tmp2 != nodoNulo) tmp2->HI = tmp;
			if(*tmp->tokenName == "*asignacion*")
			{
				arbol.agregarNodosDeListaATabla(tmp->asignacion, arbol.raiz());
			}
			if(tmp->addToTable)
			{
				arbol.agregarNodoATabla(tmp,arbol.raiz());
			}
		}
		arbol.fillTable();
		//printTree();
		semanticAnalisisPreO(arbol);
		if(semanticERROR){
			exit(0);
		}
	}
	;

inicio:
	concat_links principal {$$ = $2;}
	| principal {$$ = $1;}
	;

principal:
	declarations method principal
	{
		$2->HD = $3;
		$3->HI = $2;
		if($1 != nodoNulo){
			Nodo i;
			for(i = $1; i->HD != nodoNulo; i = i->HD){
				//Iterates through the declaration Nodes.
				//'til it finds the last one different to null.
			}
			$$ = $1;
			i->HD = $2;
			$2->HI = $$;
		} else {
			$$ = $2;
		}
	}
	| declarations method
	{
		$$ = $2;
		if($1 != nodoNulo){
			Nodo i;
			for(i = $1; i->HD != nodoNulo; i = i->HD){
				//Iterates through the declaration Nodes.
				//'til it finds the last one different to null.
			}
			i->HD = $$;
			$$->HI = $1;
			$$ = $1;
		}
	}
	;

declarations:
	assign PYC declarations
	{
		$$ = $1;
		if($3 != nodoNulo){
			$1->HD = $3;
			$3->HI = $$;
		}
	}
	| ID PYC declarations
	{
		$$ = new Caja(cuenta++,$1,NULL,NULL);
		$$->addToTable = true;
		if($3 != nodoNulo){
			$$->HD = $3;
			$3->HI = $$;
		}
	}
	| arreglo PYC declarations
	{
		$$->addToTable = true;
		if($3 != nodoNulo){
			$$->HD = $3;
			$3->HI = $$;
		}
	}
	| {$$ = nodoNulo;}
	;

assign:
	ID IGUAL tipos
	{
		$$ = new Caja(cuenta++, NULL, NULL, NULL);
		$$->tokenName = new string("*asignacion*");
		Caja* temp = new Caja( cuenta++, $2, NULL, NULL );
		$$->asignacion = new list<Caja*>();
		$$->asignacion->push_front(temp);
		temp = new Caja( cuenta++, $1, NULL, NULL );
		temp->addToTable = true;
		temp->tipo = $3->tipo;
		$$->tipo = $3->tipo;
		$$->asignacion->push_front(temp);
		$$->asignacion->push_back($3);
		// tipos se agrega desde su producción.
		}
	| ID IGUAL operaciones
	{
		$$ = new Caja(cuenta++, NULL, NULL, NULL);
		Caja* temp = new Caja( cuenta++, $2, NULL, NULL );
		$$->tokenName = new string("*asignacion*");
		$$->asignacion = $3;
		$$->asignacion->push_front(temp);
		temp = new Caja( cuenta++, $1, NULL, NULL );
		temp->addToTable = true;
		temp->tipo = entero;
		list<Caja*>::iterator it = $3->begin();
		$$->asignacion->push_front(temp);
		$$->tipo = entero;
	}
	| arreglo IGUAL tipos
	{
		$$ = $1;
		$$->addToTable = true;
		Caja* temp = new Caja( cuenta++, $2, NULL, NULL );
		$$->tipo = $3->tipo;
		$$->asignacion = new list<Caja*>();
		$$->asignacion->push_front(temp);
		$$->asignacion->push_back($3);

	}
	| arreglo IGUAL operaciones
	{
		list<Caja*>::iterator it = $3->begin();
		$$ = $1;
		$1->addToTable = true;
		Caja* temp = new Caja( cuenta++, $2, NULL, NULL );
		$$->asignacion->push_front(temp);
		$$->asignacion = $3;
		$$->tipo = entero;
	}
	| ID IGUAL method_call
	{
		$$ = new Caja(cuenta++, NULL, NULL, NULL);
		Caja* temp = new Caja( cuenta++, $2, NULL, NULL );
		$$->tokenName = new string("*asignacion*");
		$$->asignacion = new list<Caja*>();
		$$->asignacion->push_back($3);
		$$->asignacion->push_front(temp);
		temp = new Caja( cuenta++, $1, NULL, NULL );
		temp->addToTable = true;
		temp->tipo = $3->tipo;
		$$->asignacion->push_front(temp);
	}
	;

concat_links:
	LINK JAAH concat_links
	| LINK JAAH
	;

operaciones:
	operaciones OPERACIONES operando
	{
		$$ = $1;

		Caja* temp = new Caja( cuenta++, $2, NULL, NULL );
		$$->push_back(temp);

		$$->push_back($3);
		if($3->tipo == unknown){
			$3->tipo = entero;
		}
	}
	| operando OPERACIONES operando {
		$$ = new list<Caja*>();

		$$->push_back($1);

		Caja* temp = new Caja( cuenta++, $2, NULL, NULL );
		$$->push_back(temp);

		$$->push_back($3);
		if($3->tipo == unknown){
			$3->tipo = entero;
		}
		if($1->tipo == unknown){
			$1->tipo = entero;
		}
	}
	;

tipos:
	operando {$$ = $1;}
	| BOOL
	{
		$$ = new Caja(cuenta++, $1,NULL,NULL);
		$$->tipo = booleano;
	}
	| HILERA
	{
		$$ = new Caja(cuenta++, $1,NULL,NULL);
		$$->tipo = hilera;
	}
	;

operando:
	ID
	{
		$$ = new Caja(cuenta++, $1,NULL,NULL);
		$$->tipo = unknown;
	}
	| NUM
	{
		$$ = new Caja(cuenta++,NULL,NULL,NULL);
		$$->tokenName = new string("*NULL*");
		$$->tokenValue = $1;
		$$->tipo = entero;
	}
	| MENOS NUM
	{
		int value = 0-$2;
		$$ = new Caja(cuenta++,NULL,NULL,NULL);
		$$->tokenName = new string("*NULL*");
		$$->tokenValue = value;
		$$->tipo = entero;
	}
	| arreglo
	{
		$$ = $1;
		$$->tipo = unknown;
	}
	| POINTER method_call
	{
		$$ = $2;
		$$->tipo = unknown;
	}
	;

arreglo:
	ID CORI tamano_arreglo CORD
	{
		$$ = new Caja(cuenta++, $1, NULL, NULL);
		$$->array = $3;
		arbol.agregarNodosDeListaATabla($3,$$);
	}
	;

instrucciones:
	assign PYC instrucciones {
		$$ = $1;
		$$->HD = $3;
		if($3 != nodoNulo)
			$3->HI = $$;
	}
	| SALIDA DOSP tipos PYC instrucciones {
		$$ = new Caja(cuenta++, $1,$5,NULL);
		$$->params = new list<Caja*>();
		$$->params->push_back($3);
		if($5 != nodoNulo)
			$5->HI = $$;
	}
	| rule_for instrucciones {
		$$ = $1;
		$$->HD = $2;
		if($2 != nodoNulo)
			$2->HI = $$;
	}
	| rule_ELSE instrucciones {
		$$ = $1;
		$$->HD->HD = $2;
		if($2 != nodoNulo)
			$2->HI = $$;
	}
	| rule_IF instrucciones {
		$$ = $1;
		$$->HD = $2;
		if($2 != nodoNulo)
			$2->HI = $$;
	}
	| rule_While instrucciones {
		$$ = $1;
		$$->HD = $2;
		if($2 != nodoNulo)
			$2->HI = $$;
	}
	| ENTRADA DOSP ID PYC instrucciones {
		$$ = new Caja(cuenta++, $1,$5,NULL);
		$$->params = new list<Caja*>();

		Caja* temp = new Caja( cuenta++, $3, NULL, NULL );
		$$->params->push_back(temp);
		if($5 != nodoNulo)
			$5->HI = $$;
	}
	| ret PYC instrucciones {
		$$ = $1;
		$$->HD = $3;
		if($3 != nodoNulo)
		{
			$3->HI = $$;
		}

		pila.push($1);	// Guardo en pila el ret.
	}
	| PYC instrucciones {
		$$ = $2;
	}
	| method_call PYC instrucciones {
		$$ = $1;
		$$->HD = $3;
		if($3 != nodoNulo)
			$3->HI = $$;
	}
	| ID PYC instrucciones
	{
		$$ = new Caja( cuenta++, $1, $3, NULL );
		$$->addToTable = true;
		$$->HD = $3;
		if($3 != nodoNulo)
			$3->HI = $$;
		}
	| arreglo PYC instrucciones{
		$$->addToTable = true;
		$$->HD = $3;
		if($3 != nodoNulo)
			$3->HI = $$;
		}
	|{$$ = nodoNulo;}
	;

method:
	INI method_call DOSP
	instrucciones FIN
	{
		$$ = $2;
		arbol.agregarNodosDeListaATabla($2->params, $$);
		$$->addToTable = true;
		if($4 != nodoNulo)
			{$$->HMI = $4;}
		for(Nodo tmp = $4; tmp != nodoNulo; tmp = tmp->HD){
				tmp->padre = $$;
				if(*tmp->tokenName == "*asignacion*"){
					arbol.agregarNodosDeListaATabla(tmp->asignacion, $$);
				}
				if(tmp->addToTable){
					arbol.agregarNodoATabla(tmp,$$);
				}
			}

			if( pila.empty() == true )	// Si está vacío, el método no tiene returns.
			{
				$2->tipo = t_void;						// Le agrego al método
			}
			else
			{
				Caja* tmp = pila.top();	// El que está en el tope.
				pila.pop();							// Me deshago de él.

				// Me detengo hasta encontrar un tipo conocido o vaciar la pila.
				while( tmp->tipo == unknown && pila.empty() == false )
				{
					tmp = pila.top();
					pila.pop();
				}
				$2->tipo = tmp->tipo;					// Le agrego al método, si  todos fueron unknown, ese agrego.
			}

			// Vacío la pila para los otros.
			while( pila.empty() == false )
			{
				pila.pop();
			}
	}
	;

method_call:
	ID method_arguments
	{
		$$ = new Caja(cuenta++,$1,NULL,NULL);
		$$->params = $2;
	}
	;

	method_arguments:
		PARI concat_IDs_operations PARD {$$ = $2;}
		| PARI PARD {$$ = NULL;}
		;

	concat_IDs_operations:
		tipos COM concat_IDs_operations
		{
			$$ = new list<Caja*>();
			$1->addToTable = true;
			$$->push_front($1);
			arbol.mergeLists($$,$3);
			delete $3;
		}
		| operaciones COM concat_IDs_operations
		{
			$$ = new list<Caja*>();
			arbol.mergeLists($3,$1);
			arbol.mergeLists($$,$3);
			delete $1;
			delete $3;
		}
		| tipos
		{
			$$ = new list<Caja*>();
			$1->addToTable = true;
			$$->push_back($1);
		}
		| operaciones
		{
			$$ = new list<Caja*>();
			arbol.mergeLists($$,$1);
		}
		;

comparaciones:
	comparacionesbooleanas AND_OR comparaciones {
		Caja* temp = new Caja( cuenta++, $2, NULL, NULL );
		$1->push_back(temp);

		arbol.mergeLists($1,$3);
		$$ = $1;
	}
	| comparacionesbooleanas {$$ = $1;}
	;

comparacionesbooleanas:		// Devuelve listas.
	tipos COMPARACION tipos
	{
		$$ = new list<Caja*>();

		$$->push_back($1);

		Caja* temp = new Caja( cuenta++, $2, NULL, NULL );
		$$->push_back(temp);

		$$->push_back($3);
	}
	| tipos COMPARACION operaciones
	{
		$$ = new list<Caja*>();

		$$->push_back($1);

		Caja* temp = new Caja( cuenta++, $2, NULL, NULL );
		$$->push_back(temp);

		arbol.mergeLists($$,$3);	// Ojo que operaciones devuelve una lista.
	}
	| operaciones COMPARACION tipos
	{
		$$ = $1;						// Obtengo la lista que devuele operaciones.

		Caja* temp = new Caja( cuenta++, $2, NULL, NULL );
		$$->push_back(temp);

		$1->push_back($3);
	}
	| operaciones COMPARACION operaciones
	{
		$$ = $1;

		Caja* temp = new Caja( cuenta++, $2, NULL, NULL );
		$$->push_back(temp);

		arbol.mergeLists($$, $3);
	}
	;

ret:
	RET tipos {
		$$ = new Caja(cuenta++,$1,NULL,NULL);
		$$->params = new list<Caja*>();
		$$->params->push_back($2);
		if(*$2->tokenName != "*NULL*"){arbol.agregarNodoATabla($2,$$);};
		$$->tipo = $2->tipo;
	}
	| RET operaciones {
		$$ = new Caja(cuenta++,$1,NULL,NULL);
		$$->operaciones = $2;
		arbol.agregarNodosDeListaATabla($2,$$);
		$$->tipo = entero;
	}
	| RET comparaciones {
		$$ = new Caja(cuenta++,$1,NULL,NULL);
		$$->comparacion = $2;
		arbol.agregarNodosDeListaATabla($2,$$);
		$$->tipo = booleano;
	}
	;

rule_IF:
	IF DOSP comparaciones PARI instrucciones PARD
	{
		$$ = new Caja(cuenta++,$1,NULL,NULL);
		$$->comparacion = $3;
		arbol.agregarNodosDeListaATabla($3,$$);
		$$->HMI = $5;
		for(Nodo tmp = $5; tmp != nodoNulo; tmp = tmp->HD){
			tmp->padre = $$;
			if(*tmp->tokenName == "*asignacion*"){
				arbol.agregarNodosDeListaATabla(tmp->asignacion, $$);
			}
			if(tmp->addToTable){
				arbol.agregarNodoATabla(tmp,$$);
			}
		}
	}
	;

rule_ELSE:
	rule_IF ELSE PARI instrucciones PARD
	{
		$$ = new Caja(cuenta++,$2,NULL,NULL);
		$$->HMI = $4;
		for(Nodo tmp = $4; tmp != nodoNulo; tmp = tmp->HD){
			tmp->padre = $$;
			if(*tmp->tokenName == "*asignacion*"){
				arbol.agregarNodosDeListaATabla(tmp->asignacion, $$);
			}
			if(tmp->addToTable){
				arbol.agregarNodoATabla(tmp,$$);
			}
		}
		$1->HD = $$;
		$$->HI = $1;
		$$ = $1;
	}
	;

	rule_for:
		FOR DOSP assign COM comparaciones COM operaciones PARI instrucciones PARD
		{
			$$ = new Caja(cuenta++,$1,NULL,NULL);
			$$->asignacion = $3->asignacion;
			arbol.agregarNodosDeListaATabla($3->asignacion,$$);
			arbol.agregarNodosDeListaATabla($5,$$);
			arbol.agregarNodosDeListaATabla($7,$$);
			$$->comparacion = $5;
			$$->operaciones = $7;
			$$->HMI = $9;
			for(Nodo tmp = $9; tmp != nodoNulo; tmp = tmp->HD){
				tmp->padre = $$;
				if(*tmp->tokenName == "*asignacion*"){
					arbol.agregarNodosDeListaATabla(tmp->asignacion, $$);
				}
				if(tmp->addToTable){
					arbol.agregarNodoATabla(tmp,$$);
				}
			}
		}
		;

rule_While:
	WHILE DOSP comparaciones PARI instrucciones PARD {
		$$ = new Caja(cuenta++,$1,NULL,NULL);
		$$->comparacion = $3;
		arbol.agregarNodosDeListaATabla($3,$$);
		$$->HMI = $5;
		for(Nodo tmp = $5; tmp != nodoNulo; tmp = tmp->HD){
			tmp->padre = $$;
			if(*tmp->tokenName == "*asignacion*"){
				arbol.agregarNodosDeListaATabla(tmp->asignacion, $$);
			}
			if(tmp->addToTable){
				arbol.agregarNodoATabla(tmp,$$);
			}
		}
	}
	;

tamano_arreglo:
	operando {
		$$ = new list<Caja*>();
		$$->push_back($1);
	}
	| operaciones
	{
		$$ = $1;
	}
	;

OPERACIONES:
	MAS {$$ = $1;}
	| MENOS {$$ = $1;}
	| POR {$$ = $1;}
	| ENTRE {$$ = $1;}
	| MOD {$$ = $1;}
	;

//This is where we end our suffering.
%%
int main(int argc, char** argv) {
	if(argc > 1){
		yyin = fopen(argv[0],"r");
	} else {
		yyin = stdin;
	}
	yyparse();
	return 0;
}

void printError(string errormsg, char tipo){
	extern int yylineno;
	cout<< errormsg<<" en la linea: "<<yylineno<<"\n";
	if(tipo == 'a'){
		printf("El error es: %s\n",yytext);
		exit(-1);
	}
}

void yyerror(const char *s) {
	extern int yylineno;
	printf("\n%s   , en la linea %d\n",s,yylineno);
	exit(-1);
}
