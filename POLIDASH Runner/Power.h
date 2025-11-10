#pragma once
#include "Figura.h"
using namespace System; 

class Power : public Figura {
public:
	int getValor() { return num_central; }
	// El constructor que ya teníamos y que Copilot ayudó a arreglar
	Power(int x, int y, int valor, EColor color, bool esPowerUp) : Figura(x, y, 4, valor, color) {
		// Damos una velocidad inicial para que se mueva a la izquierda
		this->dx = -15;
	}

	void mover(OrientacionTramo orientacion) override {
		// El movimiento del power es simple, no necesita la orientación
		x += dx;
		y += dy; // Añadimos 'y' para que funcione en tramos verticales
	}

	// LA FUNCIÓN CORREGIDA Y COMPLETA PARA DIBUJAR
	void dibujar(Graphics^ canvas) override {
		// 1. Seleccionar el color
		Brush^ brush;
		switch (this->color) {
		case EColor::Rojo:     brush = Brushes::Red; break;
		case EColor::Amarillo: brush = Brushes::Yellow; break;
		case EColor::Azul:     brush = Brushes::Blue; break;
		default:               brush = Brushes::Black; break;
		}

		// 2. Dibujar la figura (un rectángulo)
		canvas->FillRectangle(brush, x, y, ancho, alto);

		// 3. Dibujar el número central
		String^ texto = num_central.ToString();
		Font^ fuente = gcnew Font("Arial", 12, FontStyle::Bold);
		Brush^ brochaTexto = Brushes::White;

		// Medimos el texto para poder centrarlo correctamente dentro del rectángulo
		SizeF^ tamanoTexto = canvas->MeasureString(texto, fuente);
		PointF posTexto = PointF(x + (ancho / 2) - (tamanoTexto->Width / 2),
			y + (alto / 2) - (tamanoTexto->Height / 2));

		canvas->DrawString(texto, fuente, brochaTexto, posTexto);
	}
};