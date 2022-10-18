#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

/// <summary>
/// ‚Ğ‚ç‚ª‚ÈEŠ¿š‚ğ•ÏŠ·‚·‚é‚Æ•‰’l‚ª“ü‚é‚Ì‚Å”’l‚ª‚Å‚©‚­‚È‚è‚Ü‚·
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
static string ChangeStringCode(string str) {
    string s2;
    stringstream ss;
    for (const auto &item : str) {
        ss << hex << int(item);
    }
    s2 = ss.str();
    return s2;
}
