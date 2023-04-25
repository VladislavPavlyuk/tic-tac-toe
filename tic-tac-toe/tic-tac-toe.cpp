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

// Receiver � �������� ���������� �������. ������ �� ����������� ����������� � ���, ��� ���� ������������ �������.
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

// ICommand - ���������, �������������� �������.
class ICommand abstract
{
public:
	virtual void Execute() abstract;
	virtual void UnExecute() abstract;
};

// ConcreteCommand � ���������� �������.
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

// Invoker � ��� ��������� ���������� �������.
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
	* ����� �������, ���������, ������������ ������, ������ �� ����� � ����������,
	* ������� � ����� ��������� �������.
	* ����� ����, ���� ��� ����������� ��������� �����-�� ����� �������,
	* �� ����� ������ ������������ ������ �� ������������ ������ ICommand �
	* ����������� ��� ������ Execute � Undo.
	*/
}
void Invoker(Computer* computer, ICommand* command)
{
	computer->Turn(command);
	/*
	* ����� �������, ���������, ������������ ������, ������ �� ����� � ����������,
	* ������� � ����� ��������� �������.
	* ����� ����, ���� ��� ����������� ��������� �����-�� ����� �������,
	* �� ����� ������ ������������ ������ �� ������������ ������ ICommand �
	* ����������� ��� ������ Execute � Undo.
	*/
}

int main()
{
	Player* player = new Player(); // Invoker � ��� ��������� ���������� �������
	Game* game = new Game(); // Receiver � �������� ���������� �������

	ICommand* command = new GameCommand(game, '+', 100); // ���������� �������
	Invoker(player, command);

	command = new GameCommand(game, '-', 50); // ���������� �������
	Invoker(player, command);

	command = new GameCommand(game, '*', 10); // ���������� �������
	Invoker(player, command);

	command = new GameCommand(game, '/', 2); // ���������� �������
	Invoker(player, command);

	player->Undo(4);
	player->Redo(2);

	command = new GameCommand(game, '*', 3); // ���������� �������
	Invoker(player, command);

	delete game;
	delete player;

	system("pause");

	srand(time(NULL));
	int N, M, SUM1, SUM2;
	const int  length = 3;
	int counter = 0;
	bool shoot = false;

	//������ �������� ����
	char A[length][length];

	//��������� ������ ���� ������� ����������
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++)
		{
			A[i][j] = '.';
		}
	}

	//����� ���� �� �����
	DisplayField(A, length, counter);

	//������ ����
	do {

		//��� ������
		counter = PlayerTurn(A, length, counter);

		//����� ���� �� �����
		DisplayField(A, length, counter);

		//�������� �� ������� �� �����������
		CheckHorizontalLine(A, length);

		//�� ���������
		CheckVerticalLine(A, length);

		//�� ��������� ��-��
		CheckDiagonalWestEast(A, length);

		//�� ��������� CB-��
		CheckDiagonalEastWest(A, length);

		//���� �����
		NoWiner(counter);

		//��� ����������
		counter = ComputerTurn(A, length, counter);

		//����� ���� �� �����
		DisplayField(A, length, counter);

		//�������� �� ������� �� �����������
		CheckHorizontalLine(A, length);

		//�� ���������
		CheckVerticalLine(A, length);

		//�� ��������� ��-��
		CheckDiagonalWestEast(A, length);

		//�� ��������� CB-��
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
}//����������� ����������
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


		//��������
		if ((A[N][M] != '+') &&
			(A[N][M] != '-')) {
			A[N][M] = '-';
			shoot = true;
			counter++;
		}
	} while (shoot != true);

	return counter;
}



