%{
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

		// Aquí inicia lo chido.
		generar_MIPS();	// El método se encuentra en MIPS.h
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
