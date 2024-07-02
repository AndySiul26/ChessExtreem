#pragma once
#ifndef C_LOGIN_H
#define C_LOGIN_H

namespace Chess {

    public ref struct DatosUsuario
    {
        System::String^ Nombre;
        System::String^ Contrasena;
        bool EsActivo;
    };
}


#endif // !C_LOGIN_H
