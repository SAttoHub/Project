#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

/// <summary>
/// �Ђ炪�ȁE������ϊ�����ƕ��l������̂Ő��l���ł����Ȃ�܂�
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
