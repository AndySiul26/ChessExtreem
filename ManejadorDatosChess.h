#pragma once

// Manejador de datos de Chess es un archivo de cabecera que contiene las clases y funciones necesarias para manejar los datos de la aplicacion de ajedrez.
// Lectura, escritura y manipulacion de datos de la aplicacion de ajedrez.
// Firma que representa los movimientos de los juegos de ajedrez.
// y los guarda en un archivo de texto.
// Se encarga de leer y escribir los datos de los usuarios en un archivo de texto.
// Se encarga de leer y escribir los datos de los juegos de ajedrez en un archivo de texto.
// Facilita la lectura y escritura de cadenas de movimientos 
// y los guarda en un archivo de texto.

#ifndef ManejadorDatosChess_H
	#define ManejadorDatosChess_H

#include "ChessExtreem.h"

namespace ChessExtreem
{
	// Clase ManejadorDatosChess
	class ManejadorDatosChess
	{
	private:
		// Atributos 
		std::unordered_map<char, Coordenadas> mapaEquivalencia; // Mapa que contendrá la equivalencia de las cadenas de texto con las coordenadas de las piezas del tablero
		std::unordered_map<Coordenadas, char> mapaEquivalenciaInverso; // Mapa que contendrá la equivalencia de las cadenas de texto con las coordenadas de las piezas del tablero
		bool mapaEquivalenciaInicializado; // Bandera que indica si el mapa de equivalencia ha sido inicializado

		// Atributos que equivalen a las coordenadas de las piezas
		CadenaCoordenadas cadenaCoordenadas;
		std::string cadena; // Cadena de texto que contendrá en un codigo creado previamente las coordenadas de las piezas
		std::string firma; // Firma que representa los movimientos de los juegos de ajedrez

		// Metodos de codificacion y decodificacion de coordenadas
		char EncodificarCoordenada(int x, int y); // Metodo que codifica las coordenadas de las piezas en un caracter
		char EncodificarCoordenada(Coordenadas coordenadas); // Metodo que codifica las coordenadas de las piezas en un caracter
		Coordenadas DecodificarCoordenada(char caracter); // Metodo que decodifica un caracter en coordenadas de las piezas


		void ConvertirCadenaInterna(); // Metodo que convierte la cadena interna en coordenadas de las piezas
		void GenerarFirmaInterna(); // Metodo que genera una firma de las secuencias de cada juego de ajedrez
		
		// Constructor y Destructor
	public:
		ManejadorDatosChess();
		ManejadorDatosChess(const std::string& cadena);
		ManejadorDatosChess(const std::string& cadena, const std::string& firma);
		ManejadorDatosChess(const std::string& cadena, const std::string& firma, const CadenaCoordenadas& coordenadas);
		ManejadorDatosChess(ManejadorDatosChess& otro); // Constructor de copia
		ManejadorDatosChess(ManejadorDatosChess&& otro) noexcept; // Constructor de movimiento

		~ManejadorDatosChess();

		// Metodos
		
		// Metodo que inicializa el mapa de equivalencia de las cadenas de texto con las coordenadas de las piezas del tablero
		void InicializarMapaEquivalencia();
		
		// Metodo que genera una firma de las secuencias de cada juego de ajedrez
		std::string GenerarFirma(const std::string& sequence);

		// Metodo que convierte una cadena en coordenadas de las piezas
		CadenaCoordenadas ConvertirCadena(const std::string& cadena); // Metodo que convierte una cadena en coordenadas de las piezas
		std::string ConvertirCoordenadas(const CadenaCoordenadas& coordenadas); // Metodo que convierte las coordenadas de las piezas en una cadena
	};
}

#include "ManejadorDatosChess.inl"

#endif // !ManejadorDatosChess_H