#include <iostream>
#include <string>
#include <stack>
#include <math.h>
#include <stdint.h>
#include <cassert>
#include <stdexcept>


using namespace std;

// массивы числительных
string units[] = { "","один","два","три","четыре","пять","шесть","семь","восемь","девять" };
string tens[] = { "десять","одиннадцать","двенадцать","тринадцать","четырнадцать","пятнадцать","шестнадцать","семнадцать","восемнадцать","девятнадцать" };
string scores[] = { "","десять","двадцать","тридцать","сорок","пятьдесят","шестьдесят","семьдесят","восемьдесят","девяносто" };
string hundreds[] = { "","сто","двести","триста","четыреста","пятьсот","шестьсот","семьсот","восемьсот","девятьсот" };
string thousand[] = { "тысяча","тысячи","тысяч" };
string million[] = { "миллион","миллиона","миллионов" };
string billion[] = { "миллиард","миллиарда" };


// проверка на наличие не-цифр или минуса-плюса посреди строки
bool CheckDigit(const string &s) {
	int size = s.length();
	for (int i = 0; i < size; i++) {
		if ((s[i] < '0' || s[i] > '9') && !(i == 0 && (s[i] == '+' || s[i] == '-'))) {
			return false;
		}
	}
	return true;
}

// проверка входного числа
int InputNumber() {
	string n;
	cout << "Введите число: ";
	while (cin >> n) {
		try {
			if (!CheckDigit(n)) {
				cout << "Ошибка, введите ещё раз: ";
			}
			else if ((stoll(n) <= INT32_MIN) || (stoll(n) >= INT32_MAX)) {
				cout << "Введите другое число: ";
			}
			else {
				break;
			}
		}
		catch (out_of_range& oor) {
			cout << "Введите другое число: ";
		}
	}
	return stoi(n);
}

// выбор правильной словоформы для разряда: тысяча, тысяч и т.д.
int ChooseForm(int n) {
	int index = 2;
	int digits = 1;
	int num = n;
	// определение количества знаков числа
	while ((num /= 10) > 0) digits++;
	switch (digits) {
	case 1:
		if (n == 1) {
			index = 0;
		}
		else if ((n > 1) && (n < 5)) {
			index = 1;
		}
		break;
	case 2:
		if (n >= 20) {
			if (n % 10 == 1) {
				index = 0;
			}
			else if ((n % 10 > 1) && (n % 10 < 5)) {
				index = 1;
			}
		}
		break;
	case 3:
		if ((n % 100 >= 1) && (n % 100 < 20)) {
			if (n % 100 == 1) {
				index = 0;
			}
			else if ((n % 100 > 1) && (n % 100 < 5)) {
				index = 1;
			}
		}
		else {
			if (n % 10 == 1) {
				index = 0;
			}
			else if ((n % 10 > 1) && (n % 10 < 5)) {
				index = 1;
			}
		}
		break;
	}
	return index;
}

// выбор разряда: тысячи, миллионы, и т.д.
string DefineRank(int r, int n) { 
	string rank = "";
	switch (r) {
	case 2:
		units[1] = "одна";  // для согласования с тысячей
		units[2] = "двe";
		rank = thousand[ChooseForm(n)];
		break;
	case 3:
		rank = million[ChooseForm(n)];
		break;
	case 4:
		rank = billion[ChooseForm(n)];
		break;
	default:
		units[1] = "один";
		units[2] = "два";
		break;
	}
	return rank;
}

// перевод в число прописью
string ConvertNumber(int n) { 
	string numeral = "";
	int hundreds_number = n / 100;  // число сотен
	int scores_number = n / 10 % 10;  // число десятков
	int units_number = n % 10;  // число единиц
	string space = "";
	if (hundreds_number != 0) {
		space = " ";
	}
	switch (scores_number) {
	case 1:
		numeral = hundreds[hundreds_number] + space + tens[units_number];
		break;
	case 0:
		if (hundreds_number == 0) {
			numeral = units[units_number];
		}
		else {
			numeral = hundreds[hundreds_number] + space + units[units_number];
		}
		break;
	default:
		if (units_number == 0) {
			numeral = hundreds[hundreds_number] + space + scores[scores_number];
		}
		else {
			numeral = hundreds[hundreds_number] + space + scores[scores_number] + " " + units[units_number];
		}
	}
	return numeral;
}
// перевод в прописную форму числа целиком
string TranslateNumber(int n) { 
	stack <int> nums;
	string output = "";  // итоговая строка вывода
	string minus = "";  // для отрицательных чисел
	if (n == 0) {
		output = "ноль";
	}
	if (n < 0) {
		minus = "минус ";
		n = abs(n);
	}
	if (n == INT32_MIN) {
		return "минус два миллиарда сто сорок семь миллионов четыреста восемьдесят три тысячи шестьсот сорок восемь";
	}
	// запись числа в стек тройками-разрядами
	while (n != 0) {
		int k = n % 1000;
		n = n / 1000;
		nums.push(k);
	}
	
	while (!nums.empty()) {
		string space = " ";
		int triple = nums.top();
		// перевод каждой тройки в прописную форму с указанием разряда и добавление к строке
		if (triple != 0) {
			if (nums.size() < 2) {
				space = "";
			}
			output += ConvertNumber(triple) + space + DefineRank(nums.size(), triple) + space;
		}
		nums.pop();
	}
	return minus + output;
}

void InternalTest() {
	assert(TranslateNumber(0) == "ноль");
	assert(TranslateNumber(-5) == "минус пять");
	assert(TranslateNumber(8) == "восемь");
	assert(TranslateNumber(13) == "тринадцать");
	assert(TranslateNumber(-51) == "минус пятьдесят один");
	assert(TranslateNumber(60) == "шестьдесят");
	assert(TranslateNumber(111) == "сто одиннадцать");
	assert(TranslateNumber(903) == "девятьсот три");
	assert(TranslateNumber(-555) == "минус пятьсот пятьдесят пять");
	assert(TranslateNumber(1362) == "одна тысяча триста шестьдесят два");
	assert(TranslateNumber(-5741) == "минус пять тысяч семьсот сорок один");
	assert(TranslateNumber(4040) == "четыре тысячи сорок");
	assert(TranslateNumber(10125) == "десять тысяч сто двадцать пять");
	assert(TranslateNumber(-43711) == "минус сорок три тысячи семьсот одиннадцать");
	assert(TranslateNumber(31062) == "тридцать одна тысяча шестьдесят два");
	assert(TranslateNumber(-60501) == "минус шестьдесят тысяч пятьсот один");
	assert(TranslateNumber(914358) == "девятьсот четырнадцать тысяч триста пятьдесят восемь");
	assert(TranslateNumber(-606060) == "минус шестьсот шесть тысяч шестьдесят");
	assert(TranslateNumber(7216537) == "семь миллионов двести шестнадцать тысяч пятьсот тридцать семь");
	assert(TranslateNumber(-1456014) == "минус один миллион четыреста пятьдесят шесть тысяч четырнадцать");
	assert(TranslateNumber(1010101) == "один миллион десять тысяч сто один");
	assert(TranslateNumber(23576291) == "двадцать три миллиона пятьсот семьдесят шесть тысяч двести девяносто один");
	assert(TranslateNumber(-60402090) == "минус шестьдесят миллионов четыреста двe тысячи девяносто");
	assert(TranslateNumber(48484848) == "сорок восемь миллионов четыреста восемьдесят четыре тысячи восемьсот сорок восемь");
	assert(TranslateNumber(856438291) == "восемьсот пятьдесят шесть миллионов четыреста тридцать восемь тысяч двести девяносто один");
	assert(TranslateNumber(-151515151) == "минус сто пятьдесят один миллион пятьсот пятнадцать тысяч сто пятьдесят один");
	assert(TranslateNumber(1439438721) == "один миллиард четыреста тридцать девять миллионов четыреста тридцать восемь тысяч семьсот двадцать один");
	assert(TranslateNumber(-1010101010) == "минус один миллиард десять миллионов сто одна тысяча десять");
	assert(TranslateNumber(1232000999) == "один миллиард двести тридцать два миллиона девятьсот девяносто девять");
	assert(TranslateNumber(INT32_MIN) == "минус два миллиарда сто сорок семь миллионов четыреста восемьдесят три тысячи шестьсот сорок восемь");
	assert(TranslateNumber(INT32_MAX) == "два миллиарда сто сорок семь миллионов четыреста восемьдесят три тысячи шестьсот сорок семь");
	assert(CheckDigit("123") == true);
	assert(CheckDigit("-123") == true);
	assert(CheckDigit("1-23") == false);
	assert(CheckDigit("123fg") == false);
	assert(CheckDigit("12-fg") == false);
	assert(CheckDigit("-12rg") == false);
	assert(CheckDigit("12аир") == false);
	assert(CheckDigit("пик466") == false);
	assert(CheckDigit("34-4перн") == false);
	assert(CheckDigit("-67цуит") == false);
}

int main(int argc, char** argv) {
	if (argc > 1 && (strcmp(argv[1],"--test") == 0)) {
		InternalTest();
		return 0;
	}
	cout << TranslateNumber(InputNumber());
	return 0;
}
