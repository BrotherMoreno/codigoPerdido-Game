#pragma once
using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;

public enum class EstadoCapitan
{
    Idle,
    Idle2,
    Walk,
    Run,
    Jump,
    Shot,
    Attack,
    Hurt,
    Dead,
    Recharge
};

public ref class Bala
{
public:
    int x, y;
    int speed;
    bool left;

    Bala(int px, int py, bool facingLeft)
    {
        x = px;
        y = py;
        speed = 14;
        left = facingLeft;
    }

    void Update()
    {
        x += left ? -speed : speed;
    }

    void Draw(Graphics^ g)
    {
        g->FillEllipse(Brushes::Yellow, x, y, 10, 10);
    }

    Rectangle getHitbox()
    {
        return Rectangle(x, y, 10, 10);
    }
};

// ======================================================
//                   CAPITÁN
// ======================================================
public ref class Capitan
{
private:

    // SPRITES
    Bitmap^ sprIdle;
    Bitmap^ sprIdle2;
    Bitmap^ sprWalk;
    Bitmap^ sprRun;
    Bitmap^ sprJump;
    Bitmap^ sprShot;
    Bitmap^ sprAttack;
    Bitmap^ sprHurt;
    Bitmap^ sprDead;
    Bitmap^ sprRecharge;

    // POSICIÓN
    int x, y;
    int w, h;

    // ANIMACIÓN
    EstadoCapitan estado;
    EstadoCapitan estadoPrevio;

    int frame;
    int frameWidth;
    int spriteHeight;
    int maxFrames;

    int idleFrames = 6;
    int idle2Frames = 11;
    int walkFrames = 10;
    int runFrames = 10;
    int jumpFrames = 10;
    int shotFrames = 4;
    int attackFrames = 3;
    int hurtFrames = 5;
    int deadFrames = 5;
    int rechargeFrames = 17;

    int frameDelay;
    int frameCounter;

    // MOVIMIENTO
    bool movingLeft, movingRight, movingUp, movingDown;
    bool running;
    int walkSpeed = 4;
    int runSpeed = 8;
    bool facingLeft;

    int idleTimer = 0;
    int idleSwitchTime = 90;

    bool doJump;
    bool doShot;
    bool doAttack;
    bool doHurt;
    bool doRecharge;

    // VIDA
    int vidaMax = 300;
    int vidaActual = 300;

    // MUNICIÓN
    int balasMax = 30;
    int balasActuales;
    bool recargando;
    int reloadTimer;
    int reloadTimeMax = 60;

public:

    // BALAS DISPARADAS
    List<Bala^>^ balas;

public:

    Capitan(int px, int py)
    {
        // Carga de sprites
        sprIdle = gcnew Bitmap("capitan/Idle.png");
        sprIdle2 = gcnew Bitmap("capitan/Idle_2.png");
        sprWalk = gcnew Bitmap("capitan/Walk.png");
        sprRun = gcnew Bitmap("capitan/Run.png");
        sprJump = gcnew Bitmap("capitan/Jump.png");
        sprShot = gcnew Bitmap("capitan/Shot.png");
        sprAttack = gcnew Bitmap("capitan/Attack_1.png");
        sprHurt = gcnew Bitmap("capitan/Hurt.png");
        sprDead = gcnew Bitmap("capitan/Dead.png");
        sprRecharge = gcnew Bitmap("capitan/Recharge.png");

        x = px;
        y = py;

        w = sprIdle->Width / idleFrames;
        h = sprIdle->Height;
        spriteHeight = h;

        estado = EstadoCapitan::Idle;
        estadoPrevio = estado;

        frame = 0;
        frameDelay = 4;
        frameCounter = 0;

        movingLeft = movingRight = false;
        movingUp = movingDown = false;
        running = false;

        facingLeft = false;

        doJump = doShot = doAttack = doHurt = doRecharge = false;

        vidaActual = vidaMax;

        balas = gcnew List<Bala^>();
        balasActuales = balasMax;
        recargando = false;
        reloadTimer = 0;
    }

    // ====================================================
    // INPUT
    // ====================================================
    void KeyDown(KeyEventArgs^ e)
    {
        if (estado == EstadoCapitan::Dead) return;

        if (e->KeyCode == Keys::Left)  movingLeft = true;
        if (e->KeyCode == Keys::Right) movingRight = true;
        if (e->KeyCode == Keys::Up)    movingUp = true;
        if (e->KeyCode == Keys::Down)  movingDown = true;
        if (e->KeyCode == Keys::ControlKey) running = true;

        // Ataque cuerpo a cuerpo
        if (e->KeyCode == Keys::Z)
            doAttack = true;

        // Disparo
        if (e->KeyCode == Keys::X)
        {
            if (balasActuales > 0 && !recargando)
            {
                doShot = true;
                Disparar();
                balasActuales--;
            }
        }

        // Recargar
        if (e->KeyCode == Keys::R)
        {
            if (!recargando && balasActuales < balasMax)
            {
                recargando = true;
                doRecharge = true;
                reloadTimer = 0;
            }
        }
    }

    void KeyUp(KeyEventArgs^ e)
    {
        if (e->KeyCode == Keys::Left)  movingLeft = false;
        if (e->KeyCode == Keys::Right) movingRight = false;
        if (e->KeyCode == Keys::Up)    movingUp = false;
        if (e->KeyCode == Keys::Down)  movingDown = false;

        if (e->KeyCode == Keys::ControlKey) running = false;
    }

    // ====================================================
    // DISPARO
    // ====================================================
    void Disparar()
    {
        int bx = x + (facingLeft ? -5 : w + 10);
        int by = y + h / 2 - 5;

        balas->Add(gcnew Bala(bx, by, facingLeft));
    }

    // ====================================================
    // RECIBIR DAÑO (USADO POR LOS HONGOS Y MINIBOSS)
    // ====================================================
    void RecibirDaño(int dmg)
    {
        vidaActual -= dmg;
        if (vidaActual < 0) vidaActual = 0;

        doHurt = true;
        if (vidaActual <= 0)
            estado = EstadoCapitan::Dead;
    }

    // ====================================================
    // UPDATE
    // ====================================================
    void Update()
    {
        if (estado == EstadoCapitan::Dead)
            return;

        estadoPrevio = estado;

        // RECARGA
        if (recargando)
        {
            estado = EstadoCapitan::Recharge;
            reloadTimer++;

            if (reloadTimer >= reloadTimeMax)
            {
                recargando = false;
                doRecharge = false;
                balasActuales = balasMax;
            }
        }
        else if (doHurt)
        {
            estado = EstadoCapitan::Hurt;
            if (frame >= hurtFrames - 1)
                doHurt = false;
        }
        else if (doAttack)
        {
            estado = EstadoCapitan::Attack;
            if (frame >= attackFrames - 1)
                doAttack = false;
        }
        else if (doShot)
        {
            estado = EstadoCapitan::Shot;
            if (frame >= shotFrames - 1)
                doShot = false;
        }
        else
        {
            if (movingLeft || movingRight || movingUp || movingDown)
                estado = running ? EstadoCapitan::Run : EstadoCapitan::Walk;
            else
                estado = EstadoCapitan::Idle;
        }

        // Movimiento
        if (!recargando)
        {
            if (movingLeft) { facingLeft = true;  x -= running ? runSpeed : walkSpeed; }
            if (movingRight) { facingLeft = false; x += running ? runSpeed : walkSpeed; }
            if (movingUp)    y -= walkSpeed;
            if (movingDown)  y += walkSpeed;
        }

        // Balas
        for (int i = balas->Count - 1; i >= 0; i--)
        {
            balas[i]->Update();
            if (balas[i]->x < -20 || balas[i]->x > 2000)
                balas->RemoveAt(i);
        }

        // Animación
        if (estado != estadoPrevio)
        {
            frame = 0;
            frameCounter = 0;
        }

        frameCounter++;
        if (frameCounter >= frameDelay)
        {
            frameCounter = 0;
            frame++;
            if (frame >= getMaxFrames()) frame = 0;
        }
    }

    // ====================================================
    // HITBOX (Necesario para daño de hongos y miniboss)
    // ====================================================
    Rectangle getHitbox()
    {
        return Rectangle(x + 20, y + 40, 40, 60);
    }

    // ====================================================
    // VIDA
    // ====================================================
    void setVida(int v)
    {
        vidaActual = Math::Max(0, Math::Min(v, vidaMax));
    }

    int GetVida() { return vidaActual; }

    // ====================================================
    // MUNICIÓN
    // ====================================================
    int GetBalas() { return balasActuales; }
    int GetBalasMax() { return balasMax; }

    // ====================================================
    // Sprites
    // ====================================================
    int getMaxFrames()
    {
        switch (estado)
        {
        case EstadoCapitan::Idle:      return idleFrames;
        case EstadoCapitan::Idle2:     return idle2Frames;
        case EstadoCapitan::Walk:      return walkFrames;
        case EstadoCapitan::Run:       return runFrames;
        case EstadoCapitan::Jump:      return jumpFrames;
        case EstadoCapitan::Shot:      return shotFrames;
        case EstadoCapitan::Attack:    return attackFrames;
        case EstadoCapitan::Hurt:      return hurtFrames;
        case EstadoCapitan::Dead:      return deadFrames;
        case EstadoCapitan::Recharge:  return rechargeFrames;
        }
        return 1;
    }

    Bitmap^ getCurrentSheet()
    {
        switch (estado)
        {
        case EstadoCapitan::Idle:      return sprIdle;
        case EstadoCapitan::Idle2:     return sprIdle2;
        case EstadoCapitan::Walk:      return sprWalk;
        case EstadoCapitan::Run:       return sprRun;
        case EstadoCapitan::Jump:      return sprJump;
        case EstadoCapitan::Shot:      return sprShot;
        case EstadoCapitan::Attack:    return sprAttack;
        case EstadoCapitan::Hurt:      return sprHurt;
        case EstadoCapitan::Dead:      return sprDead;
        case EstadoCapitan::Recharge:  return sprRecharge;
        }
        return sprIdle;
    }

    void Draw(Graphics^ g)
    {
        // Dibujar balas
        for each (Bala ^ b in balas)
            b->Draw(g);

        Bitmap^ sheet = getCurrentSheet();
        maxFrames = getMaxFrames();
        frameWidth = sheet->Width / maxFrames;
        spriteHeight = sheet->Height;

        Rectangle src(frameWidth * frame, 0, frameWidth, spriteHeight);
        Rectangle dest(x, y, frameWidth, spriteHeight);

        if (!facingLeft)
        {
            g->DrawImage(sheet, dest, src, GraphicsUnit::Pixel);
        }
        else
        {
            g->TranslateTransform(x + frameWidth, 0);
            g->ScaleTransform(-1, 1);
            g->DrawImage(sheet, Rectangle(0, y, frameWidth, spriteHeight), src, GraphicsUnit::Pixel);
            g->ResetTransform();
        }

        // Barra de vida
        int barW = 120;
        int vidaW = barW * vidaActual / vidaMax;

        g->FillRectangle(Brushes::Black, x, y - 18, barW, 10);
        g->FillRectangle(Brushes::Lime, x, y - 18, vidaW, 10);
        g->DrawRectangle(Pens::White, x, y - 18, barW, 10);
    }

    // ====================================================
    // EXTRA HUD
    // ====================================================
    int getX() { return x; }
    int getY() { return y; }
    int getSpeed() { return running ? runSpeed : walkSpeed; }
};
