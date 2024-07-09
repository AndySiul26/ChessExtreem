//#include "ManejadorDatosChess.h"
//
//
//namespace ChessExtreem
//{
//	//// Constructor
//	//ManejadorDatosChess::ManejadorDatosChess()
//	//{
//	//}
//
//	//// Destructor
//	//ManejadorDatosChess::~ManejadorDatosChess()
//	//{
//	//}
//
//	// Metodo que codifica las coordenadas de las piezas en un caracter
//	char ManejadorDatosChess::EncodificarCoordenada(int x, int y)
//	{
//		// Codificar las coordenadas en un caracter
//		
//		return static_cast<char>(x * 8 + y + 33);  // +33 para evitar caracteres no imprimibles	
//	}
//
//	// Metodo que codifica las coordenadas de las piezas en un caracter
//	char ManejadorDatosChess::EncodificarCoordenada(Coordenadas coordenadas)
//	{
//		// Codificar las coordenadas en un caracter
//		return EncodificarCoordenada(coordenadas.x, coordenadas.y);
//	}
//
//	// Metodo que decodifica un caracter en coordenadas de las piezas
//	Coordenadas ManejadorDatosChess::DecodificarCoordenada(char caracter)
//	{
//		// Decodificar el caracter en coordenadas
//		int valor = static_cast<int>(caracter) - 33; // -33 para evitar caracteres no imprimibles
//		return Coordenadas(valor / 8, valor % 8);
//	}
//
//	// Metodo que comprime una cadena de texto
//	std::string ManejadorDatosChess::compressRLE(const std::string& input) {
//		std::string compressed;
//		for (size_t i = 0; i < input.length(); i++) {
//			int count = 1; // Numero de veces que se repite el caracter
//			while (i + 1 < input.length() && input[i] == input[i + 1]) { // Mientras el caracter actual sea igual al siguiente
//				count++; // Incrementar el contador
//				i++; // Incrementar el indice
//			}
//			compressed += input[i]; // Agregar el caracter
//			if (count > 1) {
//				compressed += std::to_string(count); // Agregar el contador
//			}
//		}
//		return compressed; // Devolver la cadena comprimida
//	}
//
//	// Metodo que descomprime una cadena de texto
//	std::string ManejadorDatosChess::decompressRLE(const std::string& compressed) {
//		std::string decompressed; // Cadena descomprimida
//		for (size_t i = 0; i < compressed.length(); i++) {
//			char c = compressed[i]; // Caracter actual
//			if (i + 1 < compressed.length() && std::isdigit(compressed[i + 1])) { // Si el siguiente caracter es un digito
//				int count = compressed[i + 1] - '0'; // Obtener el contador
//				decompressed.append(count, c); // Agregar el caracter count veces
//				i++; // Incrementar el indice
//			}
//			else {
//				decompressed += c; // Agregar el caracter
//			}
//		}
//		return decompressed; // Devolver la cadena descomprimida
//	}
//
//	// Metodo que inicializa el mapa de equivalencia de las cadenas de texto con las coordenadas de las piezas del tablero
//	void ManejadorDatosChess::InicializarMapaEquivalencia()
//	{
//		// Inicializar el mapa de equivalencia
//		for (int i = 0; i < 8; i++)
//		{
//			for (int j = 0; j < 8; j++)
//			{
//				mapaEquivalencia[EncodificarCoordenada(i, j)] = Coordenadas(i, j);
//			}
//		}
//	}
//	
//}