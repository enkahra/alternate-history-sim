#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
	cout << "Enter English input (e.g., 'usa atom bomb does not develop'):" << endl;
	string input;
	getline(cin, input);
	istringstream iss(input);
	vector<string> words;
	string word;
	while (iss >> word)
	{
		words.push_back(word);
	}
	if (!words.empty())
	{
		string country = words[0];
		transform(country.begin(), country.end(), country.begin(), ::toupper);
	}
	else
	{
		cout << "No input provided!" << endl;
	}



	return 0;
}