#pragma once

#include <iostream>
#include <math.h>

enum Objects
{
	Nothing = 0,
	Line,
	End
};

enum LineTypes
{
	Horizontal = 0,
	Vertical
};

typedef struct {
	int x;
	int y;
} Point;

class Environment
{
private:
	int AreaWidth = 10;
	int AreaHeight = 10;
	Objects* content;

	void drawLine(int fx, int fy, int tx, int ty);
	void clrEnv();
public:
	Environment();
	~Environment();

	int getWidth();
	int getHeight();
	float getReward(int _x, int _y, int* _done);
	void NakresliHernySvet(int _x, int _y);
	int getState(int _x, int _y);
	Objects getContent(int _x, int _y);
	void genRand(int _count);
	void findLeaf(int* _x, int* _y);
};


Environment::Environment()
{
	content = new Objects[this->AreaWidth * this->AreaHeight];

	// Clear environment
	clrEnv();

	// Set lines
	this->content[2 + 2 * AreaWidth] = Objects::Line;
	this->content[3 + 2 * AreaWidth] = Objects::Line;
	this->content[4 + 2 * AreaWidth] = Objects::Line;
	this->content[5 + 2 * AreaWidth] = Objects::Line;
	this->content[6 + 2 * AreaWidth] = Objects::Line;
	this->content[7 + 2 * AreaWidth] = Objects::Line;
	this->content[4 + 3 * AreaWidth] = Objects::Line;
	this->content[4 + 4 * AreaWidth] = Objects::Line;
	this->content[4 + 5 * AreaWidth] = Objects::Line;
	this->content[4 + 6 * AreaWidth] = Objects::Line;
	this->content[4 + 7 * AreaWidth] = Objects::Line;
	this->content[3 + 5 * AreaWidth] = Objects::Line;
	this->content[2 + 5 * AreaWidth] = Objects::Line;
	this->content[2 + 6 * AreaWidth] = Objects::Line;
	this->content[7 + 3 * AreaWidth] = Objects::Line;
	this->content[7 + 4 * AreaWidth] = Objects::Line;

	// Set end
	this->content[5 + 7 * AreaWidth] = Objects::End;
}

int Environment::getWidth()
{
	return this->AreaWidth;
}

int Environment::getHeight()
{
	return this->AreaHeight;
}

void Environment::clrEnv()
{
	for (int i = 0; i < (this->AreaWidth * this->AreaHeight); i++)
		this->content[i] = Objects::Nothing;
}

float Environment::getReward(int _x, int _y, int* _done)
{
	float reward;

	switch (this->content[getState(_x, _y)])
	{
	case Objects::Nothing:
		// ak strati ciaru
		reward = -100;
		// end of game
		*_done = 1;
		break;
	case Objects::End:
		// nasiel vychod z bludiska
		reward = +100;
		// end of game
		*_done = -1;
		break;
	default:
		// pre najdenie najkratsej trasy
		reward = -1;
		*_done = 0;
		break;
	}

	return reward;
}

void Environment::NakresliHernySvet(int _x, int _y)
{
	// Vycisti konzolu
	system("clear");

	for (int j = 0; j < AreaHeight; j++)
		{
			std::cout << "|";
			for (int i = 0; i < AreaWidth; i++)
			{
				if (_x == i && _y == j)
				{
				    //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BACKGROUND_RED|FOREGROUND_INTENSITY));					
					std::cout << "\033[0;41m O \033[0m";
				    //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (0|FOREGROUND_INTENSITY));
				}
				else
				{
					switch (this->content[(j * AreaWidth) + i])
					{
					case (int)Objects::Nothing:
						//Console.ResetColor();
						std::cout << "\033[0m   ";
						break;
					case (int)Objects::Line:
						//Console.BackgroundColor = ConsoleColor.White;
						std::cout << "\033[0;47;30m X \033[0m";
						//Console.ResetColor();
						break;
					case (int)Objects::End:
					    //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BACKGROUND_GREEN));
						//Console.BackgroundColor = ConsoleColor.Green;
						//Console.ForegroundColor = ConsoleColor.Black;
						std::cout << "\033[0;42;30m E \033[0m";
					    //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (0|FOREGROUND_INTENSITY));
						//Console.ResetColor();
						break;
					}
				}
			}
			std::cout << "|" << std::endl;
		}
		std::cout << std::endl;
}

int Environment::getState(int _x, int _y)
{
	return ((_y * this->AreaWidth) + _x);
}

Objects Environment::getContent(int _x, int _y)
{
	return this->content[getState(_x, _y)];
}

void Environment::findLeaf(int* _x, int* _y)
{
	int count;

	for (int i = this->AreaWidth; i < (this->AreaWidth * this->AreaHeight) - this->AreaWidth; i++)
	{
		count = 0;

		if (this->content[i] == Objects::Line)
		{
			if (this->content[i - 1] == Objects::Line)
				count++;

			if (this->content[i + 1] == Objects::Line)
				count++;

			if (this->content[i - this->AreaWidth] == Objects::Line)
				count++;

			if (this->content[i + this->AreaWidth] == Objects::Line)
				count++;			
		}

		if (count <= 1)
		{
			*_x = i % this->AreaWidth;
			*_y = i / this->AreaWidth;
			break;
		}
	}
}

void Environment::drawLine(int fx, int fy, int tx, int ty)
{
	/*std::cout << "fx = " << fx << std::endl;
	std::cout << "fy = " << fy << std::endl;

	std::cout << "tx = " << tx << std::endl;
	std::cout << "ty = " << ty << std::endl;*/

	for (int x = fx; x < tx; x++)
	{
		for (int y = fy; y < ty; y++)
		{
			std::cout << "content = " << this->content[getState(x, y)] << std::endl;
			std::cout << "state = " << getState(x, y) << std::endl;
			this->content[getState(x, y)] = Objects::Line;
		}
	}
}

void Environment::genRand(int _count)
{
	LineTypes _type;
	Point _start;
	int _size;

	Point last_point1, last_point2;

	// Vycisti prostredie
	clrEnv();

	last_point1.x = 1;
	last_point1.y = 1;

	last_point2.x = (this->AreaWidth - 2);
	last_point2.y = (this->AreaHeight - 2);

	// vieme pocet ciar
	for (int i = 0; i < _count; i++)
	{
		_start.x = ((rand() % last_point2.x) + last_point1.x);
		_start.y = ((rand() % last_point2.y) + last_point1.y);

		// vygenerujeme nahodny tvar a dlzku ciary
		_type = (LineTypes)(rand() % 2);
		if (_type == LineTypes::Horizontal)
			_size = ((rand() % (this->AreaWidth - _start.x)) + 1);
		else
			_size = ((rand() % (this->AreaHeight - _start.y)) + 1);

		// zaciname 0. ciarou od bodu _start
		if (_type == LineTypes::Horizontal)
			drawLine((last_point1.x = _start.x), (last_point1.y = _start.y), (last_point2.x = (_start.x + _size)), (last_point2.y = _start.y));
		else
			drawLine((last_point1.x = _start.x), (last_point1.y = _start.y), (last_point2.x = _start.x), (last_point2.y = (_start.y + _size)));
	}
}

Environment::~Environment()
{

}
