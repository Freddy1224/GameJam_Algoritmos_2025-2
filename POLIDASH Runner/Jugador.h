#pragma once
#include "Figura.h"
using namespace System;

class Jugador : public Figura {
public:
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
    Jugador(int x, int y, EColor color_inicial) : Figura(x, y, 3, 0, color_inicial) {
        // Ahora el color se pasa desde fuera
    }

    // Este método es nuevo y se parece a la lógica de tu proyecto de Sprites
   

    // El método mover ahora es muy simple, como en tus ejemplos
    void mover() override {
        // Guarda la posición futura
        int futuro_x = x + dx;

        // Define los límites de la pista (asumimos que el minimapa tiene 200px de ancho)
        int limite_derecho_pista = 752 - 200 - ancho; // 752 es el ancho de tu ventana

        // Mueve solo si no se pasa de los límites
        if (futuro_x > 0 && futuro_x < limite_derecho_pista) {
            x = futuro_x;
        }

        // El movimiento en Y puede tener límites también si quieres
        y += dy;
    }

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