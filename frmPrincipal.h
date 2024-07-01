#pragma once
#include "ChessEfectos.h"
#include "SelectorPieza.h"
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
	private: System::Windows::Forms::ImageList^ f_Piezas2;



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
				   key += (pieza->getBandoBlancas()) ? L"B" : L"N";
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
				   key += (pieza->getBandoBlancas()) ? L"B" : L"N";
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

		   Label^ Casilla(bool blancas, const TipoPieza tipo, int indice)
		   {
			   auto& bando = juegoGeneral.ObtenerBando(blancas);
			   Coordenadas c = bando[tipo][indice];
			   return Casilla(c.x, c.y);
		   }
		   
		   Coordenadas ObtenerCoordenadasTableroCasilla(Label^ casilla)
		   {
			   auto datos = ObtenerDatosTag(casilla);
			   return std::move(Coordenadas{ datos->x,datos->y });
		   }
		   
		   void ActualizarImagenCasilla(Label^ casilla, String^ key)
		   {
			   auto datos = ObtenerDatosTag(casilla);
			   auto c{ std::move(Coordenadas{ datos->x, datos->y }) };

			   if (casilla->BackColor != Color::Transparent) casilla->BackColor = Color::Transparent; // Dejar color transparente			   
			   if (casilla->ImageKey == key && !datos->conEfecto) return; // Innecesario actualizar

			   casilla->ImageKey = key;
			   casilla->ImageList = f_Casilla->ImageList;
			   //casilla->Image = f_Piezas->Images[casilla->ImageKey];
		   }

		   void ActualizarImagenCasilla(Coordenadas&& c)
		   {
			   int x{ c.x }, y{ c.y };			   
			   auto casilla = Casilla(x, y);
			   String^ key = KeyPieza(x, y);

			   ActualizarImagenCasilla(casilla, key);
		   }

		   void ActualizarImagenCasilla(Coordenadas&& c, int indexOtroJuego)
		   {
			   int x{ c.x }, y{ c.y };
			   auto casilla = Casilla(x, y);
			   String^ key = KeyPieza(x, y, indexOtroJuego);

			   ActualizarImagenCasilla(casilla, key);
		   }


		   void EfectosEstadoJuego()
		   {
			   if (juegoGeneral.obtenerEstado().getEstado() == Estado::JaqueBlancas)
			   {
				   // Para mostrar el efecto de Jaque a las blancas cambiaremos a su imagen correspondiente del label
				   Label^ label = Casilla(true, TipoPieza::Rey, 0); // Obtenemos al unico rey blanco en la casilla (label) en la que se encuentra
				   label->ImageKey = "RBJ";
			   }
			   else if (juegoGeneral.obtenerEstado().getEstado() == Estado::JaqueNegras)
			   {
				   // Para mostrar el efecto de Jaque a las negras cambiaremos a su imagen correspondiente del label
				   Label^ label = Casilla(false, TipoPieza::Rey, 0); // Obtenemos al unico rey negro en la casilla (label) en la que se encuentra
				   label->ImageKey = "RNJ";
			   }
		   }

		   void OperacionesSegunEstadoJuego()
		   {
			   EfectosEstadoJuego();

			   // Revisar estado del juego
			   if (juegoGeneral.EsJuegoTerminado())
			   {
				   switch (juegoGeneral.obtenerEstado().getEstado())
				   {
				   case Estado::JaqueMateBlancas:
					   MessageBox::Show("Jaque Mate Blancas");
					   break;
				   case Estado::JaqueMateNegras:
					   MessageBox::Show("Jaque Mate Negras");
					   break;
				   case Estado::Tablas:
					   MessageBox::Show("Tablas");
					   break;
				   case Estado::RepeticionTriple:
					   MessageBox::Show("Repeticion Triple");
					   break;
				   case Estado::Ahogado:
					   MessageBox::Show("Ahogado");
					   break;
				   default:
					   break;
				   }
				   MessageBox::Show("Juego terminado");
			   }

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

				   ModoConsola::ImprimirTablero(juegoGeneral);
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

			   OperacionesSegunEstadoJuego();
			  
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
			   Point^ baseLocation = f_Casilla->Location;

			   f_Casilla->Parent = f_Tablero;
			   f_Casilla->Location = Point(baseLocation->X - f_Tablero->Location.X, baseLocation->Y - f_Tablero->Location.Y);
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
		String^ dataReceived;
		DatosCoronacion^ coronacion;

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
			this->f_Piezas2 = (gcnew System::Windows::Forms::ImageList(this->components));
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->f_Tablero))->BeginInit();
			this->SuspendLayout();
			// 
			// f_Tablero
			// 
			this->f_Tablero->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"f_Tablero.Image")));
			this->f_Tablero->Location = System::Drawing::Point(63, 144);
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
			// f_Casilla
			// 
			this->f_Casilla->BackColor = System::Drawing::Color::Transparent;
			this->f_Casilla->ImageIndex = 3;
			this->f_Casilla->ImageList = this->f_Piezas2;
			this->f_Casilla->Location = System::Drawing::Point(107, 636);
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
			// f_Piezas2
			// 
			this->f_Piezas2->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^>(resources->GetObject(L"f_Piezas2.ImageStream")));
			this->f_Piezas2->TransparentColor = System::Drawing::Color::Transparent;
			this->f_Piezas2->Images->SetKeyName(0, L"CB");
			this->f_Piezas2->Images->SetKeyName(1, L"AB");
			this->f_Piezas2->Images->SetKeyName(2, L"AN");
			this->f_Piezas2->Images->SetKeyName(3, L"CB");
			this->f_Piezas2->Images->SetKeyName(4, L"CN");
			this->f_Piezas2->Images->SetKeyName(5, L"DB");
			this->f_Piezas2->Images->SetKeyName(6, L"DN");
			this->f_Piezas2->Images->SetKeyName(7, L"PB");
			this->f_Piezas2->Images->SetKeyName(8, L"PN");
			this->f_Piezas2->Images->SetKeyName(9, L"RB");
			this->f_Piezas2->Images->SetKeyName(10, L"RN");
			this->f_Piezas2->Images->SetKeyName(11, L"TB");
			this->f_Piezas2->Images->SetKeyName(12, L"TN");
			this->f_Piezas2->Images->SetKeyName(13, L"RBJ");
			this->f_Piezas2->Images->SetKeyName(14, L"RNJ");
			// 
			// frmPrincipal
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(734, 868);
			this->Controls->Add(this->f_Casilla);
			this->Controls->Add(this->f_Tablero);
			this->MaximizeBox = false;
			this->Name = L"frmPrincipal";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
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
		MessageBox::Show("Click fondo...");
		
		SelectorPieza^ frm_Selector = gcnew SelectorPieza((juegoGeneral.obtenerEstado().esTurnoBlancas()) ? L"B": L"N");
		frm_Selector->EnviarDatos += gcnew System::EventHandler<System::String^>(this, &Chess::frmPrincipal::RecepcionPiezaSelector);
		frm_Selector->ShowDialog();
				
	/*	static bool mostrarOtroJuego{ false};
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
		}*/
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
		f_Casilla_Seleccionada->Image = f_Casilla->ImageList->Images[f_Casilla_Seleccionada->ImageKey];

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

	void MoverPieza(Coordenadas origen, Coordenadas destino)
	{
		// Primero debemos validar el movimiento
		auto& movimientos = juegoGeneral.ObtenerMovimientosPieza(origen);
		const MovimientoCompuesto* movimientoSeleccionado{ nullptr };

		for (auto& mov : movimientos)
		{
			if (mov.MC_Movimiento.Destino == destino)
			{				
				movimientoSeleccionado = &mov;
				break;
			}
		}

		if (!movimientoSeleccionado)
		{
			Console::WriteLine("Movimiento no válido");
			movimientoSeleccionado = nullptr;
			return;
		}

		if (movimientoSeleccionado->MC_Validacion.getTipoEspecial() == TipoMovimientoEspecial::PeonCoronacion)
		{
			SelectorPieza^ frm_Selector = gcnew SelectorPieza((juegoGeneral.obtenerEstado().esTurnoBlancas()) ? L"B" : L"N");
			frm_Selector->EnviarDatos += gcnew System::EventHandler<System::String^>(this, &Chess::frmPrincipal::RecepcionPiezaSelector);
			frm_Selector->ShowDialog();
			if (coronacion)
			{
				if (coronacion->Validado)
				{
					juegoGeneral.MoverPieza(origen, destino, coronacion->TipoPiezaCoronacion);
					return;
				}
			}			
			
		}
		
		juegoGeneral.MoverPieza(origen, destino);
		
		movimientoSeleccionado = nullptr;
		// ActualizarTableroInterfaz(); // esto ya se realiza en el evento Casilla_Click

	}

	System::Void f_Casilla_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		// Verificar si el juego ha terminado
		if (juegoGeneral.EsJuegoTerminado())
		{
			MessageBox::Show("Juego terminado");
			return;
		}

		Label^ label{ safe_cast<Label^>(sender) };
	
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

					MoverPieza(Coordenadas{ datos_casilla_sel->x, datos_casilla_sel->y }, Coordenadas{ datos->x, datos->y });
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
				//EfectoMovimiento(label, CasillaDestino::Coronacion);
				//ChangeImageTone(label, 255, 25, 25, 200);
			}
		}

		ActualizarTableroInterfaz();

	}


	void RecepcionPiezaSelector(System::Object^ sender, String^ data)
	{

		std::unordered_map<std::wstring, TipoPieza> representacionAPieza = {
			{ L"T", TipoPieza::Torre },
			{ L"C", TipoPieza::Caballo },
			{ L"A", TipoPieza::Alfil },
			{ L"D", TipoPieza::Dama },
			{ L"R", TipoPieza::Rey },
			{ L"P", TipoPieza::Peon }
		};

		std::wstring key = msclr::interop::marshal_as<std::wstring>(data);
		// data contendrá la representación de la pieza seleccionada como "TB", "CB", "AB", "DB", "RB", "PB"
		// necesitamos obtener su primer caracter representativo para obtener el tipo de pieza
		auto it = representacionAPieza.find(key.substr(0, 1));
		if (it != representacionAPieza.end())
		{
			TipoPieza tipoPieza = it->second;
			coronacion = gcnew DatosCoronacion(tipoPieza, true);
		}
		else
		{
			coronacion = gcnew DatosCoronacion(TipoPieza::Dama, false);
		}

		dataReceived = data;
		Console::WriteLine("Data received: " + dataReceived);
	}

private: System::Void f_Tablero_Click(System::Object^ sender, System::EventArgs^ e) {
	}
};
}
