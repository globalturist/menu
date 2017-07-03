#include "Constants.h"
#include "Console.h" 
#include <iostream>
#include <Windows.h>
#include <conio.h>

namespace
{
	constexpr int			downKey { 80 };
	constexpr int			rightKey{ 77 };
	constexpr int			leftKey { 75 };
	constexpr int			upKey   { 72 };
    unsigned constexpr char enterKey{ 13 };
	constexpr int			padding { 2 };
}

intPair Console::navigation(const intPair& jumpLength,
	const intPair& xRange,
	const intPair& yRange,
	unsigned const char symbol)
{
	int curX{ xRange.first };
	int curY{ yRange.first };
	do {
		putSymbol(curX, curY, symbol);
		gotoxy(curX, curY);
		if (_getch() == enterKey) return intPair(curX, curY);
		switch (static_cast<int>(_getch()))
		{
		case upKey:
		{
			if ((curY - jumpLength.second) < yRange.first)
			{
				curY = yRange.second;
				putSymbol(curX, yRange.first, ' ');
			}//end of if
			else
			{
				curY -= jumpLength.second;
				putSymbol(curX, curY + jumpLength.second, ' ');
			}//end of else
		}//end of case 
		break;
		case downKey:
		{
			if ((curY + jumpLength.second) > yRange.second)
			{
				curY = yRange.first;
				putSymbol(curX, yRange.second, ' ');
			}//end of if
			else
			{
				curY += jumpLength.second;
				putSymbol(curX, curY - jumpLength.second, ' ');
			}//end of else
		}//end of case 
		break;
		case leftKey:
		{
			if ((curX - jumpLength.first) < xRange.first)
			{
				curX = xRange.second;
				putSymbol(xRange.first, curY, ' ');
			}//end of if
			else
			{
				curX -= jumpLength.first;
				putSymbol(curX + jumpLength.first, curY, ' ');
			}//end of else
		}//end of case 
		break;
		case rightKey:
		{
			if ((curX + jumpLength.first) > xRange.second)
			{
				curX = xRange.first;
				putSymbol(xRange.second, curY, ' ');
			}//end of if
			else
			{
				curX += jumpLength.first;
				putSymbol(curX - jumpLength.first, curY, ' ');
			}//end of else
		}//end of case 
		break;
		}//end of switch
	} while (true);
}

void Console::setXY(int destX, int destY, int &curX, int &curY)
{
	if (destX < 0)
	{
		curX = -destX;
		std::cout << "X value was made positive.\n";
	}//end of if
	else curX = destX;
	if (destY < 0)
	{
		curY = -destY;
		std::cout << "Y value was made positive.\n";
	}//end of if
	else curY = destY;
}

inline void Console::putSymbol(int xPos, int yPos, const char symbol)
{
	gotoxy(xPos, yPos);
	std::cout << symbol;
}

void Console::clrSec(int curX, int curY, int destX, int destY)
{
	gotoxy(curX, curY);
	for (int row{}; row < destY; ++row)
		for (int col{}; col < destX; ++col)
		{
			Console::gotoxy(curX + col, curY + row);
			std::cout << ' ';
		}//end of for
	gotoxy(curX, curY);
}

void Console::drawBox(int hight, int length, int x, int y)
{
	gotoxy(x, y);
	std::cout << Shapes::topLeftAngle;
	for (int i{}; i < (length + padding); ++i) std::cout << Shapes::horizontalPiece;
	std::cout << Shapes::topRightAngle;

	for (int i{}; i < hight; ++i)
	{
		gotoxy(x, ++y);
		std::cout << Shapes::verticalPiece;
		gotoxy(x + length + 3, y); // x + length + 3 represents the furthest boarder						   
		std::cout << Shapes::verticalPiece;
	}//end of for

	gotoxy(x, ++y);
	std::cout << Shapes::bottomLeftAngle;
	for (int i{}; i < (length + padding); ++i) std::cout << Shapes::horizontalPiece;
	std::cout << Shapes::bottomRightAngle;
}

inline void Console::gotoxy(int x, int y)
{
	COORD pos = { static_cast<short>(x), static_cast<short>(y) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Console::clrScr()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE                     hStdOut;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	SetConsoleCursorPosition(hStdOut, homeCoords);
}

