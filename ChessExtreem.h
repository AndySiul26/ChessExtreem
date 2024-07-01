#pragma once
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
#include <thread>
#include <chrono> // Incluir la librería necesaria para std::chrono
#include <atomic> 
#include <mutex> // Incluir la librería necesaria para std::mutex

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
		Prohibido_FueraDeTablero,            // La posición destino no es válida ni aprobada por movimientos básicos
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
			case ChessExtreem::ValidacionMovimiento::Prohibido_FueraDeTablero:
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
        
        int x, y; // Default desasignadas
	
        // Constructor por defecto
        Coordenadas() : x(-1), y(-1) {}

        // Constructor para inicializar con valores específicos
        Coordenadas(int x, int y) : x(x), y(y) {}

        // Constructor que acepta un std::initializer_list
        Coordenadas(std::initializer_list<int> initList) {
            if (initList.size() != 2) {
                throw std::invalid_argument("Se requieren exactamente dos valores para inicializar Coordenadas.");
            }

            auto it = initList.begin();
            x = *it;
            y = *(it + 1);
        }


		// Sobrecarga de operadores para Coordenadas
        friend bool operator==(const Coordenadas& c1, const Coordenadas& c2);
        friend bool operator!=(const Coordenadas& c1, const Coordenadas& c2);
        Coordenadas absC() const
        {
            return Coordenadas{ abs(x) ,abs(y) };
        }


		// Operadores de asignación
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

    const Coordenadas CoordenadasNulas{ -1, -1 };

    // Cambiar a movimiento compuesto (crear clase Movimiento solo debe tener origen y destino encapsulando coordenadas origen destino en esa clase)
    // CAMBIAR LOGICA...

    struct Movimiento
    {
        Coordenadas Origen;
        Coordenadas Destino;

		Movimiento() : Origen{ -1, -1 }, Destino{ -1, -1 } {}

		Movimiento(const Coordenadas& origen, const Coordenadas& destino) : Origen{ origen }, Destino{ destino } {}

        Movimiento(std::initializer_list<int> initList) {
			if (initList.size() != 4) {
				throw std::invalid_argument("Se requieren exactamente cuatro valores para inicializar Movimiento.");
			}

			auto it = initList.begin();
			Origen.x = *it;
			Origen.y = *(it + 1);
			Destino.x = *(it + 2);
			Destino.y = *(it + 3);
        }

		Movimiento(const Movimiento& otro) noexcept
			: Origen{ otro.Origen }, Destino{ otro.Destino } {}

		Movimiento(Movimiento&& otro) noexcept
			: Origen{ std::move(otro.Origen) }, Destino{ std::move(otro.Destino) } {}

        Movimiento Invertir() const
        {
            return { Destino, Origen };
        }

		Movimiento& operator=(const Movimiento& otro) noexcept {
			Origen = otro.Origen;
			Destino = otro.Destino;
			return *this;
		}

		Movimiento& operator=(Movimiento&& otro) noexcept {
			Origen = std::move(otro.Origen);
			Destino = std::move(otro.Destino);
			return *this;
        }

        // Sobrecarga de operadores para Coordenadas
        friend bool operator==(const Movimiento& c1, const Movimiento& c2) {
            return c1.Origen == c2.Origen && c1.Destino == c2.Destino; // Corrección
        }

        friend bool operator!=(const Movimiento& c1, const Movimiento& c2) {
            return !(c1 == c2); // Corrección
        }

		Movimiento operator+(const Movimiento& c1) const {
			return { Origen + c1.Origen, Destino + c1.Destino };
		}

		Movimiento operator-(const Movimiento& c1) const {
			return { Origen - c1.Origen, Destino - c1.Destino };
        }

		Movimiento operator*(const Movimiento& c1) const {
			return { Origen * c1.Origen, Destino * c1.Destino };
		}

        Movimiento operator*(int escalar) const {
			return { Origen * escalar, Destino * escalar };
        }

        Movimiento operator/(const Movimiento& c1) const {
			return { Origen / c1.Origen, Destino / c1.Destino };
        }

        Movimiento operator/(int escalar) const {
			return { Origen / escalar, Destino / escalar };
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

		friend bool operator==(const MovimientoCompuesto& mc1, const MovimientoCompuesto& mc2) {
			return mc1.MC_Movimiento == mc2.MC_Movimiento && mc1.MC_MovimientoPiezaRelacionada == mc2.MC_MovimientoPiezaRelacionada;
		}

		friend bool operator!=(const MovimientoCompuesto& mc1, const MovimientoCompuesto& mc2) {
			return !(mc1 == mc2);
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


		// Setters y Getters
        void SetIndiceBando(int indice) { P_indiceBando = indice; }
        int GetIndiceBando() const {return P_indiceBando; }
		void SetVisible(bool visible) { P_visible = visible; }
		bool GetVisible() const { return P_visible; }

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

		friend class Juego;
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

					MovimientoCompuesto movimiento{ { P_posicion, {nx, ny} }, {}, true, TipoMovimientoEspecial::Nulo };
                        

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
    private:
        Coordenadas* CIE_coordenadasRef; // Puntero de referencia a otro objeto de Coordenadas
    public:
        TipoPieza CIE_tipoPieza;

        Coordenadas_InfoExtra(const Coordenadas& coordenadas, const TipoPieza tipo_pieza)
            : Coordenadas{ coordenadas }, CIE_tipoPieza(tipo_pieza), CIE_coordenadasRef{ nullptr } {}
                        
        ~Coordenadas_InfoExtra()
        {
            CIE_coordenadasRef = nullptr;
        }

        void setCoordenadasRef(Coordenadas& c)
        {
            CIE_coordenadasRef = &c;
        }

        Coordenadas& getCoordenadasRef()
        {
            return *CIE_coordenadasRef;
        }

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
        Estado EJ_estado{Estado::SinInicializar};
        TipoBando EJ_turno{ TipoBando::Nulo };
        bool EJ_Cambiado{ false };

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

		EstadoJuego(Estado estado, TipoBando turno, bool cambiado = false)
			: EJ_estado{ estado }, EJ_turno{ turno }, EJ_Cambiado{ cambiado } {}

		EstadoJuego(const EstadoJuego& otroEstado)
			: EJ_estado{ otroEstado.EJ_estado }, EJ_turno{ otroEstado.EJ_turno }, EJ_Cambiado{ otroEstado.EJ_Cambiado } {}

        Estado getEstado() const {
            return EJ_estado;
        }

		EstadoJuego operator=(const EstadoJuego& otroEstado)
		{
			if (this != &otroEstado) {
				EJ_estado = otroEstado.EJ_estado;
				EJ_turno = otroEstado.EJ_turno;
				EJ_Cambiado = otroEstado.EJ_Cambiado;
			}
			return *this;
		}

		bool esActivo() const {
            switch (EJ_estado)
            {
            case ChessExtreem::Estado::Inicializado:                
            case ChessExtreem::Estado::Activo:
                return true;
                break;
            case ChessExtreem::Estado::SinInicializar:
            case ChessExtreem::Estado::Tablas:
            case ChessExtreem::Estado::JaqueBlancas:
            case ChessExtreem::Estado::JaqueNegras:
            case ChessExtreem::Estado::JaqueMateBlancas:
            case ChessExtreem::Estado::JaqueMateNegras:
            case ChessExtreem::Estado::Ahogado:
            case ChessExtreem::Estado::RepeticionTriple:                
            default:
                return false;
                break;
            }
			
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

    class IndiceCoordenadasTipoPieza {
    public:
        int index;
        CadenaCoordenadas& cadenaRef;

        IndiceCoordenadasTipoPieza(int idx, CadenaCoordenadas& ref)
            : index(idx), cadenaRef(ref) {}

        // Constructor por copia
        IndiceCoordenadasTipoPieza(const IndiceCoordenadasTipoPieza& other)
            : index(other.index), cadenaRef(other.cadenaRef) {}

        // Operador de asignación por copia
        IndiceCoordenadasTipoPieza& operator=(const IndiceCoordenadasTipoPieza& other) {
            if (this != &other) {
                index = other.index;
                cadenaRef = other.cadenaRef;
            }
            return *this;
        }

        // Sobrecarga del operador ()
        Coordenadas& operator()() {
            return cadenaRef[index];
        }

        // Sobrecarga del operador de conversión implícita a Coordenadas&
        operator Coordenadas& () {
            return cadenaRef[index];
        }
    };

    using CadenaCoordenadasRef = std::vector<IndiceCoordenadasTipoPieza>;


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

        std::atomic<int> B_cantidadPiezas{ 0 };
        std::atomic<int> B_cantidadReyes{ 0 };
        std::atomic<int> B_cantidadDamas{ 0 };
        std::atomic<int> B_cantidadPeones{ 0 };
        std::atomic<int> B_cantidadCaballos{ 0 };
        std::atomic<int> B_cantidadAlfiles{ 0 };
        std::atomic<int> B_cantidadTorres{ 0 };        

    public:
        CadenaCoordenadasRef B_piezas;

        Bando(bool bandoBlancas = false) : B_bandoBlancas{ bandoBlancas } {}

        Bando(const Bando& otroBando) : B_bandoBlancas{ otroBando.B_bandoBlancas }
        {
            B_reyes = otroBando.B_reyes;
            B_damas = otroBando.B_damas;
            B_peones = otroBando.B_peones;
            B_caballos = otroBando.B_caballos;
            B_alfiles = otroBando.B_alfiles;
            B_torres = otroBando.B_torres;

            B_cantidadAlfiles.store(otroBando.B_cantidadAlfiles.load());
            B_cantidadCaballos.store(otroBando.B_cantidadCaballos.load());
            B_cantidadDamas.store(otroBando.B_cantidadDamas.load());
            B_cantidadPeones.store(otroBando.B_cantidadPeones.load());
            B_cantidadReyes.store(otroBando.B_cantidadReyes.load());
            B_cantidadTorres.store(otroBando.B_cantidadTorres.load());
            B_cantidadPiezas.store(otroBando.B_cantidadPiezas.load());

            CorresponderPiezas();
        }

		Bando& operator=(const Bando& otroBando)
		{
			if (this != &otroBando) {
				B_bandoBlancas = otroBando.B_bandoBlancas;

				B_reyes = otroBando.B_reyes;
				B_damas = otroBando.B_damas;
				B_peones = otroBando.B_peones;
				B_caballos = otroBando.B_caballos;
				B_alfiles = otroBando.B_alfiles;
				B_torres = otroBando.B_torres;

				B_cantidadAlfiles.store(otroBando.B_cantidadAlfiles.load());
				B_cantidadCaballos.store(otroBando.B_cantidadCaballos.load());
				B_cantidadDamas.store(otroBando.B_cantidadDamas.load());
				B_cantidadPeones.store(otroBando.B_cantidadPeones.load());
				B_cantidadReyes.store(otroBando.B_cantidadReyes.load());
				B_cantidadTorres.store(otroBando.B_cantidadTorres.load());
				B_cantidadPiezas.store(otroBando.B_cantidadPiezas.load());

				CorresponderPiezas();
			}
			return *this;
		}

        void CorresponderPiezas() {
            B_piezas.clear();

            for (size_t i = 0; i < B_reyes.size(); ++i) {
                B_piezas.emplace_back(i, B_reyes);
            }
            for (size_t i = 0; i < B_damas.size(); ++i) {
                B_piezas.emplace_back(i, B_damas);
            }
            for (size_t i = 0; i < B_peones.size(); ++i) {
                B_piezas.emplace_back(i, B_peones);
            }
            for (size_t i = 0; i < B_caballos.size(); ++i) {
                B_piezas.emplace_back(i, B_caballos);
            }
            for (size_t i = 0; i < B_alfiles.size(); ++i) {
                B_piezas.emplace_back(i, B_alfiles);
            }
            for (size_t i = 0; i < B_torres.size(); ++i) {
                B_piezas.emplace_back(i, B_torres);
            }
        }

        void AgregarPieza(const Coordenadas_InfoExtra& coordenadasIE, TableroAjedrez& tablero) {
            Coordenadas c{ coordenadasIE };

            switch (coordenadasIE.CIE_tipoPieza) {
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

			IncrementarContadores(coordenadasIE.CIE_tipoPieza);
        }

		void AgregarPieza(const Coordenadas_InfoExtra& coordenadasIE, TableroAjedrez& tablero, std::shared_ptr<Pieza>& pieza) {
			Coordenadas c{ coordenadasIE };
            int indiceBando{ 0 };

            switch (coordenadasIE.CIE_tipoPieza) {
			case TipoPieza::Torre:
				indiceBando = B_torres.size();
				B_torres.push_back(c);
				tablero.at(c.x).at(c.y) = pieza;
				break;
			case TipoPieza::Caballo:
				indiceBando = B_caballos.size();
				B_caballos.push_back(c);
				tablero.at(c.x).at(c.y) = pieza;
				break;
			case TipoPieza::Alfil:
				indiceBando = B_alfiles.size();
				B_alfiles.push_back(c);
				tablero.at(c.x).at(c.y) = pieza;
				break;
			case TipoPieza::Dama:
				indiceBando = B_damas.size();
				B_damas.push_back(c);
				tablero.at(c.x).at(c.y) = pieza;
				break;
			case TipoPieza::Rey:
				indiceBando = B_reyes.size();
				B_reyes.push_back(c);
				tablero.at(c.x).at(c.y) = pieza;
				break;
			case TipoPieza::Peon:
				indiceBando = B_peones.size();
				B_peones.push_back(c);
				tablero.at(c.x).at(c.y) = pieza;
				break;
			default:
				break;
			}

			tablero.at(c.x).at(c.y)->SetIndiceBando(indiceBando);

			IncrementarContadores(coordenadasIE.CIE_tipoPieza);
			CorresponderPiezas();
        }

        void IncrementarContadores(const TipoPieza tipo)
        {
            B_cantidadPiezas.fetch_add(1, std::memory_order_relaxed);
            switch (tipo) {
            case TipoPieza::Torre:
                B_cantidadTorres.fetch_add(1, std::memory_order_relaxed);
                break;
            case TipoPieza::Caballo:
                B_cantidadCaballos.fetch_add(1, std::memory_order_relaxed);
                break;
            case TipoPieza::Alfil:
                B_cantidadAlfiles.fetch_add(1, std::memory_order_relaxed);
                break;
            case TipoPieza::Dama:
                B_cantidadDamas.fetch_add(1, std::memory_order_relaxed);
                break;
            case TipoPieza::Rey:
                B_cantidadReyes.fetch_add(1, std::memory_order_relaxed);
                break;
            case TipoPieza::Peon:
                B_cantidadPeones.fetch_add(1, std::memory_order_relaxed);
                break;
            default:
                break;
            }
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
            AgregarPieza(Coordenadas_InfoExtra{ {2, i_lado}, TipoPieza::Alfil }, tablero);
            AgregarPieza(Coordenadas_InfoExtra{ {5, i_lado}, TipoPieza::Alfil }, tablero);

            // Creamos Dama y Rey
            AgregarPieza(Coordenadas_InfoExtra{ {3, i_lado}, TipoPieza::Dama }, tablero);
            AgregarPieza(Coordenadas_InfoExtra{ {4, i_lado}, TipoPieza::Rey }, tablero);

            // Creamos Peones
            for (int x = 0; x < 8; ++x) {
                AgregarPieza(Coordenadas_InfoExtra{ {x, i_lado + i_add}, TipoPieza::Peon }, tablero);
            }

            CorresponderPiezas();
        }

        CadenaCoordenadas& operator[](TipoPieza tipo)
        {
            switch (tipo) {
            case TipoPieza::Torre:
                return B_torres;
            case TipoPieza::Caballo:
                return B_caballos;
            case TipoPieza::Alfil:
                return B_alfiles;
            case TipoPieza::Dama:
                return B_damas;
            case TipoPieza::Rey:
                return B_reyes;
            case TipoPieza::Peon:
                return B_peones;
            default:
                throw std::invalid_argument("Tipo de pieza no válido.");
            }
        }

		const CadenaCoordenadas& operator[](TipoPieza tipo) const
		{
			switch (tipo) {
			case TipoPieza::Torre:
				return B_torres;
			case TipoPieza::Caballo:
				return B_caballos;
			case TipoPieza::Alfil:
				return B_alfiles;
			case TipoPieza::Dama:
				return B_damas;
			case TipoPieza::Rey:
				return B_reyes;
			case TipoPieza::Peon:
				return B_peones;
			default:
				throw std::invalid_argument("Tipo de pieza no válido.");
			}
		}

        // Retorna la cantidad total de piezas
        int CantidadPiezas() const { return B_cantidadPiezas.load(std::memory_order_relaxed); }

        int CantidadPiezas(TipoPieza tipo) const
        {
            switch (tipo) {
            case TipoPieza::Torre:
                return B_cantidadTorres.load(std::memory_order_relaxed);
            case TipoPieza::Caballo:
                return B_cantidadCaballos.load(std::memory_order_relaxed);
            case TipoPieza::Alfil:
                return B_cantidadAlfiles.load(std::memory_order_relaxed);
            case TipoPieza::Dama:
                return B_cantidadDamas.load(std::memory_order_relaxed);
            case TipoPieza::Rey:
                return B_cantidadReyes.load(std::memory_order_relaxed);
            case TipoPieza::Peon:
                return B_cantidadPeones.load(std::memory_order_relaxed);
            default:
                throw std::invalid_argument("Tipo de pieza no válido.");
            }
        }

        void ActualizarPosicionPieza(TipoPieza tipo, int indice, const Coordenadas& nuevaPosicion)
        {
            operator[](tipo)[indice] = nuevaPosicion;
            if (nuevaPosicion == Coordenadas{ -1, -1 }) {
                B_cantidadPiezas.fetch_sub(1, std::memory_order_relaxed);
                switch (tipo) {
                case TipoPieza::Torre:
                    B_cantidadTorres.fetch_sub(1, std::memory_order_relaxed);
                    break;
                case TipoPieza::Caballo:
                    B_cantidadCaballos.fetch_sub(1, std::memory_order_relaxed);
                    break;
                case TipoPieza::Alfil:
                    B_cantidadAlfiles.fetch_sub(1, std::memory_order_relaxed);
                    break;
                case TipoPieza::Dama:
                    B_cantidadDamas.fetch_sub(1, std::memory_order_relaxed);
                    break;
                case TipoPieza::Rey:
                    B_cantidadReyes.fetch_sub(1, std::memory_order_relaxed);
                    break;
                case TipoPieza::Peon:
                    B_cantidadPeones.fetch_sub(1, std::memory_order_relaxed);
                    break;
                default:
                    break;
                }
            }
        }

		void CambiarTipoPieza(TipoPieza tipo, int indice, TipoPieza nuevoTipo, Coordenadas coordenadas = CoordenadasNulas)
        {
            if (coordenadas == CoordenadasNulas) 
			    coordenadas = operator[](tipo)[indice];

			// operator[](tipo).erase(operator[](tipo).begin() + indice); Si se borran las coordenadas desajusta los indices
			// En su lugar se establece la coordenada en -1, -1 (coordenadas nulas)
			ActualizarPosicionPieza(tipo, indice, Coordenadas{ -1, -1 });


            switch (nuevoTipo) {
			case TipoPieza::Torre:
				B_torres.push_back(coordenadas);

				break;
			case TipoPieza::Caballo:
				B_caballos.push_back(coordenadas);
				break;
			case TipoPieza::Alfil:
				B_alfiles.push_back(coordenadas);
				break;
			case TipoPieza::Dama:
				B_damas.push_back(coordenadas);
				break;
			case TipoPieza::Rey:
				B_reyes.push_back(coordenadas);
				break;
			case TipoPieza::Peon:
				B_peones.push_back(coordenadas);
				break;
			default:
				break;
			}

			IncrementarContadores(nuevoTipo);
            CorresponderPiezas();
		}

};


    struct Jugada {
		MovimientoCompuesto J_movimiento;
		bool bandoBlancas;
		TipoPieza tipoPieza;
		int indiceBando;
        
        Jugada(MovimientoCompuesto& mc, bool blancas, TipoPieza tipo, int indice):
            J_movimiento{ mc }, bandoBlancas{ blancas }, tipoPieza{ tipo }, indiceBando {
            indice
        } {}
    };

	using Jugadas = std::vector<Jugada>;

    // Define la clase Juego para representar el juego de ajedrez
    class Juego {
    private:
		mutable std::mutex J_mutex; // Mutex para controlar las operaciones concurrentes 
		std::shared_ptr<Juego> J_copiaJuego;		

        TableroAjedrez J_tablero;
        std::shared_ptr<Pieza> casillaNula;

		void ActualizarCopiaJuego() {
			std::lock_guard<std::mutex> lock(J_mutex);            

			if (!J_copiaJuego)
				J_copiaJuego = std::make_shared<Juego>(*this);
			else
				*J_copiaJuego = *this;
			
		}

        // Definición del método convertirPieza
        std::shared_ptr<Pieza> convertirPieza(const std::shared_ptr<Pieza>& piezaOriginal, TipoPieza tipoDestino) {
            if (!piezaOriginal) return nullptr; // Verifica si la pieza original es nula

            // Extrae los atributos comunes
            Coordenadas posicion = piezaOriginal->getPosicion();
            int indiceBando = piezaOriginal->GetIndiceBando();
            bool bandoBlancas = piezaOriginal->getBandoBlancas();
            bool visible = piezaOriginal->GetVisible();
            Coordenadas otraPiezaRelacionada = piezaOriginal->getOtraPiezaRelacionada();
            CadenaMovimientos movimientosValidados = piezaOriginal->getMovimientos();
            CadenaMovimientos movimientosPasados = piezaOriginal->getMovimientosPasados();

            // Crea la nueva pieza basada en el tipo de destino
            switch (tipoDestino) {
            case TipoPieza::Torre:
                return std::make_shared<Torre>(posicion, indiceBando, bandoBlancas, visible, otraPiezaRelacionada, movimientosValidados);
            case TipoPieza::Alfil:
                return std::make_shared<Alfil>(posicion, indiceBando, bandoBlancas, visible, otraPiezaRelacionada, movimientosValidados);
			case TipoPieza::Dama:
				return std::make_shared<Dama>(posicion, indiceBando, bandoBlancas, visible, otraPiezaRelacionada, movimientosValidados);
			case TipoPieza::Rey:
				return std::make_shared<Rey>(posicion, indiceBando, bandoBlancas, visible, otraPiezaRelacionada, movimientosValidados);
			case TipoPieza::Caballo:
				return std::make_shared<Caballo>(posicion, indiceBando, bandoBlancas, visible, otraPiezaRelacionada, movimientosValidados);
			case TipoPieza::Peon:
				return std::make_shared<Peon>(posicion, indiceBando, bandoBlancas, visible, otraPiezaRelacionada, movimientosValidados);

            default:
                throw std::invalid_argument("Tipo de pieza no soportado para la conversión");
            }
        }

		void ConvertirPiezaEnTablero(const Coordenadas& coordenadas, TipoPieza tipoDestino) {
			auto& PiezaOriginal = J_tablero[coordenadas.x][coordenadas.y]; // Obtenemos la pieza original en la posición
			if (!PiezaOriginal) return; // No hay pieza en la posición
			if (PiezaOriginal->obtenerTipo() == tipoDestino) return; // No se puede convertir a la misma pieza

			auto& Bando = (PiezaOriginal->getBandoBlancas()) ? J_blancas : J_negras;
			// Eliminamos la pieza del bando correspondiente para usar esa posición en el tablero con la nueva pieza construida (para coronación)
			Bando.ActualizarPosicionPieza(PiezaOriginal->obtenerTipo(), PiezaOriginal->GetIndiceBando(), CoordenadasNulas);
			auto nuevaPieza = convertirPieza(PiezaOriginal, tipoDestino);
			// Actualizamos la pieza en el tablero
			Bando.AgregarPieza(Coordenadas_InfoExtra{ coordenadas, tipoDestino }, J_tablero, nuevaPieza);
			
		}

        std::shared_ptr<Pieza>& ObtenerPieza(const Coordenadas& coordenadas) 
        {
            if (SonCoordenadasValidas(coordenadas))
                return J_tablero[coordenadas.x][coordenadas.y];
            else
                return casillaNula;
        }

        std::shared_ptr<Pieza>& ObtenerPieza(bool blancas, TipoPieza tipo, int indice)
        {
			auto& bando = (blancas) ? J_blancas : J_negras;
            auto& coordenadas = bando[tipo][indice];
			return ObtenerPieza(coordenadas);
        }

        const std::shared_ptr<Pieza>& ObtenerPieza(const Coordenadas& coordenadas) const
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
			bando.ActualizarPosicionPieza(pieza->obtenerTipo(), pieza->GetIndiceBando(), mov.Destino);            
        }

        void P_MoverPieza(const Movimiento& movimiento)
        {
            // Actualizar datos correspondientes entre el tablero y el indice de bando 
            const Coordenadas& origen{ movimiento.Origen };
            const Coordenadas& destino{ movimiento.Destino };

            // Actualizar bandos correspondientes
            // Al poner coordenadas negativas, indica que la pieza ha sido eliminada del tablero
            ActualizarCoordenadasBando(Movimiento{ destino, CoordenadasNulas }); // Eliminar referencia de pieza antes de ahi, solo se elimina con coordenadas nulas
			ActualizarCoordenadasBando(Movimiento{ origen, destino }); // Cambiar posición de referencia de pieza

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

        bool ValidarMovimientoDentroRangoTablero(MovimientoCompuesto& mov)
        {
			if (!SonCoordenadasValidas(mov.MC_Movimiento.Origen) || !SonCoordenadasValidas(mov.MC_Movimiento.Destino))
			{
				mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Prohibido_FueraDeTablero);
				return false;
			}
			return true;
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
				// Conocer si esa posición es una coronación de peón (1ra Prioridad)
				if (Solucionar::ExistenRepeticiones(mov.MC_Movimiento.Destino.y, 0, 7)) // Se agrega esta condición para evitar que no se valide como coronación
                {
                    mov.MC_Validacion.setTipoEspecial(TipoMovimientoEspecial::PeonCoronacion);
                }                

                // Conocer si previamente es validado como movimiento de captura normal de pieza a pieza
                if (mov.MC_Validacion.getTipoValidacion() != ValidacionMovimiento::Valido_CapturaPiezaEnemiga)
                {
					// Primero comprobar si es una captura normal (2da Prioridad)
					auto& casilla_destino = ObtenerPieza(mov.MC_Movimiento.Destino);
                    if (casilla_destino)
                    {
                        if (casilla_destino->getBandoBlancas() != bandoBlancas) // Captura pieza enemiga directamente
                        {
							mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Valido_CapturaPiezaEnemiga);
                            goto fin;
                        }
                    }

					// De no ser, debemos validar si es captura al paso (3ra Prioridad)
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
                            if (J_jugadas.at(J_jugadas.size() - 1).J_movimiento.MC_Movimiento.Destino != coordenadas_CapturaAlPaso)
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
				// Primero se debe comprobar que no este en jaque
				if (PosicionEnJaque(mov.MC_Movimiento.Origen, bandoBlancas))
				{
					mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Prohibido_ReyEnJaque);
					return false;
				}

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

		void RestaurarCopiaJuegoAntesDeMovimiento(const MovimientoCompuesto& mov)
		{
			if (!J_copiaJuego) return;

			
			auto movimiento = mov.MC_Movimiento.Invertir();
			auto movimientoRel = mov.MC_MovimientoPiezaRelacionada.Invertir();

			// Restauramos la copia del juego antes de realizar el movimiento para que pueda ser usada para cada validación
			J_copiaJuego->P_MoverPieza(movimiento);

			// Restauramos la pieza relacionada si es que existe
			if (mov.MC_MovimientoPiezaRelacionada.Origen != CoordenadasNulas)
			{
                J_copiaJuego->P_MoverPieza(movimientoRel);
			}

			// Si es un movimiento de coronación, se debe eliminar la pieza coronada
			if (mov.MC_Validacion.getTipoEspecial() == TipoMovimientoEspecial::PeonCoronacion)
			{
				// Se debe convertir a peon la pieza coronada
				J_copiaJuego->ConvertirPiezaEnTablero(mov.MC_Movimiento.Origen, TipoPieza::Peon); // Se convierte en peon
			}

			// Si el movimiento eliminó una pieza, se debe restaurar
			if (mov.MC_Validacion.getTipoValidacion() == ValidacionMovimiento::Valido_CapturaPiezaEnemiga)
			{
				// Se debe crear la pieza nuevamente en la posición de destino
				auto piezaRestablecida = J_tablero[mov.MC_Movimiento.Destino.x][mov.MC_Movimiento.Destino.y]->clonar();				
				J_copiaJuego->AgregarPiezaBando({ mov.MC_Movimiento.Destino, piezaRestablecida->obtenerTipo() }, piezaRestablecida);
			}            

		}

		void AgregarPiezaBando(const Coordenadas_InfoExtra& cie, std::shared_ptr<Pieza>& pieza)
        {
			auto& bando = (pieza->getBandoBlancas()) ? J_blancas : J_negras;
			bando.AgregarPieza(cie, J_tablero, pieza);
		}

        bool ValidarPosicionReyFueraDeJaque(MovimientoCompuesto& mov)
        {
            if (!mov.MC_Validacion.getValido()) return false;

            auto& casillaPieza = ObtenerPieza(mov.MC_Movimiento.Origen);
            bool bandoBlancas = casillaPieza->getBandoBlancas();

			// Establecemos el mutex para evitar que se modifique la copia del juego mientras se valida el movimiento
			std::lock_guard <std::mutex> lock(J_mutex); 

			
			if (!J_copiaJuego) 
            { 
				ActualizarCopiaJuego();
                return false;
            }

			// Realizamos el movimiento en la copia del juego
            J_copiaJuego->P_MoverPieza(mov);
			if (mov.MC_MovimientoPiezaRelacionada.Origen != CoordenadasNulas)
            {
				J_copiaJuego->P_MoverPieza(mov.MC_MovimientoPiezaRelacionada);
			}
            auto& bando = (bandoBlancas) ? J_copiaJuego->J_blancas : J_copiaJuego->J_negras;

            // Validamos que la posición actual del unico rey 0, no este en jaque
            if (J_copiaJuego->PosicionEnJaque(bando[TipoPieza::Rey][0], bandoBlancas))
                mov.MC_Validacion.setTipoValidacion(ValidacionMovimiento::Prohibido_ReyEnJaque);

			// Restauramos el juego original
			RestaurarCopiaJuegoAntesDeMovimiento(mov);
            
            return mov.MC_Validacion.getValido();
        }

        bool EsReyEnJaque(bool BandoBlancas)
        {
            auto& bando = (BandoBlancas) ? J_blancas : J_negras;
            return PosicionEnJaque(bando[TipoPieza::Rey][0], BandoBlancas);
        }

        void ValidadorGeneralDeMovimientos(MovimientoCompuesto& movimiento)
        {
			if (!ValidarMovimientoDentroRangoTablero(movimiento)) return;
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
            /*if (!EsPiezaDelTurno(x, y))
            {
                J_tablero[x][y]->LimpiarMovimientosPosibles();
                return;
            }*/

            auto& pieza = ObtenerPieza({ x, y });
			if (!pieza) return;

            auto fncValidador = std::bind(&Juego::ValidadorGeneralDeMovimientos, this, std::placeholders::_1);
            pieza->calcularMovimientos(fncValidador);
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

        void CalcularTodosLosMovimientosBando(bool BandoBlancas) {
            
            auto& bando = (BandoBlancas) ? J_blancas : J_negras;

            for (auto& cie : bando.B_piezas) {
				auto& pieza = ObtenerPieza(cie);
				if (pieza) {
					CalcularMovimientosPieza(cie);
				}            
            
            }
        
        }

        void CalcularTodosLosMovimientos()
        {
			CalcularTodosLosMovimientosBando(true); // Bando Blancas
			CalcularTodosLosMovimientosBando(false); // Bando Negras
		}

		// Calcular todos los movimientos de cada bando de forma concurrente
		void CalcularTodosLosMovimientosConcurrente()
		{
			// No utilizar hasta que se separe cada hilo con su propio tablero o se use un mutex
			std::thread t1(&Juego::CalcularTodosLosMovimientosBando, this, true);
			std::thread t2(&Juego::CalcularTodosLosMovimientosBando, this, false);
			// PROVOCA ERRORES EN EL HEAP PORQUE NO SE PUEDE COMPARTIR EL TABLERO
			// AL GUARDAR LOS MOVIMIENTOS DE CADA PIEZA EN EL TABLERO SE PRODUCE UN ERROR DE ASIGNACIÓN DE MEMORIA
			// PORQUE SE ESTÁN COMPARTIENDO LOS PUNTEROS DE LAS PIEZAS ENTRE LOS DOS HILOS
            // 
			// Solución: Crear un tablero para cada hilo y al finalizar la ejecución de los hilos, copiar los tableros a J_tablero
            //CalcularTodosLosMovimientosBando(true); // Bando Blancas
            //CalcularTodosLosMovimientosBando(false); // Bando Negras

			t1.join();
			t2.join();
        }

        bool TieneMovimientosDisponiblesBando(bool BandoBlancas)
        {
			// Comprobar si un bando tiene movimientos disponibles
			auto& bando = (BandoBlancas) ? J_blancas : J_negras;
			for (auto& cie : bando.B_piezas) {
				
                auto& pieza = ObtenerPieza(cie);
                if (!pieza) continue;

				if (pieza->getMovimientos().size() > 0) return true;
			}
			return false;
        }

        Bando& getBando(bool bandoBlancas) {
            return (bandoBlancas) ? J_blancas : J_negras;
        }

    protected:
        EstadoJuego J_estado{};
        Bando J_blancas{ true };
        Bando J_negras{};
        Jugadas J_jugadas{};


		bool EsPiezaDelTurno(const Coordenadas& coordenadas) const
		{
			auto& pieza  = ObtenerPieza(coordenadas);
			if (!pieza) return false;

            return (pieza->getBandoBlancas() == (J_estado.getTurno() == TipoBando::Blancas));
		}

		bool EsPiezaDelTurno(int x, int y) const
		{
			return EsPiezaDelTurno(Coordenadas{ x,y });
		}

    public:
        // Constructor por defecto
        Juego() {
            Inicializar();
        }

		void CopiarTableroDeOtroJuego(const Juego& otro)
        {
            for (size_t i = 0; i < J_tablero.size(); ++i) {
                for (size_t j = 0; j < J_tablero[i].size(); ++j) {
                    if (otro.J_tablero[i][j]) {
						J_tablero[i][j] = std::move(otro.J_tablero[i][j]->clonar());
						
					}
                    else {
						J_tablero[i][j] = nullptr;
					}
				}
			}
		}

        // Constructor de copia
        Juego(const Juego& otro) {
            // Copiar el estado del juego
            J_estado = otro.J_estado;

            // Copiar el tablero
			CopiarTableroDeOtroJuego(otro);

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
			CopiarTableroDeOtroJuego(otro);

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
				CopiarTableroDeOtroJuego(otro);

                // Copiar las instancias de Bando
                J_blancas = otro.J_blancas;
                J_negras = otro.J_negras;
                J_jugadas = otro.J_jugadas;
            }
            return *this;
        }

        // Operador de asignación de movimiento
        Juego& operator=(Juego&& otro) noexcept {
			if (this == &otro) return *this;
            // Mover el estado del juego
            J_estado = std::move(otro.J_estado);

            // Mover el tablero
			CopiarTableroDeOtroJuego(otro);

            // Mover las instancias de Bando
		
            J_blancas = std::move(otro.J_blancas);
            J_negras = std::move(otro.J_negras);
            J_jugadas = std::move(otro.J_jugadas);

            return *this;
        }

		// ObtenerBando se puede usar como pública porque solo es una función que no modificará el estado del juego
		const Bando& ObtenerBando(bool bandoBlancas) const {
			return (bandoBlancas) ? J_blancas : J_negras;
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
            ActualizarCopiaJuego();

        }

        Juego ObtenerCopia()
        {
            return *this;
        }

		bool EsJuegoTerminado() const
		{
			return J_estado.getEstado() == Estado::JaqueMateBlancas ||
				J_estado.getEstado() == Estado::JaqueMateNegras ||
				J_estado.getEstado() == Estado::Ahogado ||
				J_estado.getEstado() == Estado::RepeticionTriple ||
				J_estado.getEstado() == Estado::Tablas;
		}

        bool MoverPieza(const Coordenadas& origen, const Coordenadas& destino, const TipoPieza CoronacionPieza = TipoPieza::Peon)
        {
			if (EsJuegoTerminado()) return false;

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
			case TipoMovimientoEspecial::PeonCoronacion:
				// Cambio en la pieza de peon a la seleccionada
				if (CoronacionPieza == TipoPieza::Rey || CoronacionPieza == TipoPieza::Peon) break; // No se puede coronar a un rey o un peon
				ConvertirPiezaEnTablero(destino, CoronacionPieza);
				pieza = J_tablero[destino.x][destino.y]; // Actualizar la pieza
                break;
            case TipoMovimientoEspecial::ReyEnroqueCorto:               
            case TipoMovimientoEspecial::ReyEnroqueLargo:
                P_MoverPieza(mc.MC_MovimientoPiezaRelacionada);
                break;
            default:
                break;
            }

            // pieza->LimpiarMovimientosPosibles(); // Una vez que la pieza se mueva se debe eliminar sus movimientos calculados ya que no corresponden a su posición actual
            // Actualizar lista de movimientos realizados del juego
            J_jugadas.emplace_back(Jugada{mc, pieza->getBandoBlancas(), pieza->obtenerTipo(), pieza->GetIndiceBando()});
			
			AnalizarEstadoJuego();

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

		const CadenaMovimientos& ObtenerMovimientosPieza(bool bando, TipoPieza tipo, int indice) {
			return ObtenerMovimientosPieza(getBando(bando)[tipo][indice]);
		}

        void AnalizarEstadoJuego()
        {
			// Se cambia de turno y se limpian los movimientos de las piezas
            J_estado.cambiarTurno();
            LimpiarMovimientosValidadosPiezas(); 

			// Se actualiza la copia del juego para que se pueda usar en las validaciones 
            ActualizarCopiaJuego();

			// Se calculan todos los movimientos posibles para saber el estado del juego
            CalcularTodosLosMovimientosConcurrente();

            // Comprobar estado de jaque
            TipoBando Jaque = EsReyEnJaque(true) ? TipoBando::Blancas : EsReyEnJaque(false) ? TipoBando::Negras : TipoBando::Nulo;

			// Comprobar si hay jaque o tablas
			if (Jaque != TipoBando::Nulo)
            {
				J_estado.setEstado(Jaque == TipoBando::Blancas ? Estado::JaqueBlancas : Estado::JaqueNegras);

				// Comprobar si hay jaque mate
                if (!TieneMovimientosDisponiblesBando(J_estado.esTurnoBlancas()))
                {
                    if (Jaque == TipoBando::Blancas)
                    {
						J_estado.setEstado(Estado::JaqueMateNegras);
					}
                    else
                    {
						J_estado.setEstado(Estado::JaqueMateBlancas);
					}
				}
			}
			else
			{
				if (!TieneMovimientosDisponiblesBando(J_estado.esTurnoBlancas()))
				{
					J_estado.setEstado(Estado::Ahogado);
				}
			}

			// Comprobar si hay tablas
			auto fncSoloReyesTienenMovimientos = [&]() -> bool {
                for (auto& bando : { true, false })
                {
                    
					for (auto tipo : { TipoPieza::Torre, TipoPieza::Caballo, TipoPieza::Alfil, TipoPieza::Dama, TipoPieza::Peon })
                    {
						auto bandoSeleccionado = getBando(bando);

						for (int i = 0; i < bandoSeleccionado.CantidadPiezas(tipo); ++i)
                        {
							if (bandoSeleccionado[tipo][i] == CoordenadasNulas) continue; // Pieza eliminada

                            if (ObtenerMovimientosPieza(bando, tipo, i).size() > 0)
                            {
                                return false;                                
                            }
                        }
					
                    }
				}

				if (ObtenerMovimientosPieza(true, TipoPieza::Rey, 0).size() == 0) return false;
				if (ObtenerMovimientosPieza(false, TipoPieza::Rey, 0).size() == 0) return false;

				return true;
			};
            
            auto fncRepeticionTresMovimientos = [&]()-> bool
                {
                    // Comprobar si se han realizado tres movimientos repetidos
                    // Se comprueba si los tres ultimos movimientos son iguales
                    if (J_jugadas.size() > 6)
                    {
                        if (J_jugadas[J_jugadas.size() - 1].J_movimiento.MC_Movimiento.Destino == J_jugadas[J_jugadas.size() - 3].J_movimiento.MC_Movimiento.Origen &&
							J_jugadas[J_jugadas.size() - 1].J_movimiento.MC_Movimiento.Origen == J_jugadas[J_jugadas.size() - 3].J_movimiento.MC_Movimiento.Destino &&
                            J_jugadas[J_jugadas.size() - 3].J_movimiento.MC_Movimiento.Origen == J_jugadas[J_jugadas.size() - 5].J_movimiento.MC_Movimiento.Destino &&
                            J_jugadas[J_jugadas.size() - 3].J_movimiento.MC_Movimiento.Destino == J_jugadas[J_jugadas.size() - 5].J_movimiento.MC_Movimiento.Origen &&
							J_jugadas[J_jugadas.size() - 3].bandoBlancas == J_jugadas[J_jugadas.size() - 5].bandoBlancas &&
							J_jugadas[J_jugadas.size() - 3].tipoPieza == J_jugadas[J_jugadas.size() - 5].tipoPieza &&
							J_jugadas[J_jugadas.size() - 3].indiceBando == J_jugadas[J_jugadas.size() - 5].indiceBando)
                        {
                            return true;
                        }
                    }
					return false;
                };

			// Comprobar estados de tablas
			// Primera comprobacion, solo dos reyes existen
			if (J_blancas.CantidadPiezas() == 1 && J_negras.CantidadPiezas() == 1)
			{
				// Si existe solo una pieza de cada bando, se comprueba que son reyes (el rey nunca es eliminado o capturado)
				J_estado.setEstado(Estado::Tablas);
			}
            else if (fncSoloReyesTienenMovimientos()) // Segunda comprobación si solo los reyes tiene movimientos
            {
				J_estado.setEstado(Estado::Tablas);
            }
			else if (fncRepeticionTresMovimientos()) // Tercera comprobación si se han realizado tres movimientos repetidos
			{
				J_estado.setEstado(Estado::RepeticionTriple);
				
			}

				
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

        void ImprimirTablero(Juego& juego)
        {
			if (!juego.obtenerEstado().esActivo())
            {
                return;
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

        void ImprimirTablero(Juego&& juego = {})
        {
            if (!juego.obtenerEstado().esActivo())
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
