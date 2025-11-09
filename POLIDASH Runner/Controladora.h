#pragma once
#include "Jugador.h"
#include "Power.h"
#include <vector>

using namespace System::Windows::Forms;
using namespace std;

class Controladora {
private:
	Jugador* miJugador;
	vector<Power*> arrPowers;
	bool juegoTerminado;

public:
	void crearNuevoPower(int x, int y, int valor, EColor color, bool esPowerUp) {
		// Creamos un nuevo objeto Power con los datos recibidos
		Power* nuevoPower = new Power(x, y, valor, color, esPowerUp);
		// Lo añadimos a nuestro vector de Powers
		arrPowers.push_back(nuevoPower);
	}
	Controladora(Random^ rand) { // Ahora recibe el generador de números aleatorios
		// Generamos un color aleatorio para el jugador
		int color_aleatorio = rand->Next(0, 3);
		EColor color_inicial = static_cast<EColor>(color_aleatorio);

		// Creamos al jugador con ese color
		miJugador = new Jugador(100, 300, color_inicial);
		juegoTerminado = false;
	}
	bool getJuegoTerminado() { return juegoTerminado; }

	~Controladora() {
		delete miJugador;
		for (auto p : arrPowers) {
			delete p;
		}
	}

	void gestionarColisiones() {
		// Usamos un bucle inverso (desde el final hasta el principio).
		// Esto es CRUCIAL para poder eliminar elementos de un vector de forma segura mientras lo recorremos.
		for (int i = arrPowers.size() - 1; i >= 0; i--) {
			Power* power = arrPowers[i];

			// Verificamos si el rectángulo del jugador intersecta con el del power
			if (miJugador->getRectangle().IntersectsWith(power->getRectangle())) {

				// Si hay colisión, comparamos los colores
				if (miJugador->getColor() == power->getColor()) {
					// --- ES UN POWER-UP ---
					miJugador->evolucionar(); // Gana un lado
					miJugador->sumarAlCentral(power->getValor());
					if (miJugador->getNumLados() == 10) {
						juegoTerminado = true;
					}// Suma el número
				}
				else {
					// --- ES UN POWER-DOWN ---
					miJugador->involucionar(); // Pierde un lado
					miJugador->restarAlCentral(power->getValor()); // Resta el número
				}

				// Una vez procesada la colisión, eliminamos el power
				delete arrPowers[i]; // Liberamos la memoria del objeto
				arrPowers.erase(arrPowers.begin() + i); // Lo quitamos del vector
			}
			else if (power->getX() + power->getAncho() < 0) { // Si el borde derecho del power está a la izquierda del borde 0
				// Eliminamos el power porque ya no es visible
				delete arrPowers[i];
				arrPowers.erase(arrPowers.begin() + i);
			}

		}
	}

	void dibujarIndicadores(Graphics^ canvas, int x_inicio_minimapa, int ancho_ventana) {
		// Obtenemos los datos del jugador
		int lados = miJugador->getNumLados();
		int numero = miJugador->getNumCentral();
		int angulos = (lados - 2) * 180;
		// La velocidad es la suma de los valores absolutos de dx y dy
		int velocidad = Math::Abs(miJugador->getDx()) + Math::Abs(miJugador->getDy());

		// Preparamos las herramientas de dibujo
		Font^ fuente = gcnew Font("Arial", 10, FontStyle::Bold);
		Brush^ brochaTexto = Brushes::Black;

		// Dibujamos cada línea de texto
		canvas->DrawString("Lados: " + lados, fuente, brochaTexto, x_inicio_minimapa + 10, 20);
		canvas->DrawString("Número: " + numero, fuente, brochaTexto, x_inicio_minimapa + 10, 40);
		canvas->DrawString("Sum.Ang: " + angulos + "°", fuente, brochaTexto, x_inicio_minimapa + 10, 60);
		canvas->DrawString("Velocidad: " + velocidad, fuente, brochaTexto, x_inicio_minimapa + 10, 80);
	}

	void moverMundo() {
		// ¡El método moverMundo ya no se encarga de la lógica del teclado!
		// Solo mueve los objetos basándose en su velocidad actual.
		miJugador->mover();
		for (auto p : arrPowers) {
			p->mover();
		}
	}

	void dibujarEscena(Graphics^ canvas) {
		miJugador->dibujar(canvas);

		for (auto p : arrPowers) {
			p->dibujar(canvas);
		}
	}

	Jugador* getJugador() {
		return miJugador;
	}

};