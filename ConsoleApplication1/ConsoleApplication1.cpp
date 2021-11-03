#include <locale>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <Windows.h>

using namespace std;

const int NotUsed = system("color F0");
HANDLE hConsole;

enum ConsoleColor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

void SetColor(ConsoleColor text, ConsoleColor background)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

int	GetDataFromFile(ifstream& file, int& n, float*& y)
{
	if (!file.is_open())
	{
		cerr << "\tНевозможно открыть файл\n\n\n\n" << endl;
		system("pause");
		exit(1);
	}
	else {
		file >> n;
		if (n < 3) return 1;
		y = new float[n];
		for (int i = 0; i < n; i++)
			y[i] = 0;
		int counter = 0;
		while (!file.eof()) {
			file >> y[counter++];
			if (counter > n) return 2;
		}
		if (counter < n) return 2;
	}
	return 0;
}

void SetDataToFile(ofstream& file, int n, float* yy)
{
	file << "IER = 0 - нет ошибки\nРезультат:" << endl;
	for (int i = 0; i < 3 * n - 2; i++)
		file << yy[i] << endl;
}

float CalcFirstPoint(float f1, float f2, float f3) {
	return 5. / 9 * f1 + 5. / 9 * f2 - 1. / 9 * f3;
}

float CalcSecondPoint(float f1, float f2, float f3) {
	return 2. / 9 * f1 + 8. / 9 * f2 - 1. / 9 * f3;
}

float CalcPredLastPoint(float f1, float f2, float f3) {
	return -1. / 9 * f1 + 8. / 9 * f2 + 2. / 9 * f3;
}

float CalcLastPoint(float f1, float f2, float f3) {
	return -1. / 9 * f1 + 5. / 9 * f2 + 5. / 9 * f3;
}

void CalcM(float* y, float* yy, int n) {
	int j = 1;
	for (int i = 0; j != n - 1; i += 3) {
		if (j != n - 2) {
			float* temp = new float[2];
			temp[0] = CalcFirstPoint(y[j - 1], y[j], y[j + 1]);
			temp[1] = CalcSecondPoint(y[j - 1], y[j], y[j + 1]);
			yy[i] = y[j - 1];
			yy[i + 1] = temp[0];
			yy[i + 2] = temp[1];
			j++;
		}
		else {
			float* temp = new float[4];
			temp[0] = CalcFirstPoint(y[j - 1], y[j], y[j + 1]);
			temp[1] = CalcSecondPoint(y[j - 1], y[j], y[j + 1]);
			temp[2] = CalcPredLastPoint(y[j - 1], y[j], y[j + 1]);
			temp[3] = CalcLastPoint(y[j - 1], y[j], y[j + 1]);
			yy[i] = y[j - 1];
			yy[i + 1] = temp[0];
			yy[i + 2] = temp[1];
			yy[i + 3] = y[j];
			yy[i + 4] = temp[2];
			yy[i + 5] = temp[3];
			yy[i + 6] = y[j + 1];
			j++;
		}
	}
}

int	main() {

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	int		n;
	float* y;
	float* yy;
	ifstream file_input("in.txt");
	ofstream file_output("out.txt", ios::app);

	setlocale(LC_ALL, "RUS");

	cout << "\n\n\n\tЛабораторная работа №1:\n\n\tУплотнение в три раза таблицы значений функции с равноотстоящими значениями аргумента.\n";
	cout << "\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\tПроверила: Шабунина З.А.\n\t\t\t\t\t\t\tПодготовили: Сафонов Н.С. и Черникова М.И.\n\n\n\n\n\n\n\n\n";

	int IER = GetDataFromFile(file_input, n, y);
	if (IER == 0)
	{
		yy = new float[3 * n - 2]; // инициализация массива уплотнённых значений
		CalcM(y, yy, n); // функция вычисления уплотнённых значений

		SetDataToFile(file_output, n, yy); // запись результатов в выходной файл

		delete[]yy; // освобождение памяти
		delete[]y;

		SetColor(Green, White);
		cout << "\tIER = 0 - нет ошибки\n\n\n";
	}
	else if (IER == 1) {
		file_output << "IER = 1 - уплотнённый вектор не может быть построен из-за малости n (n < 3)" << endl;
		SetColor(Red, White);
		cout << "\tIER = 1 - уплотнённый вектор не может быть построен из-за малости n (n < 3)\n\n\n\n" << endl;
	}
	else {
		file_output << "IER = 2 - количество значений y не совпадает с n" << endl;
		SetColor(Red, White);
		cout << "\tIER = 2 - количество значений y не совпадает с n\n\n\n" << endl;
	}
	SetColor(Black, White);
	system("pause");
	return 0;
}