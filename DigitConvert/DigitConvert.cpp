#include <iostream>
#include <string>
#include <regex>
#include <stack>
#include <math.h>
#include <stdint.h>

using namespace std;

string units[] = { "","один ","два ","три ","четыре ","пять ","шесть ","семь ","восемь ","девять " };
string tens[] = { "десять ","одиннадцать ","двенадцать ","тринадцать ","четырнадцать ","пятнадцать ","шестнадцать ","семнадцать ","восемнадцать ","девятнадцать " };
string scores[] = { "","десять ","двадцать ","тридцать ","сорок ","пятьдесят ","шестьдесят ","семьдесят ","восемьдесят ","девяносто " };
string hundreds[] = { "","сто ","двести ","триста ","четыреста ","пятьсот ","шестьсот ","семьсот ","восемьсот ","девятьсот " };
string thousand[] = { "тысяча ","тысячи ","тысяч " };
string million[] = { "миллион ","миллиона ","миллионов " };
string billion[] = { "миллиард ","миллиарда " };


int inputNumber()
{
	string n;
	regex regex_pattern("-?[0-9]+");
	cout << "Введите число: ";
	while (cin >> n) {
		if (!regex_match(n, regex_pattern)) {
			cout << "Ошибка, введите ещё раз: ";
		}
		else if ((stol(n) < INT32_MIN) || (stol(n) > INT32_MAX)) {
			cout << "Введите другое число: ";
		}
		else {
			break;
		}

	}

	return stoi(n);
}

int ChooseForm(int n) { //выбор правильной словоформы для разряда: тысяча, тысяч и т.д. 
	int index = 2;
	if (n == 1) {
		index = 0;
	}
	else if ((n > 1) && (n < 5)) {
		index = 1;
	}
	return index;
}

string DefineRank(int r, int n) { //выбор разряда: тысячи, миллионы, и т.д.
	string rank = "";
	switch (r) {
	case 2:
		units[1] = "одна "; //для согласования с тысячей
		units[2] = "двe ";
		rank = thousand[ChooseForm(n)];
		break;
	case 3:
		rank = million[ChooseForm(n)];
		break;
	case 4:
		rank = billion[ChooseForm(n)];
		break;
	default:
		units[1] = "один ";
		units[2] = "два ";
		break;
	}
	return rank;
}

string ConvertNumber(int n) { //перевод в число прописью
	string numeral = "";
	int hundreds_number = n / 100; //число сотен
	int scores_number = n / 10 % 10; //число десятков
	int units_number = n % 10; //число единиц
	switch (scores_number) {
	case 1:
		numeral = hundreds[hundreds_number] + tens[units_number];
		break;
	case 0:
		if (hundreds_number == 0) {
			numeral = units[units_number];
		}
		else {
			numeral = hundreds[hundreds_number] + units[units_number];
		}
		break;
	default:
		numeral = hundreds[hundreds_number] + scores[scores_number] + units[units_number];
	}
	return numeral;
}
string TranslateNumber(int n) { //перевод в прописную форму числа целиком
	stack <int> nums;
	while (n != 0) { //запись числа в стек тройками-разрядами
		int k = n % 1000;
		n = n / 1000;
		nums.push(k);
	}
	string output = ""; //итоговая строка вывода
	while (!nums.empty()) {
		int triple = nums.top();
		if (triple != 0) { //перевод каждой тройки в прописную форму с указанием разряда и добавление к строке
			output += ConvertNumber(triple) + DefineRank(nums.size(), triple);
		}
		nums.pop();
	}
	return output;
}

int main() {
	cout << TranslateNumber(inputNumber());
	return 0;
}
