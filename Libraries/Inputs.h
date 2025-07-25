#pragma once

#include <iostream>
#include <string>
#include <limits>
using namespace std;

namespace Inputs
{
    int ReadPositiveIntNumber(string Message = "Enter a Positive Number?\n")
    {
        int Number;
        cout << Message;
        cin >> Number;

        while (cin.fail() || Number <= 0)
        {
            cin.clear();
            cin.ignore();

            cout << "Invalid Number, " << Message;
            cin >> Number;
        }

        return Number;
    }

    double ReadPositiveDoubleNumber(string Message = "Enter a Positive Number?\n")
    {
        double Number;
        cout << Message;
        cin >> Number;

        while (cin.fail() || Number <= 0)
        {
            cin.clear();
            cin.ignore(); //std::numeric_limits<std::streamsize>::max(), '\n'

            cout << "Invalid Number, " << Message;
            cin >> Number;
        }

        return Number;
    }

    int ReadNumberInRange(string Message, int From, int To)
    {
        int Number;
        cout << Message << " [" << From << " - " << To << "]? ";
        cin >> Number;

        while (cin.fail() || Number < From || Number > To)
        {
            cin.clear();
            cin.ignore();

            cout << "Invalid Number, " << Message << " [" << From << " - " << To << "]? ";
            cin >> Number;
        }

        return Number;

    }

    string ReadText(string Message = "Enter a Text?\n")
    {
        string Text = "";
        cout << Message;
        getline(cin >> ws, Text);

        return Text;
    }

    int ReadNumber(string Message = "Enter a Number?\n")
    {
        int Number;
        cout << Message;
        cin >> Number;

        while (cin.fail())
        {
            cin.clear();
            cin.ignore();

            cout << "Invalid Number, Enter a valid one?" << endl;
            cin >> Number;
        }

        return Number;
    }

    char ReadChar()
    {
        char c;
        cout << "Enter Char?\n";
        cin >> c;

        return c;
    }
}