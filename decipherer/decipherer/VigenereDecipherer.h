#pragma once

#include <iostream>
#include <mutex>

#include "decipherer.h"

using namespace std;

class VigenereDecipherer : public Decipherer
{
public:
	string decipher(const string&,const int,string&);
	void SetPassword(const string& user_pass) { this->password = user_pass; }
	string string_div(const string&);
	mutex mtx;

private:
	string password;

};