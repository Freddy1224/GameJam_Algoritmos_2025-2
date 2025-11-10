#pragma once
#include "Controladora.h"
namespace POLIDASHRunner {
	//prueba commit eqqq

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>sssssssssssssss
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code herecccccc
			//

			
			teclaActual = Keys::None;
			rand = gcnew Random();
			objControladora = new Controladora(rand);

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
			delete objControladora;
		}

	protected:

	protected:

	protected:

	private:
		Controladora* objControladora;
	private: System::Windows::Forms::Timer^ tmrJuego;
		   Keys teclaActual;
		   Random^ rand;
	private: System::Windows::Forms::Timer^ tmrGenerador;

	private: System::ComponentModel::IContainer^ components;
		   /// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->tmrJuego = (gcnew System::Windows::Forms::Timer(this->components));
			this->tmrGenerador = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// tmrJuego
			// 
			this->tmrJuego->Enabled = true;
			this->tmrJuego->Interval = 33;
			this->tmrJuego->Tick += gcnew System::EventHandler(this, &MyForm::tmrJuego_Tick);
			// 
			// tmrGenerador
			// 
			this->tmrGenerador->Enabled = true;
			this->tmrGenerador->Interval = 2000;
			this->tmrGenerador->Tick += gcnew System::EventHandler(this, &MyForm::tmrGenerador_Tick);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(752, 563);
			this->KeyPreview = true;
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::MyForm_KeyDown);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::MyForm_KeyUp);
			this->ResumeLayout(false);

		}
#pragma endregion

//prueba de commit nube
	// TIMER TICK: Aquí está la lógica que te resultará familiar.
	private: System::Void tmrJuego_Tick(System::Object^ sender, System::EventArgs^ e) {
		// --- 1. Lógica de Movimiento Adaptativo ---
		int velocidad = 10;
		Jugador* jugador = objControladora->getJugador();
		// ¡NUEVO! Obtenemos la orientación actual de la controladora
		OrientacionTramo orientacion = objControladora->getOrientacion();

		// ¡NUEVO! Declaramos dx y dy aquí arriba para que existan en toda la función
		int dx = 0, dy = 0;

		// El switch que decide la dirección basándose en la orientación
		switch (orientacion) {
		case OrientacionTramo::Horizontal_Der:
			if (teclaActual == Keys::Right) dx = velocidad;
			if (teclaActual == Keys::Left)  dx = -velocidad;
			if (teclaActual == Keys::Up)    dy = -velocidad;
			if (teclaActual == Keys::Down)  dy = velocidad;
			break;
		case OrientacionTramo::Vertical_Arr:
			if (teclaActual == Keys::Up)    dy = -velocidad;
			if (teclaActual == Keys::Down)  dy = velocidad;
			if (teclaActual == Keys::Left)  dx = -velocidad;
			if (teclaActual == Keys::Right) dx = velocidad;
			break;
		case OrientacionTramo::Horizontal_Izq:
			if (teclaActual == Keys::Left)  dx = -velocidad;
			if (teclaActual == Keys::Right) dx = velocidad;
			if (teclaActual == Keys::Up)    dy = -velocidad;
			if (teclaActual == Keys::Down)  dy = velocidad;
			break;
		}
		if (teclaActual == Keys::None) { dx = 0; dy = 0; }
		jugador->setVelocidad(dx, dy);

		// --- 2. Lógica del Juego y Doble Búfer ---
		objControladora->gestionarColisiones();

		Graphics^ canvas = this->CreateGraphics();
		BufferedGraphicsContext^ espacioBuffer = BufferedGraphicsManager::Current;
		BufferedGraphics^ buffer = espacioBuffer->Allocate(canvas, this->ClientRectangle);

		// Cambia el color del fondo según el tramo
		switch (objControladora->getTramoActual()) {
		case 1: buffer->Graphics->Clear(Color::LightSkyBlue); break;
		case 2: buffer->Graphics->Clear(Color::LightGreen); break;
		case 3: buffer->Graphics->Clear(Color::LightCoral); break;
		}

		objControladora->moverMundo();
		objControladora->dibujarEscena(buffer->Graphics);

		// Dibujamos la UI (línea y minimapa)
		int minimapAncho = 200;
		int pistaAncho = this->ClientSize.Width - minimapAncho;
		buffer->Graphics->DrawLine(Pens::Black, pistaAncho, 0, pistaAncho, this->ClientSize.Height);

		// ¡CORRECCIÓN! Pasamos el alto de la ventana a dibujarIndicadores
		objControladora->dibujarIndicadores(buffer->Graphics, pistaAncho, this->ClientSize.Width, this->ClientSize.Height);

		buffer->Render(canvas);
		delete buffer;
		delete espacioBuffer;
		delete canvas;
		if (objControladora->getJuegoTerminado()) {
			tmrJuego->Stop(); // Detiene el bucle principal
			tmrGenerador->Stop(); // Detiene la creación de powers
			MessageBox::Show("¡YOU WIN!");
		}
		else if (objControladora->getJugadorHaPerdido()) { 
			tmrJuego->Stop();
			tmrGenerador->Stop();
			MessageBox::Show("¡YOU LOST!");
		}
	}
		   // KEYDOWN: Solo registra qué tecla se ha presionado.
	private: System::Void MyForm_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		teclaActual = e->KeyCode;
	}

		   // KEYUP: Si la tecla que se suelta es la que teníamos registrada, la reseteamos.
	private: System::Void MyForm_KeyUp(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		if (e->KeyCode == teclaActual) {
			teclaActual = Keys::None;
		}
	}


private: System::Void tmrGenerador_Tick(System::Object^ sender, System::EventArgs^ e) {
	int pistaAncho = this->ClientSize.Width - 200;
	int pistaAlto = this->ClientSize.Height;
	int x_inicial, y_inicial, dx_power = 0, dy_power = 0;
	// La velocidad aumenta con cada tramo
	int velocidad_power = 15 + (objControladora->getTramoActual() * 5);

	// Obtiene la orientación actual para decidir de dónde vienen los powers
	OrientacionTramo orientacion = objControladora->getOrientacion();

	switch (orientacion) {
	case OrientacionTramo::Horizontal_Der:
		x_inicial = pistaAncho; // Vienen de la derecha
		y_inicial = rand->Next(50, pistaAlto - 50);
		dx_power = -velocidad_power;
		break;
	case OrientacionTramo::Vertical_Arr:
		x_inicial = rand->Next(50, pistaAncho - 50);
		y_inicial = 0; // Vienen de arriba
		dy_power = velocidad_power;
		break;
	case OrientacionTramo::Horizontal_Izq:
		x_inicial = 0; // Vienen de la izquierda
		y_inicial = rand->Next(50, pistaAlto - 50);
		dx_power = velocidad_power;
		break;
	}

	// Genera los valores aleatorios del power (esto no cambia)
	int valor_central = rand->Next(1, 6);
	int color_aleatorio = rand->Next(0, 3);
	EColor nuevo_color = static_cast<EColor>(color_aleatorio);
	bool esPowerUp = rand->Next(0, 2) == 0;

	// Crea el power y le asigna la velocidad correcta
	Power* nuevoPower = objControladora->crearNuevoPower(x_inicial, y_inicial, valor_central, nuevo_color, esPowerUp);
	nuevoPower->setVelocidad(dx_power, dy_power);
}
};
}
