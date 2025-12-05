#pragma once
using namespace System;
using namespace System::Drawing;

public enum class EstadoTrader
{
    Idle,
    Dialogue,
    Approval
};

public ref class Trader
{
private:

    Bitmap^ sprIdle;
    Bitmap^ sprDialogue;
    Bitmap^ sprApproval;

    int x, y;
    int w, h;

    EstadoTrader estado;
    int frame;
    int frameCounter;
    int frameDelay;
    int maxFrames;
    int frameWidth;

    int idleFrames = 6;
    int dialogueFrames = 16;
    int approvalFrames = 8;

public:

    Trader(int px, int py)
    {
        sprIdle = gcnew Bitmap("trader/Idle.png");
        sprDialogue = gcnew Bitmap("trader/Dialogue.png");
        sprApproval = gcnew Bitmap("trader/Approval.png");

        x = px;
        y = py;

        w = sprIdle->Width / idleFrames;
        h = sprIdle->Height;

        estado = EstadoTrader::Idle;
        frame = 0;
        frameDelay = 5;
        frameCounter = 0;
    }

    void Update(Rectangle hitboxCapitan)
    {
        int capX = hitboxCapitan.X + hitboxCapitan.Width / 2;
        int capY = hitboxCapitan.Y + hitboxCapitan.Height / 2;

        int tX = x + w / 2;
        int tY = y - h / 2;

        int dx = Math::Abs(capX - tX);
        int dy = Math::Abs(capY - tY);

        if (dx < 150 && dy < 120 && estado != EstadoTrader::Approval)
            estado = EstadoTrader::Dialogue;
        else if (estado != EstadoTrader::Approval)
            estado = EstadoTrader::Idle;

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
        case EstadoTrader::Idle:      return idleFrames;
        case EstadoTrader::Dialogue:  return dialogueFrames;
        case EstadoTrader::Approval:  return approvalFrames;
        }
        return 1;
    }

    Bitmap^ getCurrentSheet()
    {
        switch (estado)
        {
        case EstadoTrader::Idle:      return sprIdle;
        case EstadoTrader::Dialogue:  return sprDialogue;
        case EstadoTrader::Approval:  return sprApproval;
        }
        return sprIdle;
    }

    void Draw(Graphics^ g)
    {
        Bitmap^ sheet = getCurrentSheet();
        maxFrames = getMaxFrames();
        frameWidth = sheet->Width / maxFrames;

        Rectangle src(frameWidth * frame, 0, frameWidth, sheet->Height);

        int drawX = x;
        int drawY = y - sheet->Height;

        Rectangle dest(drawX, drawY, frameWidth, sheet->Height);
        g->DrawImage(sheet, dest, src, GraphicsUnit::Pixel);
    }

    Rectangle getHitbox()
    {
        return Rectangle(x, y - h, w, h);
    }

    Point getCenter()
    {
        Rectangle hb = getHitbox();
        return Point(hb.X + hb.Width / 2, hb.Y + hb.Height / 2);
    }

    EstadoTrader GetEstado()
    {
        return estado;
    }

    void SetApproval()
    {
        estado = EstadoTrader::Approval;
        frame = 0;
        frameCounter = 0;
    }
};
