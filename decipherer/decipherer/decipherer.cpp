#include <iostream>
#include <fstream>
#include <exception>
#include <string>

#include "decipherer.h"
#include "AffineDecipherer.h"
#include "VigenereDecipherer.h"

using std::string;
using std::cout;
using std::exception;
using std::endl;

//FUNCTION RETURN INDEX OF CHAR IN ALPHABET
int Decipherer::char_number(const char& c)
{
	int char_index = 0; // symbol index from alphabet
	for (size_t i = 0; i < alphabet.size(); i++)
	{
		if (alphabet[i] == c)
		{
			char_index = i;
			break;
		}
	}

	return (char_index);
}

//FUNCTION FOR CHECK MESSED_UP SYMBOLS: return true if symbols are messed up and false if everything is OK
bool messed_up(const string &cipher,const string &alphabet)
{
	for (size_t i = 0; i < cipher.size(); i++)
	{
		for (size_t j = 0; j < alphabet.size(); j++)
		{
			if (cipher[i] == alphabet[j]) // we have equality, so let`s check nex cipher symbol
				break;

			else if (cipher[i] != alphabet[j] && j == alphabet.size() - 1)//if i equal alphaber size but not one coincedence
				return true;

			else
				continue;
		}

	}
	//if everything is good, and each symbol of cipher is on condition
	return false;
}

//FUNCTION FOR READING INPUT ENCRYPTED FILE 
string Decipherer::input(const string& path)
{
	string cipher_text; // string for file content
	char symbol = '\0'; // char sybol for input.get();

	std::ifstream input(path);

	try
	{
		if (!input.is_open())
			throw exception("Input file not found.");

		while (input.get(symbol))
		{
			cipher_text += symbol;
		}

		if (messed_up(cipher_text, alphabet))//return true if symbols are messed up and false if everything is OK
			throw exception("Output file is empty, wrong symbols in encrypted file!");

		input.close();
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
	}


	return string(cipher_text);
}

// FUNCTION FOR DIGIT NUMBER READUNG FROM ARGV[] FOR a OR b (FOR AFFINE CIPHER)
int int_from_argv(char *str)
{
	string temp;// temp string val for reading sequence of digit number from input argument (char *str)
	int int_a = 0; // return variable with digital number
	for (size_t i = 0; i < strlen(str); i++)
	{
		if (isdigit(str[i]))// check if i-element is digit
		{
			while (isdigit(str[i])) // reading sequence of str[i] while str[i] is digit number
			{
				temp += str[i];
				i++;
			}

			int_a = atoi(temp.c_str()); // convert temp string to int
			break;
		}
	}

	return (int_a);
}

//FUNCTION WRITE RESULT OF PROGRAM TO FILE( WHITCH USER WRITE IN CONSOLE )
void Decipherer::output(const string& path, const string& plaintext)
{
	ofstream out(path, ios_base::out);

	try
	{
		if (!out.is_open())
			throw exception("File not found\n");

		out << plaintext;
	}
	catch (const exception& e)
	{
		cout << e.what();
	}

	out.close();
}

//argc - count of input parameters; argv - pointer on string pull
//must console input: ./decipherer input-file output-file cipher[vig/aff] parameters[vig_key/aff_a_b}
int main(int argc,char* argv[])
{

	try 
	{
		if (argc < 5) // if incorrect argument count
		{
			throw std::exception("decipherer input_file output_file cipher[vig / aff] parameters[vig_key / aff_a_b]\n");
		}

		// BLOCK FOR VIGENERE CIPHER
		if (strcmp(argv[3], "vig") == 0 && argc == 5) // check if user chosed Vigenere cipher, so argument count must be 5
		{
			VigenereDecipherer vig;
			string cipher = "";

			vig.SetPassword(argv[4]);

			// read from file
			cipher = vig.input(argv[1]);

			// decipher cipher text using threads
			string plaintext = vig.string_div(cipher);

			// output to file ( user pointed in console filename )
			vig.output(argv[2], plaintext);
			
			return 0;
			
		}
		// BLOCK FOR AFFINE CIPHER
		else if(strcmp(argv[3], "aff") == 0 && argc == 6)// check if user choosed Affine cipher for decrypting, so count must be 6(on a condition)
		{
			AffineDecipherer aff;

			// check if a and b is dec number, else error;
			int a = int_from_argv(argv[4]);
			int b = int_from_argv(argv[5]);

			// install keys for Affine Cipher: a and b;
			aff.Set_a(a);
			aff.Set_b(b);

			// read from file
			string cipher = "";
			cipher = aff.input(argv[1]);

			// decipher cipher text using threads
			string plaintext = aff.string_div(cipher);

			// output to file ( user pointed in console filename )
			aff.output(argv[2], plaintext);

			return 0;

		}
		else // else user write another string in arv[4] that not on condition
			throw std::exception("decipherer input_file output_file cipher[vig / aff] parameters[vig_key / aff_a_b]\n");
			
	}
	catch (const std::exception &e)
	{
		cout << e.what() << std::endl;
	}

	return 0;
}