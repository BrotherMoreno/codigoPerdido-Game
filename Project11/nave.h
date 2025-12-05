#pragma once
using namespace System;
using namespace System::Drawing;

public ref class Nave
{
private:
    Bitmap^ sheet;
    int frameW, frameH;
    int totalFrames;
    int frameIndex;
    int x, y;
    int tick;
    int frameSpeed;

public:

    Nave(int posX, int posY)
    {
        sheet = gcnew Bitmap("nave_player.png");

        totalFrames = 7;
        frameW = sheet->Width / totalFrames;
        frameH = sheet->Height;

        x = posX;
        y = posY;

        frameIndex = 0;
        tick = 0;
        frameSpeed = 5;
    }

    void Update()
    {
        tick++;
        if (tick >= frameSpeed)
        {
            tick = 0;
            frameIndex = (frameIndex + 1) % totalFrames;
        }
    }

    void Draw(Graphics^ g)
    {
        Rectangle src(frameIndex * frameW, 0, frameW, frameH);
        Rectangle dest(x, y, frameW, frameH);

        g->DrawImage(sheet, dest, src, GraphicsUnit::Pixel);
    }

    void SetPosition(int nx, int ny)
    {
        x = nx;
        y = ny;
    }
};
