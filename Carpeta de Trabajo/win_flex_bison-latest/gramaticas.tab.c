/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "gramaticas.cpp" /* yacc.c:339  */

#include <stdio.h>
#include <string>
#include <stack>
#include <fstream>
#include "ArbolC++.h"
#include "Bitmap.h"

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

Bitmap bitmap;

std::ofstream codigo_mips("codigo.s");	// Creo el archivo para MIPS.

void recorrer_arbol();
void MIPS_variables_locales( Nodo nodo );
void MIPS_variables_globales(Nodo nodo);
void recorrer_arbolRec(Nodo n, int nivel);

std::string generar_etiqueta();
int MIPS_comparaciones_plantilla(list<Caja*>* lista_comparaciones);
void jmp_normal(std::string etiqueta);
void jmp_condicionado(int registro_1, int registro_2, Caja* jmp, std::string etiqueta);
void escribir_etiqueta(std::string etiqueta);
int obtener_operando(Caja* operando);
int MIPS_operaciones(list<Caja*>* operacion);

size_t generador_etiqueta = 0;

// Lo siguiente es lo necesario para el uso de variables locales:

int tamanno_total_locales = 0;

struct Nodo_Local
{
	std::string nombre;
	int acceso;	// Para saber cómo se accesa en la pila.

	Nodo_Local(std::string nombre_c)
	{
		nombre = nombre_c;
		acceso = tamanno_total_locales;	// OJO: se asume que no se ha sumado aún el tamano!!!!!
	}
};

std::list<Nodo_Local> lista_variables_locales;



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

FIN DE LOS METODOS DE BUSQUEDA DE EXISTENCIA EN LA TABLA Y ALCANCE E INICIO DE
LA GENERACIÓN DE CÓDIGO.
________________________________________________________________________________
*/


void generar_MIPS()
{
    if (!arbol.vacio())
    {
        recorrer_arbol();
    }
    codigo_mips.close();
}

// $s7 queda reservado para retornos.
std::string convertir_registro(int registro)
{
    switch( registro )
    {
        case 0:		return "$v0";
        case 1: 	return "$v1";
        case 2: 	return "$a0";
        case 3: 	return "$a1";
        case 4:		return "$a2";
        case 5: 	return "$a3";
        case 6: 	return "$t0";
        case 7: 	return "$t1";
        case 8: 	return "$t2";
        case 9: 	return "$t3";
        case 10: 	return "$t4";
        case 11: 	return "$t5";
        case 12: 	return "$t6";
        case 13: 	return "$t7";
        case 14: 	return "$s0";
        case 15: 	return "$s1";
        case 16: 	return "$s2";
        case 17: 	return "$s3";
        case 18: 	return "$s4";
        case 19: 	return "$s5";
        case 20: 	return "$s6";
        case 21: 	return "$t8";
        case 22: 	return "$t9";
        default:
				{
					std::cerr << "Error en convertir_registro: se está convitiendo algo que no existe." << std::endl;
					exit(-1);
				}
    }
}

// Falta.

string code = "";
int cuentaCiclo = 0;
int cuentaSalirCiclo = 0;
std::stack<int> cuentaStack;
void recorrer_arbolRec(Nodo nodo, int nivel)
{
		bool declaracion_metodo = false;
		int reg;
    if(nivel != 0)
    {													// La raíz no es nada del programa.
        if(nivel == 1)				// La carajada es global.
        {
            if(nodo->params != NULL)	// Es la definición del un método.
            {
								declaracion_metodo = true;
								codigo_mips << *nodo->tokenName << ": " << std::endl;

            }
            else				// Declaración global.
            {
                MIPS_variables_globales(nodo);
            }
        }
        else
        {
            if(*nodo->tokenName != "si" && *nodo->tokenName != "sino" &&
               *nodo->tokenName != "entrada" && *nodo->tokenName != "salida" &&
               *nodo->tokenName != "para" && *nodo->tokenName != "mientras" &&
               *nodo->tokenName != "ret" && nodo->params == 0x0)
            {
                // Solo entran las variables locales.
                MIPS_variables_locales(nodo);
            }
            else
            {
                // Aqui va los llamados de todo lo demás.
                if( *nodo->tokenName == "si" )
                {

                }
                else if( *nodo->tokenName == "sino" )
                {

                }
                else if( *nodo->tokenName == "para" || *nodo->tokenName == "mientras" )
                {
									if(*nodo->tokenName == "para"){
										MIPS_variables_locales(nodo);
										//^ hay que poner una condición para que trate distinto el for, ya que su nombre no es asignacion.
									}
									codigo_mips << "ciclo" << cuentaCiclo << ":\n";
									cuentaStack.push(cuentaCiclo);
									cuentaCiclo++;
                }
                else if( *nodo->tokenName == "ret" )
                {
									if(nodo->params != 0x0){
										if(*(*nodo->params->begin())->tokenName == "*NULL*"){
											codigo_mips << "li $s7,"<< (*nodo->params->begin())->tokenValue<<"\n";
											declaracion_metodo = true;
										} else {
											reg = obtener_operando(*nodo->params->begin());
										}
									} else if(nodo->comparacion != 0x0) {
										reg = MIPS_comparaciones_plantilla(nodo->comparacion);
									} else if(nodo->operaciones != 0x0){
										reg = MIPS_operaciones(nodo->operaciones);
									}
									if(!declaracion_metodo){
										codigo_mips<<"move $s7, "<< convertir_registro(reg) <<"\n";
										codigo_mips<<"jr $ra\n";
										declaracion_metodo = false;
									}
                }
								else
								{
									std::cerr << "Error en recorrer_arbolRec: no se reconoció instrucción." << std::endl;
								}
            }
        }
    }

    Nodo hijo = arbol.hijoMasIzq(nodo);
    while (hijo != nodoNulo)
    {
        recorrer_arbolRec(hijo, nivel+1);
        hijo = arbol.hermanoDer(hijo);
    }

		if(*nodo->tokenName == "para" || *nodo->tokenName == "mientras" ){//instrucción para salir del ciclo.
        if(*nodo->tokenName == "para"){
            reg = obtener_operando(*nodo->operaciones->begin());
						codigo_mips<<"addi "<< convertir_registro(reg)<<", 1\n";//asumo que todo incrementeo es solo de 1.
        }
        int registroRecibido = MIPS_comparaciones_plantilla(nodo->comparacion);//siempre contiene un zero o un uno (true o false).
        string comparison_registry =  convertir_registro(registroRecibido);
        codigo_mips << "beq " << comparison_registry <<", $zero, ciclo" << cuentaStack.top() <<"\n";
        bitmap.liberar_registro(registroRecibido);
        codigo_mips << "salir_ciclo" << cuentaStack.top() << ":\n";
				cuentaStack.pop();
    }

		if( declaracion_metodo == true )
		{
			// Al terminar el método, limpio todo lo que tenga que ver con variables locales.
			lista_variables_locales.clear();
			tamanno_total_locales = 0;

			// Necesito escribir algo de retorno.
		}
}

void recorrer_arbol()
{
    if(!arbol.vacio())
    {
        recorrer_arbolRec(arbol.raiz(), 0);
    }
		// Para terminar ejecución.
		codigo_mips << "li $v0, 10" << std::endl;
		codigo_mips << "syscall" << std::endl;
}


/*
─────────────────────────────────────────────────────────────────────────────────────────────────
																			Método MIPS
─────────────────────────────────────────────────────────────────────────────────────────────────
*/

// Antes de este método tuve que haber escrito .data
void MIPS_variables_globales(Nodo nodo)
{
    // Es medio ineficiente, pero es lo que hay.
    codigo_mips << ".data" << std::endl;

    int tamano_arreglo = -1;

    std::string tipo_variable;

    if( nodo->tipo == entero || nodo->tipo == unknown)
    {
        tipo_variable = ".word";
    }
    else if( nodo->tipo == booleano || nodo->tipo == t_void )
    {
        tipo_variable = ".byte";
    }
    else if( nodo->tipo == hilera )
    {
        tipo_variable = ".byte";	// Se asume que todas son .byte :3
    }

    // Asigno valores:
    if( nodo->asignacion != NULL )	// Variable con asignación.
    {
        // Escribo el nombre del primer elemento de la lista que pertenece a nodo.
        codigo_mips << *(*(nodo->asignacion)->begin())->tokenName << ":\t" << tipo_variable << ": ";

        // Aquí hago algo magnífico para escribir el valor que se le va a asignar a la variable.
    }
    else
    {
        // Escribo el nombre de la variable sin asignación.  Por defecto, queda en 0.
        codigo_mips << *(nodo->tokenName) << ":\t" << tipo_variable << ": 0" << std::endl;
    }

    codigo_mips << std::endl;
    codigo_mips << ".text" << std::endl;	// Luego de la declaración, continúo con el código.
}

void MIPS_variables_locales( Nodo nodo )
{
		bool repetida = false;
    int tamanno_variable = 0;	// Para saber cuánto

		Nodo_Local nodo_local = Nodo_Local(*(nodo->tokenName));

		// Antes verifico si está o no.
		for(std::list<Nodo_Local>::iterator it=lista_variables_locales.begin(); it != lista_variables_locales.end(); ++it)
		{
			if( (*it).nombre == nodo_local.nombre )
			{
				repetida = true;
			}
		}

		if( repetida == false )	// Si es una declaración de la variable local.
		{
			if( nodo->tipo == entero || nodo->tipo == unknown)
			{
					tamanno_variable = 4;
			}
			else if( nodo->tipo == booleano || nodo->tipo == t_void )
			{
					tamanno_variable = 1;
			}

			// Reservo la memoria en la pila.
			codigo_mips << "sub $sp, $sp, " + tamanno_variable << std::endl;

			// Lo guardo en la lista.
			lista_variables_locales.push_back(nodo_local);
			tamanno_total_locales += tamanno_variable;
		}

		// Hay que verificar si la variable local tiene una asignación (sea nueva o usada)
		if( nodo->asignacion != NULL )
		{
			// sw $ra, 4($sp)
			int registro = MIPS_operaciones(nodo->asignacion);

			// Guardo en la variable local (en la pila).
			codigo_mips << "sw " + convertir_registro(registro) << ", " + nodo_local.acceso << "($sp)" << std::endl;

			bitmap.liberar_registro(registro);
		}
}

/*
 * Formato de la lista: operando comparacion operando (AND/OR opcionales si es cadena de comparaciones).
 * Por ejemplo: 5 == x
 * *iterador = '5'
 * *(++iterador) = '=='
 * *(++iterador) = 'x'
 *
 * Retorna el número de registro de retorno.
 * IMPORTANTE: es deber del usuario liberar el registro que se retorna.
 */
 int MIPS_comparaciones_plantilla(list<Caja*>* lista_comparaciones)
 {
     std::list<Caja*>::iterator it_operando1 = lista_comparaciones->begin();
     std::list<Caja*>::iterator it_comparacion = it_operando1;
 		it_comparacion++;
     std::list<Caja*>::iterator it_operando2 = it_comparacion;
 		it_operando2++;
     std::list<Caja*>::iterator it_pivote = it_operando2;
 		it_pivote++;

     std::string etiqueta_true;
     std::string etiqueta_false; // SU propósito es para cuando no se cumpla la condición.

     bool primero = true;
     bool or_variable;

     int registro_1;
     int registro_2;
     int registro_retorno = bitmap.ocupar_registro();

     // Con la lista me muevo de 3 en 3 por cada comparación.
     do
     {
         // Para entender todo esto, ver los ejemplos del cuaderno.

         etiqueta_true = generar_etiqueta();
         etiqueta_false = generar_etiqueta();

 				cout<<*(*it_operando1)->tokenName<<"\n";
         registro_1 = obtener_operando( *it_operando1 );  	// Primer operando.
 				cout<<*(*it_operando2)->tokenName<<"\n";
         registro_2 = obtener_operando( *it_operando2 );		// Segundo operando.

         // Escribo el jump con compare hacia el true.
         jmp_condicionado(registro_1, registro_2, *it_comparacion, etiqueta_true);

         if( primero == true ) // La primera vez no tengo por qué hacer AND/OR.
         {
             // Esto se escribe por si es false.

             // 0 en el registro de retorno.										// false
             codigo_mips << "li " << convertir_registro(registro_retorno) << ", 0" << std::endl;
             jmp_normal(etiqueta_false);                     	// Jump hacia el false.

             escribir_etiqueta(etiqueta_true);                 // AQUÍ LLEGARÍA EL TRUE v.1
             //0 en el registro de retorno                   	// true.
             codigo_mips << "li " << convertir_registro(registro_retorno) << ", 1" << std::endl;
             primero = false;
         }
         else // Hago AND u OR al registro EAX.
         {
             if( or_variable == true )
             {
                 // ESCRIBO OR de registro_retorno con 0.
                 codigo_mips << "ori " <<
                             convertir_registro(registro_retorno) << ", "
                             << convertir_registro(registro_retorno) << ", 0" << std::endl;

                 jmp_normal(etiqueta_false);

                 escribir_etiqueta(etiqueta_true);             // AQUÍ LLEGARÍA EL TRUE v.2
                 // ESCRIBO OR del registro de retorno con 1.

                 codigo_mips << "ori " <<
                             convertir_registro(registro_retorno) << ", "
                             << convertir_registro(registro_retorno) << ", 1" << std::endl;
             }
             else  // AND
             {
                 // ESCRIBO AND de registro_retorno con 0.
                 codigo_mips << "andi " <<
                             convertir_registro(registro_retorno) << ", "
                             << convertir_registro(registro_retorno) << ", 0" << std::endl;

                 jmp_normal(etiqueta_false);                   // AQUÍ LLEGARÍA EL TRUE v.3

                 escribir_etiqueta(etiqueta_true);

                 // ESCRIBO AND de registro_retorno con 1.
                 codigo_mips << "andi " <<
                             convertir_registro(registro_retorno) << ", "
                             << convertir_registro(registro_retorno) << ", 1" << std::endl;
             }
         }
         escribir_etiqueta(etiqueta_false);                  // AQUÍ LLEGARÍA EL FALSE.

         // Si hay AND/OR, continúo, si no, termino.
         if(it_pivote != lista_comparaciones->end())
         {
             // Reconozco por medio del bool, cuándo es OR o AND.
             if( (*it_pivote)->tokenName->compare("||") )
             {
                 or_variable = true;
             }
             else
             {
                 or_variable = false;
             }
 						it_operando1 = it_pivote;
             it_operando1++;
             it_comparacion = it_operando1;
 						it_comparacion++;
             it_operando2 = it_comparacion;
 						it_operando2++;
             it_pivote = it_operando2;
 						it_pivote++;
             // Libero los registros ocupados.
             bitmap.liberar_registro(registro_1);
             bitmap.liberar_registro(registro_2);
         }
     }while( it_pivote != lista_comparaciones->end() );

     return registro_retorno;
 }


std::string generar_etiqueta()
{
    return "et" + generador_etiqueta++;
}

void jmp_normal(std::string etiqueta)
{
    codigo_mips << "j " << etiqueta << std::endl;
}

// Con el parámetro jmp se decide qué tipo de jump hacer.
void jmp_condicionado(int registro_1, int registro_2, Caja* jmp, std::string etiqueta)
{
    std::string branch_condicional;
    bool mayor_menor = false;

    // Con los mayor y menor hay que hacer una cuestión distinta.
    if( *(jmp->tokenName) == "==" )
    {
        branch_condicional = "beq";
    }
    else if( *(jmp->tokenName) == "!=" )
    {
        branch_condicional = "bne";
    }
    else if( *(jmp->tokenName) == "<=" )
    {
        branch_condicional = "ble";
    }
    else if( *(jmp->tokenName) == ">=" )
    {
        branch_condicional = "bge";
    }
    else if( *(jmp->tokenName) == "<" || *(jmp->tokenName) == ">" )
    {
        branch_condicional = "beq";
        mayor_menor = true;
    }
    else
    {
        std::cerr << "Error en el jmp condicional: no se reconoció cuál jmp hacer." << std::endl;
        exit(-1);
    }


    if( mayor_menor == false )
    {
        codigo_mips << branch_condicional
                    << " "  << convertir_registro(registro_1)
                    << ", " << convertir_registro(registro_2)
                    << ", " << etiqueta << std::endl;
    }
    else
    {
        // Hay que hacer un testeo, en el drive está el link con la explicación.
    }
}

void escribir_etiqueta(std::string etiqueta)
{
    codigo_mips << etiqueta << ": " << std::endl;
}

int MIPS_operaciones(list<Caja*>* operacion)
{
	std::string instruccion = "";
	int registro_retorno;
if(operacion->size() != 1){

		list<Caja*>::iterator it = operacion->begin();
		list<Caja*>::iterator operador = it;

		operador++;


		bool modulo = false;	// el módulo se obtiene diferente, es un caso especial.

		registro_retorno = obtener_operando(*it);	// Obtengo de una vez el primer operando.
		int registro_operacion = -1;

		do
		{
				if(*(*operador)->tokenName == "+")
				{
						instruccion = "add";
				}
				else if(*(*operador)->tokenName == "-")
				{
						instruccion = "sub";
				}
				else if(*(*operador)->tokenName == "*")
				{
						instruccion = "mul";
				}
				else if(*(*operador)->tokenName == "/")
				{
						instruccion = "div";
				}
				else if(*(*operador)->tokenName == "%")
				{
						instruccion = "div";
						modulo = true;
				}
				else
				{
						std::cerr << "Error en MIPS_operaciones: no se reconoce el operador." << std::endl;
						exit(-1);
				}
				// Siempre se busca hacer una operacion con 3 operandos (MIPS).

				instruccion += " " + convertir_registro(registro_retorno) + ", " + convertir_registro(registro_retorno);

				it = operador;
				it++;

				registro_operacion = obtener_operando(*it);		// Obtengo el otro operando.
				instruccion += ", " + convertir_registro(registro_operacion);	// La operación se guarda en registro_retorno.
				bitmap.liberar_registro(registro_operacion);	// Libero el registro del operando.

				// NOTA: no importa si es la primera iteración o no, siempre se buscar hacer la operación
				// con 3 operandos.  Note que el primer registro(retorno) sirve para almacenar, pero
				// ese mismo registro_retorno sirve también para hacer la operación con registro_operacion

				codigo_mips << instruccion << std::endl;	// Finalmente, escribo la instruccion.

				if( modulo == true )
				{
						// Al haber módulo, el resultado se guarda en $hi al hacer un div.
						codigo_mips << "move " + convertir_registro(registro_retorno) + ", $hi" << std::endl;
						modulo = false;
				}

				operador = it;
				operador++;
		} while( operador != operacion->end() ) ;
} else {
	registro_retorno = obtener_operando(*operacion->begin());
}
	return registro_retorno;
}

/*
 * Este método lo que hace es convertir el operando en MIPS, así sea un immediato,
 * una variable local, global, etc. sea lo que sea, lo devuelve en un registro.
 * Por lo que se va a escribir código en MIPS cuando sea necesario.
 * Es deber del usuario liberar el registro.
 */
 int obtener_operando(Caja* operando)
 {
  	int registro = bitmap.ocupar_registro();

 		if(operando->where != 0x0)
		{
 			if(operando->where->scope == arbol.raiz())
			{
 				if(operando->params != 0x0)
				{
					//método
					codigo_mips << "jal " + *(operando->tokenName) << std::endl;
					// Muevo lo obtenido al registro que reservé.
					codigo_mips << "move " + convertir_registro(registro) + "$s7" << std::endl;
 				}
				else
				{
					// Variable global

					// Por el momento, se almacena como si todo fuera word.  Si hay tiempo, arreglo.

					codigo_mips << "sw " + convertir_registro(registro) << ", " + *(operando->tokenName) << std::endl;
 				}
 			}
			else
			{
				// variable local.
				Nodo_Local nodo_local2 = Nodo_Local("NULO");

				for(std::list<Nodo_Local>::iterator it=lista_variables_locales.begin(); it != lista_variables_locales.end(); ++it)
				{
					if( (*it).nombre == *(operando->tokenName) )
					{
						// Encontré la variable local :)
						nodo_local2 = *it;
					}
				}

				if( nodo_local2.nombre == "NULO" )
				{
					std::cerr << "Error en obtener_operando: no se encuentra la variable local." << std::endl;
					exit(-1);
				}

				// lw $ra, 4($sp) es un ejemplo.
				// Por el momento, cargo words nada más.
				codigo_mips << "lw " + convertir_registro(registro) << ", " + nodo_local2.acceso << "($sp)" << std::endl;
 			}
 		}
		else if(*operando->tokenName == "NULL")
		{
			//Inmediato
			codigo_mips << "li " + convertir_registro(registro) + ", " << operando->tokenValue << std::endl;
 		}
		else
		{
			std::cerr << "Error en obtener_operando: no se reconoce el operando." << std::endl;
		}
     return registro;
 }


/*______________________________________________________________________________

FIN DE LOS METODOS DE LA GENERACIÓN DE CÓDIGO.
________________________________________________________________________________
*/

#line 1296 "gramaticas.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "gramaticas.tab.h".  */
#ifndef YY_YY_GRAMATICAS_TAB_H_INCLUDED
# define YY_YY_GRAMATICAS_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1232 "gramaticas.cpp" /* yacc.c:355  */

	#include <list>
	#include <string>
	using namespace std;

#line 1332 "gramaticas.tab.c" /* yacc.c:355  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    HILERA = 258,
    FOR = 259,
    WHILE = 260,
    IF = 261,
    ELSE = 262,
    COMPARACION = 263,
    AND_OR = 264,
    BOOL = 265,
    NUM = 266,
    INI = 267,
    FIN = 268,
    RET = 269,
    MAS = 270,
    MENOS = 271,
    POR = 272,
    ENTRE = 273,
    MOD = 274,
    ENTRADA = 275,
    SALIDA = 276,
    ID = 277,
    ERROR = 278,
    EJECUCION = 279,
    PARD = 280,
    CORD = 281,
    PARI = 282,
    CORI = 283,
    IGUAL = 284,
    DOSP = 285,
    COM = 286,
    PUNTO = 287,
    POINTER = 288,
    PYC = 289,
    LINK = 290,
    JAAH = 291
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 1237 "gramaticas.cpp" /* yacc.c:355  */

	string* hilera;
	int intVal;
	struct Caja* nodo;
	struct Simbolos* simbolo;
	std::list<Caja*>* params;

#line 1389 "gramaticas.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GRAMATICAS_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 1406 "gramaticas.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   209

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  69
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  151

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   291

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1308,  1308,  1343,  1344,  1348,  1365,  1382,  1390,  1399,
    1407,  1411,  1426,  1440,  1451,  1461,  1477,  1478,  1482,  1494,
    1513,  1514,  1519,  1527,  1532,  1539,  1547,  1552,  1560,  1569,
    1575,  1582,  1588,  1594,  1600,  1606,  1615,  1625,  1628,  1634,
    1642,  1648,  1652,  1697,  1705,  1706,  1710,  1718,  1726,  1732,
    1740,  1747,  1751,  1762,  1773,  1782,  1794,  1801,  1807,  1816,
    1835,  1855,  1878,  1896,  1900,  1907,  1908,  1909,  1910,  1911
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"Hilera\"", "\"para\"", "\"mientras\"",
  "\"si\"", "\"sino\"", "\"comparador\"", "\"&& o ||\"",
  "\"verdadero o falso\"", "\"numero\"", "\"ini\"", "\"fin\"", "\"ret\"",
  "\"+\"", "\"-\"", "\"*\"", "\"/\"", "\"%\"", "\"entrada\"", "\"salida\"",
  "\"identificador\"", "ERROR", "\"principal\"", "\")\"", "\"]\"", "\"(\"",
  "\"[\"", "\"=\"", "\":\"", "\",\"", "\".\"", "\"->\"", "\";\"",
  "\"enlazador de archivos\"", "\"ruta del archivo\"", "$accept", "super",
  "inicio", "principal", "declarations", "assign", "concat_links",
  "operaciones", "tipos", "operando", "arreglo", "instrucciones", "method",
  "method_call", "method_arguments", "concat_IDs_operations",
  "comparaciones", "comparacionesbooleanas", "ret", "rule_IF", "rule_ELSE",
  "rule_for", "rule_While", "tamano_arreglo", "OPERACIONES", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291
};
# endif

#define YYPACT_NINF -80

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-80)))

#define YYTABLE_NINF -11

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      20,   145,   -21,    32,   -80,   -80,    26,    25,    41,    40,
      90,   139,    41,    31,   -80,    75,    -1,    41,   -80,   159,
      41,   -80,    72,    74,    75,   128,   128,   -80,    79,   -80,
     -80,    45,   128,   -80,   128,   -80,   -80,   -80,    80,    91,
     -80,   -80,   128,   -80,   -80,   -80,   -80,   -80,   -80,   -80,
     -80,   -80,    90,    90,   -80,    78,   -80,    30,   -80,   -80,
     -80,    12,    93,    85,    96,   100,   101,   159,   103,   123,
      86,    30,   124,    53,   150,   137,   143,    19,    30,    30,
      30,   159,   159,   -80,   156,   159,   159,   182,   172,   -80,
     173,   161,   159,    30,   -80,    30,    30,   -80,    30,    30,
     157,   -80,   -80,   -80,   -80,   -80,   -80,    62,   154,   158,
     182,   172,   164,   166,   159,   159,   159,   160,   168,   -80,
     -80,   -80,   -80,   -80,   -80,    30,   159,    30,    30,   128,
     -80,   128,   -80,   -80,    30,    30,   178,   174,   179,   181,
     -80,   -80,   -80,    90,   -80,   -80,   149,   128,    30,   183,
     -80
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      10,     0,     0,     0,     2,     4,     0,     0,    10,     0,
       0,     0,    10,    17,     1,     0,     6,    10,     3,     0,
      10,    24,     0,    23,     0,    64,    63,    26,     0,    22,
      21,    23,    12,    11,    20,    15,     8,    16,     0,     0,
       5,     7,    14,    13,     9,    25,    27,    65,    66,    67,
      68,    69,     0,     0,    28,     0,    43,    41,    18,    19,
      45,    49,    48,     0,     0,     0,     0,     0,     0,     0,
       0,    41,     0,     0,     0,     0,     0,    41,    41,    41,
      41,     0,     0,    44,     0,     0,     0,    57,    56,    58,
      51,     0,     0,    41,    37,    41,    41,    42,    41,    41,
       0,    33,    32,    31,    34,    47,    46,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    20,
      39,    29,    40,    38,    36,    41,     0,    41,    41,    55,
      54,    53,    52,    50,    41,    41,     0,     0,     0,     0,
      35,    30,    60,     0,    62,    59,     0,     0,    41,     0,
      61
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -80,   -80,   -80,    54,    48,     2,   194,    -6,    37,    -7,
       0,    61,   -80,    43,   -80,    35,   -79,   -80,   -80,   -80,
     -80,   -80,   -80,   -80,   -25
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    72,     8,   110,   111,    34,
      27,    74,    16,    75,    56,    63,    89,    90,    76,    77,
      78,    79,    80,    28,    52
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       9,    53,     7,    26,    25,    32,   112,   113,     9,    53,
       7,   -10,     9,    42,     7,    13,     9,     9,     7,     7,
       9,     1,     7,    64,    65,    66,   100,    47,    48,    49,
      50,    51,    14,    67,    64,    65,    66,   133,    15,    68,
      69,    70,     1,    81,    67,    58,    59,   137,    33,    61,
      68,    69,    70,    71,    35,     2,    43,    73,    39,    17,
      36,    87,    18,     1,    71,    41,     2,    46,    44,    19,
      40,    73,    55,    10,    20,    61,    61,    73,    73,    73,
      73,    29,    19,    45,   109,   119,   108,    96,    30,    21,
      10,    11,    62,    73,    22,    73,    73,    38,    73,    73,
      23,    21,    10,    60,    88,    54,    22,    55,   129,   131,
      83,    24,    23,    55,    10,    11,   105,   106,    62,    62,
      93,    57,    53,    24,    82,    73,    84,    73,    73,   118,
      85,    86,    94,    91,    73,    73,   147,   146,   101,   102,
     103,   104,    29,    47,    48,    49,    50,    51,    73,    30,
      21,   130,   132,    92,   120,    22,   121,   122,    95,   123,
     124,    31,    29,    97,    47,    48,    49,    50,    51,    30,
      21,    98,    24,    10,    11,    22,   148,    99,   107,    12,
     115,    23,   116,   117,   125,   126,   136,    19,   138,   139,
     114,   127,    24,   128,   134,   140,   141,    47,    48,    49,
      50,    51,   135,   142,   144,   143,   145,    37,   150,   149
};

static const yytype_uint8 yycheck[] =
{
       0,    26,     0,    10,    10,    11,    85,    86,     8,    34,
       8,    12,    12,    19,    12,    36,    16,    17,    16,    17,
      20,    22,    20,     4,     5,     6,     7,    15,    16,    17,
      18,    19,     0,    14,     4,     5,     6,   116,    12,    20,
      21,    22,    22,    31,    14,    52,    53,   126,    11,    55,
      20,    21,    22,    34,    11,    35,    19,    57,    15,    34,
      12,    67,     8,    22,    34,    17,    35,    24,    20,    29,
      16,    71,    27,    28,    34,    81,    82,    77,    78,    79,
      80,     3,    29,    11,    84,    92,    84,    34,    10,    11,
      28,    29,    55,    93,    16,    95,    96,    22,    98,    99,
      22,    11,    28,    25,    67,    26,    16,    27,   114,   115,
      25,    33,    22,    27,    28,    29,    81,    82,    81,    82,
      34,    30,   147,    33,    31,   125,    30,   127,   128,    92,
      30,    30,    71,    30,   134,   135,   143,   143,    77,    78,
      79,    80,     3,    15,    16,    17,    18,    19,   148,    10,
      11,   114,   115,    30,    93,    16,    95,    96,    34,    98,
      99,    22,     3,    13,    15,    16,    17,    18,    19,    10,
      11,    34,    33,    28,    29,    16,    27,    34,    22,    34,
       8,    22,     9,    22,    27,    31,   125,    29,   127,   128,
       8,    27,    33,    27,    34,   134,   135,    15,    16,    17,
      18,    19,    34,    25,    25,    31,    25,    13,    25,   148
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    22,    35,    38,    39,    40,    41,    42,    43,    47,
      28,    29,    34,    36,     0,    12,    49,    34,    40,    29,
      34,    11,    16,    22,    33,    44,    46,    47,    60,     3,
      10,    22,    44,    45,    46,    50,    41,    43,    22,    50,
      40,    41,    44,    45,    41,    11,    50,    15,    16,    17,
      18,    19,    61,    61,    26,    27,    51,    30,    46,    46,
      25,    44,    45,    52,     4,     5,     6,    14,    20,    21,
      22,    34,    42,    47,    48,    50,    55,    56,    57,    58,
      59,    31,    31,    25,    30,    30,    30,    44,    45,    53,
      54,    30,    30,    34,    48,    34,    34,    13,    34,    34,
       7,    48,    48,    48,    48,    52,    52,    22,    42,    47,
      44,    45,    53,    53,     8,     8,     9,    22,    45,    46,
      48,    48,    48,    48,    48,    27,    31,    27,    27,    44,
      45,    44,    45,    53,    34,    34,    48,    53,    48,    48,
      48,    48,    25,    31,    25,    25,    44,    46,    27,    48,
      25
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    37,    38,    39,    39,    40,    40,    41,    41,    41,
      41,    42,    42,    42,    42,    42,    43,    43,    44,    44,
      45,    45,    45,    46,    46,    46,    46,    46,    47,    48,
      48,    48,    48,    48,    48,    48,    48,    48,    48,    48,
      48,    48,    49,    50,    51,    51,    52,    52,    52,    52,
      53,    53,    54,    54,    54,    54,    55,    55,    55,    56,
      57,    58,    59,    60,    60,    61,    61,    61,    61,    61
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     3,     2,     3,     3,     3,
       0,     3,     3,     3,     3,     3,     3,     2,     3,     3,
       1,     1,     1,     1,     1,     2,     1,     2,     4,     3,
       5,     2,     2,     2,     2,     5,     3,     2,     3,     3,
       3,     0,     5,     2,     3,     2,     3,     3,     1,     1,
       3,     1,     3,     3,     3,     3,     2,     2,     2,     6,
       5,    10,     6,     1,     1,     1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 1309 "gramaticas.cpp" /* yacc.c:1646  */
    {
	  string* root = new std::string("Programa:");
		arbol.ponerRaiz(cuenta,root);
		cuenta++;
		Nodo raiz = arbol.raiz();

		arbol.agregarHijo(arbol.raiz(),(yyvsp[0].nodo));
		for(Nodo tmp = (yyvsp[0].nodo); tmp != nodoNulo; tmp = tmp->HD)
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

		// Aquí inicia lo chido.
		generar_MIPS();	// El método se encuentra en MIPS.h
	}
#line 2625 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 1343 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.nodo) = (yyvsp[0].nodo);}
#line 2631 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 1344 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.nodo) = (yyvsp[0].nodo);}
#line 2637 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 1349 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyvsp[-1].nodo)->HD = (yyvsp[0].nodo);
		(yyvsp[0].nodo)->HI = (yyvsp[-1].nodo);
		if((yyvsp[-2].nodo) != nodoNulo){
			Nodo i;
			for(i = (yyvsp[-2].nodo); i->HD != nodoNulo; i = i->HD){
				//Iterates through the declaration Nodes.
				//'til it finds the last one different to null.
			}
			(yyval.nodo) = (yyvsp[-2].nodo);
			i->HD = (yyvsp[-1].nodo);
			(yyvsp[-1].nodo)->HI = (yyval.nodo);
		} else {
			(yyval.nodo) = (yyvsp[-1].nodo);
		}
	}
#line 2658 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 1366 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[0].nodo);
		if((yyvsp[-1].nodo) != nodoNulo){
			Nodo i;
			for(i = (yyvsp[-1].nodo); i->HD != nodoNulo; i = i->HD){
				//Iterates through the declaration Nodes.
				//'til it finds the last one different to null.
			}
			i->HD = (yyval.nodo);
			(yyval.nodo)->HI = (yyvsp[-1].nodo);
			(yyval.nodo) = (yyvsp[-1].nodo);
		}
	}
#line 2676 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 1383 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-2].nodo);
		if((yyvsp[0].nodo) != nodoNulo){
			(yyvsp[-2].nodo)->HD = (yyvsp[0].nodo);
			(yyvsp[0].nodo)->HI = (yyval.nodo);
		}
	}
#line 2688 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 1391 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[-2].hilera),NULL,NULL);
		(yyval.nodo)->addToTable = true;
		if((yyvsp[0].nodo) != nodoNulo){
			(yyval.nodo)->HD = (yyvsp[0].nodo);
			(yyvsp[0].nodo)->HI = (yyval.nodo);
		}
	}
#line 2701 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 1400 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo)->addToTable = true;
		if((yyvsp[0].nodo) != nodoNulo){
			(yyval.nodo)->HD = (yyvsp[0].nodo);
			(yyvsp[0].nodo)->HI = (yyval.nodo);
		}
	}
#line 2713 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 1407 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.nodo) = nodoNulo;}
#line 2719 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 1412 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++, NULL, NULL, NULL);
		(yyval.nodo)->tokenName = new string("*asignacion*");
		Caja* temp = new Caja( cuenta++, (yyvsp[-1].hilera), NULL, NULL );
		(yyval.nodo)->asignacion = new list<Caja*>();
		(yyval.nodo)->asignacion->push_front(temp);
		temp = new Caja( cuenta++, (yyvsp[-2].hilera), NULL, NULL );
		temp->addToTable = true;
		temp->tipo = (yyvsp[0].nodo)->tipo;
		(yyval.nodo)->tipo = (yyvsp[0].nodo)->tipo;
		(yyval.nodo)->asignacion->push_front(temp);
		(yyval.nodo)->asignacion->push_back((yyvsp[0].nodo));
		// tipos se agrega desde su producción.
		}
#line 2738 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 1427 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++, NULL, NULL, NULL);
		Caja* temp = new Caja( cuenta++, (yyvsp[-1].hilera), NULL, NULL );
		(yyval.nodo)->tokenName = new string("*asignacion*");
		(yyval.nodo)->asignacion = (yyvsp[0].params);
		(yyval.nodo)->asignacion->push_front(temp);
		temp = new Caja( cuenta++, (yyvsp[-2].hilera), NULL, NULL );
		temp->addToTable = true;
		temp->tipo = entero;
		list<Caja*>::iterator it = (yyvsp[0].params)->begin();
		(yyval.nodo)->asignacion->push_front(temp);
		(yyval.nodo)->tipo = entero;
	}
#line 2756 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 1441 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-2].nodo);
		(yyval.nodo)->addToTable = true;
		Caja* temp = new Caja( cuenta++, (yyvsp[-1].hilera), NULL, NULL );
		(yyval.nodo)->tipo = (yyvsp[0].nodo)->tipo;
		(yyval.nodo)->asignacion = new list<Caja*>();
		(yyval.nodo)->asignacion->push_front(temp);
		(yyval.nodo)->asignacion->push_back((yyvsp[0].nodo));

	}
#line 2771 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 1452 "gramaticas.cpp" /* yacc.c:1646  */
    {
		list<Caja*>::iterator it = (yyvsp[0].params)->begin();
		(yyval.nodo) = (yyvsp[-2].nodo);
		(yyvsp[-2].nodo)->addToTable = true;
		Caja* temp = new Caja( cuenta++, (yyvsp[-1].hilera), NULL, NULL );
		(yyval.nodo)->asignacion->push_front(temp);
		(yyval.nodo)->asignacion = (yyvsp[0].params);
		(yyval.nodo)->tipo = entero;
	}
#line 2785 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 1462 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++, NULL, NULL, NULL);
		Caja* temp = new Caja( cuenta++, (yyvsp[-1].hilera), NULL, NULL );
		(yyval.nodo)->tokenName = new string("*asignacion*");
		(yyval.nodo)->asignacion = new list<Caja*>();
		(yyval.nodo)->asignacion->push_back((yyvsp[0].nodo));
		(yyval.nodo)->asignacion->push_front(temp);
		temp = new Caja( cuenta++, (yyvsp[-2].hilera), NULL, NULL );
		temp->addToTable = true;
		temp->tipo = (yyvsp[0].nodo)->tipo;
		(yyval.nodo)->asignacion->push_front(temp);
	}
#line 2802 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 1483 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.params) = (yyvsp[-2].params);

		Caja* temp = new Caja( cuenta++, (yyvsp[-1].hilera), NULL, NULL );
		(yyval.params)->push_back(temp);

		(yyval.params)->push_back((yyvsp[0].nodo));
		if((yyvsp[0].nodo)->tipo == unknown){
			(yyvsp[0].nodo)->tipo = entero;
		}
	}
#line 2818 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 1494 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.params) = new list<Caja*>();

		(yyval.params)->push_back((yyvsp[-2].nodo));

		Caja* temp = new Caja( cuenta++, (yyvsp[-1].hilera), NULL, NULL );
		(yyval.params)->push_back(temp);

		(yyval.params)->push_back((yyvsp[0].nodo));
		if((yyvsp[0].nodo)->tipo == unknown){
			(yyvsp[0].nodo)->tipo = entero;
		}
		if((yyvsp[-2].nodo)->tipo == unknown){
			(yyvsp[-2].nodo)->tipo = entero;
		}
	}
#line 2839 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 1513 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.nodo) = (yyvsp[0].nodo);}
#line 2845 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 1515 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++, (yyvsp[0].hilera),NULL,NULL);
		(yyval.nodo)->tipo = booleano;
	}
#line 2854 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 1520 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++, (yyvsp[0].hilera),NULL,NULL);
		(yyval.nodo)->tipo = hilera;
	}
#line 2863 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 1528 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++, (yyvsp[0].hilera),NULL,NULL);
		(yyval.nodo)->tipo = unknown;
	}
#line 2872 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 1533 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++,NULL,NULL,NULL);
		(yyval.nodo)->tokenName = new string("*NULL*");
		(yyval.nodo)->tokenValue = (yyvsp[0].intVal);
		(yyval.nodo)->tipo = entero;
	}
#line 2883 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 1540 "gramaticas.cpp" /* yacc.c:1646  */
    {
		int value = 0-(yyvsp[0].intVal);
		(yyval.nodo) = new Caja(cuenta++,NULL,NULL,NULL);
		(yyval.nodo)->tokenName = new string("*NULL*");
		(yyval.nodo)->tokenValue = value;
		(yyval.nodo)->tipo = entero;
	}
#line 2895 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 1548 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[0].nodo);
		(yyval.nodo)->tipo = unknown;
	}
#line 2904 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 1553 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[0].nodo);
		(yyval.nodo)->tipo = unknown;
	}
#line 2913 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 1561 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++, (yyvsp[-3].hilera), NULL, NULL);
		(yyval.nodo)->array = (yyvsp[-1].params);
		arbol.agregarNodosDeListaATabla((yyvsp[-1].params),(yyval.nodo));
	}
#line 2923 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 1569 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-2].nodo);
		(yyval.nodo)->HD = (yyvsp[0].nodo);
		if((yyvsp[0].nodo) != nodoNulo)
			(yyvsp[0].nodo)->HI = (yyval.nodo);
	}
#line 2934 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 1575 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++, (yyvsp[-4].hilera),(yyvsp[0].nodo),NULL);
		(yyval.nodo)->params = new list<Caja*>();
		(yyval.nodo)->params->push_back((yyvsp[-2].nodo));
		if((yyvsp[0].nodo) != nodoNulo)
			(yyvsp[0].nodo)->HI = (yyval.nodo);
	}
#line 2946 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 1582 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-1].nodo);
		(yyval.nodo)->HD = (yyvsp[0].nodo);
		if((yyvsp[0].nodo) != nodoNulo)
			(yyvsp[0].nodo)->HI = (yyval.nodo);
	}
#line 2957 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 1588 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-1].nodo);
		(yyval.nodo)->HD->HD = (yyvsp[0].nodo);
		if((yyvsp[0].nodo) != nodoNulo)
			(yyvsp[0].nodo)->HI = (yyval.nodo);
	}
#line 2968 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 1594 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-1].nodo);
		(yyval.nodo)->HD = (yyvsp[0].nodo);
		if((yyvsp[0].nodo) != nodoNulo)
			(yyvsp[0].nodo)->HI = (yyval.nodo);
	}
#line 2979 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 1600 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-1].nodo);
		(yyval.nodo)->HD = (yyvsp[0].nodo);
		if((yyvsp[0].nodo) != nodoNulo)
			(yyvsp[0].nodo)->HI = (yyval.nodo);
	}
#line 2990 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 1606 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++, (yyvsp[-4].hilera),(yyvsp[0].nodo),NULL);
		(yyval.nodo)->params = new list<Caja*>();

		Caja* temp = new Caja( cuenta++, (yyvsp[-2].hilera), NULL, NULL );
		(yyval.nodo)->params->push_back(temp);
		if((yyvsp[0].nodo) != nodoNulo)
			(yyvsp[0].nodo)->HI = (yyval.nodo);
	}
#line 3004 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 1615 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-2].nodo);
		(yyval.nodo)->HD = (yyvsp[0].nodo);
		if((yyvsp[0].nodo) != nodoNulo)
		{
			(yyvsp[0].nodo)->HI = (yyval.nodo);
		}

		pila.push((yyvsp[-2].nodo));	// Guardo en pila el ret.
	}
#line 3019 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 1625 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[0].nodo);
	}
#line 3027 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 1628 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-2].nodo);
		(yyval.nodo)->HD = (yyvsp[0].nodo);
		if((yyvsp[0].nodo) != nodoNulo)
			(yyvsp[0].nodo)->HI = (yyval.nodo);
	}
#line 3038 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 1635 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja( cuenta++, (yyvsp[-2].hilera), (yyvsp[0].nodo), NULL );
		(yyval.nodo)->addToTable = true;
		(yyval.nodo)->HD = (yyvsp[0].nodo);
		if((yyvsp[0].nodo) != nodoNulo)
			(yyvsp[0].nodo)->HI = (yyval.nodo);
		}
#line 3050 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 1642 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo)->addToTable = true;
		(yyval.nodo)->HD = (yyvsp[0].nodo);
		if((yyvsp[0].nodo) != nodoNulo)
			(yyvsp[0].nodo)->HI = (yyval.nodo);
		}
#line 3061 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 1648 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.nodo) = nodoNulo;}
#line 3067 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 1654 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = (yyvsp[-3].nodo);
		arbol.agregarNodosDeListaATabla((yyvsp[-3].nodo)->params, (yyval.nodo));
		(yyval.nodo)->addToTable = true;
		if((yyvsp[-1].nodo) != nodoNulo)
			{(yyval.nodo)->HMI = (yyvsp[-1].nodo);}
		for(Nodo tmp = (yyvsp[-1].nodo); tmp != nodoNulo; tmp = tmp->HD){
				tmp->padre = (yyval.nodo);
				if(*tmp->tokenName == "*asignacion*"){
					arbol.agregarNodosDeListaATabla(tmp->asignacion, (yyval.nodo));
				}
				if(tmp->addToTable){
					arbol.agregarNodoATabla(tmp,(yyval.nodo));
				}
			}

			if( pila.empty() == true )	// Si está vacío, el método no tiene returns.
			{
				(yyvsp[-3].nodo)->tipo = t_void;						// Le agrego al método
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
				(yyvsp[-3].nodo)->tipo = tmp->tipo;					// Le agrego al método, si  todos fueron unknown, ese agrego.
			}

			// Vacío la pila para los otros.
			while( pila.empty() == false )
			{
				pila.pop();
			}
	}
#line 3112 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 1698 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[-1].hilera),NULL,NULL);
		(yyval.nodo)->params = (yyvsp[0].params);
	}
#line 3121 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 1705 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.params) = (yyvsp[-1].params);}
#line 3127 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 1706 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.params) = NULL;}
#line 3133 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 1711 "gramaticas.cpp" /* yacc.c:1646  */
    {
			(yyval.params) = new list<Caja*>();
			(yyvsp[-2].nodo)->addToTable = true;
			(yyval.params)->push_front((yyvsp[-2].nodo));
			arbol.mergeLists((yyval.params),(yyvsp[0].params));
			delete (yyvsp[0].params);
		}
#line 3145 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 1719 "gramaticas.cpp" /* yacc.c:1646  */
    {
			(yyval.params) = new list<Caja*>();
			arbol.mergeLists((yyvsp[0].params),(yyvsp[-2].params));
			arbol.mergeLists((yyval.params),(yyvsp[0].params));
			delete (yyvsp[-2].params);
			delete (yyvsp[0].params);
		}
#line 3157 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 1727 "gramaticas.cpp" /* yacc.c:1646  */
    {
			(yyval.params) = new list<Caja*>();
			(yyvsp[0].nodo)->addToTable = true;
			(yyval.params)->push_back((yyvsp[0].nodo));
		}
#line 3167 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 1733 "gramaticas.cpp" /* yacc.c:1646  */
    {
			(yyval.params) = new list<Caja*>();
			arbol.mergeLists((yyval.params),(yyvsp[0].params));
		}
#line 3176 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 1740 "gramaticas.cpp" /* yacc.c:1646  */
    {
		Caja* temp = new Caja( cuenta++, (yyvsp[-1].hilera), NULL, NULL );
		(yyvsp[-2].params)->push_back(temp);

		arbol.mergeLists((yyvsp[-2].params),(yyvsp[0].params));
		(yyval.params) = (yyvsp[-2].params);
	}
#line 3188 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 1747 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.params) = (yyvsp[0].params);}
#line 3194 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 1752 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.params) = new list<Caja*>();

		(yyval.params)->push_back((yyvsp[-2].nodo));

		Caja* temp = new Caja( cuenta++, (yyvsp[-1].hilera), NULL, NULL );
		(yyval.params)->push_back(temp);

		(yyval.params)->push_back((yyvsp[0].nodo));
	}
#line 3209 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 1763 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.params) = new list<Caja*>();

		(yyval.params)->push_back((yyvsp[-2].nodo));

		Caja* temp = new Caja( cuenta++, (yyvsp[-1].hilera), NULL, NULL );
		(yyval.params)->push_back(temp);

		arbol.mergeLists((yyval.params),(yyvsp[0].params));	// Ojo que operaciones devuelve una lista.
	}
#line 3224 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 1774 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.params) = (yyvsp[-2].params);						// Obtengo la lista que devuele operaciones.

		Caja* temp = new Caja( cuenta++, (yyvsp[-1].hilera), NULL, NULL );
		(yyval.params)->push_back(temp);

		(yyvsp[-2].params)->push_back((yyvsp[0].nodo));
	}
#line 3237 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 1783 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.params) = (yyvsp[-2].params);

		Caja* temp = new Caja( cuenta++, (yyvsp[-1].hilera), NULL, NULL );
		(yyval.params)->push_back(temp);

		arbol.mergeLists((yyval.params), (yyvsp[0].params));
	}
#line 3250 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 1794 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[-1].hilera),NULL,NULL);
		(yyval.nodo)->params = new list<Caja*>();
		(yyval.nodo)->params->push_back((yyvsp[0].nodo));
		if(*(yyvsp[0].nodo)->tokenName != "*NULL*"){arbol.agregarNodoATabla((yyvsp[0].nodo),(yyval.nodo));};
		(yyval.nodo)->tipo = (yyvsp[0].nodo)->tipo;
	}
#line 3262 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 1801 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[-1].hilera),NULL,NULL);
		(yyval.nodo)->operaciones = (yyvsp[0].params);
		arbol.agregarNodosDeListaATabla((yyvsp[0].params),(yyval.nodo));
		(yyval.nodo)->tipo = entero;
	}
#line 3273 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 1807 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[-1].hilera),NULL,NULL);
		(yyval.nodo)->comparacion = (yyvsp[0].params);
		arbol.agregarNodosDeListaATabla((yyvsp[0].params),(yyval.nodo));
		(yyval.nodo)->tipo = booleano;
	}
#line 3284 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 1817 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[-5].hilera),NULL,NULL);
		(yyval.nodo)->comparacion = (yyvsp[-3].params);
		arbol.agregarNodosDeListaATabla((yyvsp[-3].params),(yyval.nodo));
		(yyval.nodo)->HMI = (yyvsp[-1].nodo);
		for(Nodo tmp = (yyvsp[-1].nodo); tmp != nodoNulo; tmp = tmp->HD){
			tmp->padre = (yyval.nodo);
			if(*tmp->tokenName == "*asignacion*"){
				arbol.agregarNodosDeListaATabla(tmp->asignacion, (yyval.nodo));
			}
			if(tmp->addToTable){
				arbol.agregarNodoATabla(tmp,(yyval.nodo));
			}
		}
	}
#line 3304 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 1836 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[-3].hilera),NULL,NULL);
		(yyval.nodo)->HMI = (yyvsp[-1].nodo);
		for(Nodo tmp = (yyvsp[-1].nodo); tmp != nodoNulo; tmp = tmp->HD){
			tmp->padre = (yyval.nodo);
			if(*tmp->tokenName == "*asignacion*"){
				arbol.agregarNodosDeListaATabla(tmp->asignacion, (yyval.nodo));
			}
			if(tmp->addToTable){
				arbol.agregarNodoATabla(tmp,(yyval.nodo));
			}
		}
		(yyvsp[-4].nodo)->HD = (yyval.nodo);
		(yyval.nodo)->HI = (yyvsp[-4].nodo);
		(yyval.nodo) = (yyvsp[-4].nodo);
	}
#line 3325 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 1856 "gramaticas.cpp" /* yacc.c:1646  */
    {
			(yyval.nodo) = new Caja(cuenta++,(yyvsp[-9].hilera),NULL,NULL);
			(yyval.nodo)->asignacion = (yyvsp[-7].nodo)->asignacion;
			arbol.agregarNodosDeListaATabla((yyvsp[-7].nodo)->asignacion,(yyval.nodo));
			arbol.agregarNodosDeListaATabla((yyvsp[-5].params),(yyval.nodo));
			arbol.agregarNodosDeListaATabla((yyvsp[-3].params),(yyval.nodo));
			(yyval.nodo)->comparacion = (yyvsp[-5].params);
			(yyval.nodo)->operaciones = (yyvsp[-3].params);
			(yyval.nodo)->HMI = (yyvsp[-1].nodo);
			for(Nodo tmp = (yyvsp[-1].nodo); tmp != nodoNulo; tmp = tmp->HD){
				tmp->padre = (yyval.nodo);
				if(*tmp->tokenName == "*asignacion*"){
					arbol.agregarNodosDeListaATabla(tmp->asignacion, (yyval.nodo));
				}
				if(tmp->addToTable){
					arbol.agregarNodoATabla(tmp,(yyval.nodo));
				}
			}
		}
#line 3349 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 1878 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.nodo) = new Caja(cuenta++,(yyvsp[-5].hilera),NULL,NULL);
		(yyval.nodo)->comparacion = (yyvsp[-3].params);
		arbol.agregarNodosDeListaATabla((yyvsp[-3].params),(yyval.nodo));
		(yyval.nodo)->HMI = (yyvsp[-1].nodo);
		for(Nodo tmp = (yyvsp[-1].nodo); tmp != nodoNulo; tmp = tmp->HD){
			tmp->padre = (yyval.nodo);
			if(*tmp->tokenName == "*asignacion*"){
				arbol.agregarNodosDeListaATabla(tmp->asignacion, (yyval.nodo));
			}
			if(tmp->addToTable){
				arbol.agregarNodoATabla(tmp,(yyval.nodo));
			}
		}
	}
#line 3369 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 1896 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.params) = new list<Caja*>();
		(yyval.params)->push_back((yyvsp[0].nodo));
	}
#line 3378 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 1901 "gramaticas.cpp" /* yacc.c:1646  */
    {
		(yyval.params) = (yyvsp[0].params);
	}
#line 3386 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 1907 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.hilera) = (yyvsp[0].hilera);}
#line 3392 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 1908 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.hilera) = (yyvsp[0].hilera);}
#line 3398 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 1909 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.hilera) = (yyvsp[0].hilera);}
#line 3404 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 1910 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.hilera) = (yyvsp[0].hilera);}
#line 3410 "gramaticas.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 1911 "gramaticas.cpp" /* yacc.c:1646  */
    {(yyval.hilera) = (yyvsp[0].hilera);}
#line 3416 "gramaticas.tab.c" /* yacc.c:1646  */
    break;


#line 3420 "gramaticas.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1915 "gramaticas.cpp" /* yacc.c:1906  */

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
