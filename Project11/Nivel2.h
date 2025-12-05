#pragma once
#include "Capitan.h"
#include "Enemigo.h"

using namespace System;
using namespace System::Drawing;

public ref class ControlCombate
{
private:
    Capitan^ capitan;
    Enemigos* enemigos;
    Bitmap^ imgEnemigo;

    clock_t ultimoDaño;

public:

    ControlCombate()
    {
        capitan = gcnew Capitan(400, 300);

        imgEnemigo = gcnew Bitmap("img/dron2.png");
        enemigos = new Enemigos(imgEnemigo, 5);

        ultimoDaño = 0;
    }

    ~ControlCombate()
    {
        delete enemigos;
        delete imgEnemigo;
    }

    // ============================================================
    //              CONSULTA DE ENEMIGOS MUERTOS
    // ============================================================
    bool TodosEnemigosMuertos()
    {
        auto& lista = enemigos->GetEnemigos();
        for (int i = 0; i < (int)lista.size(); ++i)
        {
            if (!lista[i]->EstaMuerto())
                return false;
        }
        return true;
    }

    // ============================================================
    //         CANTIDAD DE ENEMIGOS VIVOS PARA NIVEL2
    // ============================================================
    int GetCantidadEnemigosVivos()
    {
        auto& lista = enemigos->GetEnemigos();
        int vivos = 0;

        for (int i = 0; i < (int)lista.size(); i++)
            if (!lista[i]->EstaMuerto()) vivos++;

        return vivos;
    }

    // ============================================================
    //                        INPUT
    // ============================================================
    void KeyDown(KeyEventArgs^ e)
    {
        capitan->KeyDown(e);
    }

    void KeyUp(KeyEventArgs^ e)
    {
        capitan->KeyUp(e);
    }

    // ============================================================
    //                        UPDATE
    // ============================================================
    void Update(Graphics^ g)
    {
        capitan->Update();

        int cx = capitan->getX() + 40;
        int cy = capitan->getY() + 40;

        enemigos->mover(g, cx, cy);

        ProcesarColisionBalas();
        ProcesarDañoCapitan();
    }

    // ============================================================
    //                        DRAW
    // ============================================================
    void Draw(Graphics^ g)
    {
        capitan->Draw(g);
        enemigos->mostrar(g, imgEnemigo);
    }

private:

    // ============================================================
    //            BALAS DEL CAPITÁN CONTRA ENEMIGOS
    // ============================================================
    void ProcesarColisionBalas()
    {
        auto& lista = enemigos->GetEnemigos();

        for (int i = capitan->balas->Count - 1; i >= 0; i--)
        {
            Rectangle hbBala = capitan->balas[i]->getHitbox();
            bool eliminar = false;

            for (int j = 0; j < lista.size(); j++)
            {
                if (!lista[j]->EstaMuerto() &&
                    hbBala.IntersectsWith(lista[j]->area()))
                {
                    lista[j]->RestarVida();
                    eliminar = true;
                    break;
                }
            }

            if (eliminar)
                capitan->balas->RemoveAt(i);
        }
    }

    // ============================================================
    //             DAÑO DE ENEMIGOS AL CAPITÁN
    // ============================================================
    void ProcesarDañoCapitan()
    {
        auto& lista = enemigos->GetEnemigos();
        Rectangle hitCap = capitan->getHitbox();

        for (int i = 0; i < lista.size(); i++)
        {
            if (!lista[i]->EstaMuerto() &&
                hitCap.IntersectsWith(lista[i]->area()))
            {
                clock_t ahora = clock();

                if (ultimoDaño == 0 || (ahora - ultimoDaño) > 600)
                {
                    capitan->RecibirDaño(5);
                    ultimoDaño = ahora;
                }

                return;
            }
        }

        ultimoDaño = 0;
    }
};
