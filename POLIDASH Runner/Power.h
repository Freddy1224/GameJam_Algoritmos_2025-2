#pragma once
#include "Figura.h"
using namespace System;

class Power : public Figura {
public:
	
	Power(int x, int y, int valor, EColor color, bool esPowerUp) : Figura(x, y, 4, valor, color) {
		
	}

	
	void mover(OrientacionTramo orientacion) override {
		x += dx;
		y += dy;
	}

	void dibujar(Graphics^ canvas) override {
		Brush^ brush;
		switch (this->color) {
		case EColor::Rojo:     brush = Brushes::Red; break;
		case EColor::Amarillo: brush = Brushes::Yellow; break;
		case EColor::Azul:     brush = Brushes::Blue; break;
		default:               brush = Brushes::Black; break;
		}

		canvas->FillRectangle(brush, x, y, ancho, alto);

		String^ texto = num_central.ToString();
		Font^ fuente = gcnew Font("Arial", 12, FontStyle::Bold);
		Brush^ brochaTexto = Brushes::White;
		SizeF^ tamanoTexto = canvas->MeasureString(texto, fuente);
		PointF posTexto = PointF(x + (ancho / 2) - (tamanoTexto->Width / 2),
			y + (alto / 2) - (tamanoTexto->Height / 2));
		canvas->DrawString(texto, fuente, brochaTexto, posTexto);
	}


	int getValor() { return num_central; }
};