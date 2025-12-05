#pragma once
using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

// ===============================
//   ESTADOS DEL HONGO
// ===============================
public enum class EstadoHongo
{
    Idle,
    Run,
    Attack,
    AttackStun,
    Hit,
    Stun,
    Die
};

public ref class Mushroom
{
private:

    Bitmap^ sprIdle;
    Bitmap^ sprRun;
    Bitmap^ sprAttack;
    Bitmap^ sprAttackStun;
    Bitmap^ sprHit;
    Bitmap^ sprStun;
    Bitmap^ sprDie;

    int x, y;
    int w, h;

    EstadoHongo estado;
    int frame;
    int frameDelay = 4;
    int frameCounter = 0;
    int frameWidth;
    int maxFrames;

    int idleFrames = 7;
    int runFrames = 8;
    int attackFrames = 10;
    int attackStunFrames = 21;
    int hitFrames = 5;
    int stunFrames = 21;
    int dieFrames = 15;

    int vidaMax = 50;
    int vidaActual = 50;

public:
    bool facingLeft;

    // --- HITBOX CIRCULAR ---
    int radius = 35;  // Radio circular (ajústalo si tu sprite es más grande/pequeño)

public:

    Mushroom(int px, int py)
    {
        sprIdle = gcnew Bitmap("hongo/Mushroom-Idle.png");
        sprRun = gcnew Bitmap("hongo/Mushroom-Run.png");
        sprAttack = gcnew Bitmap("hongo/Mushroom-Attack.png");
        sprAttackStun = gcnew Bitmap("hongo/Mushroom-AttackWithStun.png");
        sprHit = gcnew Bitmap("hongo/Mushroom-Hit.png");
        sprStun = gcnew Bitmap("hongo/Mushroom-Stun.png");
        sprDie = gcnew Bitmap("hongo/Mushroom-Die.png");

        x = px;
        y = py;

        estado = EstadoHongo::Idle;
        frame = 0;

        w = sprIdle->Width / idleFrames;
        h = sprIdle->Height;

        facingLeft = false;
    }

    void SetEstado(EstadoHongo nuevo)
    {
        if (estado != nuevo)
        {
            estado = nuevo;
            frame = 0;
            frameCounter = 0;
        }
    }

    EstadoHongo GetEstado()
    {
        return estado;
    }

    void Update()
    {
        frameCounter++;
        if (frameCounter >= frameDelay)
        {
            frameCounter = 0;
            frame++;
            if (frame >= getMaxFrames())
                frame = 0;
        }

        if (vidaActual <= 0 && estado != EstadoHongo::Die)
            SetEstado(EstadoHongo::Die);
    }

    int getMaxFrames()
    {
        switch (estado)
        {
        case EstadoHongo::Idle:       return idleFrames;
        case EstadoHongo::Run:        return runFrames;
        case EstadoHongo::Attack:     return attackFrames;
        case EstadoHongo::AttackStun: return attackStunFrames;
        case EstadoHongo::Hit:        return hitFrames;
        case EstadoHongo::Stun:       return stunFrames;
        case EstadoHongo::Die:        return dieFrames;
        }
        return 1;
    }

    Bitmap^ getSheet()
    {
        switch (estado)
        {
        case EstadoHongo::Idle:       return sprIdle;
        case EstadoHongo::Run:        return sprRun;
        case EstadoHongo::Attack:     return sprAttack;
        case EstadoHongo::AttackStun: return sprAttackStun;
        case EstadoHongo::Hit:        return sprHit;
        case EstadoHongo::Stun:       return sprStun;
        case EstadoHongo::Die:        return sprDie;
        }
        return sprIdle;
    }

    void Draw(Graphics^ g)
    {
        Bitmap^ sheet = getSheet();
        maxFrames = getMaxFrames();
        frameWidth = sheet->Width / maxFrames;

        Rectangle src(frameWidth * frame, 0, frameWidth, sheet->Height);

        if (!facingLeft)
        {
            g->DrawImage(sheet, Rectangle(x, y, frameWidth, sheet->Height), src, GraphicsUnit::Pixel);
        }
        else
        {
            g->TranslateTransform(x + frameWidth, 0);
            g->ScaleTransform(-1, 1);
            g->DrawImage(sheet, Rectangle(0, y, frameWidth, sheet->Height), src, GraphicsUnit::Pixel);
            g->ResetTransform();
        }

        // VIDA
        int barW = frameWidth;
        int barH = 7;
        int vidaW = (vidaActual * barW) / vidaMax;

        g->FillRectangle(Brushes::Black, x, y - 10, barW, barH);
        g->FillRectangle(Brushes::LimeGreen, x, y - 10, vidaW, barH);
        g->DrawRectangle(Pens::White, x, y - 10, barW, barH);

        // DEBUG del hitbox circular (opcional)
        //g->DrawEllipse(Pens::Red, getCircleX() - radius, getCircleY() - radius, radius * 2, radius * 2);
    }

    // =====================================================
    //   HITBOX CIRCULAR REAL
    // =====================================================
    int getCircleX() { return x + w / 2; }
    int getCircleY() { return y + h / 2; }

    // Para IA y colisiones
    bool ColisionCircular(int px, int py, int pradius)
    {
        int dx = px - getCircleX();
        int dy = py - getCircleY();
        int dist = (dx * dx) + (dy * dy);
        int sum = (radius + pradius);
        return dist <= (sum * sum);
    }

    // =====================================================
    //  RECT HIDDEN (solo usado para disparos)
    // =====================================================
    Rectangle getHitbox()
    {
        return Rectangle(getCircleX() - radius, getCircleY() - radius, radius * 2, radius * 2);
    }

    // POS / VIDA
    int getX() { return x; }
    int getY() { return y; }
    int getWidth() { return w; }
    int getHeight() { return h; }

    void setX(int nx) { x = nx; }
    void setY(int ny) { y = ny; }

    int getVida() { return vidaActual; }
    void setVida(int v)
    {
        vidaActual = Math::Max(0, Math::Min(v, vidaMax));
    }

    bool EstaMuerto()
    {
        return vidaActual <= 0;
    }
};
