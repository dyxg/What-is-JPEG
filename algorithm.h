
#ifndef ALGORITHM
#define ALGORITHM

#include <math.h>
#include <cstdio>
#include <vector>
#include <cstring>
#include <queue>
#include "IO.h"

const unsigned int block = 8;

const float Pi = 3.1415926535;


namespace algorithm
{

    float C(int x)
    {
        return x == 0 ? 1 / sqrt(2) : 1; //
    }

    void DCT(float matrix[block][block])
    {
        float result[block][block];
        for (int u = 0; u < block; u++)
        {
            for (int v = 0; v < block; v++)
            {
                result[u][v] = 0;
                for (int i = 0; i < block; i++)
                {
                    for (int j = 0; j < block; j++)
                    {
                        result[u][v] += matrix[i][j] * cos((2 * i + 1) * u * Pi / 16) * cos((2 * j + 1) * v * Pi / 16);
                    }
                }
                result[u][v] *= 0.25 * C(u) * C(v);
            }
        }
        for (int u = 0; u < block; u++)
        {
            for (int v = 0; v < block; v++)
            {
                matrix[u][v] = (result[u][v]);
            }
        }
    }
    /*
    const int Qy[block][block] = {
        {16, 11, 10, 16, 24, 40, 51, 61},
        {12, 12, 14, 19, 26, 58, 60, 55},
        {14, 13, 16, 24, 40, 57, 69, 56},
        {14, 17, 22, 29, 51, 87, 80, 62},
        {18, 22, 37, 56, 68, 109, 103, 77},
        {24, 35, 55, 64, 81, 104, 113, 92},
        {49, 64, 78, 87, 103, 121, 120, 101},
        {72, 92, 95, 98, 112, 100, 103, 99}};
    */
    const int Qy[block][block] = {
        {0x08, 0x06, 0x06, 0x07, 0x06, 0x05, 0x08, 0x07},
        {0x07, 0x07, 0x09, 0x09, 0x08, 0x0A, 0x0C, 0x14},
        {0x0D, 0x0C, 0x0B, 0x0B, 0x0C, 0x19, 0x12, 0x13},
        {0x0F, 0x14, 0x1D, 0x1A, 0x1F, 0x1E, 0x1D, 0x1A},
        {0x1C, 0x1C, 0x20, 0x24, 0x2E, 0x27, 0x20, 0x22},
        {0x2C, 0x23, 0x1C, 0x1C, 0x28, 0x37, 0x29, 0x2C},
        {0x30, 0x31, 0x34, 0x34, 0x34, 0x1F, 0x27, 0x39},
        {0x3D, 0x38, 0x32, 0x3C, 0x3E, 0x33, 0x34, 0x32}};

    //   Y通道量化矩阵
    /*
    const int Qc[block][block]{
        {17, 18, 24, 47, 99, 99, 99, 99},
        {18, 21, 26, 66, 99, 99, 99, 99},
        {24, 26, 56, 99, 99, 99, 99, 99},
        {47, 66, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99}};
    */
    const int Qc[block][block]{
        {0x09, 0x09, 0x09, 0x0C, 0x0B, 0x18, 0x0D, 0x0D},
        {0x18, 0x32, 0x21, 0x1C, 0x21, 0x32, 0x32, 0x32},
        {0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32},
        {0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32},
        {0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32},
        {0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32},
        {0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32},
        {0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32}};

    //   C通道量化矩阵

    void Qout(bool option)
    {
        if (option == 0)
        {
            for (int i = 0; i < block; i++)
            {
                for (int j = 0; j < block; j++)
                {
                    Putchar(Qy[i][j]);
                }
            }
        }
        else
        {
            for (int i = 0; i < block; i++)
            {
                for (int j = 0; j < block; j++)
                {
                    Putchar(Qc[i][j]);
                }
            }
        }
    }

    void quantify(int result[block][block], float tmp[block][block], int option)
    {
        // 把tmp量化入result
        // 传入0量化Y，否则量化C
        if (option == 0)
        {
            for (int i = 0; i < block; i++)
            {
                for (int j = 0; j < block; j++)
                {
                    result[i][j] = round(tmp[i][j] / Qy[i][j]);
                }
            }
        }
        else
        {
            for (int i = 0; i < block; i++)
            {
                for (int j = 0; j < block; j++)
                {
                    result[i][j] = round(tmp[i][j] / Qc[i][j]);
                }
            }
        }
    }

    const int Z[8][8] = {
        {0, 1, 5, 6, 14, 15, 27, 28},
        {2, 4, 7, 13, 16, 26, 29, 42},
        {3, 8, 12, 17, 25, 30, 41, 43},
        {9, 11, 18, 24, 31, 40, 44, 53},
        {10, 19, 23, 32, 39, 45, 52, 54},
        {20, 22, 33, 38, 46, 51, 55, 60},
        {21, 34, 37, 47, 50, 56, 59, 61},
        {35, 36, 48, 49, 57, 58, 62, 63},
    };

    void ZigZag(int line[block * block], int source[block][block])
    {
        for (int u = 0; u < block; u++)
        {
            for (int v = 0; v < block; v++)
            {
                line[Z[u][v]] = source[u][v];
            }
        }
    }

    // 编码计算

    inline unsigned int encodelength(int val)
    {
        unsigned int res = 0;
        for (val = abs(val); val; val >>= 1)
            res++;
        return res;
    }

    inline int encodeval(int val)
    {
        int len = encodelength(val);
        return val + (val < 0) * ((1 << len) - 1);
    }

    int count[256];
    std::pair<std::vector<int>, std::vector<int>> __huffman()
    {

        // 哈夫曼表生成
        // 每次取出两个出现次数最小数A,B的合并为一个C，AB的码长为C的码长+1

        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> que;
        // 构造优先队列
        int Fa[256 * 2], Len[256 * 2], cnt = 256;
        memset(Fa, -1, sizeof Fa);
        for (int i = 0; i < 256; i++)
            if (count[i])
            {
                que.push(std::make_pair(count[i], i));
            }

        while (que.size() > 1)
        {
            std::pair<int, int> A = que.top();
            que.pop();
            std::pair<int, int> B = que.top();
            que.pop();
            Fa[A.second] = cnt;
            Fa[B.second] = cnt;
            que.push(std::make_pair(A.first + B.first, cnt));
            cnt++;
        }
        // 构造哈夫曼树

        std::vector<int> A, B;
        std::vector<int> C[17];
        A.resize(17);
        std::vector<int> Rb;
        for (int i = 0; i < 256; i++)
            if (count[i])
            {
                int l = 0;
                for (int j = i; j != -1; j = Fa[j])
                {
                    l++;
                }
                // 计算码长
                if (l > 16)
                {
                    Rb.push_back(i);
                    continue;
                }
                /* 特别说明：
                    范式哈夫曼表不能出现码长>16的情况
                    但是在哈夫曼表构造直接中可能会出现
                    这里将>16的编码回收
                */
                if (l == 1)
                    l++; // 哈夫曼表避免码长为1的情况（我也不知道为什么这样会挂）
                A[l]++;
                C[l].push_back(i); // 计算每个码长对应的编码数
            }
        for (int now = 0; !Rb.empty(); now--, A[16]++, C[16].push_back(Rb.back()), Rb.pop_back())
        {
            for (int j = 15; j >= 1; j--)
                if (A[j])
                {
                    A[j]--;
                    A[16]++;
                    now += 1 << (16 - j);
                    now--;
                }
        }
        /*
            去掉一个较短的码然后可以放入一个被回收的元素
        */
        for (int i = 0; i <= 16; i++)
        {
            for (auto j : C[i])
            {
                B.push_back(j);
            }
        }
        return std::make_pair(A, B);
    }

    std::pair<std::vector<int>, std::vector<int>> Huffman(std::vector<std::pair<int, int>> source)
    {
        memset(count, 0, 256 * 4);
        for (auto i : source)
        {
            count[i.first]++;
        }
        // 统计出现次数
        return __huffman();
    }

    std::pair<std::vector<int>, std::vector<int>> Huffman(
        std::vector<std::pair<int, int>> source1,
        std::vector<std::pair<int, int>> source2)
    {
        memset(count, 0, 256 * 4);
        for (auto i : source1)
            count[i.first]++;
        for (auto i : source2)
            count[i.first]++;
        return __huffman();
    }

    void printmap(std::pair<std::vector<int>, std::vector<int>> source)
    {
        for (int i = 1; i <= 16; i++)
        {
            Putchar(source.first[i]); // 各长度数量
        }
        for (auto i : source.second)
            Putchar(i); //  范式哈夫曼编码与原值对应关系
    }
    struct huffmanmap
    {
        int iter = 0, cnt = 0;
        int c[256], Len[256], huffmanval[256];
        huffmanmap(std::pair<std::vector<int>, std::vector<int>> table)
        {
            // 构造范式哈夫曼表
            c[0] = 0;
            for (auto v : table.first)
            {
                for (int j = 1; j <= v; j++)
                {
                    if (iter)
                    {
                        if (j != 1)
                            c[iter] = c[iter - 1] + 1;
                        else
                            c[iter] = ((c[iter - 1] + 1) << (cnt - Len[iter - 1]));
                    }
                    Len[iter] = cnt;
                    iter++;
                }
                cnt++;
            }
            iter = 0;
            memset(huffmanval, -1, 256 * 4);
            for (auto it : table.second)
            {
                huffmanval[it] = iter++;
            }
        }
        std::pair<int, int> query(int v)
        {
            // 询问该值的对应长度与编码
            return std::make_pair(Len[huffmanval[v]], c[huffmanval[v]]);
        }
    };


    void outputblock(huffmanmap &mapDC, huffmanmap &mapAC, std::vector<std::pair<int, int>>::iterator &DC, std::vector<std::pair<int, int>>::iterator &AC)
    {
        // 单个块输出
        push(mapDC.query(DC->first).first, mapDC.query(DC->first).second);
        push(DC->first, DC->second);
        for (;; ++AC)
        {
            push(mapAC.query(AC->first).first, mapAC.query(AC->first).second);
            push(AC->first & 15, AC->second);
            if (AC->first == 0x00)
                break;
            // 把整个AC部分输出
        }
        ++DC;
        ++AC;
    }

    void printresult(std::pair<std::vector<int>, std::vector<int>> table1DC,
                     std::pair<std::vector<int>, std::vector<int>> table1AC,
                     std::pair<std::vector<int>, std::vector<int>> table2DC,
                     std::pair<std::vector<int>, std::vector<int>> table2AC,
                     std::pair<std::vector<std::pair<int, int>>, std::vector<std::pair<int, int>>> dataY,
                     std::pair<std::vector<std::pair<int, int>>, std::vector<std::pair<int, int>>> dataCb,
                     std::pair<std::vector<std::pair<int, int>>, std::vector<std::pair<int, int>>> dataCr)
    {
        // 输出最终数据
        huffmanmap mapYDC(table1DC), mapYAC(table1AC), mapCDC(table2DC), mapCAC(table2AC);

        std::vector<std::pair<int, int>>::iterator YDC = dataY.first.begin();
        std::vector<std::pair<int, int>>::iterator CbDC = dataCb.first.begin();
        std::vector<std::pair<int, int>>::iterator CrDC = dataCr.first.begin();
        std::vector<std::pair<int, int>>::iterator YAC = dataY.second.begin();
        std::vector<std::pair<int, int>>::iterator CbAC = dataCb.second.begin();
        std::vector<std::pair<int, int>>::iterator CrAC = dataCr.second.begin();

        for (; YDC != dataY.first.end();)
        {
            outputblock(mapYDC, mapYAC, YDC, YAC);
            outputblock(mapCDC, mapCAC, CrDC, CrAC);
            outputblock(mapCDC, mapCAC, CbDC, CbAC);
        }
    }
}
#endif