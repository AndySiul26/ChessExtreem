#pragma once
// El propósito de este archivo de cabecera es evitar la inclusión de archivos de cabecera en 
// otros archivos de cabecera cuando ya hayan sido definidos para evitar las referencias circulares.
// En este archivo se incluyen los archivos de cabecera que se necesitan en otros archivos de cabecera.
// Se incluyen los archivos de cabecera que contienen las definiciones de las clases y funciones que se
// necesitan en otros archivos de cabecera.

#ifndef GLOBALHEADERS_H
#define GLOBALHEADERS_H

#ifndef SOLUCIONAR_H
#include "solucionar.h"
#endif // !SOLUCIONAR_H

#ifndef CHESS_EXTREEM_H
#include "ChessExtreem.h"
#endif // !CHESS_EXTREEM_H

#ifndef CHESS_EFECTOS_H
#include "ChessEfectos.h"
#endif // !CHESS_EFECTOS_H

#ifndef ManejadorDatosChess_H
#include "ManejadorDatosChess.h"
#endif // !ManejadorDatosChess_H


#endif // !GLOBALHEADERS_H