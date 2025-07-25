#pragma once

#include <iostream>
using namespace std;

namespace Utils
{
    void Swap(int& A, int& B)
    {
        int Temp;

        Temp = A;
        A = B;
        B = Temp;
    }

    string Tabs(short NumberOfTabs)
    {
        string t = "";

        for (short i = 0; i < NumberOfTabs; i++)
            t += "\t";

        return t;
    }

    void ResetScreen()
    {
        system("color 0F");
        system("cls");
    }
}