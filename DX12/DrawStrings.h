#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include "DirectXMathExpansion.h"
#include "Singleton.h"
#include "TexManager.h"
#include "PrimitiveFunc.h"
#include <tchar.h>

#include <codecvt>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>
#include <math.h>

#define BUFSIZE 1024
#define PRINT_MAX_RADIX 16
#define INIT_DIGIT -1
#define INIT_FLOAT_DIGIT 6

#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define INT_BUFSIZE 32
#define MAX_DIGIT 64

enum class format_arglen {
	INT = 0,
	LONG,
};

enum class format_argtype {
	NONE = 0,
	SIGNED,
	UNSIGNED,
	FLOAT,
	STRING,
	CHAR,
	PERCENT,
};

enum format_pad {
	PAD_NO = 0,
	PAD_BLANK,
	PAD_ZERO,
};

struct format_argdesc {
	int radix;
	enum format_arglen len;
	enum format_argtype type;
	enum format_pad pad;
	int digit;
	int float_digit;
};

class DrawStrings : public Singleton<DrawStrings>
{
	friend class Singleton<DrawStrings>;
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

private:
	char digit[PRINT_MAX_RADIX + 1] = "0123456789abcdef";
	char pbuf[BUFSIZE];

	long lpow(long x, long y);
	const char *parse_arg(const char *fmt, struct format_argdesc *desc);
	unsigned long get_digit(unsigned long num, unsigned long radix);
	int luout(unsigned long num, char *buf, int size, struct format_argdesc *desc);
	int lout(signed long num, char *buf, int size, struct format_argdesc *desc);
	int lfout(double num, char *buf, int size, struct format_argdesc *desc);
	int myvsnprintf(char *str, int size, const char *fmt, va_list args);
	int myprintf(const char *fmt, va_list args);
	// “ú–{Œê‚ðŠÜ‚Þ•¶Žš—ñ‚Ì’·‚³‚ðŽæ“¾‚·‚é
	int lenByte(unsigned char c);
	int lenUtf8(std::string &str);
	std::string wide_to_utf8_cppapi(std::wstring const &src);
	std::wstring multi_to_wide_winapi(std::string const &src);
	std::string multi_to_utf8_cppapi(std::string const &src);
public:
	void DrawFormatString(XMFLOAT2 pos1, float height, XMFLOAT4 color, const char *FormatString, ...);
};