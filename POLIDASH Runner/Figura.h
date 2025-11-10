#pragma once

using namespace System::Drawing;

// colores
enum class EColor { Rojo, Amarillo, Azul };

// orientaciones de los tramos
enum class OrientacionTramo {
	Horizontal_Der,
	Vertical_Arr,
	Horizontal_Izq
};

// clase padre 
class Figura {
protected:
	int x, y;
	int dx, dy;
	int ancho, alto;
	int num_lados;
	int num_central;
	EColor color;

public:
	
	virtual void mover(OrientacionTramo orientacion) = 0;
	virtual void dibujar(Graphics^ canvas) = 0;

	
	Figura(int x, int y, int num_lados, int num_central, EColor color) {
		this->x = x;
		this->y = y;
		this->num_lados = num_lados;
		this->num_central = num_central;
		this->color = color;
		this->dx = 0;
		this->dy = 0;
		this->ancho = 50;
		this->alto = 50;
	}

	
	Rectangle getRectangle() { return Rectangle(x, y, ancho, alto); }
	EColor getColor() { return color; }
	int getNumLados() { return num_lados; }
	int getNumCentral() { return num_central; }
	void setNumCentral(int nuevo_valor) { num_central = nuevo_valor; }
	int getX() { return x; }
	int getY() { return y; }
	int getAncho() { return ancho; }
	int getDx() { return dx; }
	int getDy() { return dy; }
	void setVelocidad(int dx, int dy) {
		this->dx = dx;
		this->dy = dy;
	}
};