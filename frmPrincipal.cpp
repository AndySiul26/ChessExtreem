﻿#include "frmPrincipal.h"
#include "Login.h"  // Incluye el encabezado del nuevo formulario

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    Chess::Login^ frmLogin = gcnew Chess::Login();
    Application::Run(frmLogin);

    // Si quieres que el formulario principal se muestre después de cerrar el nuevo formulario,
    // puedes usar una condición o simplemente ejecutarlo:
    if (frmLogin->DialogResult == System::Windows::Forms::DialogResult::OK)
    {
        // Mostramos en consola los datos del usuario
        Console::WriteLine("Nombre: " + frmLogin->datosUsuario->Nombre);
        Console::WriteLine("Contraseña: " + frmLogin->datosUsuario->Contrasena);
        Console::WriteLine("Activo: " + frmLogin->datosUsuario->EsActivo);


        Chess::frmPrincipal^ formularioPrincipal = gcnew Chess::frmPrincipal();
        Application::Run(formularioPrincipal);
    }

    return 0;
}