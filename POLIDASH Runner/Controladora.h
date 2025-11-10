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
	bool jugadorHaPerdido;
	
	int tramoActual;
	OrientacionTramo orientacionActual;
	float longitudTramo1, longitudTramo2, longitudTramo3;
public:
	Power* crearNuevoPower(int x, int y, int valor, EColor color, bool esPowerUp) {
		Power* nuevoPower = new Power(x, y, valor, color, esPowerUp);
		arrPowers.push_back(nuevoPower);
		return nuevoPower;
	}

	Controladora(Random^ rand) {
		int color_aleatorio = rand->Next(0, 3);
		EColor color_inicial = static_cast<EColor>(color_aleatorio);
		miJugador = new Jugador(100, 450, color_inicial);

		juegoTerminado = false;
		jugadorHaPerdido = false;
		tramoActual = 1;
		orientacionActual = OrientacionTramo::Horizontal_Der;

		longitudTramo1 = 1500.0f;
		longitudTramo2 = 2000.0f;
		longitudTramo3 = 2500.0f;
	}

	~Controladora() {
		delete miJugador;
		for (auto p : arrPowers) {
			delete p;
		}
	}

	int getTramoActual() { return tramoActual; }
	OrientacionTramo getOrientacion() { return orientacionActual; }
	bool getJuegoTerminado() { return juegoTerminado; }
	Jugador* getJugador() { return miJugador; }

	bool getJugadorHaPerdido() { return jugadorHaPerdido; }

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
					if (miJugador->getNumLados() == 10 && tramoActual == 3) {
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

	void dibujarIndicadores(Graphics^ canvas, int x_inicio_minimapa, int ancho_ventana, int alto_ventana) {
		// Dibuja el texto (sin cambios)
		int lados = miJugador->getNumLados();
		int numero = miJugador->getNumCentral();
		int angulos = (lados - 2) * 180;
		int velocidad = Math::Abs(miJugador->getDx()) + Math::Abs(miJugador->getDy());
		Font^ fuente = gcnew Font("Arial", 10, FontStyle::Bold);
		Brush^ brochaTexto = Brushes::Black;
		canvas->DrawString("Lados: " + lados, fuente, brochaTexto, x_inicio_minimapa + 10, 20);
		canvas->DrawString("Número: " + numero, fuente, brochaTexto, x_inicio_minimapa + 10, 40);
		canvas->DrawString("Sum.Ang: " + angulos + "°", fuente, brochaTexto, x_inicio_minimapa + 10, 60);
		canvas->DrawString("Velocidad: " + velocidad, fuente, brochaTexto, x_inicio_minimapa + 10, 80);

		// DIBUJO DEL MINIMAPA EN FORMA DE U
		RectangleF minimapT1 = RectangleF(x_inicio_minimapa + 10, alto_ventana - 40, 180, 20);
		RectangleF minimapT2 = RectangleF(x_inicio_minimapa + 170, 120, 20, alto_ventana - 160);
		RectangleF minimapT3 = RectangleF(x_inicio_minimapa + 10, 120, 160, 20);

		canvas->DrawRectangle(Pens::Gray, minimapT1.X, minimapT1.Y, minimapT1.Width, minimapT1.Height);
		canvas->DrawRectangle(Pens::Gray, minimapT2.X, minimapT2.Y, minimapT2.Width, minimapT2.Height);
		canvas->DrawRectangle(Pens::Gray, minimapT3.X, minimapT3.Y, minimapT3.Width, minimapT3.Height);

		float progreso_jugador = miJugador->getProgreso();
		float mini_x, mini_y;

		if (tramoActual == 1) {
			float p = progreso_jugador / longitudTramo1;
			mini_x = minimapT1.X + (minimapT1.Width * p);
			mini_y = minimapT1.Y + (minimapT1.Height / 2);
		}
		else if (tramoActual == 2) {
			float p = (progreso_jugador - longitudTramo1) / longitudTramo2;
			mini_x = minimapT2.X + (minimapT2.Width / 2);
			mini_y = (minimapT1.Y) - (minimapT2.Height * p);
		}
		else { // Tramo 3
			float p = (progreso_jugador - (longitudTramo1 + longitudTramo2)) / longitudTramo3;
			mini_x = (minimapT2.X) - (minimapT3.Width * p);
			mini_y = minimapT3.Y + (minimapT3.Height / 2);
		}

		Brush^ brocha_jugador;
		switch (miJugador->getColor()) {
		case EColor::Rojo:     brocha_jugador = Brushes::Red; break;
		case EColor::Amarillo: brocha_jugador = Brushes::Yellow; break;
		case EColor::Azul:     brocha_jugador = Brushes::Blue; break;
		}

		canvas->FillEllipse(brocha_jugador, static_cast<float>(mini_x - 5), static_cast<float>(mini_y - 5), 10.0f, 10.0f);
	}

	void moverMundo() {
		// Guardamos el tramo actual ANTES de mover nada
		int tramo_anterior = tramoActual;

		// El jugador se mueve primero
		miJugador->mover(orientacionActual);
		for (auto p : arrPowers) {
			p->mover(orientacionActual);
		}

		// --- LÓGICA DE TRANSICIÓN (UNA SOLA VÍA) ---
		float progreso = miJugador->getProgreso();

		// Solo podemos avanzar, nunca retroceder
		if (tramoActual == 1 && progreso >= longitudTramo1) {
			tramoActual = 2;
		}
		else if (tramoActual == 2 && progreso >= longitudTramo1 + longitudTramo2) {
			tramoActual = 3;
		}

		// --- REPOSICIONAMIENTO POST-TRANSICIÓN ---
		// Si el tramo acaba de cambiar en este fotograma...
		if (tramoActual != tramo_anterior) {
			if (tramoActual == 2) {
				// Acabamos de entrar al tramo 2
				orientacionActual = OrientacionTramo::Vertical_Arr;
				// Reposicionamos al jugador en la esquina inferior derecha para que empiece a subir
				miJugador->iniciarTramo2(752 - 200, 563); // Pasamos el ancho de la pista y el alto de la ventana
			}
			else if (tramoActual == 3) {
				// Acabamos de entrar al tramo 3
				orientacionActual = OrientacionTramo::Horizontal_Izq;
				// Reposicionamos al jugador en la esquina superior derecha para que empiece a ir a la izquierda
				miJugador->iniciarTramo3(752 - 200); // Pasamos el ancho de la pista
			}
		}

		// --- Lógica de Derrota (se queda igual) ---
		float longitudTotal = longitudTramo1 + longitudTramo2 + longitudTramo3;
		if (progreso >= longitudTotal) {
			if (!juegoTerminado) {
				jugadorHaPerdido = true;
			}
		}
	}

	void dibujarEscena(Graphics^ canvas) {
		miJugador->dibujar(canvas);
		for (auto p : arrPowers) {
			p->dibujar(canvas);
		}
	}

};