
#include <iostream>
#include <windows.h> 
#include <conio.h>
using namespace std;

enum KeyCodes { ENTER = 13, ESCAPE = 27, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80, SPACEBAR = 32 };
enum Colors { DARKGREEN = 2, RED = 12, YELLOW = 14, BLUE = 9 , ORENGELINGIN = 13};
enum Objects { HALL, WALL, COIN, ENEMY , BOMB, EXIT, UNBRAKINGWALL};

void generLocation(HANDLE h, int location, int HEIGHT, int WIDTH);
void dvishik(HANDLE h, int location, int HEIGHT, int WIDTH);
void countHp(int minesHP, HANDLE h, int HEIGHT, int WIDTH);
bool countCoin(int addCoin, HANDLE h, int HEIGHT, int WIDTH);
bool countBomb(int addBomb, int minusBomb, HANDLE h, int HEIGHT, int WIDTH);
void victori(HANDLE h);

void generLocation(HANDLE h, int location[][50], int HEIGHT, int WIDTH)
{

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			location[y][x] = rand() % 5;

			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1)
				location[y][x] = UNBRAKINGWALL;

			if (x == 0 && y == 2 || x == WIDTH - 1 && y == HEIGHT - 3)
				location[y][x] = HALL;

			if (x == WIDTH - 1 && y == HEIGHT - 3)
				location[y][x] = EXIT;

			if (location[y][x] == ENEMY) {
				int prob = rand() % 6;
				if (prob != 0)
					location[y][x] = HALL;
			}
			if (location[y][x] == COIN) {
				int prob = rand() % 4;
				if (prob != 0)
					location[y][x] = HALL;
			}
			if (location[y][x] == BOMB) {
				int prob = rand() % 20;
				if (prob != 0)
					location[y][x] = HALL;
			}
		}
	}

	for (int y = 0; y < HEIGHT; y++) 
	{
		for (int x = 0; x < WIDTH; x++)
		{
			switch (location[y][x]) {
			case HALL: 
				cout << " ";
				break;
			case WALL:
				SetConsoleTextAttribute(h, DARKGREEN);
				cout << (char)177;
				break;
			case UNBRAKINGWALL:
				SetConsoleTextAttribute(h, DARKGREEN);
				cout << (char)177;
				break;
			case COIN: 
				SetConsoleTextAttribute(h, YELLOW);
				cout << (char)36;
				break;
			case ENEMY: 
				SetConsoleTextAttribute(h, RED);
				cout << (char)1;
				break;
			case BOMB:
				SetConsoleTextAttribute(h, ORENGELINGIN);
				cout << (char)15;
				break;
			case EXIT:
				SetConsoleTextAttribute(h, ORENGELINGIN);
				cout << (char)26;
				break;
			default:
				cout << location[y][x];
				break;
			}
		}
		cout << "\n";
	}
	countHp(0,h,HEIGHT, WIDTH);
	countCoin(0, h, HEIGHT, WIDTH);
	countBomb(0, 0, h, HEIGHT, WIDTH);
}

void dvishik(HANDLE h, int location[][50],int HEIGHT, int WIDTH)
{
	COORD position;
	position.X = 1;
	position.Y = 2;

	SetConsoleCursorPosition(h, position);
	SetConsoleTextAttribute(h, BLUE);
	cout << (char)1;

	while (true) {
		int code = _getch();
		if (code == 224)
			code = _getch();

		SetConsoleCursorPosition(h, position);
		SetConsoleTextAttribute(h, BLUE);
		cout << " ";

		switch (code) 
		{
		case SPACEBAR:
		{  
			if (countBomb(0, 1, h, HEIGHT, WIDTH) == true)
			{
				for (int i = 2; i > 0; --i) {
					SetConsoleCursorPosition(h, position);
					SetConsoleTextAttribute(h, ORENGELINGIN);
					cout << i;
					Sleep(1000);
				}

				COORD explosion[4] =
				{
					{position.X + 1, position.Y},
					{position.X - 1, position.Y},
					{position.X, position.Y + 1},
					{position.X, position.Y - 1}
				};

				for (int i = 0; i < 4; i++)
				{
					if (location[explosion[i].Y][explosion[i].X] != UNBRAKINGWALL && location[explosion[i].Y][explosion[i].X] != COIN && location[explosion[i].Y][explosion[i].X] != EXIT)
					{
						SetConsoleCursorPosition(h, explosion[i]);
						SetConsoleTextAttribute(h, YELLOW);
						cout << (char)177;
					}
				}

				Sleep(500);

				for (int i = 0; i < 4; i++)
				{
					if (location[explosion[i].Y][explosion[i].X] != UNBRAKINGWALL && location[explosion[i].Y][explosion[i].X] != COIN && location[explosion[i].Y][explosion[i].X] != EXIT)
					{
						location[explosion[i].Y][explosion[i].X] = HALL;
						SetConsoleCursorPosition(h, explosion[i]);
						cout << " ";
					}
				}
				break;
			}
			break;
		}
		case ESCAPE:
			exit(0);
			break;
		case RIGHT:
			if (location[position.Y][position.X + 1] != WALL && location[position.Y][position.X + 1] != UNBRAKINGWALL)
				position.X++;
			break;
		case LEFT:
			if (location[position.Y][position.X - 1] != WALL && location[position.Y][position.X - 1] != UNBRAKINGWALL)
				position.X--;
			break;
		case UP:
			if (location[position.Y - 1][position.X] != WALL && location[position.Y - 1][position.X] != UNBRAKINGWALL)
				position.Y--;
			break;
		case DOWN:
			if (location[position.Y + 1][position.X] != WALL && location[position.Y + 1][position.X] != UNBRAKINGWALL)
				position.Y++;
			break;
		default:
			cout << code << "\n";
			break;
		}

		SetConsoleCursorPosition(h, position);
		SetConsoleTextAttribute(h, BLUE);
		cout << (char)1;

		if (location[position.Y][position.X] == COIN) 
		{
			location[position.Y][position.X] = HALL;
			countCoin(1,h,HEIGHT,WIDTH);
		}
		else if (location[position.Y][position.X] == BOMB)
		{
			location[position.Y][position.X] = HALL;
			countBomb(1,0,h,HEIGHT, WIDTH);
		}
		else if (location[position.Y][position.X] == ENEMY)
		{
			location[position.Y][position.X] = HALL;
			countHp(1,h,HEIGHT, WIDTH);
		}
		else if (location[position.Y][position.X] == EXIT)
		{
			if (countCoin(0, h, HEIGHT, WIDTH) == true)
			{
				victori(h);
			}
		}
	}
}

void victori(HANDLE h)
{
	COORD infoPos = { 0,0 };
	SetConsoleCursorPosition(h, infoPos);
	for (int y = 0; y < 70; y++)
	{
		for (int x = 0; x < 70; x++)
		{
			if (x == 0 && y == 0)
			{
				SetConsoleTextAttribute(h, DARKGREEN);
				cout << "Victor!";
			}
			cout << " ";
		}
	}
	Sleep(5000);
	exit(0);
}

void countHp(int minesHP, HANDLE h, int HEIGHT,int WIDTH)
{
	static int hp = 2;

	hp -= minesHP;

	if (hp == 0)
	{
		COORD infoPos = { 0,0 };
		SetConsoleCursorPosition(h, infoPos);
		for (int y = 0; y < 70; y++)
		{
			for (int x = 0; x < 70; x++)
			{
				if (x == 0 && y == 0)
				{
					SetConsoleTextAttribute(h, RED);
					cout << "Game Over!";
				}
				cout << " ";
			}
		}
		Sleep(5000);
		exit(0);
	}

	COORD infoPos = { 0, HEIGHT + 1 }; 
	SetConsoleCursorPosition(h, infoPos);

	SetConsoleTextAttribute(h, DARKGREEN);
	cout << "HP:" << hp<< "\n";

}

bool countCoin(int addCoin, HANDLE h, int HEIGHT, int WIDTH)
{
	static int coin = 00;

	coin += addCoin;

	if (coin >= 10)
	{
		return true;
	}

	COORD infoPos = { 0, HEIGHT + 2 };
	SetConsoleCursorPosition(h, infoPos);

	SetConsoleTextAttribute(h, YELLOW);
	cout << "Coin:" << coin << "\n";

	return false;
}

bool countBomb(int addBomb, int minusBomb, HANDLE h, int HEIGHT, int WIDTH)
{
	static int bomb = 0;

	bomb += addBomb;

	if (minusBomb == 1 && bomb >= 1)
	{
		bomb -= 1;
		return true;
	}

	COORD infoPos = { 0, HEIGHT + 3 };
	SetConsoleCursorPosition(h, infoPos);

	SetConsoleTextAttribute(h, BLUE);
	cout << "Bomb:" << bomb << "\n";

	return false;
}

int main()
{
	srand(time(0));

	system("title Бомбер!");

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	const int WIDTH = 50;
	const int HEIGHT = 15;
	int location[HEIGHT][WIDTH] = {};

	CONSOLE_CURSOR_INFO info;
	info.bVisible = false;
	info.dwSize = 100;
	SetConsoleCursorInfo(h, &info);

	generLocation(h,location, HEIGHT, WIDTH);
	dvishik(h,location,HEIGHT,WIDTH);
	countHp(0,h,HEIGHT, WIDTH);
	countCoin(0, h, HEIGHT, WIDTH);
	countBomb(0, 0, h, HEIGHT, WIDTH);
}
