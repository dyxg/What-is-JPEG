#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <ctime>
#include "algorithm.h"
#include "IO.h"

//设定图片大小为SIZE*SIZE

//色深为 BIT bit

#define YR 0.299
#define YG 0.587
#define YB 0.114

#define CbR 0.5
#define CbG -0.4187
#define CbB -0.0813

#define CrR -0.1687
#define CrG -0.3313
#define CrB 0.5

struct picture
{
    unsigned int SIZEX, SIZEY;
    std::vector<std::vector<unsigned char>> R, G, B;
    // unsigned char R[SIZEX][SIZEY],G[SIZEX][SIZEY],B[SIZEX][SIZEY];

    unsigned int Xdensity = 64, Ydensity = 64;

    std::pair<std::vector<std::pair<int, int>>, std::vector<std::pair<int, int>>> Y;  // Y图层游程编码
    std::pair<std::vector<std::pair<int, int>>, std::vector<std::pair<int, int>>> Cr; // Cr图层游程编码
    std::pair<std::vector<std::pair<int, int>>, std::vector<std::pair<int, int>>> Cb; // Cb图层游程编码

    std::pair<std::vector<int>, std::vector<int>> YDC; // Y图层DC哈夫曼表
    std::pair<std::vector<int>, std::vector<int>> YAC; // Y图层AC哈夫曼表
    std::pair<std::vector<int>, std::vector<int>> CDC; // C图层DC哈夫曼表
    std::pair<std::vector<int>, std::vector<int>> CAC; // C图层AC哈夫曼表

    private: std::pair<std::vector<std::pair<int, int>>, std::vector<std::pair<int, int>>>
    TransToVector(float ValR, float ValG, float ValB, float ValC, int option)
    {
        static int Maxlength = 15;
        // 完成 颜色空间转换 + 离散余弦变换 + 量化
        std::vector<std::pair<int, int>> AC;
        std::vector<std::pair<int, int>> DC;
        AC.clear(), DC.clear();
        float tmp[block][block];
        int result[block][block];
        int line[block * block];
        int pre = 0;
        for (int i = 0; i < SIZEX; i += block)
        {
            for (int j = 0; j < SIZEY; j += block)
            {
                for (int i1 = i; i1 < i + block; i1++)
                {
                    for (int j1 = j; j1 < j + block; j1++)
                    {
                        tmp[i1 - i][j1 - j] = (ValR * R[i1][j1] + ValG * G[i1][j1] + ValB * B[i1][j1] + ValC - 128);
                    }
                }
                algorithm::DCT(tmp);
                algorithm::quantify(result, tmp, option);
                algorithm::ZigZag(line, result);
                // 对单位8*8矩阵进行处理，将该8*8的小矩形量化
                {
                    int v = pre;
                    pre = line[0];
                    line[0] -= v;
                }
                // 差分
                DC.push_back(std::make_pair((algorithm::encodelength(line[0])), algorithm::encodeval(line[0])));

                for (unsigned int iterator = 1, nowzero = 0; iterator < block * block; iterator++)
                {
                    if (line[iterator] == 0 && nowzero < Maxlength)
                        nowzero++;
                    else
                    {
                        AC.push_back(std::make_pair((unsigned int)((nowzero << (4)) | algorithm::encodelength(line[iterator])),
                                                    algorithm::encodeval(line[iterator])));
                        nowzero = 0;
                    }
                }
                while (!AC.empty() && AC.back().first == 0xF0)
                    AC.pop_back();
                AC.push_back(std::make_pair(0, 0));
            }
        }

        return std::make_pair(DC, AC);
    }
    void OutputMap(int HT, std::pair<std::vector<int>, std::vector<int>> &Map)
    { // 输出哈夫曼表
        Putchar(0xFF), Putchar(0xC4);
        unsigned int Len = Map.second.size() + 19;
        Putchar(Len >> 8), Putchar(Len & 255); //段长
        Putchar(HT);                           // HT信息
        algorithm::printmap(Map);
    }
    public: void Output(char *file)
    {
        output = fopen(file,"wb");
        Putchar(0xFF), Putchar(0xD8);
        // 文件头

        Putchar(0xFF), Putchar(0xE0);
        Putchar(0x00), Putchar(0x10);                                              //段长度
        Putchar(0x4A), Putchar(0x46), Putchar(0x49), Putchar(0x46), Putchar(0x00); // JFIF
        Putchar(0x01), Putchar(0x02);                                              //版本号
        Putchar(0x01);                                                             //密度单位
        Putchar(Xdensity >> 8), Putchar(Xdensity & 255);
        Putchar(Ydensity >> 8), Putchar(Ydensity & 255); //像素密度
        Putchar(0), Putchar(0);
        // APP0图像识别信息

        Putchar(0xFF), Putchar(0xDB);
        Putchar(0x00), Putchar(0x43);
        Putchar(0x00);
        algorithm::Qout(0);
        Putchar(0xFF), Putchar(0xDB);
        Putchar(0x00), Putchar(0x43);
        Putchar(0x01);
        algorithm::Qout(1);
        // DQT定义量化表

        Putchar(0xFF), Putchar(0xC0);
        Putchar(0x00), Putchar(0x11); //段长度
        Putchar(0x08);                //色深
        Putchar(SIZEX >> 8), Putchar(SIZEX & 255);
        Putchar(SIZEY >> 8), Putchar(SIZEY & 255);   //像素数
        Putchar(0x03);                               //各通道信息
        Putchar(0x01), Putchar(0x11), Putchar(0x00); // Y
        Putchar(0x02), Putchar(0x11), Putchar(0x01); // Cb
        Putchar(0x03), Putchar(0x11), Putchar(0x01); // Cr
        // SOF0图像基本信息

        OutputMap(0x00, YDC);
        OutputMap(0x10, YAC);
        OutputMap(0x01, CDC);
        OutputMap(0x11, CAC);
        //哈夫曼编码表

        Putchar(0xFF), Putchar(0xDA);
        Putchar(0x00), Putchar(0x0C);
        Putchar(0x03);
        Putchar(0x01);
        Putchar(0x00); // Y
        Putchar(0x02);
        Putchar(0x11); // Cb
        Putchar(0x03);
        Putchar(0x11); // Cr
        Putchar(0x00);
        Putchar(0x3F);
        Putchar(0x00);

        // SOS扫描行
        algorithm::printresult(YDC, YAC, CDC, CAC, Y, Cb, Cr); // 输出数据

        push(255, 1);

        Putchar(0xFF), Putchar(0xD9);
        flush();
        fclose(stdout);
    }
    picture(char *file, int Xsize, int Ysize)
    {
        freopen(file, "r", stdin);
        SIZEX = Xsize, SIZEY = Ysize;
        R.resize(SIZEX), G.resize(SIZEX), B.resize(SIZEX);
        for (int i = 0; i < SIZEX; i++)
        {
            R[i].resize(SIZEY);
            G[i].resize(SIZEY);
            B[i].resize(SIZEY);
        }
        for (int i = 0; i < SIZEX; i++)
        {
            for (int j = 0; j < SIZEY; j++)
            {
                R[i][j] = Getchar();
                G[i][j] = Getchar();
                B[i][j] = Getchar();
            }
        }
        fclose(stdin);
        Y = TransToVector(YR, YG, YB, 0, 0);
        Cb = TransToVector(CbR, CbG, CbB, 128, 1);
        Cr = TransToVector(CrR, CrG, CrB, 128, 1);
        // 完成编码
        // std::cerr << clock()/float(CLOCKS_PER_SEC) << std::endl;
        YDC = algorithm::Huffman(Y.first);
        YAC = algorithm::Huffman(Y.second);
        CDC = algorithm::Huffman(Cr.first, Cb.first);
        CAC = algorithm::Huffman(Cr.second, Cb.second);
    }
};

void Transform(char *file_in, char *file_out, int SIZEX, int SIZEY)
{
    picture P(file_in, SIZEX, SIZEY);
    // P.Trans();
    P.Output(file_out);
}

int main(int argc, char *argv[])
{
    // freopen(argv[5],"w",stderr);
    if (argc <= 4)
    {
        printf("too few parameters\n");
        return 0;
    }
    Transform(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]));
    std::cerr << "Time cost: " << clock() / float(CLOCKS_PER_SEC)*1000 << "ms" << std::endl;
}