#pragma once
#include "ChessEfectos.h"
#include <string>
#include <msclr/marshal_cppstd.h>

namespace Chess {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Imaging;
	using namespace ChessExtreem;
	#define WinGeneric System::Collections::Generic
	
	Juego juegoGeneral;
	std::vector<std::unique_ptr<Juego>> otrosJuegos;

	/// <summary>
	/// Resumen de frmPrincipal
	/// </summary>
	public ref class frmPrincipal : public System::Windows::Forms::Form
	{
	public:
		frmPrincipal(void)
		{
			InitializeComponent();
			//
			//TODO: agregar código de constructor aquí
			//
			
			InicializarOtrosComponentes();
		}

	protected:
		/// <summary>
		/// Limpiar los recursos que se estén usando.
		/// </summary>
		~frmPrincipal()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::ComponentModel::IContainer^ components;
	private: System::Windows::Forms::PictureBox^ f_Tablero;
	private: System::Windows::Forms::ImageList^ f_Piezas;

	private: System::Windows::Forms::Label^ f_Casilla;
	private: System::Windows::Forms::Label^ f_Casilla_Seleccionada;
	private: System::Windows::Forms::ImageList^ f_Efectos;



	private: static WinGeneric::List<System::Tuple<Label^, float>^>^ brightnessList;
	private: 

		   void AdjustBrightness(Label^ label, float brillo)
		   {
			   // Crear una instancia de EncontrarLabel con el valor a encontrar
			   EncontrarLabel^ myObject = gcnew EncontrarLabel(label);

			   // Crear un predicado que apunte al método de instancia 
			   Predicate<Tuple<Label^, float>^>^ match = gcnew Predicate<Tuple<Label^, float>^>(myObject, &EncontrarLabel::LabelMatches);

			   // Usar FindIndex para encontrar el índice del primer elemento que cumple con la condición
			   int index = brightnessList->FindIndex(match);

			   // Si se encuentra el Label, actualizar su brillo
			   if (index != -1)
			   {
				   float currentBrightness = brightnessList[index]->Item2;
				   float diferenciaBrillo = currentBrightness - 1.0;
				   float newBrightness = brillo - (diferenciaBrillo*2);

				   // Crear una nueva Tuple con el brillo actualizado
				   brightnessList[index] = System::Tuple::Create(label, brillo);

				   EstablecerBrillo(label, newBrightness);

			   }
		   }
		   		   
		   void InicializarOtrosComponentes()
		   {
			   
			   brightnessList = gcnew WinGeneric::List<System::Tuple<Label^, float>^>();
			   CreateChessBoard();
			   
		   }

		   void CopyLabelsAndSetLocation(Label^ origen, Label^ destino, Point ubicacion)
		   {
			   destino->BackColor = origen->BackColor;
			   destino->AutoSize = false;
			   destino->Size = origen->Size;
			   destino->Location = ubicacion;			   
			   destino->ImageList = origen->ImageList;			   
			   destino->Visible = true;
			   destino->Click += gcnew System::EventHandler(this, &frmPrincipal::f_Casilla_Click);			   
			   destino->BringToFront();			   
		   }

		   void CopyLabelsAndSetLocation(Label^ origen, Label^ destino, Point ubicacion, String^ Imgkey)
		   {
			   CopyLabelsAndSetLocation(origen, destino, ubicacion);
			   destino->ImageKey = Imgkey;

		   }

		   String^ KeyPieza(int x, int y)
		   {
			   std::wstring key;

			   // Mapeo de tipos de piezas a caracteres de representación
			   std::unordered_map<TipoPieza, std::wstring> piezaARepresentacion = {
				   { TipoPieza::Torre, L"T" },
				   { TipoPieza::Caballo, L"C" },
				   { TipoPieza::Alfil, L"A" },
				   { TipoPieza::Dama, L"D"},
				   { TipoPieza::Rey, L"R" },
				   { TipoPieza::Peon, L"P" }
			   };
			   auto& pieza = juegoGeneral[x][y];

			   if (pieza)
			   {
				   auto it = piezaARepresentacion.find(pieza->obtenerTipo());
				   if (it != piezaARepresentacion.end()) {
					   key = it->second;
				   }
				   key += (pieza->getBandoBlancas()) ? L"B.png" : L"N.png";
			   }

			   return msclr::interop::marshal_as<String^>(key);
		   }

		   String^ KeyPieza(int x, int y, int indexOtroJuego)
		   {
			   std::wstring key;

			   // Mapeo de tipos de piezas a caracteres de representación
			   std::unordered_map<TipoPieza, std::wstring> piezaARepresentacion = {
				   { TipoPieza::Torre, L"T" },
				   { TipoPieza::Caballo, L"C" },
				   { TipoPieza::Alfil, L"A" },
				   { TipoPieza::Dama, L"D"},
				   { TipoPieza::Rey, L"R" },
				   { TipoPieza::Peon, L"P" }
			   };
			   auto& pieza = (*otrosJuegos[indexOtroJuego])[x][y];

			   if (pieza)
			   {
				   auto it = piezaARepresentacion.find(pieza->obtenerTipo());
				   if (it != piezaARepresentacion.end()) {
					   key = it->second;
				   }
				   key += (pieza->getBandoBlancas()) ? L"B.png" : L"N.png";
			   }

			   return msclr::interop::marshal_as<String^>(key);
		   }

		   void InicializarLabelsCasillas()
		   {
			   if (labels)
				   return;

			   labels = gcnew array<Label^>(64);
		   }

		   Label^ Casilla(int x, int y)
		   {
			   InicializarLabelsCasillas();
			   int index = x * 8 + y; // Obtener indice con coordenadas de tablero

			   return labels[index];

		   }

		   Label^ Casilla(const Coordenadas& c)
		   {
			   return Casilla(c.x, c.y);
		   }
		   
		   Coordenadas ObtenerCoordenadasTableroCasilla(Label^ casilla)
		   {
			   auto datos = ObtenerDatosTag(casilla);
			   return std::move(Coordenadas{ datos->x,datos->y });
		   }
		   
		   void ActualizarImagenCasilla(Coordenadas&& c)
		   {
			   int x{ c.x }, y{ c.y };
			   auto datos = ObtenerDatosTag(Casilla(c));
			   if (Casilla(x, y)->BackColor != Color::Transparent) Casilla(x, y)->BackColor = Color::Transparent; // Dejar color transparente
			   if (Casilla(x, y)->ImageKey == KeyPieza(x, y) && !datos->conEfecto) return; // Innecesario actualizar

			   Casilla(x, y)->ImageKey = KeyPieza(x, y);
			   Casilla(x, y)->Image = f_Piezas->Images[KeyPieza(x, y)];
		   }

		   void ActualizarImagenCasilla(Coordenadas&& c, int indexOtroJuego)
		   {
			   int x{ c.x }, y{ c.y };
			   auto datos = ObtenerDatosTag(Casilla(c));
			   if (Casilla(x, y)->BackColor != Color::Transparent) Casilla(x, y)->BackColor = Color::Transparent; // Dejar color transparente
			   if (Casilla(x, y)->ImageKey == KeyPieza(x, y, indexOtroJuego) && !datos->conEfecto ) return; // Innecesario actualizar

			   Casilla(x, y)->ImageKey = KeyPieza(x, y, indexOtroJuego);
			   Casilla(x, y)->Image = f_Piezas->Images[KeyPieza(x, y, indexOtroJuego)];
		   }

		   // Actualiza la visualización del tablero en la interfaz segun el tableroGeneral del juego
		   void ActualizarTablero(int indexOtroJuego, bool mostrarAunqueNoHayaCambios, bool usarJuegoGeneral)
		   {
			   if (usarJuegoGeneral)
			   {
				   if (!labels)
				   {
					   throw gcnew ChessException(L"¡Controles para las casillas del tablero no cargadas!", ChessInterfazError::LabelsNoCargados);
					   return;
				   }

				   if (!juegoGeneral.obtenerEstado().getCambiado() && !mostrarAunqueNoHayaCambios) return;

				   for (int x{ 0 }; x < 8; ++x)
				   {
					   for (int y{ 0 }; y < 8; ++y)
					   {
						   ActualizarImagenCasilla(Coordenadas{ x,y });
					   }
				   }

				   ModoConsola::ImprimirTablero(std::move(juegoGeneral));
			   }
			   else
			   {
				   if (indexOtroJuego < 0 || indexOtroJuego > otrosJuegos.size() - 1) return;

				   if (!labels)
				   {
					   throw gcnew ChessException(L"¡Controles para las casillas del tablero no cargadas!", ChessInterfazError::LabelsNoCargados);
					   return;
				   }

				   if (!otrosJuegos[indexOtroJuego]->obtenerEstado().getCambiado() && !mostrarAunqueNoHayaCambios) return;

				   for (int x{ 0 }; x < 8; ++x)
				   {
					   for (int y{ 0 }; y < 8; ++y)
					   {
						   // Actualiza la imagen de la casilla solo si es diferente para optimizar
						   ActualizarImagenCasilla(Coordenadas{ x,y }, indexOtroJuego);
						   
					   }
				   }

				   ModoConsola::ImprimirTablero(std::move(juegoGeneral));
			   }
		   }

		   void ActualizarTableroInterfaz()
		   {
			   ActualizarTablero(0, false, true);
		   }

		   void ActualizarTableroInterfaz(bool mostrarAunqueNoHayaCambios)
		   {
			   ActualizarTablero(0, mostrarAunqueNoHayaCambios, true);
		   }

		   void ActualizarTableroInterfaz(int indexOtroJuego)
		   {
			   ActualizarTablero(indexOtroJuego, false, false);
		   }

		   void ActualizarTableroInterfaz(int indexOtroJuego, bool mostrarAunqueNoHayaCambios)
		   {
			   ActualizarTablero(indexOtroJuego, mostrarAunqueNoHayaCambios, false);
		   }


		   Point LocalizacionConCoordenadasTablero(int x, int y)
		   {
			   int labelWidth = this->f_Casilla->Width;
			   int labelHeight = this->f_Casilla->Height;
			   Point baseLocation = this->f_Casilla->Location;
			   return Point(baseLocation.X + x * labelWidth, baseLocation.Y - (y * labelHeight));
		   }

		   void CreateChessBoard() {
			   Console::WriteLine("Creando juego...");
			   juegoGeneral.CrearJuego();
			   // Poner invisible la casilla base
			   f_Casilla->Parent = f_Tablero;
			   f_Casilla->Location = Point(104 - f_Tablero->Location.X, 551 - f_Tablero->Location.Y);
			   f_Casilla->Visible = false;

			   int rows = 8;
			   int cols = 8;

			   InicializarLabelsCasillas();
			   this->SuspendLayout();
			   for (int x = 0; x < rows; ++x) {
				   for (int y = 0; y < cols; ++y) {
					   int index = x * cols + y;
					   labels[index] = gcnew Label();
					   String^ key_img = KeyPieza(x,y);
					   
					   if (key_img)
						   CopyLabelsAndSetLocation(f_Casilla, labels[index], LocalizacionConCoordenadasTablero(x,y), key_img);
					   else
					   {
						   CopyLabelsAndSetLocation(f_Casilla, labels[index], LocalizacionConCoordenadasTablero(x,y));
					   }

					   // Datos de referencia que guarda cada label
					   CasillaDatosTag^ cdt = gcnew CasillaDatosTag(x, y);
					   labels[index]->Tag = cdt;

					   // Añadir el label al formulario
					   this->Controls->Add(labels[index]);
					   brightnessList->Add(Tuple::Create(labels[index], 1.0f));
					   labels[index]->Parent = f_Tablero;
				   }
			   }
			   f_Tablero->SendToBack();
			   this->ResumeLayout(false);
		   }

		   

	protected:


	private: 
		/// <summary>
		/// Variable del diseñador necesaria.
		/// </summary>
		array<Label^>^ labels;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Método necesario para admitir el Diseñador. No se puede modificar
		/// el contenido de este método con el editor de código.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(frmPrincipal::typeid));
			this->f_Tablero = (gcnew System::Windows::Forms::PictureBox());
			this->f_Piezas = (gcnew System::Windows::Forms::ImageList(this->components));
			this->f_Casilla = (gcnew System::Windows::Forms::Label());
			this->f_Efectos = (gcnew System::Windows::Forms::ImageList(this->components));
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->f_Tablero))->BeginInit();
			this->SuspendLayout();
			// 
			// f_Tablero
			// 
			this->f_Tablero->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"f_Tablero.Image")));
			this->f_Tablero->Location = System::Drawing::Point(60, 60);
			this->f_Tablero->Name = L"f_Tablero";
			this->f_Tablero->Size = System::Drawing::Size(600, 600);
			this->f_Tablero->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->f_Tablero->TabIndex = 0;
			this->f_Tablero->TabStop = false;
			this->f_Tablero->Click += gcnew System::EventHandler(this, &frmPrincipal::f_Tablero_Click);
			// 
			// f_Piezas
			// 
			this->f_Piezas->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^>(resources->GetObject(L"f_Piezas.ImageStream")));
			this->f_Piezas->TransparentColor = System::Drawing::Color::Transparent;
			this->f_Piezas->Images->SetKeyName(0, L"CB.png");
			this->f_Piezas->Images->SetKeyName(1, L"AB.png");
			this->f_Piezas->Images->SetKeyName(2, L"AN.png");
			this->f_Piezas->Images->SetKeyName(3, L"CB.png");
			this->f_Piezas->Images->SetKeyName(4, L"CN.png");
			this->f_Piezas->Images->SetKeyName(5, L"DB.png");
			this->f_Piezas->Images->SetKeyName(6, L"DN.png");
			this->f_Piezas->Images->SetKeyName(7, L"PB.png");
			this->f_Piezas->Images->SetKeyName(8, L"PN.png");
			this->f_Piezas->Images->SetKeyName(9, L"RB.png");
			this->f_Piezas->Images->SetKeyName(10, L"RBJ.png");
			this->f_Piezas->Images->SetKeyName(11, L"RN.png");
			this->f_Piezas->Images->SetKeyName(12, L"RNJ.png");
			this->f_Piezas->Images->SetKeyName(13, L"TB.png");
			this->f_Piezas->Images->SetKeyName(14, L"TN.png");
			// 
			// f_Casilla
			// 
			this->f_Casilla->BackColor = System::Drawing::Color::Transparent;
			this->f_Casilla->ImageIndex = 3;
			this->f_Casilla->ImageList = this->f_Piezas;
			this->f_Casilla->Location = System::Drawing::Point(103, 552);
			this->f_Casilla->Name = L"f_Casilla";
			this->f_Casilla->Size = System::Drawing::Size(64, 64);
			this->f_Casilla->TabIndex = 1;
			this->f_Casilla->Click += gcnew System::EventHandler(this, &frmPrincipal::f_Casilla_Click);
			// 
			// f_Efectos
			// 
			this->f_Efectos->ColorDepth = System::Windows::Forms::ColorDepth::Depth32Bit;
			this->f_Efectos->ImageSize = System::Drawing::Size(60, 60);
			this->f_Efectos->TransparentColor = System::Drawing::Color::Transparent;
			// 
			// frmPrincipal
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(734, 711);
			this->Controls->Add(this->f_Casilla);
			this->Controls->Add(this->f_Tablero);
			this->MaximizeBox = false;
			this->Name = L"frmPrincipal";
			this->Text = L"frmPrincipal";
			this->Load += gcnew System::EventHandler(this, &frmPrincipal::frmPrincipal_Load);
			this->Click += gcnew System::EventHandler(this, &frmPrincipal::frmPrincipal_Click);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->f_Tablero))->EndInit();
			this->ResumeLayout(false);

		}

		
#pragma endregion
	private: System::Void frmPrincipal_Load(System::Object^ sender, System::EventArgs^ e) {
		
		/*for (auto i : Solucionar::Rango(4, -5))
		{
			Console::WriteLine("Papas... " + i);
			
		}*/
	}

	private: System::Void frmPrincipal_Click(System::Object^ sender, System::EventArgs^ e) {
		Console::WriteLine("Click fondo...");

		
		static bool mostrarOtroJuego{ false};
		mostrarOtroJuego = !mostrarOtroJuego;

		if (mostrarOtroJuego)
		{
			if (otrosJuegos.size() < 1)
			{
				otrosJuegos.emplace_back(std::make_unique<Juego>(std::move(juegoGeneral.ObtenerCopia())));
				otrosJuegos[0]->MoverPieza(Coordenadas{ 0,0 }, Coordenadas{ 0, 4 });
			}

			ActualizarTableroInterfaz(0, true);
			
		}
		else
		{
			ActualizarTableroInterfaz(true);
		}
	}
	
private: 

	void EfectoSeleccionCasilla(Label^ label)
	{
		

		ChangeImageTone(label, 20, 125, 10);
		
	}

	void EfectoSeleccionCasilla()
	{
		if (!f_Casilla_Seleccionada) return;

		// Restaura la imagen original desde el ImageList
		f_Casilla_Seleccionada->Image = f_Piezas->Images[f_Casilla_Seleccionada->ImageKey];

	}
		
	void MostrarPosiblesMovimientos(Label^ cPieza)
	{
		Coordenadas coordenadas{ std::move(ObtenerCoordenadasTableroCasilla(cPieza) )};
		auto& movimientos = juegoGeneral.ObtenerMovimientosPieza(coordenadas);
		for (auto& mov : movimientos)
		{
			if (!mov.MC_Validacion.getValido()) continue;

			Color color = ColorSegunValidacion(mov.MC_Validacion);
			EfectoMovimiento(Casilla(mov.MC_Movimiento.Destino), color);
		}
	}

	bool EsPiezaDelTurno(Label^ label)
	{
		auto datos = ObtenerDatosTag(label);
		return juegoGeneral.EsPiezaDelTurno(datos->x, datos->y);
	}

	void SeleccionarCasilla(Label^ label)
	{
		// No va el turno de ese bando 
		if (!EsPiezaDelTurno(label))
			throw gcnew System::ArgumentException(L"Error: Turno de bando distinto al bando de la pieza seleccionada");

		ActualizarTableroInterfaz(true);
		if (f_Casilla_Seleccionada == label) // Deselección
		{
			
			ActualizarDatosCasilla(label, false, false);
			f_Casilla_Seleccionada = nullptr;
			
		}
		else
		{
			
			// Seleccion de nueva pieza
			f_Casilla_Seleccionada = label;
			ActualizarDatosCasilla(label, true, true);
			EfectoSeleccionCasilla(label);
			MostrarPosiblesMovimientos(label);
			
		}
	}

	void EfectoMovimiento(Label^ label, CasillaDestino destino)
	{
		label->BackColor = getColorCasillaDestino(destino); 
	}

	void EfectoMovimiento(Label^ label, Color color)
	{
		label->BackColor = color; 
	}

	System::Void f_Casilla_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		Label^ label{ safe_cast<Label^>(sender) };
		/*if (!label->Tag)
		{
			label->Tag = true;
			AdjustBrightness(safe_cast<Label^>(sender), .5);
		}
		else
		{			

			label->Tag = nullptr;
			AdjustBrightness(safe_cast<Label^>(sender), 1);
		}*/

	/*	if (!label->Tag)
		{
			label->Tag = true;
			label->ImageKey = "";
		}
		else
		{			
			label->Tag = nullptr;
			label->ImageKey = L"AB.PNG";
		}*/
		
		/*if (!label->Tag)
		{
			label->Tag = true;
			ChangeImageTone(label, 10,30,120);
		}
		else
		{

			label->Tag = nullptr;
			ChangeImageTone(label, -10,-30,-120);			
		}*/

			
		// HighlightEdges(label, Color::FromArgb(255,10,10,10));

		CasillaDatosTag^ datos = dynamic_cast<CasillaDatosTag^>(label->Tag);
		
		auto& pieza = juegoGeneral[datos->x][datos->y];
		bool bandoBlancas{ false };

		if (pieza)
			bandoBlancas = pieza->getBandoBlancas();

		if (f_Casilla_Seleccionada)
		{
			
			if (f_Casilla_Seleccionada != label)
			{
				CasillaDatosTag^ datos_casilla_sel = dynamic_cast<CasillaDatosTag^>(f_Casilla_Seleccionada->Tag);
				if(!pieza || juegoGeneral[datos_casilla_sel->x][datos_casilla_sel->y]->getBandoBlancas() != bandoBlancas)
				{
					SeleccionarCasilla(f_Casilla_Seleccionada); // Deseleccionar
					juegoGeneral.MoverPieza(Coordenadas{ datos_casilla_sel->x, datos_casilla_sel->y }, Coordenadas{ datos->x, datos->y });
				}
				else
				{
					SeleccionarCasilla(label);
				}
			} else
				SeleccionarCasilla(f_Casilla_Seleccionada); // Deseleccionar
		}
		else
		{
			if(!KeyPieza(datos->x, datos->y)->Equals(""))
			{
				try
				{
					SeleccionarCasilla(label);
				}
				catch (Exception^ e)
				{
					Console::WriteLine(e->Message);
				}				
					
			}
			else
			{
				
				// EfectoMovimiento(label, static_cast<CasillaDestino>(Solucionar::generarNumeroAleatorio(3)));
				EfectoMovimiento(label, CasillaDestino::Coronacion);
				//ChangeImageTone(label, 255, 25, 25, 200);
			}
		}

		ActualizarTableroInterfaz();

	}
private: System::Void f_Tablero_Click(System::Object^ sender, System::EventArgs^ e) {
	}
};
}
