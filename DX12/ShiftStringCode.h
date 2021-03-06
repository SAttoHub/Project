#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

/// <summary>
/// ひらがな・漢字を変換すると負値が入るので数値がでかくなります
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
static inline string ChangeStringCode(string str) {
    string s2;
    stringstream ss;
    for (const auto &item : str) {
        ss << hex << int(item);
    }
    s2 = ss.str();
    return s2;
}
