
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

//Генерация локаций 
void generLocation(HANDLE h, int location[][50], int HEIGHT, int WIDTH)
{

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			// Рандомайзер спавна активностей в лабиринте
			location[y][x] = rand() % 5;
			//Стений по краям
			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1)
				location[y][x] = UNBRAKINGWALL;
			//Вход
			if (x == 0 && y == 2 || x == WIDTH - 1 && y == HEIGHT - 3)
				location[y][x] = HALL;
			//Выход
			if (x == WIDTH - 1 && y == HEIGHT - 3)
				location[y][x] = EXIT;

			if (location[y][x] == ENEMY) 
			{
				// Вероятность того , останется враг или нет 6%
				int prob = rand() % 6; 
				if (prob != 0)
					location[y][x] = HALL;
			}
			if (location[y][x] == COIN) 
			{
				// Вероятность того , останется монета или нет 4%
				int prob = rand() % 4;
				if (prob != 0)
					location[y][x] = HALL;
			}
			if (location[y][x] == BOMB) 
			{
				// Вероятность того , останется бомба или нет 20%
				int prob = rand() % 20;
				if (prob != 0)
					location[y][x] = HALL;
			}
		}
	}

	// Показ локации
	for (int y = 0; y < HEIGHT; y++) 
	{
		for (int x = 0; x < WIDTH; x++)
		{
			switch (location[y][x]) 
			{
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
	//Вывод информаций
	countHp(0,h,HEIGHT, WIDTH);
	countCoin(0, h, HEIGHT, WIDTH);
	countBomb(0, 0, h, HEIGHT, WIDTH);
}

//Робота игры
void dvishik(HANDLE h, int location[][50],int HEIGHT, int WIDTH)
{
	//Размещение ГГ
	COORD position;
	position.X = 1;
	position.Y = 2;

	// Функция установки курсора в любую точку на экране консоли
	SetConsoleCursorPosition(h, position);
	SetConsoleTextAttribute(h, BLUE);
	cout << (char)1;

	//Игровой движок
	while (true) 
	{
		// Функция приостанавливает работу программы, ждёт реакции пользователя
		int code = _getch();
		if (code == 224)
			code = _getch();// Если первый код был 224, то повторный вызов функции вернёт только один, нормальный код

		// стирание ГГ в старой позиции
		SetConsoleCursorPosition(h, position);
		SetConsoleTextAttribute(h, BLUE);
		cout << " ";

		//Что делать при нажатой клавищи
		switch (code) 
		{
		case SPACEBAR:
		{  
			//Если у игрока есть бомба будет взрыв
			if (countBomb(0, 1, h, HEIGHT, WIDTH) == true)//Проверка
			{
				//Отчет взрыва
				for (int i = 2; i > 0; --i) 
				{
					SetConsoleCursorPosition(h, position);
					SetConsoleTextAttribute(h, ORENGELINGIN);
					cout << i;
					Sleep(1000);
				}

				//Масив кординатов взрыва
				COORD explosion[4] =
				{
					{position.X + 1, position.Y},
					{position.X - 1, position.Y},
					{position.X, position.Y + 1},
					{position.X, position.Y - 1}
				};

				//Анимация взрыва
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
			//Выйти с игры
			exit(0);
			break;
		case RIGHT:
			//ПРоверка если на ту позицае которой хочит станть игрок нету стен то он перемешается
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

		// Показ ГГ в новой позиции
		SetConsoleCursorPosition(h, position);
		SetConsoleTextAttribute(h, BLUE);
		cout << (char)1;

		// Взаимодействие ГГ с другими объектами в лабиринте
		if (location[position.Y][position.X] == COIN) 
		{
			location[position.Y][position.X] = HALL;
			//Добовляет монеты
			countCoin(1,h,HEIGHT,WIDTH);
		}
		else if (location[position.Y][position.X] == BOMB)
		{
			location[position.Y][position.X] = HALL;
			//Добовляет бомбу
			countBomb(1,0,h,HEIGHT, WIDTH);
		}
		else if (location[position.Y][position.X] == ENEMY)
		{
			location[position.Y][position.X] = HALL;
			//Наносит урон ГГ
			countHp(1,h,HEIGHT, WIDTH);
		}
		else if (location[position.Y][position.X] == EXIT)
		{
			//Если игрок коснулся выхода 
			//Вызивает функцию для проверки, естли у него 10 монет
			if (countCoin(0, h, HEIGHT, WIDTH) == true)
			{
				victori(h);
			}
		}
	}
}

void victori(HANDLE h)
{
	//Выстовляет курсор на 0 0 кординатох
	COORD infoPos = { 0,0 };
	SetConsoleCursorPosition(h, infoPos);

	//Стирает локацию и вывод слова победа
	for (int y = 0; y < 50; y++)
	{
		for (int x = 0; x < 44; x++)
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
		//Выстовляет курсор на 0 0 кординатох
		COORD infoPos = { 0,0 };
		SetConsoleCursorPosition(h, infoPos);

		//Стирает локацию и вывод слова Game over
		for (int y = 0; y < 50; y++)
		{
			for (int x = 0; x < 44; x++)
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

	//Кординати информаци про ХП
	COORD infoPos = { 0, HEIGHT + 1 }; 
	SetConsoleCursorPosition(h, infoPos);

	//Вывод информаци про ХП
	SetConsoleTextAttribute(h, DARKGREEN);
	cout << "HP: " << hp<< "\n";

}

bool countCoin(int addCoin, HANDLE h, int HEIGHT, int WIDTH)
{
	static int coin = 00;

	coin += addCoin;

	//Кординати информаци про монет
	COORD infoPos = { 0, HEIGHT + 2 };
	SetConsoleCursorPosition(h, infoPos);

	//Вывод информаци про монет
	SetConsoleTextAttribute(h, YELLOW);
	cout << "Coin: " << coin  << "\n";
	cout << "Exit cost 10 coin" << "\n";

	//Проверка естли у игрока есть 10 монет
	if (coin >= 10)
	{
		return true;
	}


	return false;
}

bool countBomb(int addBomb, int minusBomb, HANDLE h, int HEIGHT, int WIDTH)
{
	static int bomb = 0;

	bomb += addBomb;

	//Проверка естли у игрока бомбы 
	if (minusBomb == 1 && bomb >= 1)
	{
		bomb -= 1;

		//Кординати информаци про бомбы
		COORD infoPos = { 0, HEIGHT + 4 };
		SetConsoleCursorPosition(h, infoPos);

		//Вывод информаци про бомбы
		SetConsoleTextAttribute(h, BLUE);
		cout << "Bomb: " << bomb << "\n";

		return true;
	}

	//Кординати информаци про бомбы
	COORD infoPos = { 0, HEIGHT + 4 };
	SetConsoleCursorPosition(h, infoPos);

	//Вывод информаци про бомбы
	SetConsoleTextAttribute(h, BLUE);
	cout << "Bomb: " << bomb << "\n";

	return false;
}

int main()
{
	srand(time(0));

	//Запуск алгоритма генераций случайних чисел
	system("title Бомбер!");

	// Дискриптор окна консоли
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	//Размери лаберинта
	const int WIDTH = 50;
	const int HEIGHT = 15;
	int location[HEIGHT][WIDTH] = {};

	// Скрыть стандартный мигающий курсор
	CONSOLE_CURSOR_INFO info;
	info.bVisible = false;
	info.dwSize = 100;
	SetConsoleCursorInfo(h, &info);

	// Генерация игри
	generLocation(h,location, HEIGHT, WIDTH);
	dvishik(h,location,HEIGHT,WIDTH);
}
