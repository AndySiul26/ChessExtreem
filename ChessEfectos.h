#pragma once
#ifndef CHESS_EFECTOS_H
#define CHESS_EFECTOS_H

#include "ChessExtreem.h"


namespace Chess
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Imaging;
	using namespace ChessExtreem;

	public enum class CasillaDestino
	{
		SinValidacion,
		Libre,
		Captura,
		Coronacion,
		Compuesto,
	};

	Color getColorCasillaDestino(CasillaDestino destino)
	{
		switch (destino)
		{
		case Chess::CasillaDestino::SinValidacion:
			return Color::Transparent; 
			break; // Innecesario pero por protocolo
		case Chess::CasillaDestino::Libre:
			return Color::FromArgb(125, 10, 255, 10); // Verde
			break;
		case Chess::CasillaDestino::Captura:
			return Color::FromArgb(125, 255, 10, 10); // Rojo
			break;
		case Chess::CasillaDestino::Coronacion:
			return Color::FromArgb(150, 220, 192, 7); // Amarillo
			break;
		case Chess::CasillaDestino::Compuesto:
			return Color::FromArgb(125, 23, 217, 255); // Azul
			break;
		default:
			return Color::Transparent; // Amarillo
			break;
		}
	}

	Color ColorSegunValidacion(const Validacion& validacion)
	{
		// Mapeo de tipos de tipos de validación a tipos de destino para asignar un color específico
		std::unordered_map<ValidacionMovimiento, CasillaDestino> ValidacionesTipoDestino = {
			{ ValidacionMovimiento::Valido_Coronacion, CasillaDestino::Coronacion },
			{ ValidacionMovimiento::Valido_CapturaPiezaEnemiga, CasillaDestino::Captura},
			{ ValidacionMovimiento::Valido_MovimientoCompuesto, CasillaDestino::Compuesto},
			{ ValidacionMovimiento::Valido_PosicionLibre, CasillaDestino::Libre},			
		};

		CasillaDestino TipoDestino{ CasillaDestino::SinValidacion };

		auto it = ValidacionesTipoDestino.find(validacion.getTipoValidacion());
		if (it != ValidacionesTipoDestino.end()) {
			TipoDestino = it->second;
			
		}
		
		return getColorCasillaDestino(TipoDestino);

	}

	ref class EncontrarLabel {
	public:
		Label^ valueToFind;

		EncontrarLabel(Label^ value) {
			valueToFind = value;
		}

		bool LabelMatches(Tuple<Label^, float>^ tupla) {
			return tupla->Item1 == valueToFind;
		}
	};

	void EstablecerBrillo(Label^ label, float brillo)
	{
		if (!label->Image) return;

		Bitmap^ original = gcnew Bitmap(label->Image);
		Bitmap^ adjustedImage = gcnew Bitmap(original->Width, original->Height);

		array<array<float>^>^ ptsArray = {
			gcnew array<float>{ brillo, 0, 0, 0, 0 },
			gcnew array<float>{ 0, brillo, 0, 0, 0 },
			gcnew array<float>{ 0, 0, brillo, 0, 0 },
			gcnew array<float>{ 0, 0, 0, 1, 0 },
			gcnew array<float>{ 0, 0, 0, 0, 1 }
		};

		ImageAttributes^ imageAttributes = gcnew ImageAttributes();
		imageAttributes->SetColorMatrix(gcnew ColorMatrix(ptsArray), ColorMatrixFlag::Default, ColorAdjustType::Bitmap);

		Graphics^ g = Graphics::FromImage(adjustedImage);
		g->DrawImage(original, System::Drawing::Rectangle(0, 0, original->Width, original->Height), 0, 0, original->Width, original->Height, GraphicsUnit::Pixel, imageAttributes);

		label->Image = adjustedImage;
	}

	Bitmap^ DetectEdges(Bitmap^ original, Color edgeColor) {
		// Convertir la imagen a formato 32bppArgb para manejar la transparencia
		Bitmap^ bmp = gcnew Bitmap(original->Width, original->Height, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->DrawImage(original, 0, 0, original->Width, original->Height);

		// Matrices del operador de Sobel
		array<int, 2>^ Gx = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
		array<int, 2>^ Gy = { { 1, 2, 1}, { 0, 0, 0}, {-1, -2, -1} };

		Bitmap^ edgeBmp = gcnew Bitmap(bmp->Width, bmp->Height, PixelFormat::Format32bppArgb);

		for (int y = 1; y < bmp->Height - 1; y++) {
			for (int x = 1; x < bmp->Width - 1; x++) {
				int pixelX = 0;
				int pixelY = 0;

				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						Color pixelColor = bmp->GetPixel(x + j, y + i);
						int intensity = (pixelColor.R + pixelColor.G + pixelColor.B) / 3;

						pixelX += intensity * Gx[i + 1, j + 1];
						pixelY += intensity * Gy[i + 1, j + 1];
					}
				}

				int edgeValue = Math::Min(Math::Abs(pixelX) + Math::Abs(pixelY), 255);
				if (edgeValue > 128) { // Threshold for edge detection						   
					edgeBmp->SetPixel(x, y, edgeColor);
				}
				else {
					edgeBmp->SetPixel(x, y, Color::FromArgb(0, 0, 0, 0)); // Transparent pixel
				}
			}
		}

		return edgeBmp;
	}

	// Función para aplicar el operador de Canny para la detección de bordes
	Bitmap^ ApplyCannyEdgeDetection(Bitmap^ original, int lowThreshold, int highThreshold, Color edgeColor) {
		// Convertir la imagen a formato 32bppArgb para manejar la transparencia
		Bitmap^ bmp = gcnew Bitmap(original->Width, original->Height, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(bmp);
		g->DrawImage(original, 0, 0, original->Width, original->Height);

		// Convertir la imagen a escala de grises
		Bitmap^ grayBmp = gcnew Bitmap(bmp->Width, bmp->Height, PixelFormat::Format32bppArgb);
		for (int y = 0; y < bmp->Height; y++) {
			for (int x = 0; x < bmp->Width; x++) {
				Color color = bmp->GetPixel(x, y);
				int gray = (color.R + color.G + color.B) / 3;
				grayBmp->SetPixel(x, y, Color::FromArgb(color.A, gray, gray, gray));
			}
		}

		// Aplicar la detección de bordes de Canny (simplificada)
		Bitmap^ edgeBmp = gcnew Bitmap(grayBmp->Width, grayBmp->Height, PixelFormat::Format32bppArgb);
		for (int y = 1; y < grayBmp->Height - 1; y++) {
			for (int x = 1; x < grayBmp->Width - 1; x++) {
				int gx = -grayBmp->GetPixel(x - 1, y - 1).R - 2 * grayBmp->GetPixel(x - 1, y).R - grayBmp->GetPixel(x - 1, y + 1).R
					+ grayBmp->GetPixel(x + 1, y - 1).R + 2 * grayBmp->GetPixel(x + 1, y).R + grayBmp->GetPixel(x + 1, y + 1).R;

				int gy = -grayBmp->GetPixel(x - 1, y - 1).R - 2 * grayBmp->GetPixel(x, y - 1).R - grayBmp->GetPixel(x + 1, y - 1).R
					+ grayBmp->GetPixel(x - 1, y + 1).R + 2 * grayBmp->GetPixel(x, y + 1).R + grayBmp->GetPixel(x + 1, y + 1).R;

				int edgeValue = Math::Min(Math::Abs(gx) + Math::Abs(gy), 255);

				if (edgeValue > lowThreshold && edgeValue < highThreshold) {
					edgeBmp->SetPixel(x, y, edgeColor);
				}
				else {
					edgeBmp->SetPixel(x, y, Color::FromArgb(0, 0, 0, 0)); // Transparent pixel
				}
			}
		}

		return edgeBmp;
	}

	void HighlightEdges(Label^ label, Color edgeColor) {
		if (label->Image == nullptr) {
			return;
		}

		Bitmap^ originalBitmap = gcnew Bitmap(label->Image);
		Bitmap^ edgeBitmap = DetectEdges(originalBitmap, edgeColor);

		// Crear un nuevo bitmap para combinar la imagen original con los bordes
		Bitmap^ resultBitmap = gcnew Bitmap(originalBitmap->Width, originalBitmap->Height, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(resultBitmap);

		// Dibujar la imagen original
		g->DrawImage(originalBitmap, 0, 0, originalBitmap->Width, originalBitmap->Height);

		// Dibujar los bordes
		g->DrawImage(edgeBitmap, 0, 0, edgeBitmap->Width, edgeBitmap->Height);

		// Asignar la nueva imagen con los bordes resaltados al Label
		label->Image = resultBitmap;
	}

	void HighlightEdges(Label^ label, Color edgeColor, int lowThreshold, int highThreshold) {
		if (label->Image == nullptr) {
			return;
		}

		Bitmap^ originalBitmap = gcnew Bitmap(label->Image);
		Bitmap^ edgeBitmap = ApplyCannyEdgeDetection(originalBitmap, lowThreshold, highThreshold, edgeColor);

		// Crear un nuevo bitmap para combinar la imagen original con los bordes
		Bitmap^ resultBitmap = gcnew Bitmap(originalBitmap->Width, originalBitmap->Height, PixelFormat::Format32bppArgb);
		Graphics^ g = Graphics::FromImage(resultBitmap);

		// Dibujar la imagen original
		g->DrawImage(originalBitmap, 0, 0, originalBitmap->Width, originalBitmap->Height);

		// Dibujar los bordes
		g->DrawImage(edgeBitmap, 0, 0, edgeBitmap->Width, edgeBitmap->Height);

		// Asignar la nueva imagen con los bordes resaltados al Label
		label->Image = resultBitmap;
	}

	void ChangeImageTone(Label^ label, int redTone, int greenTone, int blueTone, int alphaIncrement = 0)
	{
		if (label->Image == nullptr) {
			return;
		}

		Bitmap^ bitmap = gcnew Bitmap(label->Image);

		for (int y = 0; y < bitmap->Height; y++)
		{
			for (int x = 0; x < bitmap->Width; x++)
			{
				Color pixelColor = bitmap->GetPixel(x, y);
				int red = pixelColor.R + redTone;
				int green = pixelColor.G + greenTone;
				int blue = pixelColor.B + blueTone;
				int alpha = pixelColor.A + alphaIncrement;

				// Asegurarnos de que los valores de color estén dentro del rango válido
				red = Math::Max(0, Math::Min(255, red));
				green = Math::Max(0, Math::Min(255, green));
				blue = Math::Max(0, Math::Min(255, blue));
				alpha = Math::Max(0, Math::Min(255, alpha));
				
				bitmap->SetPixel(x, y, Color::FromArgb(alpha, red, green, blue));
			}
		}
		label->Image = bitmap;
	}

	public ref class CasillaDatosTag
	{
	public:
		int x;
		int y;
		bool seleccionada;
		bool conEfecto;

		CasillaDatosTag(int x, int y, bool seleccionada, bool conEfecto)
		{
			this->x = x;
			this->y = y;
			this->seleccionada = seleccionada;
			this->conEfecto = conEfecto;
		}

		CasillaDatosTag(int x, int y)
		{
			this->x = x;
			this->y = y;
			this->seleccionada = false;
			this->conEfecto = false;
		}
	};

	CasillaDatosTag^ ObtenerDatosTag(Label^ label)
	{
		return dynamic_cast<CasillaDatosTag^>(label->Tag);
	}

	void ActualizarDatosCasilla(Label^ casilla, bool seleccionada, bool conEfectos)
	{
		CasillaDatosTag^ datos_casilla_sel = ObtenerDatosTag(casilla);
		datos_casilla_sel->seleccionada = seleccionada;
		datos_casilla_sel->conEfecto = conEfectos;
		casilla->Tag = datos_casilla_sel;
	}


	public enum class ChessInterfazError
	{
		Ninguno,
		LabelsNoCargados,
	};

	public ref class ChessException : public Exception
	{
	public:
		ChessException(String^ message, ChessInterfazError error) : Exception(message)
		{
			this->errorCode = error;
		}

		property ChessInterfazError ErrorCode
		{
			ChessInterfazError get() { return errorCode; }
		}

	private:
		ChessInterfazError errorCode;
	};

	public ref class DatosCoronacion
	{
	private:
		TipoPieza tipoPieza;
		bool validado;

	public:
		DatosCoronacion(TipoPieza tipoPieza, bool validado)
		{
			this->tipoPieza = tipoPieza;
			this->validado = validado;
		}

		property TipoPieza TipoPiezaCoronacion
		{
			void set(TipoPieza value) { tipoPieza = value; }
			TipoPieza get() { return tipoPieza; }
		}

		property bool Validado
		{
			void set(bool value) { validado = value; }
			bool get() { return validado; }
		}
	};

}


#endif // !CHESS_EFECTOS_H
