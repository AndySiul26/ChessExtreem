#pragma once
#include "CLogin.h"

namespace Chess {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Resumen de Login
	/// </summary>
	public ref class Login : public System::Windows::Forms::Form
	{
	public:
		Login(void)
		{
			InitializeComponent();
			//
			//TODO: agregar código de constructor aquí
			//
		}

		property DatosUsuario^ datosUsuario;

	protected:
		/// <summary>
		/// Limpiar los recursos que se estén usando.
		/// </summary>
		~Login()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Panel^ panel1;
	protected:
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ InfoLU;
	private: System::Windows::Forms::TextBox^ textBox2;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ ImgLogo;

	private:
		/// <summary>
		/// Variable del diseñador necesaria.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Método necesario para admitir el Diseñador. No se puede modificar
		/// el contenido de este método con el editor de código.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(Login::typeid));
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->ImgLogo = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->InfoLU = (gcnew System::Windows::Forms::Label());
			this->panel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel1->Controls->Add(this->ImgLogo);
			this->panel1->Controls->Add(this->label2);
			this->panel1->Controls->Add(this->button1);
			this->panel1->Controls->Add(this->textBox2);
			this->panel1->Controls->Add(this->textBox1);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->InfoLU);
			this->panel1->Location = System::Drawing::Point(42, 39);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(415, 366);
			this->panel1->TabIndex = 0;
			// 
			// ImgLogo
			// 
			this->ImgLogo->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ImgLogo.Image")));
			this->ImgLogo->Location = System::Drawing::Point(121, 15);
			this->ImgLogo->Name = L"ImgLogo";
			this->ImgLogo->Size = System::Drawing::Size(197, 218);
			this->ImgLogo->TabIndex = 6;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->label2->Location = System::Drawing::Point(12, 325);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(150, 16);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Olvide la contraseña";
			this->label2->TextAlign = System::Drawing::ContentAlignment::BottomRight;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(323, 322);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 4;
			this->button1->Text = L"Entrar";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Login::button1_Click);
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(112, 281);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(286, 20);
			this->textBox2->TabIndex = 3;
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(112, 242);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(286, 20);
			this->textBox1->TabIndex = 2;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->label1->Location = System::Drawing::Point(12, 284);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(86, 16);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Contraseña";
			this->label1->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// InfoLU
			// 
			this->InfoLU->AutoSize = true;
			this->InfoLU->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->InfoLU->ForeColor = System::Drawing::SystemColors::ButtonHighlight;
			this->InfoLU->Location = System::Drawing::Point(12, 245);
			this->InfoLU->Name = L"InfoLU";
			this->InfoLU->Size = System::Drawing::Size(61, 16);
			this->InfoLU->TabIndex = 0;
			this->InfoLU->Text = L"Usuario";
			this->InfoLU->TextAlign = System::Drawing::ContentAlignment::TopRight;
			// 
			// Login
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Navy;
			this->ClientSize = System::Drawing::Size(502, 443);
			this->Controls->Add(this->panel1);
			this->MaximizeBox = false;
			this->Name = L"Login";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Login";
			this->Load += gcnew System::EventHandler(this, &Login::Login_Load);
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		this->DialogResult = System::Windows::Forms::DialogResult::OK;
		
		datosUsuario = gcnew DatosUsuario();
		datosUsuario->Nombre = textBox1->Text;
		datosUsuario->Contrasena = textBox2->Text;
		datosUsuario->EsActivo = true;

		this->Close();
	}
private: System::Void Login_Load(System::Object^ sender, System::EventArgs^ e) {
}
};
}
