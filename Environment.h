#pragma once

#include <iostream>
#include <math.h>

enum Objects : int
{
	Nothing = 0,
	Line,
	End
};

enum LineTypes : int
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
	int* content;

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
};


Environment::Environment()
{
	content = new int[this->AreaWidth * this->AreaHeight] 
	{ 
        	0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 1, 0, 0, 0, 1, 0, 1, 0, 1,
            0, 0, 0, 0, 0, 1, 1, 1, 0, 0,
            1, 0, 0, 1, 0, 1, 0, 1, 0, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 1, 0, 0, 0, 1, 0, 0, 1, 0,
            0, 1, 1, 1, 1, 1, 0, 0, 1, 0,
            0, 1, 0, 1, 0, 1, 0, 0, 1, 0,
            0, 1, 1, 1, 0, 1, 0, 0, 2, 0,
    };
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
		reward = -0.75;
		// end of game
		*_done = 0;
		break;
	case Objects::End:
		// nasiel vychod z bludiska
		reward = +1.0;
		// end of game
		*_done = -1;
		break;
	default:
		// pre najdenie najkratsej trasy
		reward = -0.04;
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
	return (Objects) this->content[getState(_x, _y)];
}

Environment::~Environment()
{

}
