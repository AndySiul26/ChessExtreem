﻿#pragma once
#ifndef CHESS_EXTREEM_H
#define CHESS_EXTREEM_H

#include<vector>
#include<memory>
#include<utility>
#include<array>
#include<algorithm>
#include<cmath>
#include<iostream>
#include<unordered_map> // Incluir la librería necesaria para std::unordered_map
#include <functional>
#include "solucionar.h"

namespace ChessExtreem
{
    enum class TipoBando
    {
        Nulo,
        Blancas,
        Negras
    };

    // <- SECCIÓN DE REGLAS DEL AJEDREZ
    // Enumeración para representar las posibles violaciones de movimiento de una pieza
    enum class ValidacionMovimiento
    {
        Valido_PosicionLibre,	            // El movimiento es válido
        Valido_CapturaPiezaEnemiga,         // El movimiento es válido, captura una pieza enemiga
        Valido_MovimientoCompuesto,         // El movimiento es válido, se mueve junto con otra pieza
        Valido_Coronacion,                  // El movimiento es válido, es peon y se corona
        Prohibido_MovimientoAunNoValidado,	// El movimiento no ha sido validado aún
        Prohibido_NoHayPiezaEnOrigen,       // Cuando se señala un cuadro vacío para mover 
        Prohibido_PiezaMismoBando,	        // La pieza destino es del mismo bando
        Prohibido_ReyEnJaque,		        // El movimiento pondría al rey en jaque
        Prohibido_PosicionObstruida,	        // La posición destino no es válida ni aprobada por movimientos básicos                        
        Prohibido_PosicionLibrePeroInvalida,	        // La posición destino no es válida ni aprobada por movimientos básicos                        

    };

    enum class TipoMovimientoEspecial
    {
        Nulo,               // Movimiento Especial no asignado
        PeonSalidaDoble,    // Movimiento de peón de dos casillas desde su posición inicial
        PeonCaptura,        // Captura de un peón enemigo
        PeonCoronacion,     // Coronación de un peón cuando llega a la última fila
        PeonCapturaAlPaso,  // Captura al paso
        ReyEnroqueCorto,    // Enroque corto (con la torre de rey)
        ReyEnroqueLargo     // Enroque largo (con la torre de dama)
    };

    class Validacion {
    private:
        bool V_Valido;
        ValidacionMovimiento V_TipoValidacion;
        TipoMovimientoEspecial V_TipoEspecial;

    public:

        Validacion(bool valido, ValidacionMovimiento tipo_validacion, TipoMovimientoEspecial tipo_especial)
            : V_Valido{ valido }, V_TipoValidacion{ tipo_validacion }, V_TipoEspecial{ tipo_especial } 
        {}

        Validacion()
            : V_Valido(false),
            V_TipoValidacion(ValidacionMovimiento::Prohibido_MovimientoAunNoValidado),
            V_TipoEspecial(TipoMovimientoEspecial::Nulo)
        {}

        // Constructor de copia
        Validacion(const Validacion& otro) noexcept
            : V_Valido{ otro.V_Valido }, 
            V_TipoValidacion{ otro.V_TipoValidacion},
            V_TipoEspecial{otro.V_TipoEspecial}
        {}

        // Constructor de movimiento
        Validacion(Validacion&& otro) noexcept
            : V_Valido{ std::move(otro.V_Valido) }, 
            V_TipoValidacion{ std::move(otro.V_TipoValidacion) },
            V_TipoEspecial{std::move(otro.V_TipoEspecial)}
        {}

        // Operador de asignación de movimiento
        Validacion& operator=(const Validacion& otro) noexcept {
            V_Valido = otro.V_Valido;
            V_TipoValidacion = otro.V_TipoValidacion;
            V_TipoEspecial = otro.V_TipoEspecial;
            return *this;
        }

        // Operador de asignación de movimiento
        Validacion& operator=(Validacion&& otro) noexcept {
            V_Valido = std::move(otro.V_Valido);
            V_TipoValidacion = std::move(otro.V_TipoValidacion);
            V_TipoEspecial = std::move(otro.V_TipoEspecial);
            return *this;
        }

        // SETTERS AND GETTERS
        void setValido(bool valido) { V_Valido = valido; }
        bool getValido() const { return V_Valido; }

        bool esEspecial() const
        {
            switch (V_TipoEspecial)
            {
            case ChessExtreem::TipoMovimientoEspecial::Nulo:
                return false;
                break;
            case ChessExtreem::TipoMovimientoEspecial::PeonSalidaDoble:
            case ChessExtreem::TipoMovimientoEspecial::PeonCaptura:
            case ChessExtreem::TipoMovimientoEspecial::PeonCoronacion:
            case ChessExtreem::TipoMovimientoEspecial::PeonCapturaAlPaso:
            case ChessExtreem::TipoMovimientoEspecial::ReyEnroqueCorto:
            case ChessExtreem::TipoMovimientoEspecial::ReyEnroqueLargo:
                return true;
            default:
                return false;
                break;
            }
        }

        void setTipoEspecial(TipoMovimientoEspecial tipo)
        {
            V_TipoEspecial = tipo;
            switch (V_TipoEspecial)
            {            
            case ChessExtreem::TipoMovimientoEspecial::PeonSalidaDoble:
                setTipoValidacion(ValidacionMovimiento::Valido_PosicionLibre);
                break;
            case ChessExtreem::TipoMovimientoEspecial::PeonCaptura:
                setTipoValidacion(ValidacionMovimiento::Valido_CapturaPiezaEnemiga);
                break;
            case ChessExtreem::TipoMovimientoEspecial::PeonCoronacion:
                setTipoValidacion(ValidacionMovimiento::Valido_Coronacion);
                break;
            case ChessExtreem::TipoMovimientoEspecial::PeonCapturaAlPaso:
                setTipoValidacion(ValidacionMovimiento::Valido_CapturaPiezaEnemiga);
                break;
            case ChessExtreem::TipoMovimientoEspecial::ReyEnroqueCorto:
                setTipoValidacion(ValidacionMovimiento::Valido_MovimientoCompuesto);
                break;
            case ChessExtreem::TipoMovimientoEspecial::ReyEnroqueLargo:
                setTipoValidacion(ValidacionMovimiento::Valido_MovimientoCompuesto);
                break;
            default:
                break;
            }
        }

        TipoMovimientoEspecial getTipoEspecial() const
        {
            return V_TipoEspecial;
        }

        bool esMovimientoObstruido() const 
        {
            
            switch (V_TipoValidacion)
            {
            case ChessExtreem::ValidacionMovimiento::Prohibido_PosicionObstruida:
            case ChessExtreem::ValidacionMovimiento::Prohibido_PiezaMismoBando:
            case ChessExtreem::ValidacionMovimiento::Valido_CapturaPiezaEnemiga:
                return true;
            default:
                return false;
            }
        }

        void setTipoValidacion(ValidacionMovimiento tipo_validacion) 
        {
            V_TipoValidacion = tipo_validacion; 
            switch (tipo_validacion)
            {
            case ChessExtreem::ValidacionMovimiento::Valido_PosicionLibre:
            case ChessExtreem::ValidacionMovimiento::Valido_CapturaPiezaEnemiga:
            case ChessExtreem::ValidacionMovimiento::Valido_MovimientoCompuesto:
            case ChessExtreem::ValidacionMovimiento::Valido_Coronacion:
                V_Valido = true;
                break;
            case ChessExtreem::ValidacionMovimiento::Prohibido_NoHayPiezaEnOrigen:
            case ChessExtreem::ValidacionMovimiento::Prohibido_MovimientoAunNoValidado:
            case ChessExtreem::ValidacionMovimiento::Prohibido_PiezaMismoBando:
            case ChessExtreem::ValidacionMovimiento::Prohibido_ReyEnJaque:
            case ChessExtreem::ValidacionMovimiento::Prohibido_PosicionObstruida:
            case ChessExtreem::ValidacionMovimiento::Prohibido_PosicionLibrePeroInvalida:
                V_Valido = false;
                break;
            default:
                break;
            }
        }
        const ValidacionMovimiento getTipoValidacion() const { return V_TipoValidacion; }
    };
    // SECCIÓN DE REGLAS DEL AJEDREZ ->

    // <- SECCION DE COORDENADAS
    struct Coordenadas {
        int x{ -1 }, y{ -1 }; // Default desasignadas

        friend bool operator==(const Coordenadas& c1, const Coordenadas& c2);
        friend bool operator!=(const Coordenadas& c1, const Coordenadas& c2);
        Coordenadas absC() const
        {
            return Coordenadas{ abs(x) ,abs(y) };
        }

        Coordenadas operator+(const Coordenadas& c1) const;
        Coordenadas operator-(const Coordenadas& c1) const;
        Coordenadas operator*(const Coordenadas& c1) const;
        Coordenadas operator*(int escalar) const;
        Coordenadas operator/(const Coordenadas& c1) const;
        Coordenadas operator/(int escalar) const;
    };

    // Sobrecarga de operadores para Coordenadas
    bool operator==(const Coordenadas& c1, const Coordenadas& c2) {
        return c1.x == c2.x && c1.y == c2.y; // Corrección
    }

    bool operator!=(const Coordenadas& c1, const Coordenadas& c2) {
        return !(c1 == c2); // Corrección
    }

    Coordenadas Coordenadas::operator+(const Coordenadas& c1) const {
        return { x + c1.x, y + c1.y };
    }

    Coordenadas Coordenadas::operator-(const Coordenadas& c1) const {
        return { x - c1.x, y - c1.y };
    }

    Coordenadas Coordenadas::operator*(const Coordenadas& c1) const {
        return { x * c1.x, y * c1.y };
    }

    Coordenadas Coordenadas::operator*(int escalar) const {
        return { x * escalar, y * escalar };
    }

    Coordenadas Coordenadas::operator/(const Coordenadas& c1) const {
        return { x / c1.x, y / c1.y };
    }

    Coordenadas Coordenadas::operator/(int escalar) const {
        return { x / escalar, y / escalar };
    }

    // Cambiar a movimiento compuesto (crear clase Movimiento solo debe tener origen y destino encapsulando coordenadas origen destino en esa clase)
    // CAMBIAR LOGICA...

    struct Movimiento
    {
        Coordenadas Origen;
        Coordenadas Destino;
        // Sobrecarga de operadores para Coordenadas
        friend bool operator==(const Movimiento& c1, const Movimiento& c2) {
            return c1.Origen == c2.Origen && c1.Destino == c2.Destino; // Corrección
        }

        friend bool operator!=(const Movimiento& c1, const Movimiento& c2) {
            return !(c1 == c2); // Corrección
        }
    };

    class MovimientoCompuesto
    {
    public:

        Movimiento MC_Movimiento;
        Movimiento MC_MovimientoPiezaRelacionada;
        Validacion MC_Validacion;

        MovimientoCompuesto() = default;
        MovimientoCompuesto(const Movimiento& mov, const Movimiento& mov_rel={}, bool especial = false, TipoMovimientoEspecial tipo = TipoMovimientoEspecial::Nulo)
            : MC_Movimiento{ mov}, MC_MovimientoPiezaRelacionada{ mov_rel} , MC_Validacion{false, ValidacionMovimiento::Prohibido_MovimientoAunNoValidado, tipo}
        {}

        // Constructor por copia
        MovimientoCompuesto(const MovimientoCompuesto& mc)
            : MC_Movimiento{ mc.MC_Movimiento}, MC_MovimientoPiezaRelacionada{ mc.MC_MovimientoPiezaRelacionada},
            MC_Validacion{ mc.MC_Validacion } {}

        // Constructor por movimiento
        MovimientoCompuesto(MovimientoCompuesto&& mc) noexcept
            :
            MC_Movimiento{ std::move(mc.MC_Movimiento) }, MC_MovimientoPiezaRelacionada{ std::move(mc.MC_MovimientoPiezaRelacionada) },
            MC_Validacion{ std::move(mc.MC_Validacion) } {}

        MovimientoCompuesto& operator=(const MovimientoCompuesto& mc) {
            MC_Movimiento = mc.MC_Movimiento;
            MC_MovimientoPiezaRelacionada = mc.MC_MovimientoPiezaRelacionada;
            
            return *this;
        }

        MovimientoCompuesto& operator=(MovimientoCompuesto&& mc) noexcept {
            MC_Movimiento = std::move(mc.MC_Movimiento);
            MC_MovimientoPiezaRelacionada = std::move(mc.MC_MovimientoPiezaRelacionada);
            return *this;
        }

    };
    
	using CadenaMovimientos = std::vector<MovimientoCompuesto>;
	using CadenaCoordenadas = std::vector<Coordenadas>;

    // Nota EX-- X = 0 A 7 -> CHAR [A -> H] -> AA = I [BUSCAR SOLUCIÓN DE HUELLAS DE SECUENCIAS DE MOVIMIENTOS]

    int indexCadenaMovimientos(const CadenaMovimientos& cm, const Coordenadas& c)
    {
        // Comprobamos si esta vacio
        if (cm.empty())
            return -1;

        // Buscar en la CadenaMovimientos
        CadenaMovimientos::const_iterator it;

        it = std::find_if(cm.begin(), cm.end(), [&c](const MovimientoCompuesto& c_) {
            return c.x == c_.MC_Movimiento.Destino.x && c.y == c_.MC_Movimiento.Destino.y;
            });


        if (it != cm.end())
            return std::distance(cm.begin(), it);


        // Default
        return -1; // Indicador de que no existe en la cadena de movimientos

    }

    // SECCION DE COORDENADAS ->

    // <- SECCIÓN DE TIPOS DE PIEZAS
    enum class TipoPieza {
        Torre,
        Caballo,
        Alfil,
        Dama,
        Rey,
        Peon
    };

    // Sobrecarga del operador << para TipoPieza
    std::ostream& operator<<(std::ostream& os, const TipoPieza& pieza) {
        switch (pieza) {
        case TipoPieza::Torre:
            os << "Torre";
            break;
        case TipoPieza::Caballo:
            os << "Caballo";
            break;
        case TipoPieza::Alfil:
            os << "Alfil";
            break;
        case TipoPieza::Dama:
            os << "Dama";
            break;
        case TipoPieza::Rey:
            os << "Rey";
            break;
        case TipoPieza::Peon:
            os << "Peon";
            break;
        default:
            os << "Tipo de pieza desconocido";
            break;
        }
        return os;
    }

    // Clase base para las piezas
    class Pieza
    {

    protected:

        Coordenadas P_posicion;
        bool P_bandoBlancas;
        bool P_visible;
        Coordenadas P_otraPiezaRelacionada; // Utiliza shared_ptr para la relación de propiedad 		
        CadenaMovimientos P_movimientosValidados; // Los movimientos deben ser eliminados cuando se mueva la pieza realmente
        CadenaMovimientos P_movimientosPasados; // Movimientos donde ha estado la pieza
        int P_indiceBando;

    public:

        // Constructores
        Pieza() = default;

        // Constructor de copia
        Pieza(const Pieza& otraPieza) = default;

        Pieza(const Coordenadas& posicion, const int indiceBando, const bool bandoBlancas, const bool visible,
            const Coordenadas& otraPiezaRelacionada = {},
            const CadenaMovimientos& movimientosValidados = {}, const CadenaMovimientos& movimientosPasados = {}) :
            P_posicion{ posicion }, P_bandoBlancas{ bandoBlancas }, P_visible{ visible },
            P_otraPiezaRelacionada{ otraPiezaRelacionada }, P_movimientosValidados{ movimientosValidados },
            P_indiceBando{ indiceBando }, P_movimientosPasados{movimientosPasados} {}

        void SetIndiceBando(int indice) { P_indiceBando = indice; }
        int GetIndiceBando() const {return P_indiceBando; }

        // Destructores
        virtual ~Pieza() = default; // Destructor virtual para permitir la destrucción de subclases

        // Función virtual pura para clonar
        virtual std::shared_ptr<Pieza> clonar() const = 0;

        // [FUNCIONES VIRTUALES PURAS]
        // Calculo de movimientos básicos de la pieza (Validando simplemente que no se salgan del tablero)

        virtual void calcularMovimientos(const std::function<void(MovimientoCompuesto&)>& fncValidador= {}) = 0;
        virtual TipoPieza obtenerTipo() const = 0;

        // Métodos de acceso (getters y setters)
        const Coordenadas& getPosicion() const { return P_posicion; }

        // Versiones distintas de setPosicion
        void setPosicion(const Coordenadas& destino)
        {
            P_posicion = destino;
        }

        void guardarMovimientoCompuesto(const MovimientoCompuesto& movimiento)
        {
            P_movimientosPasados.push_back(movimiento);            
        }

        bool getBandoBlancas() const { return P_bandoBlancas; }
        void setBandoBlancas(bool nuevoBando) { P_bandoBlancas = nuevoBando; }

        TipoBando getBando() const 
        { 
            return (P_bandoBlancas) ? TipoBando::Blancas : TipoBando::Negras;
        }

        const Coordenadas& getOtraPiezaRelacionada() const { return P_otraPiezaRelacionada; }
        void setOtraPiezaRelacionada(Coordenadas& nuevaPiezaRelacionada) { P_otraPiezaRelacionada = nuevaPiezaRelacionada; }

        // Movimientos posibles 
        CadenaMovimientos& getMovimientos()
        {
            //if (P_movimientosValidados.empty())
            //    calcularMovimientos();
            return P_movimientosValidados;
        }

        // Movimientos realizados
        const CadenaMovimientos& getMovimientosPasados() const { return P_movimientosPasados; }
        // void setMovimientosValidos(const CadenaMovimientos& nuevosMovimientos) { movimientosValidos = nuevosMovimientos; }

        int IndexMovimientoValidado(Movimiento& mov)
        {
            if (P_movimientosValidados.empty()) return -1;
            
            for (int i{ 0 }; auto & temp_mov : P_movimientosValidados)
            {
                if (temp_mov.MC_Movimiento == mov && temp_mov.MC_Validacion.getValido()) return i;
                ++i;
            }

            return -1;
        }

        MovimientoCompuesto MovimientoPosible(int index) const
        {
            if (index < 0 || index >= P_movimientosValidados.size()) return {};

            return P_movimientosValidados[index];
        }

        void LimpiarMovimientosPosibles()
        {
            if (!P_movimientosValidados.empty()) P_movimientosValidados.clear();
        }
    };

    class Torre : public Pieza
    {
    private:
    public:
        Torre() = default;

        Torre(const Coordenadas& posicion, int indiceBando, bool bandoBlancas, bool visible,
            const Coordenadas& otraPiezaRelacionada = {},
            const CadenaMovimientos& movimientosValidados = {})
            : Pieza(posicion, indiceBando, bandoBlancas, visible, otraPiezaRelacionada, movimientosValidados) {}

        TipoPieza obtenerTipo() const override
        {
            return TipoPieza::Torre;
        }

        std::shared_ptr<Pieza> clonar() const override {
            return std::make_shared<Torre>(*this);
        }

        void calcularMovimientos(const std::function<void(MovimientoCompuesto&)>& fncValidador = {}) override {
            P_movimientosValidados.clear();

            auto procesarMovimiento = [&](int dx, int dy) {
                for (int i = 1; ; ++i) {
                    int nuevaX = P_posicion.x + i * dx;
                    int nuevaY = P_posicion.y + i * dy;
                    if (nuevaX < 0 || nuevaX >= 8 || nuevaY < 0 || nuevaY >= 8) break;

                    MovimientoCompuesto movimiento{ { P_posicion, {nuevaX, nuevaY} } };
                    if (fncValidador) {
                        fncValidador(movimiento);
                    }

                    bool valido = movimiento.MC_Validacion.getValido();
                    bool obstruido = movimiento.MC_Validacion.esMovimientoObstruido();

                    if (valido) {
                        P_movimientosValidados.push_back(std::move(movimiento));
                    }

                    if (obstruido) break;
                }
                };

            // Procesar todas las direcciones horizontales y verticales
            procesarMovimiento(1, 0);  // Derecha
            procesarMovimiento(-1, 0); // Izquierda
            procesarMovimiento(0, 1);  // Arriba
            procesarMovimiento(0, -1); // Abajo
        }


    };

    class Alfil : public Pieza
    {
    public:
        Alfil() = default;

        Alfil(const Coordenadas& posicion, int indiceBando, bool bandoBlancas, bool visible,
            const Coordenadas& otraPiezaRelacionada = {},
            const CadenaMovimientos& movimientosValidados = {})
            : Pieza(posicion, indiceBando, bandoBlancas, visible, otraPiezaRelacionada, movimientosValidados) {}

        TipoPieza obtenerTipo() const override
        {
            return TipoPieza::Alfil;
        }

        std::shared_ptr<Pieza> clonar() const override {
            return std::make_shared<Alfil>(*this);
        }

        void calcularMovimientos(const std::function<void(MovimientoCompuesto&)>& fncValidador = {}) override {
            P_movimientosValidados.clear();

            auto procesarMovimiento = [&](int dx, int dy) {
                for (int i = 1; ; ++i) {
                    int nuevaX = P_posicion.x + i * dx;
                    int nuevaY = P_posicion.y + i * dy;
                    if (nuevaX < 0 || nuevaX >= 8 || nuevaY < 0 || nuevaY >= 8) break;

                    MovimientoCompuesto movimiento{ { P_posicion, {nuevaX, nuevaY} } };
                    if (fncValidador) {
                        fncValidador(movimiento);
                    }

                    // Verificar si el movimiento es válido y si está obstruido antes de moverlo al vector
                    bool valido = movimiento.MC_Validacion.getValido();
                    bool obstruido = movimiento.MC_Validacion.esMovimientoObstruido();

                    if (valido) {
                        P_movimientosValidados.push_back(std::move(movimiento));
                    }

                    if (obstruido) break;
                }
                };

            // Procesar todas las direcciones diagonales
            procesarMovimiento(1, 1);  // Diagonal superior derecha
            procesarMovimiento(-1, 1); // Diagonal superior izquierda
            procesarMovimiento(1, -1); // Diagonal inferior derecha
            procesarMovimiento(-1, -1);// Diagonal inferior izquierda
        }
    };

    class Peon : public Pieza
    {
    public:
        Peon() = default;

        Peon(const Coordenadas& posicion, int indiceBando, bool bandoBlancas, bool visible,
            const Coordenadas& otraPiezaRelacionada = {},
            const CadenaMovimientos& movimientosValidados = {})
            : Pieza(posicion, indiceBando, bandoBlancas, visible, otraPiezaRelacionada, movimientosValidados) {}

        TipoPieza obtenerTipo() const override
        {
            return TipoPieza::Peon;
        }

        std::shared_ptr<Pieza> clonar() const override {
            return std::make_shared<Peon>(*this);
        }

        void calcularMovimientos(const std::function<void(MovimientoCompuesto&)>& fncValidador= {}) override
        {
            P_movimientosValidados.clear();

            int direction = P_bandoBlancas ? 1 : -1; // Determina la dirección de avance del peón
            bool ultimoMovimientoObstruido{ false }; // Punto de interrupción para bucles o validaciones posteriores

            // Avance de un paso
            if (P_posicion.y + direction >= 0 && P_posicion.y + direction < 8) {
                MovimientoCompuesto movimiento{ { P_posicion, {P_posicion.x, P_posicion.y + direction} } };
                if (fncValidador != nullptr) {
                    fncValidador(movimiento);
                }
                ultimoMovimientoObstruido = movimiento.MC_Validacion.esMovimientoObstruido();
                P_movimientosValidados.push_back(movimiento);
            }

            // Agregar comprobaciones de validación de ultimo movimiento validado para evitar 
            // agregar movimientos en casillas posteriores de un cierto punto vertical, horizontal o diagonal
            
            // Primer movimiento de dos pasos
            if ((!ultimoMovimientoObstruido) && (P_posicion.y == 1 && P_bandoBlancas) || (P_posicion.y == 6 && !P_bandoBlancas)) {
                if (P_posicion.y + 2 * direction >= 0 && P_posicion.y + 2 * direction < 8) {
                    MovimientoCompuesto movimiento{ { P_posicion, {P_posicion.x, P_posicion.y + 2 * direction} },{}, true, TipoMovimientoEspecial::PeonSalidaDoble };
                    if (fncValidador != nullptr) {
                        fncValidador(movimiento);
                    }
                    P_movimientosValidados.push_back(movimiento);
                }
            }

            // Ataques en diagonal
            if (P_posicion.x + 1 < 8 && P_posicion.y + direction >= 0 && P_posicion.y + direction < 8) {
                MovimientoCompuesto movimiento{ { P_posicion, {P_posicion.x + 1, P_posicion.y + direction} } , {}, true, TipoMovimientoEspecial::PeonCaptura};
                if (fncValidador != nullptr) {
                    fncValidador(movimiento);
                }
                P_movimientosValidados.push_back(movimiento);
            }
            if (P_posicion.x - 1 >= 0 && P_posicion.y + direction >= 0 && P_posicion.y + direction < 8) {
                MovimientoCompuesto movimiento{ { P_posicion, {P_posicion.x - 1, P_posicion.y + direction} } , {}, true , TipoMovimientoEspecial::PeonCaptura };
                if (fncValidador != nullptr) {
                    fncValidador(movimiento);
                }
                P_movimientosValidados.push_back(movimiento);
            }
        }
    };

    class Rey : public Pieza
    {
    public:
        Rey() = default;

        Rey(const Coordenadas& posicion, int indiceBando, bool bandoBlancas, bool visible,
            const Coordenadas& otraPiezaRelacionada = {},
            const CadenaMovimientos& movimientosValidados = {})
            : Pieza(posicion, indiceBando, bandoBlancas, visible, otraPiezaRelacionada, movimientosValidados) {}

        TipoPieza obtenerTipo() const override
        {
            return TipoPieza::Rey;
        }

        std::shared_ptr<Pieza> clonar() const override {
            return std::make_shared<Rey>(*this);
        }

        void calcularMovimientos(const std::function<void(MovimientoCompuesto&)>& fncValidador = {}) override {
            P_movimientosValidados.clear();

            // Movimientos normales del Rey (una casilla en cualquier dirección)
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx != 0 || dy != 0) {
                        int nx = P_posicion.x + dx;
                        int ny = P_posicion.y + dy;

                        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
                            MovimientoCompuesto movimiento{ { P_posicion, {nx, ny} } };

                            if (fncValidador) {
                                fncValidador(movimiento);
                            }

                            if (movimiento.MC_Validacion.getValido()) {
                                P_movimientosValidados.push_back(std::move(movimiento));
                            }
                        }
                    }
                }
            }

            // Considerar enroque (corto y largo)
            auto considerarEnroque = [&](int dx) {
                bool obstruido = false;
                for (int i = 1; i <= 2; ++i) {
                    int nx = P_posicion.x + i * (dx / 2);
                    int ny = P_posicion.y;

                    MovimientoCompuesto movimiento{ { P_posicion, {nx, ny} }, {}, true,
                        (dx > 0 ? TipoMovimientoEspecial::ReyEnroqueCorto : TipoMovimientoEspecial::ReyEnroqueLargo) };

                    if (fncValidador) {
                        fncValidador(movimiento);
                    }

                    if (!movimiento.MC_Validacion.getValido() || movimiento.MC_Validacion.esMovimientoObstruido()) {
                        obstruido = true;
                        break;
                    }
                }

                if (!obstruido) {
                    int enroqueX = P_posicion.x + dx;
                    int enroqueY = P_posicion.y;

                    MovimientoCompuesto enroqueMovimiento{ { P_posicion, {enroqueX, enroqueY} }, {}, true,
                        (dx > 0 ? TipoMovimientoEspecial::ReyEnroqueCorto : TipoMovimientoEspecial::ReyEnroqueLargo) };

                    if (fncValidador) {
                        fncValidador(enroqueMovimiento);
                    }

                    if (enroqueMovimiento.MC_Validacion.getValido()) {
                        P_movimientosValidados.push_back(std::move(enroqueMovimiento));
                    }
                }
                };

            // Enroque corto y largo
            considerarEnroque(2);  // Enroque corto
            considerarEnroque(-2); // Enroque largo
        }
    };

    class Dama : public Pieza
    {
    public:
        Dama() = default;

        Dama(const Coordenadas& posicion, int indiceBando, bool bandoBlancas, bool visible,
            const Coordenadas& otraPiezaRelacionada = {},
            const CadenaMovimientos& movimientosValidados = {})
            : Pieza(posicion, indiceBando, bandoBlancas, visible, otraPiezaRelacionada, movimientosValidados) {}

        TipoPieza obtenerTipo() const override
        {
            return TipoPieza::Dama;
        }

        std::shared_ptr<Pieza> clonar() const override {
            return std::make_shared<Dama>(*this);
        }

        void calcularMovimientos(const std::function<void(MovimientoCompuesto&)>& fncValidador= {}) override
        {
            P_movimientosValidados.clear();

            // Creamos objetos temporales de Torre y Alfil en la posición de la Dama
            Torre torreTemp;
            torreTemp.setPosicion(P_posicion);
            Alfil alfilTemp;
            alfilTemp.setPosicion(P_posicion);

            // Obtenemos los movimientos de la Torre y los combinamos con los movimientos del Alfil
            torreTemp.calcularMovimientos(fncValidador);
            alfilTemp.calcularMovimientos(fncValidador);

            for (const auto& move : torreTemp.getMovimientos()) {
                P_movimientosValidados.push_back(move);
            }
            for (const auto& move : alfilTemp.getMovimientos()) {
                P_movimientosValidados.push_back(move);
            }
        }
    };

    class Caballo : public Pieza
    {
    public:
        Caballo() = default;

        Caballo(const Coordenadas& posicion, int indiceBando, bool bandoBlancas, bool visible,
            const Coordenadas& otraPiezaRelacionada = {},
            const CadenaMovimientos& movimientosValidados = {})
            : Pieza(posicion, indiceBando, bandoBlancas, visible, otraPiezaRelacionada, movimientosValidados) {}


        TipoPieza obtenerTipo() const override
        {
            return TipoPieza::Caballo;
        }

        std::shared_ptr<Pieza> clonar() const override {
            return std::make_shared<Caballo>(*this);
        }
        
        void calcularMovimientos(const std::function<void(MovimientoCompuesto&)>& fncValidador= {}) override
        {
            P_movimientosValidados.clear();

            // Movimientos del caballo en forma de L
            int dx[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };
            int dy[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };

            for (int i = 0; i < 8; ++i) {
                int nx = P_posicion.x + dx[i];
                int ny = P_posicion.y + dy[i];
                if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
                    MovimientoCompuesto movimiento{ { P_posicion, {nx, ny} } };

                    if (fncValidador != nullptr) {
                        fncValidador(movimiento);
                    }
                    P_movimientosValidados.push_back(movimiento);
                }
            }
        }
    };

    struct Coordenadas_InfoExtra : public Coordenadas
    {
        TipoPieza CIE_tipoPieza;
        explicit Coordenadas_InfoExtra(const Coordenadas& coordenadas, const TipoPieza tipo_pieza)
        : Coordenadas{coordenadas}, CIE_tipoPieza(tipo_pieza) {}
        
    };
    // SECCION DE TIPOS DE PIEZAS ->
    
    // <- SECCIÓN DE ADMINISTRACIÓN INTERNA DEL JUEGO 
    // Define el tipo de array para representar el tablero de ajedrez con std::shared_ptr
    using TableroAjedrez = std::array<std::array<std::shared_ptr<Pieza>, 8>, 8>;

    // Define una enumeración para los posibles estados del juego
    enum class Estado {
        SinInicializar,
        Inicializado,
        Activo,
        Tablas,
        JaqueBlancas,
        JaqueNegras,
        JaqueMateBlancas,
        JaqueMateNegras,
        Ahogado,
        RepeticionTriple,
    };

    // Define la clase EstadoJuego para representar el estado del juego
    class EstadoJuego {
    private:
        Estado EJ_estado;
        TipoBando EJ_turno;
        bool EJ_Cambiado;
    protected:
        void setTurno(TipoBando bando) { EJ_turno = bando; }

        // Establece a true el estado de cambiado para el juego
        void setCambiado() { EJ_Cambiado = true; }
                
        void setEstado(Estado nuevoEstado) {
            EJ_estado = nuevoEstado;
        }

        void cambiarTurno()
        {
            EJ_turno = (EJ_turno == TipoBando::Blancas) ? TipoBando::Negras : TipoBando::Blancas;
        }

        friend class Juego;

    public:
        // Constructor por defecto
        EstadoJuego() : EJ_estado(Estado::SinInicializar), EJ_turno{ TipoBando::Nulo }, EJ_Cambiado{ false } {}

        Estado getEstado() const {
            return EJ_estado;
        }

        // Al obtener el estado de cambiado, este al leerse cambiará a false automaticamente para solo informar un estado actual
        bool getCambiado() 
        {
            bool tempCambiado{ EJ_Cambiado };
            EJ_Cambiado = false;
            return tempCambiado;
        }

        TipoBando getTurno() const { return EJ_turno; }

        bool esTurnoBlancas() const noexcept { return EJ_turno == TipoBando::Blancas; }
        
    };

    using Coordenadas_InfoExtra_Piezas = std::vector<Coordenadas_InfoExtra>;

    class Bando
    {
    private:
        bool B_bandoBlancas;

        CadenaCoordenadas B_reyes;
        CadenaCoordenadas B_damas;
        CadenaCoordenadas B_peones;
        CadenaCoordenadas B_caballos;
        CadenaCoordenadas B_alfiles;
        CadenaCoordenadas B_torres;
    public:
        Coordenadas_InfoExtra_Piezas B_piezas;

        explicit Bando(bool bandoBlancas = false) : B_bandoBlancas{ bandoBlancas } {}

        void AgregarPieza(const Coordenadas_InfoExtra& coordenadasIE, TableroAjedrez& tablero)
        {
            Coordenadas c{ coordenadasIE };
            switch (coordenadasIE.CIE_tipoPieza)
            {
            case TipoPieza::Torre:
                B_torres.push_back(c);
                tablero.at(c.x).at(c.y) = std::make_shared<Torre>(c, B_torres.size() - 1, B_bandoBlancas, true);
                break;
            case TipoPieza::Caballo:
                B_caballos.push_back(c);
                tablero.at(c.x).at(c.y) = std::make_shared<Caballo>(c, B_caballos.size() - 1, B_bandoBlancas, true);
                break;
            case TipoPieza::Alfil:
                B_alfiles.push_back(c);
                tablero.at(c.x).at(c.y) = std::make_shared<Alfil>(c, B_alfiles.size() - 1, B_bandoBlancas, true);
                break;
            case TipoPieza::Dama:
                B_damas.push_back(c);
                tablero.at(c.x).at(c.y) = std::make_shared<Dama>(c, B_damas.size() - 1, B_bandoBlancas, true);
                break;
            case TipoPieza::Rey:
                B_reyes.push_back(c);
                tablero.at(c.x).at(c.y) = std::make_shared<Rey>(c, B_reyes.size() - 1, B_bandoBlancas, true);
                break;
            case TipoPieza::Peon:
                B_peones.push_back(c);
                tablero.at(c.x).at(c.y) = std::make_shared<Peon>(c, B_peones.size() - 1, B_bandoBlancas, true);
                break;
            default:
                break;
            }

            // Agregamos a la lista general de piezas del bando
            B_piezas.push_back(coordenadasIE);
        }

        void CrearBando(TableroAjedrez& tablero)
        {
            const int i_lado = (B_bandoBlancas) ? 0 : 7;
            const int i_add = (B_bandoBlancas) ? 1 : -1;



            // Creamos Torres
            AgregarPieza(Coordenadas_InfoExtra{ {0, i_lado} ,TipoPieza::Torre }, tablero);
            AgregarPieza(Coordenadas_InfoExtra{ {7, i_lado}, TipoPieza::Torre }, tablero);

            // Creamos Caballos
            AgregarPieza(Coordenadas_InfoExtra{ {1, i_lado}, TipoPieza::Caballo }, tablero);
            AgregarPieza(Coordenadas_InfoExtra{ {6, i_lado}, TipoPieza::Caballo }, tablero);

            // Creamos Alfiles
            AgregarPieza(Coordenadas_InfoExtra{ { 2, i_lado}, TipoPieza::Alfil }, tablero);
            AgregarPieza(Coordenadas_InfoExtra{ {5, i_lado}, TipoPieza::Alfil }, tablero);

            // Creamos Dama y Rey
            AgregarPieza(Coordenadas_InfoExtra{ { 3, i_lado}, TipoPieza::Dama }, tablero);
            AgregarPieza(Coordenadas_InfoExtra{ { 4, i_lado}, TipoPieza::Rey }, tablero);

            // Creamos Peones
            for (int x = 0; x < 8; ++x)
            {
                AgregarPieza(Coordenadas_InfoExtra{ { x, i_lado + i_add}, TipoPieza::Peon }, tablero);
            }
        }

        // const Coordenadas& getCoordenadas(const TipoPieza tipo, int indice)
        CadenaCoordenadas& operator[](TipoPieza tipo) 
        {
            switch (tipo)
            {
            case ChessExtreem::TipoPieza::Torre:
                return B_torres;
            case ChessExtreem::TipoPieza::Caballo:
                return B_caballos;
            case ChessExtreem::TipoPieza::Alfil:
                return B_alfiles;
            case ChessExtreem::TipoPieza::Dama:
                return B_damas;
            case ChessExtreem::TipoPieza::Rey:
                return B_reyes;
            case ChessExtreem::TipoPieza::Peon:
                return B_peones;
            default:
                return B_peones;
            }
        }

    };


    // Define la clase Juego para representar el juego de ajedrez
    class Juego {
    private:
        TableroAjedrez J_tablero;
        std::shared_ptr<Pieza> casillaNula;

        std::shared_ptr<Pieza>& ObtenerPieza(const Coordenadas& coordenadas)
        {
            if (SonCoordenadasValidas(coordenadas))
                return J_tablero[coordenadas.x][coordenadas.y];
            else
                return casillaNula;
        }

        void ActualizarCoordenadasBando(const Movimiento& mov)
        {
            auto& pieza = ObtenerPieza(mov.Origen);
            // Comprobamos que exista la pieza antes de alterar alguna coordenada del bando
            if (!pieza) return;

            auto& bando = (pieza->getBandoBlancas()) ? J_blancas : J_negras;
            bando[pieza->obtenerTipo()][pieza->GetIndiceBando()] = mov.Destino;
        }

        void P_MoverPieza(const Movimiento& movimiento)
        {
            // Actualizar datos correspondientes entre el tablero y el indice de bando 
            const Coordenadas& origen{ movimiento.Origen };
            const Coordenadas& destino{ movimiento.Destino };

            // Actualizar bandos correspondientes
            // Al poner coordenadas negativas, indica que la pieza ha sido eliminada del tablero
            ActualizarCoordenadasBando(Movimiento{ destino, Coordenadas{-1,-1} });
            ActualizarCoordenadasBando(Movimiento{ origen, destino });

            // Actualizar tablero
            // Movemos las piezas con punteros inteligentes entre sí
            J_tablero[destino.x][destino.y] = std::move(J_tablero[origen.x][origen.y]);
            J_tablero[destino.x][destino.y]->setPosicion(destino);
        }

        // Movimiento directo en el tablero del juego
        void P_MoverPieza(const MovimientoCompuesto& movimiento)
        {
            // Actualizar datos correspondientes entre el tablero y el indice de bando 
            const Coordenadas& origen{ movimiento.MC_Movimiento.Origen };
            const Coordenadas& destino{ movimiento.MC_Movimiento.Destino };

            P_MoverPieza(movimiento.MC_Movimiento);
            J_tablero[destino.x][destino.y]->guardarMovimientoCompuesto(movimiento);

            J_estado.setCambiado();
        }

        // Recorre lineas horizontales, verticales y diagonales para ver si la posicion actual esta amenazada
        bool PosicionLineasEnJaque(const Coordenadas& posicion, bool bando_blancas_pos) {
            // auto& casilla = ObtenerPieza(posicion);
           // if (!casilla)
             //   return false;

            auto lineaEnJaque = [&](const Coordenadas& base, bool esRecta = false) -> bool {
                Coordenadas mov_atras;
                Coordenadas mov_adelante;
                bool recorrido_atras{ true };
                bool recorrido_adelante{ true };
                bool jaque_encontrado{ false };

                auto seguir_recorrido = [&](const Coordenadas& mov, bool& recorrido) -> void {
                    if (!recorrido) return;

                    auto comprobar_a_un_paso = [&]() -> bool
                        {
                            auto distanciaAbs{ (posicion - mov).absC() };
                            if (distanciaAbs.x < 2 && distanciaAbs.y < 2) return true;

                            return false;
                        };

                    if (SonCoordenadasValidas(mov)) {
                        auto& casilla_mov = ObtenerPieza(mov);
                        if (casilla_mov) {
                            bool valor{ false }; // valor predeterminado a false cambiable segun el caso

                            if (casilla_mov->getBandoBlancas() != bando_blancas_pos) {
                                TipoPieza tipo_pieza = casilla_mov->obtenerTipo();
                                if (esRecta) {
                                    switch (tipo_pieza) {
                                    case ChessExtreem::TipoPieza::Torre:
                                    case ChessExtreem::TipoPieza::Dama:
                                        jaque_encontrado = true;
                                        break;
                                    case ChessExtreem::TipoPieza::Rey:
                                        jaque_encontrado = comprobar_a_un_paso();
                                        break;
                                    default:
                                        break;
                                    }
                                }
                                else {
                                    switch (tipo_pieza) {
                                        // Solo si está a un paso [ACTUALIZAR]
                                    case ChessExtreem::TipoPieza::Dama:
                                    case ChessExtreem::TipoPieza::Alfil:
                                        jaque_encontrado = true;
                                        break;
                                    case ChessExtreem::TipoPieza::Rey:
                                        jaque_encontrado = comprobar_a_un_paso();
                                        break;
                                    case ChessExtreem::TipoPieza::Peon: {
                                        // Solo si está a un paso [ACTUALIZAR]
                                        int dir_pos_y = casilla_mov->getBandoBlancas() ? 1 : -1;
                                        int dir_pos_rel = (mov.y > posicion.y) ? -1 : 1;
                                        if (dir_pos_y == dir_pos_rel)
                                        {
                                            jaque_encontrado = comprobar_a_un_paso();
                                        }
                                        break;
                                    }
                                    default:
                                        break;
                                    }
                                }
                            }
                            recorrido = valor;
                        }
                    }
                    else {
                        recorrido = false;
                    }
                    };

                for (int i = 1;; ++i) {
                    if (recorrido_atras) mov_atras = posicion - (base * i);
                    if (recorrido_adelante) mov_adelante = posicion + (base * i);
                    seguir_recorrido(mov_adelante, recorrido_adelante);
                    seguir_recorrido(mov_atras, recorrido_atras);
                    if (!(recorrido_adelante || recorrido_atras)) {
                        return jaque_encontrado;
                    }
                }
                };

            if (lineaEnJaque(Coordenadas{ 1, 0 }, true)) // Horizontal
                return true;
            if (lineaEnJaque(Coordenadas{ 0, 1 }, true)) // Vertical
                return true;
            if (lineaEnJaque(Coordenadas{ 1, 1 })) // Diagonal Derecha
                return true;
            if (lineaEnJaque(Coordenadas{ -1, 1 })) // Diagonal Izquierda
                return true;

            return false; // Añadir un return false al final si en ninguna dirección detecta jaque
        }

        bool PosicionElesEnJaque(const Coordenadas& posicion, bool bando_blancas_pos)
        {
            // auto& casillaPos = ObtenerPieza(posicion);
            //if (!casillaPos) return false;
            // Funcion para saber si hay caballos enemigos que ataquen la posicion dada
            for (int x : Solucionar::Rango(-2, 2))
            {
                for (int y : Solucionar::Rango(-2, 2))
                {
                    if (Solucionar::ExistenRepeticiones(x, 0, y)) continue;
                    Coordenadas posRel{ posicion + Coordenadas{ x, y } };

                    if (!SonCoordenadasValidas(posRel)) continue;
                    auto& casillaRel = ObtenerPieza(posRel);
                    if (!casillaRel) continue;
                    if (casillaRel->getBandoBlancas() == bando_blancas_pos) continue;
                    if (casillaRel->obtenerTipo() != TipoPieza::Caballo) continue;

                    return true; // La posición es atacada por un caballo

                }
            }

            return false;
        }

        bool PosicionEnJaque(const Coordenadas& posicion, bool bando_blancas_pos)
        {
            // Recorrer lineas horizontales, verticales y diagonales, posiciones eles alrededor
            // en busqueda de piezas que puedan capturar la posición dada con relación al bando perteneciente
            return PosicionLineasEnJaque(posicion, bando_blancas_pos)
                || PosicionElesEnJaque(posicion, bando_blancas_pos);
        }

        // Validaciones de Movimientos
        bool ValidarPosicionRelativaInicial(MovimientoCompuesto& mov)
        {
            auto& casilla_origen = ObtenerPieza(mov.MC_Movimiento.Origen);
            auto& casilla_destino = ObtenerPieza(mov.MC_Movimiento.Destino);

            // Comprobación de si la casilla está vacia
            if (!casilla_destino)
            {
                mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Valido_PosicionLibre);
                mov.MC_Validacion.setValido(true);
                return true;
            }
            else
            {
                // Comprobación de si la casilla está ocupada por una pieza de su mismo bando
                if (casilla_origen->getBandoBlancas() == casilla_destino->getBandoBlancas())
                {
                    mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Prohibido_PiezaMismoBando);

                    return false;
                }
                else
                {
                    mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Valido_CapturaPiezaEnemiga);

                    return true;
                }

            }

        }

        // Movimientos especiales
        bool ValidarPosicionPeon(MovimientoCompuesto& mov)
        {
            if (!mov.MC_Validacion.getValido()) return false;

            auto& casilla_origen = ObtenerPieza(mov.MC_Movimiento.Origen);
            bool bandoBlancas{ casilla_origen->getBandoBlancas() };

            auto& casilla_destino = ObtenerPieza(mov.MC_Movimiento.Destino);

            // Conocer si es previamente validado como captura
            if (mov.MC_Validacion.getTipoEspecial() == TipoMovimientoEspecial::PeonCaptura)
            {
                // Conocer si previamente es validado como movimiento de captura normal de pieza a pieza
                if (mov.MC_Validacion.getTipoValidacion() != ValidacionMovimiento::Valido_CapturaPiezaEnemiga)
                {
                    // De no ser, debemos validar si es captura al paso
                    Coordenadas coordenadas_CapturaAlPaso{ mov.MC_Movimiento.Destino.x, mov.MC_Movimiento.Destino.y + ((bandoBlancas) ? -1 : 1) };

                    auto& casillaCapturaAlPaso = ObtenerPieza(coordenadas_CapturaAlPaso);
                    if (!casillaCapturaAlPaso)
                    {
                        // No existe pieza en posición de captura al paso
                        mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Prohibido_PosicionLibrePeroInvalida);
                        goto fin;
                    }
                    if (casillaCapturaAlPaso->obtenerTipo() != TipoPieza::Peon)
                    {
                        // Si es diferente de peon no es valido este movimiento
                        mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Prohibido_PosicionLibrePeroInvalida);
                        goto fin;
                    }
                    else
                    {

                        // Comprobación de que la ultima jugada destino corresponda a la misma casillaCapturaAlPaso
                        // Condición de salida invalidada 
                        if (J_jugadas.size() > 0)
                        {
                            if (J_jugadas.at(J_jugadas.size() - 1).MC_Movimiento.Destino != coordenadas_CapturaAlPaso)
                            {
                                // Si es diferente el destino de la jugada anterior a las coordenadas de la casilla de posible captura al paso es invalido, aunque asi fuese un peon que cumpla con la condición de dos pasos anteriores
                                mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Prohibido_PosicionLibrePeroInvalida);
                                goto fin;
                            }
                        }

                        // Debemos comprobar que el peon sea del otro bando
                        auto& peonACapturar = casillaCapturaAlPaso;

                        if (peonACapturar->getBandoBlancas() == bandoBlancas)
                        {
                            mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Prohibido_PiezaMismoBando);
                            goto fin;
                        }
                        else
                        {
                            // El peon del bando contrario a capturar, debe haber dado dos pasos en su movimiento anterior
                            auto& mov_pasados = peonACapturar->getMovimientosPasados();
                            if (mov_pasados.size() < 1)
                            {
                                mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Prohibido_PosicionLibrePeroInvalida);
                                goto fin;
                            }
                            else
                            {
                                int lastIndexMov{ static_cast<int>(mov_pasados.size()) - 1 };
                                auto& ultimoMovimiento = mov_pasados.at(lastIndexMov);
                                // Comprobar si el ultimo movimiento tuvo dos pasos 
                                if (abs(ultimoMovimiento.MC_Movimiento.Origen.y - ultimoMovimiento.MC_Movimiento.Destino.y) != 2)
                                {
                                    mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Prohibido_PosicionLibrePeroInvalida);
                                    goto fin;
                                }
                                else
                                {
                                    // Tuvo dos pasos, por tanto, es una captura al paso
                                    mov.MC_Validacion.setTipoEspecial(TipoMovimientoEspecial::PeonCapturaAlPaso);
                                    // Solo se pasan las coordenadas origen del peon enemigo, porque es una captura
                                    mov.MC_MovimientoPiezaRelacionada.Origen = coordenadas_CapturaAlPaso;
                                }

                            }
                        }
                    }
                }
            }
            else
            {
                // Si no es validada como captura, es un movimiento vertical
                // La primera comprobación que se hace aquí, es que, no exista alguna pieza en esa posición
                if (casilla_destino)
                    mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Prohibido_PosicionObstruida);
                else
                {
                    // Comprobar si es la ultima o primera casilla vertical es un movimiento de coronación del peon (pedir selección de cambio de pieza)
                    if (Solucionar::ExistenRepeticiones(mov.MC_Movimiento.Destino.y, 0, 7))
                    {
                        mov.MC_Validacion.setTipoEspecial(TipoMovimientoEspecial::PeonCoronacion);
                    }
                    else
                    {
                        mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Valido_PosicionLibre);

                    }

                }
            }

            fin:
            return mov.MC_Validacion.getValido();
        }

        bool ValidarPosicionRey(MovimientoCompuesto& mov)
        {
            if (!mov.MC_Validacion.getValido()) return false;

            auto& casilla_origen = ObtenerPieza(mov.MC_Movimiento.Origen);
            bool bandoBlancas{ casilla_origen->getBandoBlancas() };
            auto& casilla_destino = ObtenerPieza(mov.MC_Movimiento.Destino);

            if (Solucionar::compara(mov.MC_Validacion.getTipoEspecial(),
                TipoMovimientoEspecial::ReyEnroqueCorto,
                TipoMovimientoEspecial::ReyEnroqueLargo))
            {
                // Comprobación inicial, si el movimiento es de tipo enroque 
                // el rey no debe haberse movido            
                if (casilla_origen->getMovimientosPasados().size() > 0)
                {
                    mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Prohibido_PosicionLibrePeroInvalida);
                    return false;
                }

                // Para esto, no debe existir ninguna pieza en la casilla destino
                if (casilla_destino)
                {
                    mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Prohibido_PosicionObstruida);
                    return false;
                }
                auto pos_torre_rel = mov.MC_Movimiento.Origen + Coordenadas{ (mov.MC_Validacion.getTipoEspecial() == TipoMovimientoEspecial::ReyEnroqueCorto) ? 3 : -4,0 };
                auto& casilla_Torre = ObtenerPieza(pos_torre_rel);
                // Comprobar que la torre en cuestión se encuentre en la posición inicial para el enroque
                if (casilla_Torre)
                {
                    // Comprobar que la torre para el enroque no se haya movido antes
                    if (casilla_Torre->getMovimientosPasados().size() > 0)
                    {
                        mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Prohibido_PosicionLibrePeroInvalida);
                        return false;
                    }
                    else
                    {
                        // Comprobar que cada casilla hasta llegar al destino este libre de ataque
                        // De validarse, dar coordenadas origen->destino de la torre en movimiento rey pieza relacionada

                        int direccion{ (mov.MC_Validacion.getTipoEspecial() == TipoMovimientoEspecial::ReyEnroqueCorto) ? 1 : -1 };
                        for (auto x : Solucionar::Rango(mov.MC_Movimiento.Origen.x + direccion, pos_torre_rel.x - direccion))
                        {
                            Coordenadas rev_pos{ x,mov.MC_Movimiento.Origen.y };
                            if (PosicionEnJaque(rev_pos, bandoBlancas))
                            {
                                mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Prohibido_PosicionLibrePeroInvalida);
                                return false;
                            }

                        }

                        // Si se comprueba que la linea de enroque no tiene casillas atacadas se valida como movimiento compuesto
                        mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Valido_MovimientoCompuesto);
                        // y se agrega el movimiento compuesto en relación a la torre
                        mov.MC_MovimientoPiezaRelacionada.Origen = pos_torre_rel;
                        mov.MC_MovimientoPiezaRelacionada.Destino = pos_torre_rel + Coordenadas{ ((mov.MC_Validacion.getTipoEspecial() == TipoMovimientoEspecial::ReyEnroqueCorto) ? -2 : 3),0 };
                        return true;
                    }
                }
                else
                {
                    mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Prohibido_PosicionLibrePeroInvalida);
                    return false;
                }
            }
            else
            {
                // No es movimiento de tipo enroque
                // Si es un movimiento previamente validado se deja como está 
                return mov.MC_Validacion.getValido();
            }
        }

        bool ValidarPosicionReyFueraDeJaque(MovimientoCompuesto& mov)
        {
            if (!mov.MC_Validacion.getValido()) return false;

            auto& casillaPieza = ObtenerPieza(mov.MC_Movimiento.Origen);
            bool bandoBlancas = casillaPieza->getBandoBlancas();

            Juego copiaJuego{ *this };
            
            copiaJuego.P_MoverPieza(mov);
            auto& bando = (bandoBlancas) ? copiaJuego.J_blancas : copiaJuego.J_negras;

            // Validamos que la posición actual del unico rey 0, no este en jaque
            if (copiaJuego.PosicionEnJaque(bando[TipoPieza::Rey][0], bandoBlancas))
                mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Prohibido_ReyEnJaque);
            
            
            return mov.MC_Validacion.getValido();
        }


        void ValidadorGeneralDeMovimientos(MovimientoCompuesto& movimiento)
        {
            if (!ValidarPosicionRelativaInicial(movimiento)) return;

            auto& pieza = ObtenerPieza(movimiento.MC_Movimiento.Origen);

            switch (pieza->obtenerTipo()) {
            case ChessExtreem::TipoPieza::Torre:
                // Validaciones específicas para la torre...
                break;
            case ChessExtreem::TipoPieza::Caballo:
                // Validaciones específicas para el caballo...
                break;
            case ChessExtreem::TipoPieza::Alfil:
                // Validaciones específicas para el alfil...
                break;
            case ChessExtreem::TipoPieza::Dama:
                // Validaciones específicas para la dama...
                break;
            case ChessExtreem::TipoPieza::Rey:
                ValidarPosicionRey(movimiento);
                break;
            case ChessExtreem::TipoPieza::Peon:
                ValidarPosicionPeon(movimiento);
                break;
            default:
                break;
            }

            // Validar si el movimiento no expone al rey en jaque directamente
            if(movimiento.MC_Validacion.getValido())
                ValidarPosicionReyFueraDeJaque(movimiento);

        }

        void CalcularMovimientosPieza(int x, int y)
        {
            if (!EsPiezaDelTurno(x, y))
            {
                J_tablero[x][y]->LimpiarMovimientosPosibles();
                return;
            }

            auto fncValidador = std::bind(&Juego::ValidadorGeneralDeMovimientos, this, std::placeholders::_1);
            J_tablero[x][y]->calcularMovimientos(fncValidador);
        }

        void CalcularMovimientosPieza(const Coordenadas& c)
        {
            CalcularMovimientosPieza(c.x, c.y);
        }

        void EliminacionPieza(int x, int y)
        {
            if (!SonCoordenadasValidas(Coordenadas{ x,y })) return;

            if (J_tablero[x][y]) J_tablero[x][y] = nullptr;
        }

        void EliminacionPieza(const Coordenadas& c)
        {
            EliminacionPieza(c.x, c.y);
        }

    protected:
        EstadoJuego J_estado{};
        Bando J_blancas{ true };
        Bando J_negras{};
        CadenaMovimientos J_jugadas{};

    public:
        // Constructor por defecto
        Juego() {
            Inicializar();
        }

        // Constructor de copia
        Juego(const Juego& otro) {
            // Copiar el estado del juego
            J_estado = otro.J_estado;

            // Copiar el tablero
            for (size_t i = 0; i < J_tablero.size(); ++i) {
                for (size_t j = 0; j < J_tablero[i].size(); ++j) {
                    if (otro.J_tablero[i][j]) {
                        J_tablero[i][j] = otro.J_tablero[i][j]->clonar();
                    }
                    else {
                        J_tablero[i][j] = nullptr;
                    }
                }
            }

            // Copiar las instancias de Bando
            J_blancas = otro.J_blancas;
            J_negras = otro.J_negras;
            J_jugadas = otro.J_jugadas;
        }

        // Constructor de movimiento
        Juego(Juego&& otro) noexcept {
            // Mover el estado del juego
            J_estado = std::move(otro.J_estado);

            // Mover el tablero
            J_tablero = std::move(otro.J_tablero);

            // Mover las instancias de Bando
            J_blancas = std::move(otro.J_blancas);
            J_negras = std::move(otro.J_negras);
            J_jugadas = std::move(otro.J_jugadas);
        }

        // Operador de asignación de copia
        Juego& operator=(const Juego& otro) {
            if (this != &otro) {
                // Copiar el estado del juego
                J_estado = otro.J_estado;

                // Copiar el tablero
                for (size_t i = 0; i < J_tablero.size(); ++i) {
                    for (size_t j = 0; j < J_tablero[i].size(); ++j) {
                        if (otro.J_tablero[i][j]) {
                            J_tablero[i][j] = otro.J_tablero[i][j]->clonar();
                        }
                        else {
                            J_tablero[i][j] = nullptr;
                        }
                    }
                }

                // Copiar las instancias de Bando
                J_blancas = otro.J_blancas;
                J_negras = otro.J_negras;
                J_jugadas = otro.J_jugadas;
            }
            return *this;
        }

        // Operador de asignación de movimiento
        Juego& operator=(Juego&& otro) noexcept {
            // Mover el estado del juego
            J_estado = std::move(otro.J_estado);

            // Mover el tablero
            J_tablero = std::move(otro.J_tablero);

            // Mover las instancias de Bando
            J_blancas = std::move(otro.J_blancas);
            J_negras = std::move(otro.J_negras);
            J_jugadas = std::move(otro.J_jugadas);

            return *this;
        }

        bool SonCoordenadasValidas(const Coordenadas& coordenadas) const
        {
            return (coordenadas.x >= 0 && coordenadas.x <= 7) &&
                   (coordenadas.y >= 0 && coordenadas.y <= 7);
        }

        void Inicializar()
        {
            // Inicializa el tablero de ajedrez con piezas nulas
            for (auto& fila : J_tablero) {
                for (auto& casilla : fila) {
                    casilla = nullptr;
                }
            }
            J_estado.setEstado(Estado::Inicializado);
        }

        // Metodo que crea las piezas y activa lo necesario para comenzar el juego y activarlo
        void CrearJuego()
        {
            if (J_estado.getEstado() != Estado::Inicializado) Inicializar();

            J_blancas.CrearBando(J_tablero);
            J_negras.CrearBando(J_tablero);


            J_estado.setEstado(Estado::Activo);
            J_estado.setTurno(TipoBando::Blancas);

        }

        Juego ObtenerCopia()
        {
            return *this;
        }

        bool MoverPieza(const Coordenadas& origen, const Coordenadas& destino)
        {
            // Logica de validación de movimiento            
            auto pieza = J_tablero[origen.x][origen.y];
            

            // Si la pieza pertenece a un bando diferente al turno actual se sale
            if (!EsPiezaDelTurno(origen)) return false;

            if (!pieza) return false;

            auto& movimientos{ pieza->getMovimientos() };
            if (movimientos.empty()) CalcularMovimientosPieza(origen); // si no se han calculado ahora se hará
            
            if (movimientos.empty()) return false; // Si después del calculo nuevo no obtiene movimientos debe salirse

            // Movimiento de pieza
            Movimiento mov{ origen,destino };
            int index_movp{ pieza->IndexMovimientoValidado(mov) };

            if (index_movp < 0) return false;

            auto mc = movimientos[index_movp];

            // Validado
            P_MoverPieza(MovimientoCompuesto{ mov });
                        
            // Realizar el movimiento relacionado 
            switch (mc.MC_Validacion.getTipoEspecial())
            {
            case TipoMovimientoEspecial::PeonCapturaAlPaso:
                // Eliminación de peon enemigo
                EliminacionPieza(mc.MC_MovimientoPiezaRelacionada.Origen);
                break;
            case TipoMovimientoEspecial::ReyEnroqueCorto:               
            case TipoMovimientoEspecial::ReyEnroqueLargo:
                P_MoverPieza(mc.MC_MovimientoPiezaRelacionada);
                break;
            default:
                break;
            }
            pieza->LimpiarMovimientosPosibles(); // Una vez que la pieza se mueva se debe eliminar sus movimientos calculados ya que no corresponden a su posición actual
            // Actualizar lista de movimientos realizados del juego
            J_jugadas.push_back(std::move(mc));

            J_estado.cambiarTurno();
            LimpiarMovimientosValidadosPiezas();
            return true; // Llegar al final significa que se ha movido la pieza
        }

        const CadenaMovimientos& ObtenerMovimientosPieza(int x, int y) {
            auto& movimientos{ J_tablero[x][y]->getMovimientos() };
            if (movimientos.empty()) CalcularMovimientosPieza(x, y);
            
            return movimientos;
        }

        const CadenaMovimientos& ObtenerMovimientosPieza(Coordenadas c) {
            
            return ObtenerMovimientosPieza(c.x, c.y);
        }

        // Métodos para obtener el estado del juego y establecerlo
        /*EstadoJuego& obtenerEstado() {
            return estado;
        }*/

        EstadoJuego& obtenerEstado() {
            return J_estado;
        }

        const Pieza& getTablero(Coordenadas c) const 
        { 
            if (c.x < 0 || c.x > 7 || c.y < 0 || c.y > 7)  {
                throw std::out_of_range("Índice de fila fuera de rango");
            }
            return *J_tablero[c.x][c.y]; 
        }

        const std::array<std::shared_ptr<Pieza>,8>& operator[](int fila) const 
        {
            if (fila < 0 || fila > 7) {
                throw std::out_of_range("Índice de fila fuera de rango");
            }

            return J_tablero[fila];
        }

        bool EsPiezaDelTurno(int x, int y)
        {
            return J_tablero[x][y]->getBandoBlancas() == J_estado.esTurnoBlancas();
        }

        bool EsPiezaDelTurno(Coordenadas c)
        {
            return EsPiezaDelTurno(c.x, c.y);
        }

        void LimpiarMovimientosValidadosPiezas()
        {
            for (size_t i = 0; i < J_tablero.size(); ++i) {
                for (size_t j = 0; j < J_tablero[i].size(); ++j) {                  
                    if (J_tablero[i][j])
                        J_tablero[i][j]->LimpiarMovimientosPosibles();                  
                }
            }
        }

        /*void establecerEstado(Estado nuevoEstado) {
            estado.setEstado(nuevoEstado);
        }*/
    };

    // <- SECCIÓN DE MODO DE JUEGO POR CONSOLA (PUEDE FUNCIONAR SOLO COMO DEMOSTRACIÓN) 
    namespace ModoConsola
    {        
        Juego juego;

        void IniciarJuego()
        {
            juego.CrearJuego();
        }

        void ImprimirTablero(Juego&& juego = {})
        {
            if (juego.obtenerEstado().getEstado() != Estado::Activo)
            {
                std::cout << "Juego no inicializado, usando el global...\n";
                juego = std::move(ModoConsola::juego);
                juego.CrearJuego();
            }
            
            // Mapeo de tipos de piezas a caracteres de representación
            std::unordered_map<TipoPieza, char> piezaARepresentacion = {
                { TipoPieza::Torre, 'T' },
                { TipoPieza::Caballo, 'C' },
                { TipoPieza::Alfil, 'A' },
                { TipoPieza::Dama, 'D' },
                { TipoPieza::Rey, 'R' },
                { TipoPieza::Peon, 'P' }
            };

            // Imprime la fila superior del tablero
            std::cout << "   A B C D E F G H\n";

            // Iterar sobre cada fila del tablero
            for (int y = 0; y < 8; ++y) {
                // Imprime el número de la fila
                std::cout << y + 1 << " |";

                // Iterar sobre cada columna de la fila actual
                for (int x = 0; x < 8; ++x) {
                    // Obtener la pieza en la posición actual
                    auto& pieza = juego[x][y];

                    // Imprimir la representación de la pieza si no es nula
                    if (pieza) {
                        // Obtener el tipo de pieza y buscar su representación
                        char representacion = ' ';
                        auto tipoPieza = pieza->obtenerTipo();
                        auto it = piezaARepresentacion.find(tipoPieza);
                        if (it != piezaARepresentacion.end()) {
                            representacion = it->second;
                        }
                        std::cout << representacion;
                    }
                    // Imprimir un espacio si la pieza es nula
                    else {
                        std::cout << "x";
                    }

                    // Imprimir un espacio después de cada pieza
                    std::cout << "|";
                }

                // Imprimir la fila actual
                std::cout << "\n";

                // Imprimir la fila inferior del tablero si no es la última fila
                if (y < 7) {
                    std::cout << "   ----------------\n";
                }
            }

            // Imprimir la fila inferior del tablero
            std::cout << "   A B C D E F G H\n\n";
        }
    }
    // SECCIÓN DE MODO DE JUEGO POR CONSOLA (PUEDE FUNCIONAR SOLO COMO DEMOSTRACIÓN) ->
}


#endif // !CHESS_EXTREEM_H