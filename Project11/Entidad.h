#pragma once
#pragma once
#include <iostream>
#include <vector>
#include<time.h>
using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
	
class Entidad
{
protected:

	int x, y;
	int dx, dy;
	int ancho, alto;
	int iDx;//identificador inicial para animar sprites xd
public:

	Entidad() {
		x = y = 0;
		dx = dy = 0;
		ancho, alto = 0;
		iDx = 0;
	}
	void SetDx(int value)
	{
		dx = value;
	}
	void SetDy(int value)
	{
		dy = value;
	}

	int GetX() {
		return x;
	}
	int GetY() {
		return y;
	}
	int GetAncho() {
		return ancho;
	}
	int GetAlto() {
		return alto;
	}
	Rectangle area() {
		return Rectangle(x, y, ancho, alto);
	}
	virtual Rectangle NextArea() {
		//redefinir con virtual en la de jugador, no te olvide ctmrrrrrr    !!! IMPORTANTE  !!!
		return Rectangle(x + dx, y + dy, ancho, alto); //hitboxParaColision-------------> no definida aun en funcion
	}
	Rectangle HitBox()
	{
		return Rectangle(x + ancho / 4, y + alto / 5, ancho / 2.2, alto * 4 / 5);

	}
	Rectangle NextHitBox()
	{
		return Rectangle((x + ancho / 4) + dx, (y + alto / 5) + dy, ancho / 2.2, alto * 4 / 5); //hitboxParaColision-------------> no definida aun en funcion
	}
	virtual void mover(Graphics^ g) {
		y += dy;
		x += dx;
	}
	virtual void mostrar(Graphics^ g, Bitmap^ img) {
		g->FillRectangle(Brushes::Black, area());
	}

};

