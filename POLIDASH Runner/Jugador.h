#pragma once
#include "Figura.h"

using namespace System;

class Jugador : public Figura {
private:
    float progreso;
public:
    void iniciarTramo2(int anchoPista, int altoVentana) {
        // Coloca al jugador en la esquina inferior derecha
        x = anchoPista - ancho;
        y = altoVentana - alto;
    }

    void iniciarTramo3(int anchoPista) {
        // Coloca al jugador en la esquina superior derecha
        x = anchoPista - ancho;
        y = 0;
    }
    Jugador(int x, int y, EColor color_inicial) : Figura(x, y, 3, 0, color_inicial) {
        progreso = 0.0f; // El jugador empieza con 0 progreso
        // Ahora el color se pasa desde fuera
    }

    void sumarAlCentral(int valor) {
        num_central += valor;
    }

    // Disminuye el número central, con la regla de que no puede ser menor que 0
    void restarAlCentral(int valor) {
        num_central -= valor;
        if (num_central < 0) {
            num_central = 0; // Regla del PDF
        }
    }

    void mover(OrientacionTramo orientacion) override {
        // 1. Aplicamos el movimiento simple (la velocidad ya fue calculada en MyForm)
        x += dx;
        y += dy;

        // 2. Calculamos el progreso basado en la orientación del tramo
        switch (orientacion) {
        case OrientacionTramo::Horizontal_Der:
            progreso += dx; // Avanzar es moverse a la derecha (dx positivo)
            break;
        case OrientacionTramo::Vertical_Arr:
            progreso -= dy; // Avanzar es moverse hacia arriba (dy negativo)
            break;
        case OrientacionTramo::Horizontal_Izq:
            progreso -= dx; // Avanzar es moverse a la izquierda (dx negativo)
            break;
        }

        // 3. Aplicamos límites para que el jugador no se salga de la pista
        int limite_derecho_pista = 752 - 200 - ancho; // Ancho Ventana - Ancho Minimapa - Ancho Jugador
        int limite_inferior_pista = 563 - alto; // Alto Ventana - Alto Jugador

        if (x < 0) x = 0;
        if (x > limite_derecho_pista) x = limite_derecho_pista;
        if (y < 0) y = 0;
        if (y > limite_inferior_pista) y = limite_inferior_pista;
    }
    float getProgreso() { return progreso; }

    void dibujar(Graphics^ canvas) override {
        // (Esta función no cambia, deja el código que ya tenías y que corregiste con Copilot)
        // ...
        // 1. Seleccionar el color...
        Brush^ brush;
        switch (this->color) {
        case EColor::Rojo:     brush = Brushes::Red; break;
        case EColor::Amarillo: brush = Brushes::Yellow; break;
        case EColor::Azul:     brush = Brushes::Blue; break;
        default:               brush = Brushes::Black; break;
        }

        // 2. Calcular los vértices...
        cli::array<Point>^ puntos = gcnew cli::array<Point>(num_lados);
        float radio = ancho / 2.0f;
        float anguloPaso = 360.0f / num_lados;

        for (int i = 0; i < num_lados; i++) {
            float anguloActual = static_cast<float>((i * anguloPaso - 90) * (Math::PI / 180.0));
            int puntoX = x + static_cast<int>(radio * Math::Cos(anguloActual));
            int puntoY = y + static_cast<int>(radio * Math::Sin(anguloActual));
            puntos[i] = Point(puntoX, puntoY);
        }

        // 3. Dibujar el polígono...
        canvas->FillPolygon(brush, puntos);

        // 4. Dibujar el número...
        String^ texto = num_central.ToString();
        Font^ fuente = gcnew Font("Arial", 12, FontStyle::Bold);
        Brush^ brochaTexto = Brushes::White;
        SizeF^ tamanoTexto = canvas->MeasureString(texto, fuente);
        PointF posTexto = PointF(x - tamanoTexto->Width / 2, y - tamanoTexto->Height / 2);
        canvas->DrawString(texto, fuente, brochaTexto, posTexto);
    }

    void evolucionar() {
        if (num_lados < 10) num_lados++;
    }

    void involucionar() {
        if (num_lados > 3) num_lados--;
    }
};