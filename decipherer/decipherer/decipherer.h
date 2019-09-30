#pragma once
#include <iostream>

using namespace std;

class Decipherer
{
public:
	string input(const string& path);
	int char_number(const char&);
	void output(const string&, const string&);

protected:
	const string alphabet = "abcdefghijklmnopqrstuvwxyz";

};