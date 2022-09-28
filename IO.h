#include <cstdio>

#ifndef myIO
#define myIO

#define inMAXBUF 1024 // 读入缓冲区大小

unsigned char inS[inMAXBUF];
unsigned int inbuf = inMAXBUF;

unsigned char Getchar()
{ // 从缓冲区内获取一个字符
    if (inbuf == inMAXBUF)
    {
        inbuf = 0;
        fread(inS, 1, inMAXBUF, stdin);
    }
    return inS[inbuf++];
}

#define outMAXBUF 1024 // 输出缓冲区大小

unsigned char outS[outMAXBUF];
unsigned int outbuf = 0;
void Putchar(unsigned char ch, bool option = 0)
{ // 把ch写入缓冲区
    if (outbuf == outMAXBUF)
    {
        outbuf = 0;
        fwrite(outS, 1, outMAXBUF, stdout);
    }
    outS[outbuf++] = ch;
    if (outbuf == outMAXBUF)
    {
        outbuf = 0;
        fwrite(outS, 1, outMAXBUF, stdout);
    }
    if (option && ch == 0xFF)
    {
        outS[outbuf++] = 0x00;
    }
}
void flush()
{ // 输出缓冲区内剩余的元素
    fwrite(outS, 1, outbuf, stdout);
}

void push(int len, int v)
{ // 写入Len个bit，值为v

    static int L = 0, x = 0;
    if (len == 255 && v == 1)
    {
        if (!L)
            return;
        while (L < 8)
            x = (x << 1) | 1, L++;
        Putchar(x, 1);
        x = 0, L = 0;
        return;
    }
    x = (x << len) | v;
    L += len;
    while (L >= 8)
    {
        L -= 8;
        Putchar(x >> L, 1);
        x &= ((1 << L) - 1);
    }
}

#endif
