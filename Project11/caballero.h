#pragma once
using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

// ===============================
//   ESTADOS DEL CABALLERO
// ===============================
public enum class EstadoCaballero
{
    Idle,
    Run,
    Jump,
    Attack,
    AttackCrouch,
    AttackAir,
    CrouchIdle,
    Roll,
    Slide,
    Hurt,
    Death,
    Pray,
    Climb,
    Hanging,
    HealthStatus
};

public ref class Caballero
{
private:

    // ===============================
    //   SPRITES
    // ===============================
    Bitmap^ sprIdle;
    Bitmap^ sprRun;
    Bitmap^ sprJump;
    Bitmap^ sprAttack;
    Bitmap^ sprCrouchAttack;
    Bitmap^ sprAirAttack;
    Bitmap^ sprCrouchIdle;
    Bitmap^ sprRoll;
    Bitmap^ sprSlide;
    Bitmap^ sprHurt;
    Bitmap^ sprDeath;
    Bitmap^ sprPray;
    Bitmap^ sprClimb;
    Bitmap^ sprHanging;
    Bitmap^ sprHealth;

    // ===============================
    //   POSICIÓN Y TAMAÑO
    // ===============================
    int x, y;
    int w, h;

    // ===============================
    //   ANIMACIÓN
    // ===============================
    EstadoCaballero estado;
    int frame;
    int frameDelay = 4;
    int frameCounter = 0;
    int frameWidth;
    int maxFrames;

    // ===============================
    //   FRAMES POR ANIMACIÓN
    // ===============================
    int idleFrames = 6;
    int runFrames = 8;
    int jumpFrames = 8;
    int attackFrames = 6;
    int crouchAttackFrames = 6;
    int airAttackFrames = 6;
    int crouchIdleFrames = 4;
    int rollFrames = 10;
    int slideFrames = 6;
    int hurtFrames = 4;
    int deathFrames = 8;
    int prayFrames = 10;
    int climbFrames = 6;
    int hangingFrames = 4;
    int healthFrames = 5;

    // ===============================
    //   VIDA
    // ===============================
    int vidaMax = 100;
    int vidaActual = 50; // ⭐ empieza a la mitad

public:
    // Lo modificará la IA / form
    bool facingLeft;

public:

    Caballero(int px, int py)
    {
        // CARGA DE SPRITES
        sprIdle = gcnew Bitmap("caballero/Idle.png");
        sprRun = gcnew Bitmap("caballero/Run.png");
        sprJump = gcnew Bitmap("caballero/Jump.png");
        sprAttack = gcnew Bitmap("caballero/Attacks.png");
        sprCrouchAttack = gcnew Bitmap("caballero/crouch_attacks.png");
        sprAirAttack = gcnew Bitmap("caballero/attack_from_air.png");
        sprCrouchIdle = gcnew Bitmap("caballero/crouch_idle.png");
        sprRoll = gcnew Bitmap("caballero/Roll.png");
        sprSlide = gcnew Bitmap("caballero/Slide.png");
        sprHurt = gcnew Bitmap("caballero/Hurt.png");
        sprDeath = gcnew Bitmap("caballero/Death.png");
        sprPray = gcnew Bitmap("caballero/Pray.png");
        sprClimb = gcnew Bitmap("caballero/Climb.png");
        sprHanging = gcnew Bitmap("caballero/Hanging.png");
        sprHealth = gcnew Bitmap("caballero/Health.png");

        x = px;
        y = py;

        estado = EstadoCaballero::Idle;
        frame = 0;

        w = sprIdle->Width / idleFrames;
        h = sprIdle->Height/4;

        facingLeft = false;
    }

    // ===============================
    //   ESTADO
    // ===============================
    void SetEstado(EstadoCaballero nuevo)
    {
        if (estado != nuevo)
        {
            estado = nuevo;
            frame = 0;
            frameCounter = 0;
        }
    }

    EstadoCaballero GetEstado()
    {
        return estado;
    }

    // ===============================
    //   UPDATE (solo animación)
// ===============================
    void Update()
    {
        frameCounter++;
        if (frameCounter >= frameDelay)
        {
            frameCounter = 0;
            frame++;
            if (frame >= getMaxFrames()) frame = 0;
        }
    }

    int getMaxFrames()
    {
        switch (estado)
        {
        case EstadoCaballero::Idle:         return idleFrames;
        case EstadoCaballero::Run:          return runFrames;
        case EstadoCaballero::Jump:         return jumpFrames;
        case EstadoCaballero::Attack:       return attackFrames;
        case EstadoCaballero::AttackCrouch: return crouchAttackFrames;
        case EstadoCaballero::AttackAir:    return airAttackFrames;
        case EstadoCaballero::CrouchIdle:   return crouchIdleFrames;
        case EstadoCaballero::Roll:         return rollFrames;
        case EstadoCaballero::Slide:        return slideFrames;
        case EstadoCaballero::Hurt:         return hurtFrames;
        case EstadoCaballero::Death:        return deathFrames;
        case EstadoCaballero::Pray:         return prayFrames;
        case EstadoCaballero::Climb:        return climbFrames;
        case EstadoCaballero::Hanging:      return hangingFrames;
        case EstadoCaballero::HealthStatus: return healthFrames;
        }
        return 1;
    }

    Bitmap^ getSheet()
    {
        switch (estado)
        {
        case EstadoCaballero::Idle:         return sprIdle;
        case EstadoCaballero::Run:          return sprRun;
        case EstadoCaballero::Jump:         return sprJump;
        case EstadoCaballero::Attack:       return sprAttack;
        case EstadoCaballero::AttackCrouch: return sprCrouchAttack;
        case EstadoCaballero::AttackAir:    return sprAirAttack;
        case EstadoCaballero::CrouchIdle:   return sprCrouchIdle;
        case EstadoCaballero::Roll:         return sprRoll;
        case EstadoCaballero::Slide:        return sprSlide;
        case EstadoCaballero::Hurt:         return sprHurt;
        case EstadoCaballero::Death:        return sprDeath;
        case EstadoCaballero::Pray:         return sprPray;
        case EstadoCaballero::Climb:        return sprClimb;
        case EstadoCaballero::Hanging:      return sprHanging;
        case EstadoCaballero::HealthStatus: return sprHealth;
        }
        return sprIdle;
    }

    // ===============================
    //   DRAW + FLIP + VIDA
    // ===============================
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

        // BARRA DE VIDA
        int barW = frameWidth;
        int barH = 10;
        int vidaW = (vidaActual * barW) / vidaMax;

        g->FillRectangle(Brushes::Black, x, y - 12, barW, barH);
        g->FillRectangle(Brushes::Red, x, y - 12, vidaW, barH);
        g->DrawRectangle(Pens::White, x, y - 12, barW, barH);
    }

    // ===============================
    // HITBOXES
    // ===============================
    Rectangle getHitbox()
    {
        int hbW = (int)(w * 0.4);
        int hbH = (int)(h * 0.8);
        int offsetX = (int)(w * 0.3);
        int offsetY = (int)(h * 0.2);
        return Rectangle(x + offsetX, y + offsetY, hbW, hbH);
    }

    Rectangle getAttackHitbox()
    {
        Rectangle hb = getHitbox();
        int aw = hb.Width;
        if (!facingLeft)
        {
            return Rectangle(hb.Right, hb.Y, aw, hb.Height);
        }
        else
        {
            return Rectangle(hb.X - aw, hb.Y, aw, hb.Height);
        }
    }

    // ===============================
    // POS / VIDA
    // ===============================
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
        if (vidaActual <= 0)
            SetEstado(EstadoCaballero::Death);
    }

    bool EstaMuerto()
    {
        return vidaActual <= 0;
    }
};
