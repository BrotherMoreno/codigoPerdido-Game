#pragma once
using namespace System;
using namespace System::Drawing;

public ref class Parallax
{
private:
    Bitmap^ sky;
    Bitmap^ back;
    Bitmap^ mountain;
    Bitmap^ mid;
    Bitmap^ shortLayer;
    Bitmap^ ground;

    int sx, sx2;
    int bx, bx2;
    int mx, mx2;
    int midx, midx2;
    int shx, shx2;
    int gx, gx2;

    int formW, formH;

    void reset(int% a, int% b, int w)
    {
        if (a <= -w) a = b + w;
        if (b <= -w) b = a + w;
    }

public:

    Parallax(int width, int height)
    {
        formW = width;
        formH = height;

        sky = gcnew Bitmap("forest_sky.png");
        sky = gcnew Bitmap(sky, formW, formH);

        back = gcnew Bitmap("forest_back.png");
        back = gcnew Bitmap(back, formW, formH);

        mountain = gcnew Bitmap("forest_mountain.png");
        mountain = gcnew Bitmap(mountain, formW, formH);

        mid = gcnew Bitmap("forest_mid.png");
        mid = gcnew Bitmap(mid, formW, formH);

        shortLayer = gcnew Bitmap("forest_short.png");
        shortLayer = gcnew Bitmap(shortLayer, formW, formH);

        ground = gcnew Bitmap("forest_long.png");
        ground = gcnew Bitmap(ground, formW, formH);

        sx = sx2 = bx = bx2 = mx = mx2 = midx = midx2 = shx = shx2 = gx = gx2 = 0;
        sx2 = bx2 = mx2 = midx2 = shx2 = gx2 = formW;
    }

    void Update()
    {
        sx -= 2;   sx2 -= 2;
        bx -= 4;   bx2 -= 4;
        mx -= 3;   mx2 -= 3;
        midx -= 10; midx2 -= 10;
        shx -= 20; shx2 -= 20;
        gx -= 20;  gx2 -= 20;

        reset(sx, sx2, formW);
        reset(bx, bx2, formW);
        reset(mx, mx2, formW);
        reset(midx, midx2, formW);
        reset(shx, shx2, formW);
        reset(gx, gx2, formW);
    }

    void Draw(Graphics^ g)
    {
        int skyY = 0;
        int backY = formH * 0.005;
        int mountainY = formH * 0.005;
        int midY = formH * 0.06;
        int shortY = formH * 0.02;
        int groundY = formH * -0.01;

        g->DrawImage(sky, Rectangle(sx, skyY, formW, formH));
        g->DrawImage(sky, Rectangle(sx2, skyY, formW, formH));

        g->DrawImage(mountain, Rectangle(mx, mountainY, formW, formH));
        g->DrawImage(mountain, Rectangle(mx2, mountainY, formW, formH));

        g->DrawImage(back, Rectangle(bx, backY, formW, formH));
        g->DrawImage(back, Rectangle(bx2, backY, formW, formH));

        g->DrawImage(mid, Rectangle(midx, midY, formW, formH));
        g->DrawImage(mid, Rectangle(midx2, midY, formW, formH));


        g->DrawImage(ground, Rectangle(gx, groundY, formW, formH));
        g->DrawImage(ground, Rectangle(gx2, groundY, formW, formH)); 

        g->DrawImage(shortLayer, Rectangle(shx, shortY, formW, formH));
        g->DrawImage(shortLayer, Rectangle(shx2, shortY, formW, formH));
    }
};
