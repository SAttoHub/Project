#include "DrawStrings.h"

long DrawStrings::lpow(long x, long y)
{
	long ret = 1;
	int i;

	for (i = 0; i < y; i++) {
		ret *= x;
	}

	return ret;
}

const char *DrawStrings::parse_arg(const char *fmt, format_argdesc *desc)
{
    bool complete;
    unsigned int digit_size;

    assert(*fmt == '%');

    complete = false;
    desc->len = format_arglen::INT;
    desc->type = format_argtype::NONE;
    desc->radix = 10;
    desc->pad = PAD_NO;
    desc->digit = INIT_DIGIT;
    desc->float_digit = INIT_FLOAT_DIGIT;

    while (*++fmt) {
        switch (*fmt) {
        case 'l':
            desc->len = format_arglen::LONG;
            break;

        case 'd':
            desc->type = format_argtype::SIGNED;
            complete = true;
            goto out;

        case 'u':
            desc->type = format_argtype::UNSIGNED;
            complete = true;
            goto out;

        case 'x':
            desc->type = format_argtype::UNSIGNED;
            desc->radix = 16;
            complete = true;
            goto out;

        case 'f':
            desc->type = format_argtype::FLOAT;
            complete = true;
            goto out;

        case 's':
            desc->type = format_argtype::STRING;
            complete = true;
            goto out;

        case 'c':
            desc->type = format_argtype::CHAR;
            complete = true;
            goto out;

        case '%':
            desc->type = format_argtype::PERCENT;
            complete = true;
            goto out;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '.':
            if (*fmt == '0') {
                desc->pad = PAD_ZERO;
                fmt++;
            }
            else {
                desc->pad = PAD_BLANK;
            }

            /* integer part */
            digit_size = 0;

            while ((*fmt >= '0') && (*fmt <= '9')) {
                digit_size = digit_size * 10 + (*fmt - '0');
                fmt++;
            }

            desc->digit = digit_size;

            /* float part */
            digit_size = 0;

            if (*fmt == '.') {
                fmt++;

                while ((*fmt >= '0') && (*fmt <= '9')) {
                    digit_size = digit_size * 10 + (*fmt - '0');
                    fmt++;
                }

                if (digit_size > 0) {
                    desc->float_digit = digit_size;
                }
            }

            fmt--;
            break;

        default:
            fprintf(stderr, "Unknown mark: %d\n", *fmt);
            desc->type = format_argtype::NONE;
            complete = true;
            goto out;
        }
    }

out:

    if (!complete) {
        assert("Unknown/incomplete expression");
    }

    /* Bypass last expression char */
    if (*fmt != '\0') {
        fmt++;
    }

    return fmt;
}

unsigned long DrawStrings::get_digit(unsigned long num, unsigned long radix)
{
    unsigned long digit = 0;

    while (num != 0) {
        num /= radix;
        digit++;
    }

    return digit;
}

int DrawStrings::luout(unsigned long num, char *buf, int size, format_argdesc *desc)
{
    int ret, digits;
    int i;

    assert(desc->radix > 2 && desc->radix <= PRINT_MAX_RADIX);

    digits = 0;

    if (num == 0) {
        digits++;
    }
    else {
        digits = get_digit(num, desc->radix);
    }

    assert(digits > 0);
    assert(digits <= size);
    assert(desc->digit <= size);

    if (desc->digit == INIT_DIGIT) {
        ret = digits;

        for (; digits != 0; digits--) {
            buf[digits - 1] = digit[num % desc->radix];
            num = num / desc->radix;
        }
    }
    else {
        assert(digits <= desc->digit);
        ret = desc->digit;

        for (i = 0; i < desc->digit - digits; i++) {
            switch (desc->pad) {
            case PAD_BLANK:
                buf[i] = ' ';
                break;

            case PAD_ZERO:
                buf[i] = '0';

            case PAD_NO:
            default:
                break;
            }
        }

        for (i = 0; i < digits; i++) {
            buf[desc->digit - i - 1] = digit[num % desc->radix];
            num = num / desc->radix;
        }
    }

    return ret;
}

int DrawStrings::lout(signed long num, char *buf, int size, format_argdesc *desc)
{
    assert(desc->radix > 2 && desc->radix <= PRINT_MAX_RADIX);

    if (num < 0) {
        /* Make room for the '-' */
        assert(size >= 2);

        num *= -1;
        buf[0] = '-';

        return luout(num, buf + 1, size - 1, desc) + 1;
    }

    return luout(num, buf, size, desc);
}

int DrawStrings::lfout(double num, char *buf, int size, format_argdesc *desc)
{
    bool sign = num < 0.0;
    int ret = 0;
    static char inf[] = "inf";
    static char nan[] = "NaN";
    unsigned long ulpart, dpart;
    char str[INT_BUFSIZE];
    int int_begin;
    int len;
    int dlen;
    int digit = desc->digit;
    int float_digit = desc->float_digit;

    if (isinf(num)) {
        if (sign) {
            *buf++ = '-';
            ret++;
        }

        strcpy(buf, inf);
        ret += int(strlen(inf));
    }
    else if (isnan(num)) {
        strcpy(buf, nan);
        ret = int(strlen(nan));
    }
    else {
        if (sign) {
            num = -num;
            buf[ret++] = '-';
        }

        int_begin = ret;
        ulpart = (unsigned long)num;
        /* round off to @float_digit decimal places */
        dpart = (num - ulpart) * lpow(10, desc->float_digit) + unsigned long(0.5);

        if (float_digit == INIT_FLOAT_DIGIT) {
            desc->digit = INIT_FLOAT_DIGIT;
        }
        else {
            desc->digit = desc->float_digit;
        }

        dlen = luout(dpart, &buf[ret], size, desc);

        if (digit == INIT_DIGIT || digit < float_digit ||
            digit - float_digit - 1 < (int)get_digit(ulpart, 10)) {
            desc->digit = INIT_DIGIT;
        }
        else {
            desc->digit = digit - float_digit - 1;
        }

        len = luout(ulpart, str, size, desc);
        str[len++] = '.';
        /* move float part to right place */
        memmove(&buf[int_begin + len], &buf[int_begin], dlen);
        /* move integer part from str to right place */
        memmove(&buf[int_begin], str, len);

        ret += dlen + len;
    }

    return ret;
}

int DrawStrings::myvsnprintf(char *str, int size, const char *fmt, va_list args)
{
    struct format_argdesc desc = { 0 };
    const char *s;
    char *p;
    long num;
    unsigned long unum;
    int len;
    double dnum;

    if (size < 1) {
        return 0;
    }

    p = str;

    while (*fmt) {
        while (*fmt != '\0' && *fmt != '%' && size != 0) {
            *p++ = *fmt++;
            size--;
        }

        if (*fmt == '\0' || size == 0) {
            break;
        }

        assert(*fmt == '%');
        fmt = parse_arg(fmt, &desc);

        len = 0;

        switch (desc.type) {
        case format_argtype::SIGNED:
            if (desc.len == format_arglen::LONG) {
                num = va_arg(args, long);
            }
            else {
                num = va_arg(args, int);
            }

            len = lout(num, p, size, &desc);
            break;

        case format_argtype::UNSIGNED:
            if (desc.len == format_arglen::LONG) {
                unum = va_arg(args, unsigned long);
            }
            else {
                unum = va_arg(args, unsigned int);
            }

            len = luout(unum, p, size, &desc);
            break;

        case format_argtype::FLOAT:
            if (desc.len == format_arglen::LONG) {
                dnum = va_arg(args, double);
            }
            else {
                dnum = (float)va_arg(args, double);
            }

            len = lfout(dnum, p, size, &desc);
            break;

        case format_argtype::STRING:
            s = va_arg(args, char *);

            if (!s) {
                s = "<*NULL*>";
            }

            len = int(strlen(s));
            len = MIN(size, len);
            strncpy(p, s, len);
            break;

        case format_argtype::CHAR:
            *p = (unsigned char)va_arg(args, int);
            len = 1;
            break;

        case format_argtype::PERCENT:
            *p = '%';
            len = 1;
            break;

        default:
            fprintf(stderr, "Error: Unknown print type %d\n", desc.type);
            break;
        }

        p += len;
        size -= len;
    }

    assert(p >= str);
    return int(p - str);
}

int DrawStrings::myprintf(const char *fmt, va_list args)
{
    int n;
    n = myvsnprintf(pbuf, sizeof(pbuf), fmt, args);
    pbuf[n] = '\0';

    return n;
}

int DrawStrings::lenByte(unsigned char c)
{
    if ((c >= 0x00) && (c <= 0x7f))
    {
        return 1;
    }
    else if ((c >= 0xc2) && (c <= 0xdf))
    {
        return 2;
    }
    else if ((c >= 0xe0) && (c <= 0xef))
    {
        return 3;
    }
    else if ((c >= 0xf0) && (c <= 0xf7))
    {
        return 4;
    }

    return 0;
}

int DrawStrings::lenUtf8(std::string &str)
{
    auto _pstr = str.c_str();

    int i = 0;
    int len = 0;

    while (_pstr[i] != '\0')
    {
        len++;
        i += lenByte(_pstr[i]);
    }

    return len;
}

std::string DrawStrings::wide_to_utf8_cppapi(std::wstring const &src)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(src);
}

std::wstring DrawStrings::multi_to_wide_winapi(std::string const &src)
{
    auto const dest_size = ::MultiByteToWideChar(CP_ACP, 0U, src.data(), -1, nullptr, 0U);
    std::vector<wchar_t> dest(dest_size, L'\0');
    if (::MultiByteToWideChar(CP_ACP, 0U, src.data(), -1, dest.data(), int(dest.size())) == 0) {
        throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
    }
    dest.resize(std::char_traits<wchar_t>::length(dest.data()));
    dest.shrink_to_fit();
    return std::wstring(dest.begin(), dest.end());
}

std::string DrawStrings::multi_to_utf8_cppapi(std::string const &src)
{
    auto const wide = multi_to_wide_winapi(src);
    return wide_to_utf8_cppapi(wide);
}

void DrawStrings::DrawFormatString(XMFLOAT2 pos1, float height, XMFLOAT4 color, const char *FormatString, ...)
{
    va_list args;
    va_start(args, FormatString);
    myprintf(FormatString, args);
    va_end(args);

    int n = int(strlen(pbuf));

    TCHAR buf[256];
    ZeroMemory(buf, 256);
#ifdef UNICODE
    MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, pbuf, n, buf, (sizeof buf) / 2);
#else
    strcpy(buf, pbuf);
#endif

    std::string s = pbuf;
    int c = lenUtf8(multi_to_utf8_cppapi(s));
    float next = 0;

    for (int i = 0; i < c; i++) {
        int Num = TexManager::GetStringTexture(&buf[i]);
        // フォントテクスチャのデータ
        TEXTMETRIC tm = TexManager::TextureData[Num].tm;
        GLYPHMETRICS gm = TexManager::TextureData[Num].gm;
        // height と　矩形の高さの差から拡大率を出す
        float ext = height / (float)tm.tmHeight;
        // 文字の描画開始高さ
        float h = pos1.y + ((float)tm.tmAscent - (float)gm.gmptGlyphOrigin.y) * ext;
        // 文字の横幅
        float x = gm.gmBlackBoxX * ext;
        // 文字の縦幅
        float y = gm.gmBlackBoxY * ext;

        DrawGraphEffColor(XMFLOAT2(pos1.x + next, h), XMFLOAT2(pos1.x + next + x, h + y), Num, color);

        // 次に描画する文字のxを保存する
        next += gm.gmCellIncX * ext;
    }
}