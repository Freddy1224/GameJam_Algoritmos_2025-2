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
		// --- 1. Lógica de Movimiento ---
		int velocidad = 10;
		Jugador* jugador = objControladora->getJugador();

		// Revisa el estado de la tecla en cada tick, igual que en tu proyecto de sprites
		switch (teclaActual) {
		case Keys::Up:    jugador->setVelocidad(0, -velocidad); break;
		case Keys::Down:  jugador->setVelocidad(0, velocidad); break;
		case Keys::Left:  jugador->setVelocidad(-velocidad, 0); break;
		case Keys::Right: jugador->setVelocidad(velocidad, 0); break;
		case Keys::None:  // Si no hay ninguna tecla, velocidad 0
		default:          // Para cualquier otra tecla, velocidad 0
			jugador->setVelocidad(0, 0);
			break;
		}

		// --- 2. Doble Búfer y Dibujado ---
		objControladora->gestionarColisiones();
		Graphics^ canvas = this->CreateGraphics();
		BufferedGraphicsContext^ espacioBuffer = BufferedGraphicsManager::Current;
		BufferedGraphics^ buffer = espacioBuffer->Allocate(canvas, this->ClientRectangle);

		buffer->Graphics->Clear(Color::White);

		objControladora->moverMundo();
		objControladora->dibujarEscena(buffer->Graphics);
		
		int minimapAncho = 200; // Ancho del minimapa en píxeles
		int pistaAncho = this->ClientSize.Width - minimapAncho;

		// Dibuja una línea vertical para separar la pista del minimapa
		buffer->Graphics->DrawLine(Pens::Black, pistaAncho, 0, pistaAncho, this->ClientSize.Height);

		// Le pedimos a la controladora que dibuje los indicadores en el minimapa
		objControladora->dibujarIndicadores(buffer->Graphics, pistaAncho, this->ClientSize.Width);

		buffer->Render(canvas);

		delete buffer;
		delete espacioBuffer;
		delete canvas;
		if (objControladora->getJuegoTerminado()) {
			tmrJuego->Stop(); // Detiene el bucle principal
			tmrGenerador->Stop(); // Detiene la creación de powers
			MessageBox::Show("¡YOU WIN!");
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
	int minimapAncho = 200; // Usamos el mismo valor que en el otro timer
	int pistaAncho = this->ClientSize.Width - minimapAncho;

	// Posición X inicial ahora es el ancho de la PISTA
	int x_inicial = this->ClientSize.Width;
	int y_inicial = rand->Next(50, this->ClientSize.Height - 50);
	// Generamos una posición Y aleatoria dentro de los límites de la ventana
	

	// 2. Generar valores aleatorios para el Power
	// Un número aleatorio entre 1 y 5 (el 6 no se incluye)
	int valor_central = rand->Next(1, 6);
	// Un número aleatorio para el color (0=Rojo, 1=Amarillo, 2=Azul)
	int color_aleatorio = rand->Next(0, 3);
	EColor nuevo_color = static_cast<EColor>(color_aleatorio);

	// Cambia la llamada para incluir el argumento faltante 'esPowerUp'.
	// Por ejemplo, si quieres que todos los Powers sean PowerUp, pon 'true'.
	// Si quieres alternar, puedes usar rand->Next(0, 2) == 0 para aleatorio.

	bool esPowerUp = rand->Next(0, 2) == 0; // Genera true o false aleatoriamente
	objControladora->crearNuevoPower(x_inicial, y_inicial, valor_central, nuevo_color, esPowerUp);
}
};
}
