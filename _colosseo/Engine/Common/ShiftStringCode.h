#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>

/// <summary>
/// �Ђ炪�ȁE������ϊ�����ƕ��l������̂Ő��l���ł����Ȃ�܂�
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
static std::string ChangeStringCode(std::string str) {
    std::string s2;
    std::stringstream ss;
    for (const auto &item : str) {
        ss << std::hex << int(item);
    }
    s2 = ss.str();
    return s2;
}
