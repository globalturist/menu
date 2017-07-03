#ifndef CONSOLE_GRAPHICS_H
#define CONSOLE_GRAPHICS_H

#include <utility>

using intPair = std::pair<int, int>;

namespace Console
{
	intPair navigation(const intPair& jumpLength,
		const intPair& xRange,
		const intPair& yRange,
		unsigned const char symbol);
	void setXY(int destX, int destY, int &curX, int &curY);
	void putSymbol(int xPos, int yPos, const char symbol);
	void clrSec(int curX, int curY, int destX, int destY);
	void drawBox(int hight, int length, int x, int y);
	void gotoxy(int x, int y);
	void clrScr();
}

#endif
