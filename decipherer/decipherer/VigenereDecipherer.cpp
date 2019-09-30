#include "decipherer.h"
#include "VigenereDecipherer.h"



//FUNCTION WHICH DECIPHER VIGENERE
string VigenereDecipherer::decipher(const string& cipher_text,const int start_index, string &result)
{
	// vig cipher: y = x + k (mod alphabet)
	// vig decipher: x = y - k (mod alphabet)

	std::lock_guard<std::mutex> lock(mtx); // use lock_guard to to synchronize thread

	string plain_text = "";// our result string after deciphering
	int plain = 0; // var for save index of plain text

	for (size_t i = 0; i < cipher_text.size(); i++)
	{
		// vig decipher: plain = cipher - password (mod alphabet)
		plain = (char_number(cipher_text[i]) - char_number(password[(i+start_index) % password.size()]));//return index of both: cipher, password and decipher ciphertext

		if (plain < 0) // during decipherer plain can be < 0(index of cipher below index of password), 
					   // so we must take on the module by alphabet, while palin below zero
		{
			while (plain < 0)
				plain += alphabet.size();
		}

		plain_text += alphabet[plain % alphabet.size()]; 
	}

	result += plain_text; // sum each part of plaintext from each thread

	return (result);
}

//FUNCTION FRO DIVISION STRING(STR) INTO PARTS TO DIFFERENT THREADS
string VigenereDecipherer::string_div(const string& str)// str is cipher text
{
	thread thread[10]; // pull of threads

	int thread_count = 0;// count of thread count(for thread join)
	int part_count = 10; // var for STR_PARTS count;
	size_t part_len = str.size() / part_count; // len of each STR_PARTS, except last
	string str_parts = ""; // string for str parts; to different threads
	string result = ""; // sum of str_parts from threads

	int start_index = 0; // index for synchronize str_parts and password, because Vigenere is depends on password index

	for (size_t i = 0; i < str.size(); i++)
	{
		if (i > NULL && (i % part_len) == NULL)
		{
			str_parts += str[i];

			thread[thread_count] = std::thread(&VigenereDecipherer::decipher, this, str_parts,start_index,ref(result));

			start_index = i + 1;
			thread_count++; // increase thread count(for .join())
			str_parts.clear(); // clear each part after send to thread
		}
		else if ((str.size() - i) < part_len) // check the last str_parts and str rest, because str.size() not always div totally,
											  // so we add last (str.size()-i) char to last str_parts;
		{
			while (i != str.size()) // 
			{
				str_parts += str[i];
				i++;
			}
			thread[thread_count] = std::thread(&VigenereDecipherer::decipher, this,str_parts,start_index,ref(result));
			start_index = i + 1;
			thread_count++;
		}
		else
			str_parts += str[i];
	}
	for (int i = 0; i < thread_count; i++) // join each thread from pull
		thread[i].join();

	return (result);
}
