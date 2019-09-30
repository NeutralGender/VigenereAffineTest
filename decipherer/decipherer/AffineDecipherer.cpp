#include "decipherer.h"
#include "AffineDecipherer.h"


// EUCLIDEAN ALFORTHM FOR FINDING GREATEST COMMON DIVISOR
int gcd(int a, int b)
{
	if (b == 0) // finish
		return (a);
	return gcd(b, a % b);
}

//FUNCTION FOR FINDING (a)^(-1) mod ( alphabet.size() ), BACK NUMBER IN MULTIPLICATIVE GROUP BY MODULE OF ALPHABET SIZE
int AffineDecipherer::back_number(int number, int mod)
{
	//mod = number * q + r; where r - rest of out division mod on number, q - int coefficient( mod div number )
	// this algorithm describes finding back number by "tables of coefficent(mod div number)"

	int result = 0; // current element of table, the last element will be result
	int next = 1; // on a condition of "table" method
	int current_rest = mod;
	int next_rest = number;

	try
	{
		if (gcd(number, mod) != 1 || number <= 0 || mod <= 0)
			throw exception(" Sorry, you input INCORRECT password! ");

		while (next_rest != 0)
		{
			//if (mod <= 0 || number <= 0)
				//throw exception("Division by zero\n");

			int q = (current_rest) / (next_rest);// int coefficient

			int nnT = result - q * next;// next+1
			result = next;// now result = next => in table result moved one cell to the right
			next = nnT;// now next = next+1 => in table next moved one cell to the right

			int nnR = current_rest - q * next_rest;
			current_rest = next_rest;
			next_rest = nnR;
		}

	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
	}

	if (next_rest > 1)
	{
		cout << "NO_SOLUTION\n";
		return -1;
	}
	if (result < 0)
		result += mod;

	return result;
}


string AffineDecipherer::decipher(const string& cipher_text,string &result)//result is our plaintext sum
{
	// aff cipher: y = ax + b (mod alphabet);
	// aff decipher: x = (y - b)*(back_number(a,alphabet)) (mod alphabet);

	std::lock_guard<std::mutex> lock(mtx);

	int x = 0; // index of plain_text element after deciphering
	int back_a = back_number(a, alphabet.size());// finding back_number by EUCLIDEAN algorithm
	string plain_text = "";// result string for plain text

	for (size_t i = 0; i < cipher_text.size(); i++)
	{
		x = (char_number(cipher_text[i]) - b) * back_a;

		if (x < 0)
		{
			while (x < 0)
				x += alphabet.size();
		}

		plain_text += alphabet[x % alphabet.size()];
	}
	//cout << plain_text << endl;
	result += plain_text;

	return (result);
}


//FUNCTION FRO DIVISION STRING(STR) INTO PARTS TO DIFFERENT THREADS
string AffineDecipherer::string_div(const string& str)
{
	thread thread[10]; // pull of threads
	//thread thd;

	int thread_count = 0;// count of thread we use
	int part_count = 10; // var for STR_PARTS count;
	size_t part_len = str.size() / part_count; // len of each STR_PARTS, except last
	string str_parts = ""; // string for str parts; to different threads
	string result = "";//plaintext sum after decrypting

	int start_index = 0; // index for synchronize str_parts and password, because Vigenere is depends on password index

	for (size_t i = 0; i < str.size(); i++)
	{
		if (i > NULL && (i % part_len) == NULL)
		{
			str_parts += str[i];

			thread[thread_count] = std::thread(&AffineDecipherer::decipher, this, str_parts,ref(result));

			start_index = i + 1;// new start_index for synchronyze
			thread_count++;
			str_parts.clear();// clear string before new thread
		}
		else if ((str.size() - i) < part_len) // check the last str_parts and str rest, because str.size() not always div totally,
											  // so we add last (str.size()-i) char to last str_parts;
		{
			while (i != str.size()) 
			{
				str_parts += str[i];
				i++;
			}
			thread[thread_count] = std::thread(&AffineDecipherer::decipher, this, str_parts,ref(result));
			thread_count++;
			//cout << str_parts << endl;
		}
		else
			str_parts += str[i];
	}
	for (int i = 0; i < thread_count; i++) // join each thread from pull
		thread[i].join();

	return result;
}