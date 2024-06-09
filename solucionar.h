#pragma once
#ifndef SOLUCIONAR_H
#define SOLUCIONAR_H

#include <vector>
#include <iostream>
#include <cstring>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <ctime>
#include <cstdlib>
#include <limits>

#define s_imprimir_con_salto(...) do { std::cout << __VA_ARGS__ << '\n'; } while(0)
#define s_imprimir std::cout << 
#define s_imprimir_salto std::cout << '\n';
#define s_asigna std::cin >>

namespace Solucionar
{

    template<typename T>
    T input(const char* mensaje)
    {
        T x;
        std::cout << mensaje;
        std::cin >> x;
        return x;
    }

    bool estaEnRango(int valor, int rangoInicio, int rangoFin) {
        return (valor >= rangoInicio && valor <= rangoFin);
    }

    bool cadenaConstVacia(const char* cadena) {
        return std::strcmp(cadena, "") == 0;
    }


    typedef int(*funcion_i)();
    using vector_fn_i = std::vector<funcion_i>;

    // Construcción de range iterador

    class Rango {
    private:
        int r_inicio;
        int r_fin;
        int r_paso;

    public:
        class Iterador {
        private:
            int i_valor;
            int i_paso;
            int i_fin;

        public:
            Iterador(int valor, int paso, int fin) : i_valor(valor), i_paso(paso), i_fin(fin) {}

            int operator*() const {
                return i_valor;
            }

            Iterador& operator++() {
                i_valor += i_paso;
                return *this;
            }

            bool operator!=(const Iterador& otro) const {
                if (i_paso > 0) {
                    return i_valor < otro.i_fin;
                }
                else {
                    return i_valor > otro.i_fin;
                }
            }
        };

        Rango(int inicio, int fin, int paso = 1)
            : r_inicio(inicio), r_fin((fin < 0 || fin < inicio) ? fin - 1 : fin + 1), r_paso{ (inicio > fin && paso > 0) ? -1 * paso : paso }
        {
            assert(r_paso != 0);
        }

        Rango(int n_elementos) : r_inicio(0), r_fin(n_elementos), r_paso(1) {}

        Iterador begin() {
            return Iterador(r_inicio, r_paso, r_fin);
        }

        Iterador end() {
            if (r_paso > 0) {
                return Iterador(r_fin, r_paso, r_fin);
            }
            else {
                return Iterador(r_fin + 1, r_paso, r_fin);
            }
        }
    };

    // Base case for variadic template recursion
    template <typename T>
    bool compara(const T& first) {
        return false; // No more arguments to compare, return false
    }

    // Recursive case for variadic template
    template <typename T, typename... Args>
    bool compara(const T& first, const T& second, const Args&... args) {
        if (first == second) {
            return true; // Found a match
        }
        return compara(first, args...); // Recur with the remaining arguments
    }

    template <typename T, typename... Args>
    bool ExistenRepeticiones(T first, Args... args) {
        std::unordered_set<T> uniqueValues;
        uniqueValues.insert(first);
        (uniqueValues.insert(args), ...);
        return uniqueValues.size() != sizeof...(args) + 1;
    }

    // Función para generar un número aleatorio entre 1 y 3
    int generarNumeroAleatorio(unsigned int Ub) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        return std::rand() % (Ub+1) + 1;
    }

}

#endif // !SOLUCIONAR_H
