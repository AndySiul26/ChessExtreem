#pragma once

namespace Chess {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Resumen de SelectorPieza
	/// </summary>
	public ref class SelectorPieza : public System::Windows::Forms::Form
	{
	public:

		event EventHandler<String^>^ EnviarDatos;


		SelectorPieza(void)
		{
			InitializeComponent();
			//
		}

		SelectorPieza(String^ datosIniciales)
		{
			InitializeComponent();
			//
			//TODO: agregar código de constructor aquí
			//
			ColorBando = datosIniciales;
		}

	protected:
		/// <summary>
		/// Limpiar los recursos que se estén usando.
		/// </summary>
		~SelectorPieza()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ImageList^ f_Piezas;
	private: System::Windows::Forms::Label^ label0;
	private: array<Label^>^ labels;


	protected:
	private: System::ComponentModel::IContainer^ components;

	private:

		String^ ColorBando;

		/// <summary>
		/// Variable del diseñador necesaria.
		/// </summary>

		void InicializarLabels(void)
		{
			labels = gcnew array<Label^>(4);
			array<String^>^ piezas = { L"C", L"A", L"T", L"D"};
			this->SuspendLayout();
			for (int i = 0; i < 4; i++)
			{
				labels[i] = gcnew Label();
				labels[i]->BackColor = System::Drawing::Color::Transparent;
				labels[i]->ImageKey = piezas[i] + ColorBando;
				labels[i]->ImageList = this->f_Piezas;
				labels[i]->Location = System::Drawing::Point(label0->Location.X + 64 * i, label0->Location.Y);
				labels[i]->Name = "pieza" + i;
				labels[i]->AutoSize = false;
				labels[i]->Size = System::Drawing::Size(64, 64);
				labels[i]->TabIndex = 2;
				labels[i]->BringToFront();
				labels[i]->Click += gcnew System::EventHandler(this, &SelectorPieza::label0_Click);
				this->Controls->Add(labels[i]);
				labels[i]->Parent = this;
				labels[i]->Visible = true;
			}

			label0->Visible = false; // Ocultar el label0 porque solo es de referencia
			this->ResumeLayout();

		}



#pragma region Windows Form Designer generated code
		/// <summary>
		/// Método necesario para admitir el Diseñador. No se puede modificar
		/// el contenido de este método con el editor de código.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(SelectorPieza::typeid));
			this->f_Piezas = (gcnew System::Windows::Forms::ImageList(this->components));
			this->label0 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// f_Piezas
			// 
			this->f_Piezas->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^>(resources->GetObject(L"f_Piezas.ImageStream")));
			this->f_Piezas->TransparentColor = System::Drawing::Color::Transparent;
			this->f_Piezas->Images->SetKeyName(0, L"CB");
			this->f_Piezas->Images->SetKeyName(1, L"AB");
			this->f_Piezas->Images->SetKeyName(2, L"AN");
			this->f_Piezas->Images->SetKeyName(3, L"CB");
			this->f_Piezas->Images->SetKeyName(4, L"CN");
			this->f_Piezas->Images->SetKeyName(5, L"DB");
			this->f_Piezas->Images->SetKeyName(6, L"DN");
			this->f_Piezas->Images->SetKeyName(7, L"PB");
			this->f_Piezas->Images->SetKeyName(8, L"PN");
			this->f_Piezas->Images->SetKeyName(9, L"RB");
			this->f_Piezas->Images->SetKeyName(10, L"RBJ");
			this->f_Piezas->Images->SetKeyName(11, L"RN");
			this->f_Piezas->Images->SetKeyName(12, L"RNJ");
			this->f_Piezas->Images->SetKeyName(13, L"TB");
			this->f_Piezas->Images->SetKeyName(14, L"TN");
			// 
			// label0
			// 
			this->label0->BackColor = System::Drawing::Color::Transparent;
			this->label0->ImageIndex = 3;
			this->label0->ImageList = this->f_Piezas;
			this->label0->Location = System::Drawing::Point(6, 9);
			this->label0->Name = L"label0";
			this->label0->Size = System::Drawing::Size(64, 64);
			this->label0->TabIndex = 2;
			this->label0->Click += gcnew System::EventHandler(this, &SelectorPieza::label0_Click);
			// 
			// SelectorPieza
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->ClientSize = System::Drawing::Size(308, 79);
			this->Controls->Add(this->label0);
			this->StartPosition = FormStartPosition::CenterScreen;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"SelectorPieza";
			this->Text = L"SelectorPieza";
			this->Load += gcnew System::EventHandler(this, &SelectorPieza::frm_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	
	private: System::Void label0_Click(System::Object^ sender, System::EventArgs^ e) {
		Label^ label = (Label^)sender;
		EnviarDatos(this, label->ImageKey);
		this->Close();
	}
private: System::Void frm_Load(System::Object^ sender, System::EventArgs^ e) {
	InicializarLabels();
}
};
}
