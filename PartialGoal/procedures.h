#pragma once
#include <iostream>

using std::cin;
using std::cout;

// ���������� ����� ����������� ������ � �������� ����� �� ����������
// ���������� ������ ���� ���������� ���������� �����
// ���������� - ������ + - * /
void PostfixLight();

// ���������� ����������� ������ � �������. �������� ����������.
void Postfix();

// ���������� ����������� ������ � ��������������� ����������
void PostfixExt();

// ���������� � �������� ����������� ������ � �������. �������� ����������.
void TranslateAndCalculate();