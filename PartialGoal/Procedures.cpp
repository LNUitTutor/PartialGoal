#include "procedures.h"
#include <string>
#include <stack>
#include <stdexcept>
#include <sstream>

using std::string;
using std::stack;

// ���������� ����� ���������� ������ � �������� ����� �� ����������
// ���������� ������ ���� ���������� ��������� �����
// ���������� - ����� + - * /
void PostfixLight()
{
	stack<char> stack;
	string source, dest;
	cout << "���������� ������ ���� ���������� ���, �������� - �����.\n"
		<< "������� ������������ ����� ��� �����: ";
	getline(cin, source);
	if (source.empty()) return;
	for (int i = 0; i < source.length(); ++i)
	{ // �������� ����� ����� �������� �����
		switch (source[i])
		{
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			dest += source[i]; break; // ������� -> ������������ �����
		case '+': case '-':
			while (!stack.empty())    // �� ����� �������� � ����� -> � dest
			{
				dest += stack.top();
				stack.pop();
			}
			stack.push(source[i]); break; // ����� ���� � ����
		case '*': case '/':     // � ����� � dest �������� ������ ���������
			while (!stack.empty() && (stack.top() == '*' || stack.top() == '/'))
			{
				dest += stack.top();
				stack.pop();
			}
			stack.push(source[i]); break; // ����� ���� � ����
		default:
			throw std::invalid_argument("Illegal symbol in the source");
		}
	}
	while (!stack.empty()) // ����� �������� �� ������������� �����
	{
		dest += stack.top();
		stack.pop();
	}
	cout << "��������� �����: " << dest << '\n';
}

// ���������� ��������� ��������
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
	{   // �������� ����� ����� �������� �����
		if (isdigit(c))
			dest += c; // ������� -> ������������ �����
		else
		{
			int p = Priority(c);
			switch (c)
			{
			case '+': case '-': case '*': case '/':
				while (Priority(stack.top()) >= p)
				{            // ������ �������� ��������� ����
					dest += stack.top();
					stack.pop();
				}
				stack.push(c); break;
			case '(':        // ������� ���������� �������
				stack.push(c); break;
			case ')':        // ��������� ���������� �������
				while (stack.top() != '(')
				{
					dest += stack.top();
					stack.pop();
				}            // ����� ��� ����� �������
				stack.pop(); break;
			} /* switch */
		} /* if */
	} /* for */
	while (stack.top() != '#')
	{   // ����� �������� �� ������������� �����
		dest += stack.top();
		stack.pop();
	}
	return dest;
}
// ���������� ���������� ������ � �������. �������� ����������.
void Postfix()
{
	string source;
	cout << "���������� ������ ���� ���������� ���, �������� - �����.\n"
		<< "������� ������������ ����� � �������� ���������� �����:\n";
	getline(cin, source);
	if (source.empty()) return;
	cout << "��������� �����: " << ToPostfix(source) << '\n';
}

// ��������, �� �������� ������ �� ������ �����
bool IsDigital(char c)
{
	return '0' <= c && c <= '9' || toupper(c) == 'E' || c == '.';
}

// ��������� ����� ������� ��������: ������ +, ���������� - �� �������
void Prepare(string& source)
{
	// ����������� ����� ������� �������� � ����� source
	//  �������� - �� ������� �����:
	if (source[0] == '-') source = '0' + source;  // ���� ���� �������
	else if (source[0] == '+') source.erase(0, 1);// �������� ������� ����
	//  ���� - ���� ����� ����� (; ������������ ��� �
	int i = 1;           // ������ �����, ��� ��������
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

// ���������� ���������� ������ � ��������������� ����������
string Translate(string source)
{
	Prepare(source);
	// ����������� ���� ��� ������������� ������
	std::ostringstream dest;
	stack<char> stack; stack.push('#');
	// �������� ����� ����� �������� �����
	for (int i = 0; i < source.length(); )
	{
		if (IsDigital(source[i]))
		{
			while (i < source.length() && IsDigital(source[i])) dest << source[i++];
			dest << ' ';  // ��������� �������
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
				{            // ������ �������� ��������� ����
					dest << stack.top();
					stack.pop();
				}
				stack.push(c); break;
			}
			case '(':        // ������� ���������� �������
				stack.push(c); break;
			case ')':        // ��������� ���������� �������
				while (stack.top() != '(')
				{
					dest << stack.top();
					stack.pop();
				}            // ����� ��� ����� �������
				stack.pop(); break;
			default:
				throw std::invalid_argument("Illegal symbol in the source");
			} /* switch */
			++i;
		} /* if */
	} /* for */
	while (stack.top() != '#')
	{   // ����� �������� �� ������������� �����
		dest << stack.top();
		stack.pop();
	}
	return dest.str();
}

void PostfixExt()
{
	string source;
	cout << "������� ������������ �����: ";
	getline(cin, source);
	if (source.empty()) return;
	try
	{
		cout << "��������� �����: " << Translate(source) << '\n';
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

// ���������� � �������� ���������� ������ � �������. �������� ����������.
void TranslateAndCalculate()
{
	string source, dest;
	cout << "���������� ������ ���� ���������� ���, �������� - �����.\n"
		<< "������� ������������ ����� � �������� ���������� �����:\n";
	getline(cin, source);
	if (source.empty()) return;
	try
	{
		dest = ToPostfix(source);
		cout << "��������� �����: " << dest << '\n';
		cout << "�������� = " << Calculate(dest) << "\n\n";
	}
	catch (std::invalid_argument& exc)
	{
		cout << exc.what() << '\n';
	}
}

