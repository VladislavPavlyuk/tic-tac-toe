#include <iostream>
#include<time.h>
#include<stdlib.h>
#include <vector>

using namespace std;

void WhoIsWinner(int SUM1, int SUM2);
void DisplayField(char A[][3], const int length, int counter);
void CheckHorizontalLine(char A[][3], const int length);
void CheckVerticalLine(char A[][3], const int length);
void CheckDiagonalWestEast(char A[][3], const int length);
void CheckDiagonalEastWest(char A[][3], const int length);
void NoWiner(int counter);

int PlayerTurn(char A[][3], const int length, int counter);

int ComputerTurn(char A[][3], const int length, int counter);

// Receiver – конечный получатель команды. Именно он располагает информацией о том, как надо обрабатывать команду.
class Game
{
	int result = 0;
public:
	void Operation(char _operator, int operand)
	{
		switch (_operator)
		{
		case '+':
			result += operand;
			break;
		case '-':
			result -= operand;
			break;
		case '*':
			result *= operand;
			break;
		case '/':
			result /= operand;
			break;
		}
		char buffer[100];
		sprintf_s(buffer, "Current value = %d (following %c %d)\n", result, _operator, operand);
		cout << buffer;
	}
};

// ICommand - интерфейс, представляющий команду.
class ICommand abstract
{
public:
	virtual void Execute() abstract;
	virtual void UnExecute() abstract;
};

// ConcreteCommand – конкретная команда.
class GameCommand : public ICommand
{
	char _operator;
	int operand;
	Game* game;
public:
	GameCommand(Game* game, char _operator, int operand)
	{
		this->game = game;
		this->_operator = _operator;
		this->operand = operand;
	}
	void Execute()
	{
		game->Operation(_operator, operand);
	}
	void UnExecute()
	{
		game->Operation(Undo(_operator), operand);
	}
private:
	char Undo(char _operator)
	{
		char undo;
		switch (_operator)
		{
		case '+':
			undo = '-';
			break;
		case '-':
			undo = '+';
			break;
		case '*':
			undo = '/';
			break;
		case '/':
			undo = '*';
			break;
		default:
			undo = ' ';
			break;
		}
		return undo;
	}
};

// Invoker – это инициатор выполнения команды.
class Player
{
	vector<ICommand*> commands;
	int current = 0;
public:
	void Redo(int levels)
	{
		cout << "\n---- Redo " << levels << " levels\n";
		for (int i = 0; i < levels; i++)
			if (current < commands.size() - 1)
				commands[current++]->Execute();
	}
	void Undo(int levels)
	{
		cout << "\n---- Undo " << levels << " levels\n";
		for (int i = 0; i < levels; i++)
			if (current > 0)
				commands[--current]->UnExecute();
	}
	void Turn(ICommand* command)
	{
		command->Execute();
		this->commands.push_back(command);
		current++;
	}
	~Player()
	{
		for (int i = 0; i < commands.size(); i++)
			delete commands[i];
	}
};
class Computer
{
	vector<ICommand*> commands;
	int current = 0;
public:
	void Redo(int levels)
	{
		cout << "\n---- Redo " << levels << " levels\n";
		for (int i = 0; i < levels; i++)
			if (current < commands.size() - 1)
				commands[current++]->Execute();
	}
	void Undo(int levels)
	{
		cout << "\n---- Undo " << levels << " levels\n";
		for (int i = 0; i < levels; i++)
			if (current > 0)
				commands[--current]->UnExecute();
	}
	void Turn(ICommand* command)
	{
		command->Execute();
		this->commands.push_back(command);
		current++;
	}
	~Computer()
	{
		for (int i = 0; i < commands.size(); i++)
			delete commands[i];
	}
};

void Invoker(Player* player, ICommand* command)
{
	player->Turn(command);
	/*
	* Таким образом, инициатор, отправляющий запрос, ничего не знает о получателе,
	* который и будет выполнять команду.
	* Кроме того, если нам потребуется применить какие-то новые команды,
	* мы можем просто унаследовать классы от абстрактного класса ICommand и
	* реализовать его методы Execute и Undo.
	*/
}
void Invoker(Computer* computer, ICommand* command)
{
	computer->Turn(command);
	/*
	* Таким образом, инициатор, отправляющий запрос, ничего не знает о получателе,
	* который и будет выполнять команду.
	* Кроме того, если нам потребуется применить какие-то новые команды,
	* мы можем просто унаследовать классы от абстрактного класса ICommand и
	* реализовать его методы Execute и Undo.
	*/
}

int main()
{
	Player* player = new Player(); // Invoker – это инициатор выполнения команды
	Game* game = new Game(); // Receiver – конечный получатель команды

	ICommand* command = new GameCommand(game, '+', 100); // конкретная команда
	Invoker(player, command);

	command = new GameCommand(game, '-', 50); // конкретная команда
	Invoker(player, command);

	command = new GameCommand(game, '*', 10); // конкретная команда
	Invoker(player, command);

	command = new GameCommand(game, '/', 2); // конкретная команда
	Invoker(player, command);

	player->Undo(4);
	player->Redo(2);

	command = new GameCommand(game, '*', 3); // конкретная команда
	Invoker(player, command);

	delete game;
	delete player;

	system("pause");

	srand(time(NULL));
	int N, M, SUM1, SUM2;
	const int  length = 3;
	int counter = 0;
	bool shoot = false;

	//массив игрового поля
	char A[length][length];

	//заполняем массив поля пустыми значениями
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++)
		{
			A[i][j] = '.';
		}
	}

	//вывод поля на экран
	DisplayField(A, length, counter);

	//начало игры
	do {

		//ход игрока
		counter = PlayerTurn(A, length, counter);

		//вывод поля на экран
		DisplayField(A, length, counter);

		//проверка на выигрыш по горизонтали
		CheckHorizontalLine(A, length);

		//по вертикали
		CheckVerticalLine(A, length);

		//по диагонали СЗ-ЮВ
		CheckDiagonalWestEast(A, length);

		//по диагонали CB-ЮЗ
		CheckDiagonalEastWest(A, length);

		//если ничья
		NoWiner(counter);

		//ход компьютера
		counter = ComputerTurn(A, length, counter);

		//вывод поля на экран
		DisplayField(A, length, counter);

		//проверка на выигрыш по горизонтали
		CheckHorizontalLine(A, length);

		//по вертикали
		CheckVerticalLine(A, length);

		//по диагонали СЗ-ЮВ
		CheckDiagonalWestEast(A, length);

		//по диагонали CB-ЮЗ
		CheckDiagonalEastWest(A, length);

	} while (counter < 9);



	return 0;
}

void WhoIsWinner(int SUM1, int SUM2) {
	if (SUM1 == 3)
	{
		cout << "\nComputer won!\nGame is over";
		exit(0);
	}
	if (SUM2 == 3)
	{
		cout << "\nPlayer won!\nGame is over";
		exit(0);
	}
}//Определение победителя
void DisplayField(char A[][3], const int length, int counter) {
	system("cls");
	//cout << "counter = " << counter << "\n";
	cout << " 123\n";
	for (int i = 0; i < length; i++) {
		cout << i + 1;
		for (int j = 0; j < length; j++)
		{
			cout << A[i][j];
		}
		cout << endl;
	}
}
void CheckHorizontalLine(char A[][3], const int length) {
	for (int i = 0; i < length; i++) {
		int SUM1 = 0;
		int SUM2 = 0;
		for (int j = 0; j < length; j++)
		{
			if (A[i][j] == '-')
			{
				SUM1 = SUM1 + 1;
			}

			if (A[i][j] == '+')
			{
				SUM2 = SUM2 + 1;
			}
		}
		if ((SUM1 != 3) || (SUM2 != 3)) WhoIsWinner(SUM1, SUM2);
	}
}
void CheckVerticalLine(char A[][3], const int length) {
	for (int j = 0; j < length; j++) {
		int SUM1 = 0;
		int SUM2 = 0;
		for (int i = 0; i < length; i++)
		{
			if (A[i][j] == '-')
			{
				SUM1 = SUM1 + 1;
			}

			if (A[i][j] == '+')
			{
				SUM2 = SUM2 + 1;
			}
		}
		if ((SUM1 != 3) || (SUM2 != 3)) WhoIsWinner(SUM1, SUM2);
	}
}
void CheckDiagonalWestEast(char A[][3], const int length) {
	int SUM1 = 0;
	int SUM2 = 0;
	int j = 0;
	for (int i = 0; i < length; i++)
	{
		if (A[i][j] == '-')
		{
			SUM1 = SUM1 + 1;
		}

		if (A[i][j] == '+')
		{
			SUM2 = SUM2 + 1;
		}
		j++;
	}
	if ((SUM1 != 3) || (SUM2 != 3)) WhoIsWinner(SUM1, SUM2);
}
void CheckDiagonalEastWest(char A[][3], const int length) {
	int SUM1 = 0;
	int SUM2 = 0;
	int j = 2;
	for (int i = 0; i < length; i++)
	{
		if (A[i][j] == '-')
		{
			SUM1 = SUM1 + 1;
		}

		if (A[i][j] == '+')
		{
			SUM2 = SUM2 + 1;
		}
		j--;
	}
	if ((SUM1 != 3) || (SUM2 != 3)) WhoIsWinner(SUM1, SUM2);
}

int PlayerTurn(char A[][3], const int length, int counter) {
	bool shoot = false;
	int N, M;
	do {
		cout << "\nPlease enter a field...";
		cin >> N;
		cin >> M;
		if ((N > 0) && (N < 4) &&
			(M > 0) && (M < 4) &&
			(A[N - 1][M - 1] != '-') &&
			(A[N - 1][M - 1] != '+'))
		{
			A[N - 1][M - 1] = '+';
			shoot = true;
			counter++;
		}
	} while (shoot != true);
	return counter;
}
void NoWiner(int counter) {
	if (counter == 9) {
		cout << "\n NO WINNER. GAME OVER \n";
		system("pause");
		exit(0);
	}

};
int ComputerTurn(char A[][3], const int length, int counter) {

	bool shoot = false;
	do {
		int N = rand() % 3;
		int M = rand() % 3;
		//cout << N << M << endl;


		//проверка
		if ((A[N][M] != '+') &&
			(A[N][M] != '-')) {
			A[N][M] = '-';
			shoot = true;
			counter++;
		}
	} while (shoot != true);

	return counter;
}



