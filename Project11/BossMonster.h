#pragma once
using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

public enum class EstadoMiniBoss
{
    Idle,
    Run,
    TransitionCharge,
    Charge,
    Attack,
    Hit,
    Death
};

public ref class MiniBoss
{
private:

    // Sprites
    Bitmap^ sprTransitionCharge;
    Bitmap^ sprRun;
    Bitmap^ sprCharge;
    Bitmap^ sprHit;
    Bitmap^ sprDeath;
    Bitmap^ sprAttack;

    int x, y;
    int w, h;

    EstadoMiniBoss estado;
    int frame = 0;
    int frameDelay = 4;
    int frameCounter = 0;
    int frameWidth;
    int maxFrames;

    // Frames por animación
    int transitionFrames = 4;
    int runFrames = 8;
    int chargeFrames = 6;
    int hitFrames = 4;
    int deathFrames = 6;
    int attackFrames = 8;

    int vidaMax = 150;
    int vidaActual = 150;

    bool dialogoMostrado = false;
    bool muerteFinalizada = false;

public:

    bool facingLeft = false;

public:

    MiniBoss(int px, int py)
    {
        sprTransitionCharge = gcnew Bitmap("bossMonster/transition.png");
        sprRun = gcnew Bitmap("bossMonster/run.png");
        sprCharge = gcnew Bitmap("bossMonster/charge.png");
        sprHit = gcnew Bitmap("bossMonster/hit.png");
        sprDeath = gcnew Bitmap("bossMonster/death.png");
        sprAttack = gcnew Bitmap("bossMonster/attack.png");

        x = px;
        y = py;

        estado = EstadoMiniBoss::TransitionCharge;

        ActualizarDimensiones();
    }

    // ======================================================
    //      Mostrar diálogo inicial o final
    // ======================================================
    void MostrarDialogo(Label^ lbl, array<String^>^ dialogos, int% index, bool% active)
    {
        active = true;
        index = 0;

        lbl->Visible = true;
        lbl->Text = dialogos[index];
    }

    // ======================================================
    // Actualizar tamaño de sprite según animación
    // ======================================================
    void ActualizarDimensiones()
    {
        Bitmap^ s = getSheet();
        maxFrames = getMaxFrames();

        if (s == nullptr || maxFrames <= 0) return;

        frameWidth = s->Width / maxFrames;
        w = frameWidth;
        h = s->Height;
    }

    // ======================================================
    // Cambiar estado
    // ======================================================
    void SetEstado(EstadoMiniBoss nuevo)
    {
        if (estado != nuevo)
        {
            estado = nuevo;
            frame = 0;
            frameCounter = 0;
            ActualizarDimensiones();
        }
    }

    EstadoMiniBoss GetEstado() { return estado; }

    bool EstaMuerto()
    {
        return (vidaActual <= 0);
    }

    // ======================================================
    // UPDATE
    // ======================================================
    void Update()
    {
        // Animación de muerte con final
        if (vidaActual <= 0)
        {
            estado = EstadoMiniBoss::Death;
            ActualizarDimensiones();

            if (!muerteFinalizada)
            {
                frameCounter++;
                if (frameCounter >= frameDelay)
                {
                    frameCounter = 0;
                    frame++;

                    if (frame >= deathFrames)
                    {
                        frame = deathFrames - 1;
                        muerteFinalizada = true;
                    }
                }
            }
            return;
        }

        // Animaciones normales
        frameCounter++;
        if (frameCounter >= frameDelay)
        {
            frameCounter = 0;
            frame++;

            if (frame >= getMaxFrames())
                frame = 0;
        }
    }

    // ======================================================
    // Frames
    // ======================================================
    int getMaxFrames()
    {
        switch (estado)
        {
        case EstadoMiniBoss::Idle:             return runFrames;
        case EstadoMiniBoss::Run:              return runFrames;
        case EstadoMiniBoss::TransitionCharge: return transitionFrames;
        case EstadoMiniBoss::Charge:           return chargeFrames;
        case EstadoMiniBoss::Attack:           return attackFrames;
        case EstadoMiniBoss::Hit:              return hitFrames;
        case EstadoMiniBoss::Death:            return deathFrames;
        }
        return 1;
    }

    // ======================================================
    // Sprite de estado
    // ======================================================
    Bitmap^ getSheet()
    {
        switch (estado)
        {
        case EstadoMiniBoss::Idle:             return sprRun;
        case EstadoMiniBoss::Run:              return sprRun;
        case EstadoMiniBoss::TransitionCharge: return sprTransitionCharge;
        case EstadoMiniBoss::Charge:           return sprCharge;
        case EstadoMiniBoss::Attack:           return sprAttack;
        case EstadoMiniBoss::Hit:              return sprHit;
        case EstadoMiniBoss::Death:            return sprDeath;
        }
        return sprRun;
    }

    // ======================================================
    // DRAW
    // ======================================================
    void Draw(Graphics^ g)
    {
        Bitmap^ sheet = getSheet();
        if (sheet == nullptr) return;

        maxFrames = getMaxFrames();
        frameWidth = sheet->Width / maxFrames;

        Rectangle src(frameWidth * frame, 0, frameWidth, sheet->Height);

        if (!facingLeft)
        {
            g->DrawImage(
                sheet,
                Rectangle(x, y, frameWidth, sheet->Height),
                src,
                GraphicsUnit::Pixel
            );
        }
        else
        {
            g->TranslateTransform(x + frameWidth, 0);
            g->ScaleTransform(-1, 1);

            g->DrawImage(
                sheet,
                Rectangle(0, y, frameWidth, sheet->Height),
                src,
                GraphicsUnit::Pixel
            );

            g->ResetTransform();
        }

        // --- Barra de vida ---
        int vidaW = (vidaActual * frameWidth) / vidaMax;

        g->FillRectangle(Brushes::Black, x, y - 12, frameWidth, 10);
        g->FillRectangle(Brushes::Red, x, y - 12, vidaW, 10);
        g->DrawRectangle(Pens::White, x, y - 12, frameWidth, 10);
    }

    // ======================================================
    // HITBOX
    // ======================================================
    Rectangle getHitbox()
    {
        return Rectangle(
            x + w * 0.2,
            y + h * 0.2,
            w * 0.6,
            h * 0.6
        );
    }

    Rectangle getAttackHitbox()
    {
        Rectangle hb = getHitbox();
        if (!facingLeft)
            return Rectangle(hb.Right, hb.Y, hb.Width, hb.Height);
        else
            return Rectangle(hb.X - hb.Width, hb.Y, hb.Width, hb.Height);
    }

    // ======================================================
    // VIDA
    // ======================================================
    int getVida() { return vidaActual; }
    void setVida(int v) { vidaActual = Math::Max(0, Math::Min(v, vidaMax)); }

    // ======================================================
    // POSICIÓN
    // ======================================================
    int getX() { return x; }
    int getY() { return y; }
    int getWidth() { return w; }
    int getHeight() { return h; }

    void setX(int nx) { x = nx; }
    void setY(int ny) { y = ny; }

    // ======================================================
    //  NUEVO: Saber si la animación de muerte terminó
    // ======================================================
    bool IsDeathFinished()
    {
        return muerteFinalizada;
    }
};
