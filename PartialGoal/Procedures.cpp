#include "procedures.h"
#include <string>
#include <stack>
#include <stdexcept>
#include <sstream>

using std::string;
using std::stack;

// Перетворює прості арифметичні вирази з інфіксної форми до постфіксної
// операндами можуть бути одноцифрові натуральні числа
// операціями - бінарні + - * /
void PostfixLight()
{
	stack<char> stack;
	string source, dest;
	cout << "Операндами можуть бути одноцифрові цілі, операції - бінарні.\n"
		<< "Задайте арифметичний вираз без дужок: ";
	getline(cin, source);
	if (source.empty()) return;
	for (int i = 0; i < source.length(); ++i)
	{ // аналізуємо кожну літеру заданого рядка
		switch (source[i])
		{
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			dest += source[i]; break; // операнд -> результуючий рядок
		case '+': case '-':
			while (!stack.empty())    // усі знаки операцій зі стека -> в dest
			{
				dest += stack.top();
				stack.pop();
			}
			stack.push(source[i]); break; // новий знак в стек
		case '*': case '/':     // зі стека в dest операції рівного пріоритету
			while (!stack.empty() && (stack.top() == '*' || stack.top() == '/'))
			{
				dest += stack.top();
				stack.pop();
			}
			stack.push(source[i]); break; // новий знак в стек
		default:
			throw std::invalid_argument("Illegal symbol in the source");
		}
	}
	while (!stack.empty()) // решта операцій до результуючого рядка
	{
		dest += stack.top();
		stack.pop();
	}
	cout << "Польський запис: " << dest << '\n';
}

// обчислення пріоритету операцій
int Priority(char c)
{
	switch (c)
	{
	case '*': case '/': return 2;
	case '+': case '-': return 1;
	case '#': case '(': case ')': return 0;
	default: return 10;
	}
}

string ToPostfix(string source)
{
	string dest;
	stack<char> stack; stack.push('#');
	for (char c : source)
	{   // аналізуємо кожну літеру заданого рядка
		if (isdigit(c))
			dest += c; // операнд -> результуючий рядок
		else
		{
			int p = Priority(c);
			switch (c)
			{
			case '+': case '-': case '*': case '/':
				while (Priority(stack.top()) >= p)
				{            // старші операції полишають стек
					dest += stack.top();
					stack.pop();
				}
				stack.push(c); break;
			case '(':        // початок пріоритетної частини
				stack.push(c); break;
			case ')':        // закінчення пріоритетної частини
				while (stack.top() != '(')
				{
					dest += stack.top();
					stack.pop();
				}            // дужку теж треба забрати
				stack.pop(); break;
			} /* switch */
		} /* if */
	} /* for */
	while (stack.top() != '#')
	{   // решта операцій до результуючого рядка
		dest += stack.top();
		stack.pop();
	}
	return dest;
}
// Перетворює арифметичні вирази з дужками. Операнди одноцифрові.
void Postfix()
{
	string source;
	cout << "Операндами можуть бути одноцифрові цілі, операції - бінарні.\n"
		<< "Задайте арифметичний вираз з довільним вкладенням дужок:\n";
	getline(cin, source);
	if (source.empty()) return;
	cout << "Польський запис: " << ToPostfix(source) << '\n';
}

// перевіряє, чи належить символ до запису числа
bool IsDigital(char c)
{
	return '0' <= c && c <= '9' || toupper(c) == 'E' || c == '.';
}

// опрацьовує знаки унарних операцій: вилучає +, перетворює - на бінарний
void Prepare(string& source)
{
	// перетворимо знаки унарних операцій у рядку source
	//  спочатку - на початку рядка:
	if (source[0] == '-') source = '0' + source;  // мінус став бінарним
	else if (source[0] == '+') source.erase(0, 1);// вилучили унарний плюс
	//  потім - після кожної дужки (; перетворення такі ж
	int i = 1;           // індекс літери, яку аналізуємо
	while (i < source.length())
	{
		if (source[i - 1] == '(')
		{
			if (source[i] == '-')
			{
				source.insert(i, "0"); ++i;
			}
			else if (source[i] == '+') source.erase(i, 1);
		}
		++i;
	}
}

// Перетворює арифметичні вирази з багатоцифровими операндами
string Translate(string source)
{
	Prepare(source);
	// приготували місце для перетвореного виразу
	std::ostringstream dest;
	stack<char> stack; stack.push('#');
	// аналізуємо кожну літеру заданого рядка
	for (int i = 0; i < source.length(); )
	{
		if (IsDigital(source[i]))
		{
			while (i < source.length() && IsDigital(source[i])) dest << source[i++];
			dest << ' ';  // завершили операнд
		}
		else
		{
			char c = source[i];
			switch (c)
			{
			case '+': case '-': case '*': case '/':
			{
				int p = Priority(c);
				while (Priority(stack.top()) >= p)
				{            // старші операції полишають стек
					dest << stack.top();
					stack.pop();
				}
				stack.push(c); break;
			}
			case '(':        // початок пріоритетної частини
				stack.push(c); break;
			case ')':        // закінчення пріоритетної частини
				while (stack.top() != '(')
				{
					dest << stack.top();
					stack.pop();
				}            // дужку теж треба забрати
				stack.pop(); break;
			default:
				throw std::invalid_argument("Illegal symbol in the source");
			} /* switch */
			++i;
		} /* if */
	} /* for */
	while (stack.top() != '#')
	{   // решта операцій до результуючого рядка
		dest << stack.top();
		stack.pop();
	}
	return dest.str();
}

void PostfixExt()
{
	string source;
	cout << "Задайте арифметичний вираз: ";
	getline(cin, source);
	if (source.empty()) return;
	try
	{
		cout << "Польський запис: " << Translate(source) << '\n';
	}
	catch (std::invalid_argument& exc)
	{
		cout << exc.what() << '\n';
	}
}

int Calculate(string expr)
{
	stack<int> stack;
	for (char c: expr)
	{
		if (isdigit(c))
		{
			stack.push(c - '0');
		}
		else
		{
			int right = stack.top(); stack.pop();
			int left = stack.top(); stack.pop();
			switch (c)
			{
			case '+': stack.push(left + right); break;
			case '-': stack.push(left - right); break;
			case '*': stack.push(left * right); break;
			case '/': stack.push(left / right); break;
			}
		}
	}
	return stack.top();
}

// Перетворює і обчислює арифметичні вирази з дужками. Операнди одноцифрові.
void TranslateAndCalculate()
{
	string source, dest;
	cout << "Операндами можуть бути одноцифрові цілі, операції - бінарні.\n"
		<< "Задайте арифметичний вираз з довільним вкладенням дужок:\n";
	getline(cin, source);
	if (source.empty()) return;
	try
	{
		dest = ToPostfix(source);
		cout << "Польський запис: " << dest << '\n';
		cout << "Значення = " << Calculate(dest) << "\n\n";
	}
	catch (std::invalid_argument& exc)
	{
		cout << exc.what() << '\n';
	}
}

