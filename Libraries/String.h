#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

namespace String
{
	vector<string> SplitString(string Str, string Delim)
	{
		vector<string> vString;

		short pos = 0;
		string Word;

		while ((pos = Str.find(Delim)) != string::npos)
		{
			Word = Str.substr(0, pos);

			if (Word != "")
				vString.push_back(Word);

			Str.erase(0, pos + Delim.length());
		}

		if (Str != "")
			vString.push_back(Str);

		return vString;
	}
	
    string JoinString(vector<string> vString, string Delim)
	{
		string Str = "";

		for (const string& Element : vString)
		{
			Str += Element + Delim;
		}

		return Str.substr(0, Str.length() - Delim.length());
	}
}