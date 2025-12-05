#pragma once
#include "Entidad.h"
#include <vector>
#include <cmath>

enum SpriteEnemigo
{
    accionDañoIzquierdaaEnemigo,
    CaminarDerechaEnemigo,
    CaminarIzquierdaEnemigo,
    accionDañoDerechaEnemigo
};

class Enemigo : public Entidad {
private:
    SpriteEnemigo accion;
    int iDx;
    int visionRadio = 120;
    int margen = 30;
    int vida = 5;
    bool murioRecien = false;  // <<----- NUEVO

public:

    Enemigo(Bitmap^ img, int px, int py)
    {
        x = px;
        y = py;
        iDx = 0;
        accion = CaminarDerechaEnemigo;

        if (rand() % 2 == 0) {
            dx = rand() % 11 - 5;
            if (dx == 0) dx = 5;
            dy = 0;
        }
        else {
            dy = rand() % 11 - 5;
            if (dy == 0) dy = 5;
            dx = 0;
        }

        ancho = img->Width / 4;
        alto = img->Height / 3;
    }

    void RestarVida() {
        if (vida > 0) vida--;
        if (vida <= 0) murioRecien = true;   // <<--- IMPORTANTE
    }

    int GetVida() { return vida; }
    bool EstaMuerto() { return vida <= 0; }
    bool MurioRecien() { return murioRecien; }
    void LimpiarMuerte() { murioRecien = false; }

    // ============================================
    //  IA DEL ENEMIGO
    // ============================================
    void Mover(Graphics^ g, int jugadorX, int jugadorY, const std::vector<Enemigo*>& todos) {

        if (EstaMuerto()) return; // Muerto: no se mueve

        int centroX = x + ancho / 2;
        int centroY = y + alto / 2;
        int dxJugador = jugadorX - centroX;
        int dyJugador = jugadorY - centroY;
        double distancia = std::sqrt(dxJugador * dxJugador + dyJugador * dyJugador);

        // ======== Separación ========
        double sepX = 0, sepY = 0;
        for (auto& otro : todos) {
            if (otro == this || otro->EstaMuerto()) continue;

            int ox = otro->GetX() + otro->GetAncho() / 2;
            int oy = otro->GetY() + otro->GetAlto() / 2;
            double dist = std::sqrt((centroX - ox) * (centroX - ox) +
                (centroY - oy) * (centroY - oy));

            if (dist < ancho && dist > 0.1) {
                sepX += (centroX - ox) / dist;
                sepY += (centroY - oy) / dist;
            }
        }

        // ======== Movimiento ========
        double movX = 0, movY = 0;
        const int velocidad = 5;

        if (distancia < visionRadio && distancia > margen) {
            movX = dxJugador;
            movY = dyJugador;
        }
        else {
            movX = dx;
            movY = dy;
        }

        movX += sepX * 2;
        movY += sepY * 2;

        double norm = std::sqrt(movX * movX + movY * movY);
        if (norm != 0) {
            dx = (int)(velocidad * movX / norm);
            dy = (int)(velocidad * movY / norm);
        }

        if (!(x + dx >= 0 && x + ancho + dx <= g->VisibleClipBounds.Width)) dx *= -1;
        if (!(y + dy >= 0 && y + alto + dy <= g->VisibleClipBounds.Height)) dy *= -1;

        if (dx < 0) accion = CaminarIzquierdaEnemigo;
        else accion = CaminarDerechaEnemigo;

        x += dx;
        y += dy;
    }

    // ============================================
    //      DIBUJO
    // ============================================
    void mostrar(Graphics^ g, Bitmap^ img) {

        if (EstaMuerto()) return;

        // ===== barra de vida =====
        int barraAncho = ancho;
        int barraAlto = 8;
        int vidaMax = 5;
        int anchoVida = (int)((barraAncho - 2) * vida / (double)vidaMax);

        Rectangle fondoBarra(x, y - barraAlto - 2, barraAncho, barraAlto);
        Rectangle vidaBarra(x + 1, y - barraAlto - 1, anchoVida, barraAlto - 2);
        g->FillRectangle(Brushes::Gray, fondoBarra);
        g->FillRectangle(Brushes::LimeGreen, vidaBarra);
        g->DrawRectangle(Pens::Black, fondoBarra);

        // ===== sprites =====
        Rectangle corte = Rectangle(iDx * ancho, accion * alto, ancho, alto);
        g->DrawImage(img, area(), corte, GraphicsUnit::Pixel);

        iDx = (iDx + 1) % 4;
    }
};


//
// ===============================
//          MANEJADOR DE ENEMIGOS
// ===============================
class Enemigos {
private:
    std::vector<Enemigo*> enemigos;
    bool muerteReciente = false;

public:

    Enemigos(Bitmap^ img, int cant)
    {
        int minDist = 60;
        int ancho = img->Width / 10;
        int alto = img->Height / 8;
        int maxW = 700, maxH = 400;

        for (int i = 0; i < cant; i++) {
            int x, y;
            do {
                x = rand() % (maxW - ancho);
                y = rand() % (maxH - alto);
            } while (false);

            enemigos.push_back(new Enemigo(img, x, y));
        }
    }

    ~Enemigos()
    {
        for (Enemigo* E : enemigos) delete E;
    }

    // ============================
    //      DETECTAR MUERTES
    // ============================
    bool HuboMuerteReciente() {
        return muerteReciente;
    }

    void ConsumirMuerte() {
        muerteReciente = false;
    }

    int GetCantidadVivos() {
        int vivos = 0;
        for (auto* e : enemigos)
            if (!e->EstaMuerto()) vivos++;
        return vivos;
    }

    // ============================
    // MOVIMIENTO
    // ============================
    void mover(Graphics^ g, int jugadorX, int jugadorY)
    {
        for (Enemigo* E : enemigos) {
            int vidaAntes = E->GetVida();

            E->Mover(g, jugadorX, jugadorY, enemigos);

            if (E->MurioRecien()) {
                muerteReciente = true;
                E->LimpiarMuerte();
            }
        }
    }

    // ============================
    // DIBUJAR
    // ============================
    void mostrar(Graphics^ g, Bitmap^ img)
    {
        for (auto* E : enemigos)
            E->mostrar(g, img);
    }

    std::vector<Enemigo*>& GetEnemigos() { return enemigos; }
};
