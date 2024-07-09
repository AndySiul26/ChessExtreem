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
	using VectorTipoPieza = std::vector<TipoPieza>;

	struct DatosBasicosJugada
	{
		Coordenadas coordenadas;
		TipoPieza tipoPieza;
	};
}

namespace std { // Especialización de la plantilla hash para la estructura DatosBasicosJugada
	template<>
	struct hash<ChessExtreem::DatosBasicosJugada> {
		size_t operator()(const ChessExtreem::DatosBasicosJugada& jugada) const;
	};
}

namespace ChessExtreem
{
	class DatosChessGame
	{
	public:
		std::vector<DatosBasicosJugada> jugadas;
		
		// Constructor y Destructor
		DatosChessGame();
		DatosChessGame(const Jugadas& jugadas);
		DatosChessGame(const CadenaCoordenadas& cc, const VectorTipoPieza& vtp);
		DatosChessGame(const DatosChessGame& otro);
		DatosChessGame(DatosChessGame&& otro) noexcept;

		DatosChessGame& operator= (const DatosChessGame& otro);

		~DatosChessGame();

	};

	

	// Clase ManejadorDatosChess
	class ManejadorDatosChess
	{
	private:
		// Atributos que equivalen a las coordenadas de las piezas
		DatosChessGame juego;
		std::string cadena; // Cadena de texto que contendrá en un codigo creado previamente las coordenadas de las piezas
		std::string firma; // Firma que representa los movimientos de los juegos de ajedrez

		// Metodos de codificacion y decodificacion de coordenadas
		char EncodificarCoordenada(int x, int y); // Metodo que codifica las coordenadas de las piezas en un caracter
		char EncodificarCoordenada(Coordenadas coordenadas); // Metodo que codifica las coordenadas de las piezas en un caracter
		char EncodificarTipoPieza(TipoPieza tipoPieza); // Metodo que codifica un tipo de pieza en un caracter
		Coordenadas DecodificarCoordenada(char caracter); // Metodo que decodifica un caracter en coordenadas de las piezas
		TipoPieza DecodificarTipoPieza(char caracter); // Metodo que decodifica un caracter en un tipo de pieza

		// Metodos de codificacion y decodificacion de jugadas
		std::string EncodificarJugada(const DatosBasicosJugada& jugada); // Metodo que codifica las coordenadas de las piezas y tipos de piezas en una cadena de texto
		DatosBasicosJugada DecodificarJugada(const std::string& jugada); // Metodo que decodifica una cadena de texto en coordenadas de las piezas y tipos de piezas
		

		void ConvertirCadenaInterna(); // Metodo que convierte una cadena en un juego de ajedrez

		void GenerarFirmaInterna(); // Metodo que genera una firma de las secuencias de cada juego de ajedrez
		
		// Constructor y Destructor
	public:
		ManejadorDatosChess();
		ManejadorDatosChess(const std::string& cadena);
		ManejadorDatosChess(const std::string& cadena, const std::string& firma);
		ManejadorDatosChess(const std::string& cadena, const std::string& firma, const DatosChessGame& juego);
		ManejadorDatosChess(ManejadorDatosChess& otro); // Constructor de copia
		ManejadorDatosChess(ManejadorDatosChess&& otro) noexcept; // Constructor de movimiento

		~ManejadorDatosChess();

		// Metodos
		
		// Metodo que genera una firma de las secuencias de cada juego de ajedrez
		std::string GenerarFirma(const std::string& sequence);

		// Metodo que convierte una cadena en coordenadas de las piezas
		DatosChessGame ConvertirCadena(const std::string& cadena); // Metodo que convierte una cadena en coordenadas de las piezas
		std::string ConvertirJuego(const DatosChessGame& juego); // Metodo que convierte las coordenadas de las piezas en una cadena
	};
}



#include "ManejadorDatosChess.inl"

#endif // !ManejadorDatosChess_H