#pragma once
#include "Figura.h"
using namespace System;

class Jugador : public Figura {
private:
    float progreso;

public:
    Jugador(int x, int y, EColor color_inicial) : Figura(x, y, 3, 0, color_inicial) {
        progreso = 0.0f;
    }

   
    void sumarAlCentral(int valor) {
        num_central += valor;
    }
    void restarAlCentral(int valor) {
        num_central -= valor;
        if (num_central < 0) {
            num_central = 0;
        }
    }
    void evolucionar() {
        if (num_lados < 10) num_lados++;
    }
    void involucionar() {
        if (num_lados > 3) num_lados--;
    }

    
    void iniciarTramo2(int anchoPista, int altoVentana) {
        x = anchoPista - ancho;
        y = altoVentana - alto;
    }
    void iniciarTramo3(int anchoPista) {
        x = anchoPista - ancho;
        y = 0;
    }

    
    float getProgreso() { return progreso; }

    
    void mover(OrientacionTramo orientacion) override {
        x += dx;
        y += dy;

        switch (orientacion) {
        case OrientacionTramo::Horizontal_Der: progreso += dx; break;
        case OrientacionTramo::Vertical_Arr:   progreso -= dy; break;
        case OrientacionTramo::Horizontal_Izq: progreso -= dx; break;
        }

        int limite_derecho_pista = 752 - 200 - ancho;
        int limite_inferior_pista = 563 - alto;
        if (x < 0) x = 0;
        if (x > limite_derecho_pista) x = limite_derecho_pista;
        if (y < 0) y = 0;
        if (y > limite_inferior_pista) y = limite_inferior_pista;
    }

    void dibujar(Graphics^ canvas) override {
        Brush^ brush;
        switch (this->color) {
        case EColor::Rojo:     brush = Brushes::Red; break;
        case EColor::Amarillo: brush = Brushes::Yellow; break;
        case EColor::Azul:     brush = Brushes::Blue; break;
        default:               brush = Brushes::Black; break;
        }

        cli::array<Point>^ puntos = gcnew cli::array<Point>(num_lados);
        float radio = ancho / 2.0f;
        float anguloPaso = 360.0f / num_lados;

        for (int i = 0; i < num_lados; i++) {
            float anguloActual = static_cast<float>((i * anguloPaso - 90) * (Math::PI / 180.0));
            int puntoX = x + static_cast<int>(radio * Math::Cos(anguloActual));
            int puntoY = y + static_cast<int>(radio * Math::Sin(anguloActual));
            puntos[i] = Point(puntoX, puntoY);
        }

        canvas->FillPolygon(brush, puntos);

        String^ texto = num_central.ToString();
        Font^ fuente = gcnew Font("Arial", 12, FontStyle::Bold);
        Brush^ brochaTexto = Brushes::White;
        SizeF^ tamanoTexto = canvas->MeasureString(texto, fuente);
        PointF posTexto = PointF(x - tamanoTexto->Width / 2, y - tamanoTexto->Height / 2);
        canvas->DrawString(texto, fuente, brochaTexto, posTexto);
    }
};