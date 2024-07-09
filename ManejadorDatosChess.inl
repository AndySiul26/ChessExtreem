
#include "ManejadorDatosChess.h"
#include <stdexcept>

// Manejadro de datos de Chess inl file es un archivo de implementacion que contiene las clases y funciones necesarias para manejar los datos de la aplicacion de ajedrez.

	
// Metodos generales
	
size_t std::hash<ChessExtreem::DatosBasicosJugada>::operator()(const ChessExtreem::DatosBasicosJugada& jugada) const {
	std::hash<int> hash;
	auto coord = jugada.coordenadas;
	int tipo = hash(static_cast<int>(jugada.tipoPieza));

	return hash(coord.x) ^ hash(coord.y) ^ tipo; // Se realiza un XOR entre las coordenadas y el tipo de pieza			
}



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


	const char COMPRESSION_INDICATOR = '\''; // Usamos '~' como indicador de compresión

	std::string compressRLE(const std::string& input) {
		if (input.empty()) {
			return input;
		}

		std::string compressed = std::string(1, COMPRESSION_INDICATOR); // Añadimos el indicador al inicio
		for (size_t i = 0; i < input.length(); i++) {
			int count = 1;
			while (i + 1 < input.length() && input[i] == input[i + 1]) {
				count++;
				i++;
			}
			compressed += input[i];
			if (count > 1) {
				compressed += std::to_string(count);
			}
		}
		return compressed;
	}

	std::string decompressRLE(const std::string& compressed) {
		if (compressed.empty()) {
			return compressed;
		}

		// Verificamos si la cadena está comprimida
		if (compressed[0] != COMPRESSION_INDICATOR) {
			return compressed; // Si no está comprimida, devolvemos la cadena original
		}

		std::string decompressed;
		for (size_t i = 1; i < compressed.length(); i++) { // Empezamos desde 1 para saltar el indicador
			if (isValidCoordinate(compressed[i])) {
				int count = 1;
				size_t digiCount = 0;
				if (i + 1 < compressed.length() && std::isdigit(compressed[i + 1])) {
					count = 0;
					while (i + 1 < compressed.length() && std::isdigit(compressed[i + 1])) {
						digiCount++; // Contar el número de dígitos
						count = count * 10 + (compressed[i + 1] - '0');
						i++;
					}
				}
				decompressed.append(count, compressed[i - digiCount]);
			}
			else if (compressed[i] != COMPRESSION_INDICATOR) {
				// Manejar error: carácter inválido en la secuencia comprimida
				throw std::runtime_error("Carácter inválido en la secuencia comprimida");
			}
		}
		return decompressed;
	}

	// Función auxiliar para verificar si una cadena está comprimida
	bool isCompressed(const std::string& str) {
		return !str.empty() && str[0] == COMPRESSION_INDICATOR;
	}

	// Metodo para serializar las coordenadas
	std::string SerializarCoordenadas(const Coordenadas& coord) {
		return std::to_string(coord.x) + "," + std::to_string(coord.y);
	}

	// Metodos del DatosChessGame << Inicio
	
	// Constructor y Destructor

	// Constructor
	DatosChessGame::DatosChessGame()
	{
		jugadas.clear(); // Limpiar las jugadas
	}

	// Constructor
	DatosChessGame::DatosChessGame(const Jugadas& jugadas)
	{
		for (auto& jugada : jugadas)
		{
			// Agregar las coordenadas y el tipo de pieza
			this->jugadas.push_back({ jugada.J_movimiento.MC_Movimiento.Origen, jugada.tipoPieza });
			this->jugadas.push_back({ jugada.J_movimiento.MC_Movimiento.Destino, jugada.tipoPieza });
		}
	}

	// Constructor
	DatosChessGame::DatosChessGame(const CadenaCoordenadas& cc, const VectorTipoPieza& vtp)
	{
		if(cc.size() != vtp.size())
			throw std::runtime_error("Las cadenas de coordenadas y los tipos de piezas no tienen la misma longitud"); // Lanzar una excepción (las cadenas de coordenadas y los tipos de piezas no tienen la misma longitud

		for (size_t i = 0; i < cc.size(); i++)
		{
			// Agregar las coordenadas y el tipo de pieza
			jugadas.push_back({ cc[i], vtp[i] });
		}

	}

	// Constructor de copia
	DatosChessGame::DatosChessGame(const DatosChessGame& otro)
	{
		jugadas = otro.jugadas; // Copiar las jugadas	
	}

	// Constructor de movimiento
	DatosChessGame::DatosChessGame(DatosChessGame&& otro) noexcept
	{
		jugadas = std::move(otro.jugadas); // Mover las jugadas
	}

	// Destructor
	DatosChessGame::~DatosChessGame()
	{
		jugadas.clear(); // Limpiar las jugadas
	}

	// Operador de asignación
	DatosChessGame& DatosChessGame::operator= (const DatosChessGame& otro)
	{
		if (this != &otro)
		{
			jugadas = otro.jugadas; // Copiar las jugadas
		}
		return *this; // Devolver el objeto actual
	}

	// Metodos del DatosChessGame << Fin
	
	// Metodos del ManejadorDatosChess << Inicio
	
	// Constructor
	ManejadorDatosChess::ManejadorDatosChess()
	{

	}

	// Constructor
	ManejadorDatosChess::ManejadorDatosChess(const std::string& cadena)
	{
		this->cadena = cadena; // Asignar la cadena
		ConvertirCadenaInterna(); // Convertir la cadena interna
		GenerarFirmaInterna(); // Generar la firma
		
	}

	// Constructor
	ManejadorDatosChess::ManejadorDatosChess(const std::string& cadena, const std::string& firma)
	{
		this->cadena = cadena; // Asignar la cadena
		ConvertirCadenaInterna(); // Convertir la cadena interna
		this->firma = firma; // Asignar la firma
	}

	// Constructor
	ManejadorDatosChess::ManejadorDatosChess(const std::string& cadena, const std::string& firma, const DatosChessGame& juego)
	{
		this->cadena = cadena; // Asignar la cadena
		this->firma = firma; // Asignar la firma
		this->juego = juego; // Asignar el juego
	}

	// Constructor de copia
	ManejadorDatosChess::ManejadorDatosChess(ManejadorDatosChess& otro)
	{
		cadena = otro.cadena; // Asignar la cadena
		firma = otro.firma; // Asignar la firma
		juego = otro.juego; // Asignar el juego
	}

	// Constructor de movimiento
	ManejadorDatosChess::ManejadorDatosChess(ManejadorDatosChess&& otro) noexcept
	{
		cadena = std::move(otro.cadena); // Mover la cadena
		firma = std::move(otro.firma); // Mover la firma
		juego = std::move(otro.juego); // Mover las coordenadas
	}

	// Destructor
	ManejadorDatosChess::~ManejadorDatosChess()
	{
		// Limpiar los datos
		juego.jugadas.clear();
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

	char ManejadorDatosChess::EncodificarTipoPieza(TipoPieza tipoPieza) {
		return static_cast<char>((int)tipoPieza + 130);
	}

	TipoPieza ManejadorDatosChess::DecodificarTipoPieza(char caracter) {
		return static_cast<TipoPieza>(caracter - 130);
	}

	std::string ManejadorDatosChess::EncodificarJugada(const DatosBasicosJugada& jugada) {
		// Concatenar dos chars para la jugada
		
        std::string str;
        str.push_back(EncodificarCoordenada(jugada.coordenadas));
        str.push_back(EncodificarTipoPieza(jugada.tipoPieza));
		// si se tienen dos caracteres a,b se concatenan en un solo string ab
		return str;
	}

	DatosBasicosJugada ManejadorDatosChess::DecodificarJugada(const std::string& jugada) {
		// Decodificar la jugada
		if (jugada.length() != 2) {
			throw std::runtime_error("La jugada no tiene el formato correcto");
		}
		return { DecodificarCoordenada(jugada[0]), DecodificarTipoPieza(jugada[1]) };
	}

	std::string ManejadorDatosChess::GenerarFirma(const std::string& sequence) {
		const int PRIME = 31;
		long long hash = 0;
		for (char c : sequence) {
			hash = (hash * PRIME + c) % 1000000007;
		}
		return std::to_string(hash);
	}

	// Metodo que convierte una cadena en un juego de ajedrez
	void ManejadorDatosChess::ConvertirCadenaInterna()
	{
		// Convertir la cadena en un juego de ajedrez
		juego = ConvertirCadena(cadena); // Convertir la cadena en coordenadas		
	}

	// Metodo que genera una firma de las secuencias de cada juego de ajedrez
	void ManejadorDatosChess::GenerarFirmaInterna()
	{
		// Generar la firma interna
		firma = GenerarFirma(cadena); // Generar la firma de la cadena
	}

	// Metodo que convierte una cadena en coordenadas de las piezas
	DatosChessGame ManejadorDatosChess::ConvertirCadena(const std::string& cadena)
	{
		if(isCompressed(cadena)) {
			return ConvertirCadena(decompressRLE(cadena));
		}
		// Convertir la cadena en coordenadas
		CadenaCoordenadas cc; // Cadena de coordenadas
		VectorTipoPieza vtp; // Vector de tipos de pieza

		for (size_t i = 0; i < cadena.length(); i+=2)
		{
			// Decodificar la jugada
			DatosBasicosJugada jugada = DecodificarJugada(cadena.substr(i, 2));
			cc.push_back(jugada.coordenadas); // Agregar las coordenadas
			vtp.push_back(jugada.tipoPieza); // Agregar el tipo de pieza
		}

		return {cc,vtp}; // Devolver las coordenadas
	}

	// Metodo que convierte las coordenadas de las piezas en una cadena
	std::string ManejadorDatosChess::ConvertirJuego(const DatosChessGame& juego)
	{
		// Convertir las coordenadas en una cadena
		std::string cadena; // Cadena de texto
		for (auto jugada : juego.jugadas)
		{
			cadena += EncodificarJugada(jugada); // Codificar la jugada y agregarla a la cadena
		}
		return cadena; // Devolver la cadena
	}

	// Metodos del ManejadorDatosChess << Fin

}

