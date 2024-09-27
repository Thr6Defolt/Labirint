
#include <iostream>
#include <windows.h> 
#include <conio.h>
using namespace std;

enum KeyCodes { ENTER = 13, ESCAPE = 27, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80, SPACEBAR = 32 };
enum Colors { DARKGREEN = 2, RED = 12, YELLOW = 14, BLUE = 9 , ORENGELINGIN = 13};
enum Objects { HALL, WALL, COIN, ENEMY , BOMB, EXIT, ECSPLOSIVE};

void generLocation(HANDLE h, int location, int HEIGHT, int WIDTH);
void dvishik(HANDLE h, int location, int HEIGHT, int WIDTH);
void infoGame(int addCoin, int addBomb, int minesHP);

void generLocation(HANDLE h, int location[][50], int HEIGHT, int WIDTH)
{

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			location[y][x] = rand() % 5;

			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1)
				location[y][x] = WALL;

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
			case ECSPLOSIVE:
				SetConsoleTextAttribute(h, YELLOW);
				cout << (char)177;
				Sleep(500);
				cout << " ";
				break;
			default:
				cout << location[y][x];
				break;
			}
		}
		cout << "\n";
	}
	infoGame(0,0,0);
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
			for (int i = 5;i > 0;--i)
			{
				SetConsoleCursorPosition(h, position);
				cout << i;
				cout << " ";
				Sleep(1000);
			}
			location[position.Y][position.X + 1] = ECSPLOSIVE;
			location[position.Y][position.X - 1] = ECSPLOSIVE;
			location[position.Y + 1][position.X] = ECSPLOSIVE;
			location[position.Y - 1][position.X] = ECSPLOSIVE;
			break;
		case ESCAPE:
			// cout << "ESCAPE\n";
			break;
		case RIGHT:
			if (location[position.Y][position.X + 1] != WALL)
				position.X++;
			break;
		case LEFT:
			if (location[position.Y][position.X - 1] != WALL)
				position.X--;
			break;
		case UP:
			if (location[position.Y - 1][position.X] != WALL)
				position.Y--;
			break;
		case DOWN:
			if (location[position.Y + 1][position.X] != WALL)
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
			//infoGame(1, 0, 0);
		}
		else if (location[position.Y][position.X] == BOMB)
		{
			location[position.Y][position.X] = HALL;
			//infoGame(0, 1, 0);
		}
		else if (location[position.Y][position.X] == ENEMY)
		{
			location[position.Y][position.X] = HALL;
			//infoGame(0, 0, 5);
		}
		else if (location[position.Y][position.X] == EXIT)
		{
			location[position.Y][position.X] = HALL;
		}
	}
}

void infoGame(int addCoin, int addBomb, int minesHP)
{
	int coin = 0;
	int bomb = 0;
	int hp = 10;

	cout << "HP:" << hp - minesHP << "\n";
	cout << "Coin:" << coin + addCoin << "\n";
	cout << "Bomb:" << bomb + addBomb << "\n";
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
	infoGame(0,0,0);
}
