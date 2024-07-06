
#include "ManejadorDatosChess.h"

// Manejadro de datos de Chess inl file es un archivo de implementacion que contiene las clases y funciones necesarias para manejar los datos de la aplicacion de ajedrez.

// Implementacion de la clase ManejadorDatosChess
namespace ChessExtreem
{
	// Metodos generales
	// Función auxiliar para verificar si un carácter es una coordenada válida
	bool isValidCoordinate(char c) {
		return c >= 'A' && c <= 'Z';
	}

	bool isValidCoordinate(int x, int y) { // Función auxiliar para verificar si una coordenada es válida dentro del tablero de ajedrez
		return x >= 0 && x < 8 && y >= 0 && y < 8;
	}

	// Metodo que comprime una cadena de texto
	std::string compressRLE(const std::string& input) {
		std::string compressed;
		for (size_t i = 0; i < input.length(); i++) {
			int count = 1; // Numero de veces que se repite el caracter
			while (i + 1 < input.length() && input[i] == input[i + 1]) { // Mientras el caracter actual sea igual al siguiente
				count++; // Incrementar el contador
				i++; // Incrementar el indice
			}
			compressed += input[i]; // Agregar el caracter
			if (count > 1) {
				compressed += std::to_string(count); // Agregar el contador
			}
		}
		return compressed; // Devolver la cadena comprimida
	}

	// Metodo que descomprime una cadena de texto
	std::string decompressRLE(const std::string& compressed) {
		std::string decompressed; // Cadena descomprimida
		for (size_t i = 0; i < compressed.length(); i++) { // Recorrer la cadena comprimida
			if (isValidCoordinate(compressed[i])) { // Si el caracter es una coordenada válida
				int count = 1; // Numero de veces que se repite el caracter
				if (i + 1 < compressed.length() && std::isdigit(compressed[i + 1])) { // Si el siguiente caracter es un digito
					count = 0; // Reiniciar el contador
					while (i + 1 < compressed.length() && std::isdigit(compressed[i + 1])) { // Mientras el siguiente caracter sea un digito
						count = count * 10 + (compressed[i + 1] - '0'); // Incrementar el contador
						i++; // Incrementar el indice
					}
				}
				decompressed.append(count, compressed[i]); // Agregar el caracter a la cadena descomprimida
			}
			else {
				// Manejar error: carácter inválido en la secuencia comprimida
				throw std::runtime_error("Carácter inválido en la secuencia comprimida"); // Lanzar una excepción
			}
		}
		return decompressed;
	}

	// Metodo para serializar las coordenadas
	std::string SerializarCoordenadas(const Coordenadas& coord) {
		return std::to_string(coord.x) + "," + std::to_string(coord.y);
	}

	
	// Metodos del ManejadorDatosChess << Inicio
	// Constructor
	ManejadorDatosChess::ManejadorDatosChess()
	{
		// Inicializar el mapa de equivalencia
		InicializarMapaEquivalencia();
	}

	// Constructor
	ManejadorDatosChess::ManejadorDatosChess(const std::string& cadena)
	{
		// Inicializar el mapa de equivalencia
		InicializarMapaEquivalencia();
		this->cadena = cadena; // Asignar la cadena
		ConvertirCadenaInterna(); // Convertir la cadena interna
		GenerarFirmaInterna(); // Generar la firma
		
	}

	// Constructor
	ManejadorDatosChess::ManejadorDatosChess(const std::string& cadena, const std::string& firma)
	{
		// Inicializar el mapa de equivalencia
		InicializarMapaEquivalencia();
		this->cadena = cadena; // Asignar la cadena
		ConvertirCadenaInterna(); // Convertir la cadena interna
		this->firma = firma; // Asignar la firma
	}

	// Constructor
	ManejadorDatosChess::ManejadorDatosChess(const std::string& cadena, const std::string& firma, const CadenaCoordenadas& coordenadas)
	{
		// Inicializar el mapa de equivalencia
		InicializarMapaEquivalencia();
		this->cadena = cadena; // Asignar la cadena
		this->firma = firma; // Asignar la firma
		cadenaCoordenadas = coordenadas; // Asignar las coordenadas
	}

	// Constructor de copia
	ManejadorDatosChess::ManejadorDatosChess(ManejadorDatosChess& otro)
	{
		// Inicializar el mapa de equivalencia
		InicializarMapaEquivalencia();
		cadena = otro.cadena; // Asignar la cadena
		firma = otro.firma; // Asignar la firma
		cadenaCoordenadas = otro.cadenaCoordenadas; // Asignar las coordenadas
	}

	// Constructor de movimiento
	ManejadorDatosChess::ManejadorDatosChess(ManejadorDatosChess&& otro) noexcept
	{
		// Inicializar el mapa de equivalencia
		InicializarMapaEquivalencia();
		cadena = std::move(otro.cadena); // Mover la cadena
		firma = std::move(otro.firma); // Mover la firma
		cadenaCoordenadas = std::move(otro.cadenaCoordenadas); // Mover las coordenadas
	}

	// Destructor
	ManejadorDatosChess::~ManejadorDatosChess()
	{
		// Limpiar el mapa de equivalencia
		mapaEquivalencia.clear();
		cadenaCoordenadas.clear();
		cadena.clear();
		firma.clear();
	}

	// Metodo que codifica las coordenadas de las piezas en un caracter
	char ManejadorDatosChess::EncodificarCoordenada(int x, int y)
	{
		// Usamos caracteres de 'A' a 'Z' para las 64 posibles coordenadas
		return static_cast<char>((x * 8 + y) + 'A'); // Devolver el caracter
	}

	// Metodo que codifica las coordenadas de las piezas en un caracter
	char ManejadorDatosChess::EncodificarCoordenada(Coordenadas coordenadas)
	{
		// Codificar las coordenadas en un caracter
		return EncodificarCoordenada(coordenadas.x, coordenadas.y);
	}

	// Metodo que decodifica un caracter en coordenadas de las piezas
	Coordenadas ManejadorDatosChess::DecodificarCoordenada(char caracter)
	{
		int value = caracter - 'A'; // Obtener el valor del caracter
		return { value / 8, value % 8 }; // Devolver las coordenadas
	}

	std::string ManejadorDatosChess::GenerarFirma(const std::string& sequence) {
		const int PRIME = 31;
		long long hash = 0;
		for (char c : sequence) {
			hash = (hash * PRIME + c) % 1000000007;
		}
		return std::to_string(hash);
	}

	

	// Metodo que convierte la cadena interna
	void ManejadorDatosChess::ConvertirCadenaInterna()
	{
		// Convertir la cadena interna
		cadenaCoordenadas = ConvertirCadena(cadena); // Convertir la cadena en coordenadas
	}

	// Metodo que genera una firma de las secuencias de cada juego de ajedrez
	void ManejadorDatosChess::GenerarFirmaInterna()
	{
		// Generar la firma interna
		firma = GenerarFirma(cadena); // Generar la firma de la cadena
	}

	// Metodo que inicializa el mapa de equivalencia de las cadenas de texto con las coordenadas de las piezas del tablero
	void ManejadorDatosChess::InicializarMapaEquivalencia()
	{
		// Inicializar el mapa de equivalencia
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{				
				mapaEquivalencia[EncodificarCoordenada(i, j)] = Coordenadas(i, j);
				mapaEquivalenciaInverso[Coordenadas(i, j)] = EncodificarCoordenada(i, j);
			}
		}
	}

	// Metodo que convierte una cadena en coordenadas de las piezas
	CadenaCoordenadas ManejadorDatosChess::ConvertirCadena(const std::string& cadena)
	{
		// Convertir la cadena en coordenadas
		CadenaCoordenadas coordenadas; // Coordenadas de las piezas
		for (size_t i = 0; i < cadena.length(); i++)
		{
			// Si el mapa de equivalencia (std::unordered_map<char, Coordenadas>) ya esta inicializado usarlo en vez de calcularlo
			if(mapaEquivalenciaInicializado)
				coordenadas.push_back(mapaEquivalencia[cadena[i]]); // Agregar las coordenadas
			else
				coordenadas.push_back(DecodificarCoordenada(cadena[i])); // Decodificar el caracter y agregar las coordenadas
		}
		return coordenadas; // Devolver las coordenadas
	}

	// Metodo que convierte las coordenadas de las piezas en una cadena
	std::string ManejadorDatosChess::ConvertirCoordenadas(const CadenaCoordenadas& coordenadas)
	{
		// Convertir las coordenadas en una cadena
		std::string cadena; // Cadena de texto
		for (Coordenadas coord : coordenadas)
		{
			if(!isValidCoordinate(coord.x, coord.y)) // Verificar si las coordenadas son válidas
				throw std::runtime_error("Coordenadas inválidas"); // Lanzar una excepción (coordenadas inválidas)
			// Si el mapa de equivalencia (std::unordered_map<Coordenadas, char>) ya esta inicializado usarlo en vez de calcularlo
			if(mapaEquivalenciaInicializado)
				cadena += mapaEquivalenciaInverso[coord]; // Agregar el caracter
			else
				cadena += EncodificarCoordenada(coord); // Codificar las coordenadas y agregar el caracter
		}
		return cadena; // Devolver la cadena
	}

	// Metodos del ManejadorDatosChess << Fin

}
