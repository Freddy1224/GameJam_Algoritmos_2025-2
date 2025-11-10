#pragma once
#include "Controladora.h"

namespace POLIDASHRunner {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			rand = gcnew Random();
			objControladora = new Controladora(rand);
			teclaActual = Keys::None;
		}

	protected:
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
			delete objControladora;
		}

	private:
		Controladora* objControladora;
		Keys teclaActual;
		Random^ rand;
	private: System::Windows::Forms::Timer^ tmrJuego;
	private: System::Windows::Forms::Timer^ tmrGenerador;
	private: System::Windows::Forms::Button^ btnJugador;
	private: System::Windows::Forms::Button^ btnAutomatico;
	private: System::ComponentModel::IContainer^ components;

#pragma region Windows Form Designer generated code
		   void InitializeComponent(void)
		   {
			   this->components = (gcnew System::ComponentModel::Container());
			   this->tmrJuego = (gcnew System::Windows::Forms::Timer(this->components));
			   this->tmrGenerador = (gcnew System::Windows::Forms::Timer(this->components));
			   this->btnJugador = (gcnew System::Windows::Forms::Button());
			   this->btnAutomatico = (gcnew System::Windows::Forms::Button());
			   this->SuspendLayout();
			   // 
			   // tmrJuego
			   // 
			   this->tmrJuego->Interval = 33;
			   this->tmrJuego->Tick += gcnew System::EventHandler(this, &MyForm::tmrJuego_Tick);
			   // 
			   // tmrGenerador
			   // 
			   this->tmrGenerador->Interval = 2000;
			   this->tmrGenerador->Tick += gcnew System::EventHandler(this, &MyForm::tmrGenerador_Tick);
			   // 
			   // btnJugador
			   // 
			   this->btnJugador->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)),
				   static_cast<System::Int32>(static_cast<System::Byte>(0)));
			   this->btnJugador->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->btnJugador->Location = System::Drawing::Point(406, 221);
			   this->btnJugador->Name = L"btnJugador";
			   this->btnJugador->Size = System::Drawing::Size(150, 60);
			   this->btnJugador->TabIndex = 0;
			   this->btnJugador->Text = L"Jugador";
			   this->btnJugador->UseVisualStyleBackColor = false;
			   this->btnJugador->Click += gcnew System::EventHandler(this, &MyForm::btnJugador_Click);
			   // 
			   // btnAutomatico
			   // 
			   this->btnAutomatico->BackColor = System::Drawing::SystemColors::ActiveCaption;
			   this->btnAutomatico->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->btnAutomatico->Location = System::Drawing::Point(153, 221);
			   this->btnAutomatico->Name = L"btnAutomatico";
			   this->btnAutomatico->Size = System::Drawing::Size(150, 60);
			   this->btnAutomatico->TabIndex = 1;
			   this->btnAutomatico->Text = L"Automático";
			   this->btnAutomatico->UseVisualStyleBackColor = false;
			   this->btnAutomatico->Click += gcnew System::EventHandler(this, &MyForm::btnAutomatico_Click);
			   // 
			   // MyForm
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->ClientSize = System::Drawing::Size(752, 563);
			   this->Controls->Add(this->btnAutomatico);
			   this->Controls->Add(this->btnJugador);
			   this->KeyPreview = true;
			   this->Name = L"MyForm";
			   this->Text = L"POLIDASH Runner";
			   this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::MyForm_KeyDown);
			   this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::MyForm_KeyUp);
			   this->ResumeLayout(false);

		   }
#pragma endregion

	private: void IniciarJuego() {
		btnJugador->Visible = false;
		btnAutomatico->Visible = false;
		tmrJuego->Enabled = true;
		tmrGenerador->Enabled = true;
	}

	private: System::Void btnJugador_Click(System::Object^ sender, System::EventArgs^ e) {
		IniciarJuego();
	}

	private: System::Void btnAutomatico_Click(System::Object^ sender, System::EventArgs^ e) {
		objControladora->activarModoAutomatico();
		IniciarJuego();
	}

	private: System::Void MyForm_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		teclaActual = e->KeyCode;
	}

	private: System::Void MyForm_KeyUp(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		if (e->KeyCode == teclaActual) {
			teclaActual = Keys::None;
		}
	}

	private: System::Void tmrGenerador_Tick(System::Object^ sender, System::EventArgs^ e) {
		int pistaAncho = this->ClientSize.Width - 200;
		int pistaAlto = this->ClientSize.Height;
		int x_inicial = 0, y_inicial = 0, dx_power = 0, dy_power = 0;
		int velocidad_power = 15 + (objControladora->getTramoActual() * 5);
		OrientacionTramo orientacion = objControladora->getOrientacion();

		switch (orientacion) {
		case OrientacionTramo::Horizontal_Der:
			x_inicial = pistaAncho;
			y_inicial = rand->Next(50, pistaAlto - 50);
			dx_power = -velocidad_power;
			break;
		case OrientacionTramo::Vertical_Arr:
			x_inicial = rand->Next(50, pistaAncho - 50);
			y_inicial = 0;
			dy_power = velocidad_power;
			break;
		case OrientacionTramo::Horizontal_Izq:
			x_inicial = 0;
			y_inicial = rand->Next(50, pistaAlto - 50);
			dx_power = velocidad_power;
			break;
		}

		int valor_central = rand->Next(1, 6);
		int color_aleatorio = rand->Next(0, 3);
		EColor nuevo_color = static_cast<EColor>(color_aleatorio);
		bool esPowerUp = rand->Next(0, 2) == 0;
		Power* nuevoPower = objControladora->crearNuevoPower(x_inicial, y_inicial, valor_central, nuevo_color, esPowerUp);
		nuevoPower->setVelocidad(dx_power, dy_power);
	}

	private: System::Void tmrJuego_Tick(System::Object^ sender, System::EventArgs^ e) {
		if (!objControladora->esModoAutomatico()) {
			int velocidad = 10;
			Jugador* jugador = objControladora->getJugador();
			OrientacionTramo orientacion = objControladora->getOrientacion();
			int dx = 0, dy = 0;

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
		}

		objControladora->gestionarColisiones();

		Graphics^ canvas = this->CreateGraphics();
		BufferedGraphicsContext^ espacioBuffer = BufferedGraphicsManager::Current;
		BufferedGraphics^ buffer = espacioBuffer->Allocate(canvas, this->ClientRectangle);

		switch (objControladora->getTramoActual()) {
		case 1: buffer->Graphics->Clear(Color::LightSkyBlue); break;
		case 2: buffer->Graphics->Clear(Color::LightGreen); break;
		case 3: buffer->Graphics->Clear(Color::LightCoral); break;
		default: buffer->Graphics->Clear(Color::White); break;
		}

		objControladora->moverMundo();
		objControladora->dibujarEscena(buffer->Graphics);

		int minimapAncho = 200;
		int pistaAncho = this->ClientSize.Width - minimapAncho;
		buffer->Graphics->DrawLine(Pens::Black, pistaAncho, 0, pistaAncho, this->ClientSize.Height);
		objControladora->dibujarIndicadores(buffer->Graphics, pistaAncho, this->ClientSize.Width, this->ClientSize.Height);

		buffer->Render(canvas);
		delete buffer;
		delete espacioBuffer;
		delete canvas;

		if (objControladora->getJuegoTerminado()) {
			tmrJuego->Stop();
			tmrGenerador->Stop();
			MessageBox::Show("¡YOU WIN!");
		}
		else if (objControladora->getJugadorHaPerdido()) {
			tmrJuego->Stop();
			tmrGenerador->Stop();
			MessageBox::Show("¡YOU LOST!");
		}
	}
	};
}