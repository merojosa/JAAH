#include <cstdlib>
#include <iostream>

// La cantidad de registros que puede haber.  Se incluyen: $v, $a, $t, $s.
#define CANTIDAD_REGISTROS 23
// NOTA: se eliminó $s7 para que sea usado solo para retorno.

class Bitmap
{
	private:
		// true es disponible, false ocupado.
		bool* puntero_bitmap;
		int registros_disponibles;

	public:
		Bitmap();

		~Bitmap();

		/*
		 * Devuelve el número del registro disponible [0-CANTIDAD_REGISTROS-1]
		 * Si devuelve -1, no hay registros disponibles.
		 */
		int ocupar_registro();

		/*
		 * Libera el registro del parámetros.
		 * Si es mayor o igual a CANTIDAD_REGISTROS, no hace nada.
		 */
		void liberar_registro(int numero_registro);

};

Bitmap::Bitmap()
{
	registros_disponibles = CANTIDAD_REGISTROS;

	puntero_bitmap = new bool[CANTIDAD_REGISTROS];

	for( size_t contador = 0; contador < CANTIDAD_REGISTROS; ++contador )
		puntero_bitmap[contador] = true;
}

Bitmap::~Bitmap()
{
	registros_disponibles = 0;
	delete[] puntero_bitmap;
}

int Bitmap::ocupar_registro()
{
	int posicion = 0;
	if( registros_disponibles < 0 )	// Error.
	{
		std::cerr << "No quedan más registros en el bitmap." << std::endl;
		exit(-1);
	}
	else
	{
		while( puntero_bitmap[posicion] == false )
		{
			++posicion;
			if( posicion >= CANTIDAD_REGISTROS )
			{
				std::cerr << "No quedan más registros en el bitmap." << std::endl;
				exit(-1);
			}
		}

		// puntero_bitmap[posicion] es true, implica que está disponible.
		--registros_disponibles;					// Un registro menos disponible.
		puntero_bitmap[posicion] = false;	// Ya no está disponible.

		return posicion;
	}
}

void Bitmap::liberar_registro(int registro)
{
	if( registro >= 0 && registro < CANTIDAD_REGISTROS )
	{
		puntero_bitmap[registro] = true;	// Disponible.
		++registros_disponibles;
	}
}
