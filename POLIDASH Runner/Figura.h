#pragma once

using namespace System::Drawing;

// Usamos un enum para manejar los colores de forma más limpia
enum class EColor { Rojo, Amarillo, Azul };

// Clase Padre Abstracta: define las propiedades y métodos que TODOS los objetos tendrán
class Figura {
protected:
	int x, y;
	int dx, dy; // Velocidad en cada eje
	int ancho, alto;
	int num_lados;
	int num_central;
	EColor color;

public:

	int getDx() { return dx; }
	int getDy() { return dy; }

	int getAncho() { return ancho; }// El "= 0" significa que las clases hijas ESTÁN OBLIGADAS a implementar estos métodos
	int getNumCentral() { return num_central; }
	void setNumCentral(int nuevo_valor) { num_central = nuevo_valor; }
	virtual void mover() = 0;
	virtual void dibujar(Graphics^ canvas) = 0;

	// Constructor para inicializar propiedades comunes
	Figura(int x, int y, int num_lados, int num_central, EColor color) {
		this->x = x;
		this->y = y;
		this->num_lados = num_lados;
		this->num_central = num_central;
		this->color = color;
		// Valores por defecto
		this->dx = 0;
		this->dy = 0;
		this->ancho = 50; // Tamaño por defecto, podemos ajustarlo
		this->alto = 50;
	}

	// Método para obtener el rectángulo de colisión del objeto
	Rectangle getRectangle() {
		return Rectangle(x, y, ancho, alto);
	}

	// Getters para acceder a las propiedades desde fuera
	EColor getColor() { return color; }
	int getNumLados() { return num_lados; }
	int getX() { return x; }
	int getY() { return y; }


	// Método para establecer la velocidad del objeto
	void setVelocidad(int dx, int dy) {
		this->dx = dx;
		this->dy = dy;
	}
};