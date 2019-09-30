#pragma once
#include <iostream>
#include <mutex>

#include "decipherer.h"

using namespace std;

class AffineDecipherer : public Decipherer
{
public:
	AffineDecipherer() : a(0), b(0) {}
	void Set_a(int user_a) { this->a = user_a; }
	void Set_b(int user_b) { this->b = user_b; }
	string decipher(const string&,string&);
	int back_number(int, int);
	string string_div(const string& str);
	mutex mtx;

private:
	int a;
	int b;

};