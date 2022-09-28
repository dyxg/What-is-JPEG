#include<cstdio>
#include<vector>
#include<algorithm>
#include<cstring>

#include "algorithm.h"
#include "IO.h"


const unsigned int SIZE=256;

const unsigned int BIT=8;

const unsigned int Maxlength = 15;

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

struct picture {
    


    unsigned char R[SIZE][SIZE],G[SIZE][SIZE],B[SIZE][SIZE];

    unsigned int Xpixel = SIZE, Ypixel=SIZE,Xdensity=64,Ydensity=64;

    std::pair<std::vector < encode > ,std::vector< encode > > Y;          // Y图层游程编码
    std::pair<std::vector < encode > ,std::vector< encode > > Cr;         // Cr图层游程编码
    std::pair<std::vector < encode > ,std::vector< encode > > Cb;         // Cb图层游程编码

    std::pair< std::vector<int> ,std::vector<int> > YDC;                   // Y图层DC哈夫曼表
    std::pair< std::vector<int> ,std::vector<int> > YAC;                   // Y图层AC哈夫曼表
    std::pair< std::vector<int> ,std::vector<int> > CDC;                   // C图层DC哈夫曼表
    std::pair< std::vector<int> ,std::vector<int> > CAC;                   // C图层AC哈夫曼表

    std::pair<std::vector < encode > ,std::vector< encode > >
                TransToVector(double ValR,double ValG,double ValB,double ValC,int option) {   
        // 完成 颜色空间转换 + 离散余弦变换 + 量化
        std::vector< encode > AC;
        std::vector< encode > DC;
    // int Cnt=0;
        AC.clear(),DC.clear();
        double tmp[block][block];
        int result[block][block];
        int line[block*block];
        int pre=0;
        for(int i=0;i<SIZE;i+=block) {
            for(int j=0;j<SIZE;j+=block) {
                for(int i1=i;i1<i+block;i1++) {
                    for(int j1=j;j1<j+block;j1++) {
                        tmp[i1-i][j1-j]=(ValR*R[i1][j1]+ValG*G[i1][j1]+ValB*B[i1][j1]+ValC-128);
                        
                    }
                }
                algorithm::DCT(tmp);
                algorithm::quantify(result,tmp,option);
                algorithm::ZigZag(line,result);
                // 对单位8*8矩阵进行处理，将该8*8的小矩形量化
                {
                    int v=pre;
                    pre=line[0];    
                    line[0]-=v;
                }
                // 差分
                DC.push_back((encode){(algorithm::encodelength(line[0])),algorithm::encodeval(line[0])});

                for(unsigned int iterator=1,nowzero=0;iterator<block*block;iterator++) {
                    if (line[iterator]==0&&nowzero<Maxlength) nowzero++;
                    else {
                        AC.push_back(encode{(unsigned int)((nowzero<<(4))|algorithm::encodelength(line[iterator])),
                                                            algorithm::encodeval(line[iterator])});
                        nowzero=0;
                    }
                }
                while(!AC.empty()&&AC.back().length==0xF0) AC.pop_back();
                AC.push_back(encode{0,0});
            }
        }

        return std::make_pair(DC,AC);
    }
    void Trans() {
        Y=TransToVector(YR,YG,YB,0,0);
        Cb=TransToVector(CbR,CbG,CbB,128,1);
        Cr=TransToVector(CrR,CrG,CrB,128,1);
        // 完成编码
        YDC=algorithm::Huffman(Y.first);
        YAC=algorithm::Huffman(Y.second);
        CDC=algorithm::Huffman(Cr.first,Cb.first);
        CAC=algorithm::Huffman(Cr.second,Cb.second);
        // 哈夫曼表构造
    }
    void OutputMap(int HT,std::pair< std::vector<int> ,std::vector<int> > &Map) {    // 输出哈夫曼表
        Putchar(0xFF),Putchar(0xC4);
        unsigned int Len=Map.second.size()+19;
        Putchar(Len>>8),Putchar(Len&255);//段长
        Putchar(HT);                   //HT信息
        algorithm::printmap(Map);
    }
    void Output(char *file) {
        freopen(file,"w",stdout);
        Putchar(0xFF),Putchar(0xD8);
        // 文件头

        Putchar(0xFF),Putchar(0xE0);
        Putchar(0x00),Putchar(0x10);//段长度
        Putchar(0x4A),Putchar(0x46),Putchar(0x49),Putchar(0x46),Putchar(0x00); // JFIF
        Putchar(0x01),Putchar(0x02);//版本号
        Putchar(0x01); //密度单位
        Putchar(Xdensity>>8),Putchar(Xdensity&255);
        Putchar(Ydensity>>8),Putchar(Ydensity&255);//像素密度
        Putchar(0),Putchar(0);
        // APP0图像识别信息

        Putchar(0xFF),Putchar(0xDB);
        Putchar(0x00),Putchar(0x43);
        Putchar(0x00);
        algorithm::Qout(0);
        Putchar(0xFF),Putchar(0xDB);
        Putchar(0x00),Putchar(0x43);
        Putchar(0x01);
        algorithm::Qout(1);
        // DQT定义量化表


        Putchar(0xFF),Putchar(0xC0);
        Putchar(0x00),Putchar(0x11);//段长度
        Putchar(0x08);       //色深
        Putchar(Xpixel>>8),Putchar(Xpixel&255);
        Putchar(Ypixel>>8),Putchar(Ypixel&255);//像素数
        Putchar(0x03);//各通道信息
        Putchar(0x01),Putchar(0x11),Putchar(0x00);//Y
        Putchar(0x02),Putchar(0x11),Putchar(0x01);//Cb
        Putchar(0x03),Putchar(0x11),Putchar(0x01);//Cr
        //SOF0图像基本信息

        OutputMap(0x00,YDC);
        OutputMap(0x10,YAC);
        OutputMap(0x01,CDC);
        OutputMap(0x11,CAC);
        //哈夫曼编码表
        
        Putchar(0xFF),Putchar(0xDA);
        Putchar(0x00),Putchar(0x0C);
        Putchar(0x03);
        Putchar(0x01);Putchar(0x00);//Y
        Putchar(0x02);Putchar(0x11);//Cb
        Putchar(0x03);Putchar(0x11);//Cr
        Putchar(0x00);Putchar(0x3F);Putchar(0x00);
        
        // SOS扫描行
        algorithm::printresult(YDC,YAC,CDC,CAC,Y,Cb,Cr);    // 输出数据

        push(255,1);

        Putchar(0xFF),Putchar(0xD9);
        #ifndef DEBUG
        flush();
        fclose(stdout);
        #endif
    }
    picture(char *file) {
        freopen(file,"r",stdin);
        for(int i=0;i<SIZE;i++) {
            for(int j=0;j<SIZE;j++) {
                R[i][j]=Getchar();
                G[i][j]=Getchar();
                B[i][j]=Getchar();
                // R[i][j]=rand()%256;
                // G[i][j]=rand()%256;
                // B[i][j]=rand()%256;
                // R[i][j]=0x00;
                // G[i][j]=0x00;
                // B[i][j]=0x00;
            }
        }
        fclose(stdin);
    }
};

void Transform(char *file_in,char *file_out) {
    picture P(file_in);
    P.Trans();
    P.Output(file_out);
}

int main(int argc,char *argv[]) {
    Transform("demo.bin","demo.jpeg");
}
