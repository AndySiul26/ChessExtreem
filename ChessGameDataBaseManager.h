#pragma once
#ifndef CHESS_GAME_DATA_BASE_MANAGER
#define CHESS_GAME_DATA_BASE_MANAGER

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

class ChessGameDatabase {
private:
    const std::string BASE_FILENAME = "dataPartidas";
    const std::string FILE_EXTENSION = ".txt";
    const int MAX_GAMES_PER_FILE = 300;
    std::set<std::string> uniqueSignatures;

    std::string getNextFileName() {
        int fileIndex = 0;
        std::string fileName;
        do {
            fileName = BASE_FILENAME + std::to_string(fileIndex).insert(0, 3 - std::to_string(fileIndex).length(), '0') + FILE_EXTENSION;
            fileIndex++;
        } while (fs::exists(fileName) && getGameCount(fileName) >= MAX_GAMES_PER_FILE);
        return fileName;
    }

    int getGameCount(const std::string& fileName) {
        std::ifstream file(fileName);
        return std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
    }

    void loadUniqueSignatures() {
        for (const auto& entry : fs::directory_iterator(".")) {
            if (entry.path().extension() == FILE_EXTENSION && entry.path().stem().string().find(BASE_FILENAME) == 0) {
                std::ifstream file(entry.path());
                std::string line;
                while (std::getline(file, line)) {
                    size_t colonPos = line.find(':');
                    if (colonPos != std::string::npos) {
                        uniqueSignatures.insert(line.substr(0, colonPos));
                    }
                }
            }
        }
    }

public:
    ChessGameDatabase() {
        loadUniqueSignatures();
    }

    bool addGame(const std::string& signature, const std::string& compressedMoves) {
        if (uniqueSignatures.find(signature) != uniqueSignatures.end()) {
            return false;  // La secuencia ya existe
        }

        std::string fileName = getNextFileName();
        std::ofstream file(fileName, std::ios::app);
        file << signature << ":" << compressedMoves << std::endl;
        uniqueSignatures.insert(signature);
        return true;
    }
};

#endif
