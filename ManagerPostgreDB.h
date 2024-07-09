#pragma once
#ifndef MANAGER_POSTGRE_DB_H
#define MANAGER_POSTGRE_DB_H

#include <iostream>
#include <pqxx/pqxx>
// Para leer archivos de configuración
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <filesystem>
#include <algorithm>

// Función para leer el archivo de configuración
std::map<std::string, std::string> readConfigFile(const std::string& filename) {

    if (!std::filesystem::exists(filename)) {
		std::cerr << "No se encontró el archivo de configuración: " << filename << std::endl;
		return {};
	}

    std::map<std::string, std::string> config;
    std::ifstream file(filename);
    // Comprobar si existe el archivo


    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo de configuración: " << filename << std::endl;
        return config;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Ignorar líneas vacías y comentarios
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream lineStream(line);
        std::string key;
        if (std::getline(lineStream, key, '=')) {
            std::string value;
            if (std::getline(lineStream, value)) {
                config[key] = value;
            }
        }
    }

    file.close();
    return config;
}


class ChessDatabase {
private:
    pqxx::connection conn;

public:
    ChessDatabase(const std::string& connection_string)
        : conn(connection_string) {}

    void insertOrUpdateGame(const std::string& firma, const std::string& secuencia, const std::string& jugadores) {
        pqxx::work txn(conn);
        txn.exec_params(
            "SELECT insertar_actualizar_jugada($1, $2, $3)",
            firma, secuencia, jugadores);
        txn.commit();
    }

    std::string getFullSequence(const std::string& firma) {
        pqxx::work txn(conn);
        pqxx::result r = txn.exec_params(
            "SELECT obtener_secuencia_completa($1)",
            firma);
        return r[0][0].as<std::string>();
    }

    int getGameId(const std::string& firma) {
		pqxx::work txn(conn);
		pqxx::result r = txn.exec_params(
			"SELECT obtener_id_por_firma($1)",
			firma);
		return r[0][0].as<int>();
	}

    void addBestGame(int jugadaId, const std::string& razon) {
        pqxx::work txn(conn);
        txn.exec_params(
            "SELECT anadir_mejor_jugada($1, $2)",
            jugadaId, razon);
        txn.commit();
    }

    void addBestGame(const std::string& firma, const std::string& razon) {
		int jugadaId = getGameId(firma);
		addBestGame(jugadaId, razon);
	}
};

int testPostgre() {
    setlocale(LC_ALL, "ES_MX.utf-8");
    try {
        // Leer archivo de configuración
        std::map<std::string, std::string> config = readConfigFile("DataBase\\conexion.txt");
        std::string connection_string;
        for(const auto& [key, value] : config) {
			connection_string += key + "=" + value + " ";
		}

        ChessDatabase db(connection_string);

        // Ejemplo de uso
        db.insertOrUpdateGame("ABC123", "A1B2C3D4E5", "JUGADOR-JUGADOR");
        std::string secuencia = db.getFullSequence("ABC123");
        std::cout << "Secuencia recuperada: " << secuencia << std::endl;

        // Añadir a mejores jugadas (asumiendo que conocemos el ID)
        db.addBestGame("ABC123", "Estrategia excepcional");
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}


#endif // !MANAGER_POSTGRE_DB_H
