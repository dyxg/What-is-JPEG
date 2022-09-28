
#ifndef ALGORITHM
#define ALGORITHM

#include<math.h>
#include<cstdio>
#include<vector>
#include<cstring>

#include "IO.h"


const unsigned int block = 8;

const double Pi = 3.1415926535;


struct encode {
    unsigned int length;
    int v;
};

namespace algorithm {


double C(int x) {
    return x==0 ? 1/sqrt(2) : 1; // 
}

void DCT(double matrix[block][block]) {
    double result[block][block];
    for(int u=0;u<block;u++) {
        for(int v=0;v<block;v++) {
            result[u][v]=0;
            for(int i=0;i<block;i++) {
                for(int j=0;j<block;j++) {
                    result[u][v]+=matrix[i][j]*cos((2*i+1)*u*Pi/16)*cos((2*j+1)*v*Pi/16);
                }
            }
            result[u][v]*=0.25*C(u)*C(v);
        }
    }
    for(int u=0;u<block;u++) {
        for(int v=0;v<block;v++) {
            matrix[u][v]=(result[u][v]);
        }
    }
}


const int Qy[block][block]={
    {16,11,10,16,24,40,51,61},
    {12,12,14,19,26,58,60,55},
    {14,13,16,24,40,57,69,56},
    {14,17,22,29,51,87,80,62},
    {18,22,37,56,68,109,103,77},
    {24,35,55,64,81,104,113,92},
    {49,64,78,87,103,121,120,101},
    {72,92,95,98,112,100,103,99}
};

//   Y通道量化矩阵

const int Qc[block][block] {
    {17,18,24,47,99,99,99,99},
    {18,21,26,66,99,99,99,99},
    {24,26,56,99,99,99,99,99},
    {47,66,99,99,99,99,99,99},
    {99,99,99,99,99,99,99,99},
    {99,99,99,99,99,99,99,99},
    {99,99,99,99,99,99,99,99},
    {99,99,99,99,99,99,99,99}
};


//   C通道量化矩阵

void Qout(bool option) {
    if (option==0) {
        for(int i=0;i<block;i++) {
            for(int j=0;j<block;j++) {
                Putchar(Qy[i][j]);
            }
        }
    }
    else {
        for(int i=0;i<block;i++) {
            for(int j=0;j<block;j++) {
                Putchar(Qc[i][j]);
            }
        }
    }
}

void quantify(int result[block][block],double tmp[block][block],int option) {   
    // 把tmp量化入result
    // 传入0量化Y，否则量化C
    if (option==0) {
        for(int i=0;i<block;i++) {
            for(int j=0;j<block;j++) {
                result[i][j]=round(tmp[i][j]/Qy[i][j]);
            }
        }
    }
    else {
        for(int i=0;i<block;i++) {
            for(int j=0;j<block;j++) {
                result[i][j]=round(tmp[i][j]/Qc[i][j]);
            }
        }
    }
}

const int Z[8][8] = {
    {0 ,1 ,5 ,6 ,14,15,27,28},
    {2 ,4 ,7 ,13,16,26,29,42},
    {3 ,8 ,12,17,25,30,41,43},
    {9 ,11,18,24,31,40,44,53},
    {10,19,23,32,39,45,52,54},
    {20,22,33,38,46,51,55,60},
    {21,34,37,47,50,56,59,61},
    {35,36,48,49,57,58,62,63},
} ;

void ZigZag(int line[block*block],int source[block][block]) {
    for(int u=0;u<block;u++) {
        for(int v=0;v<block;v++) {
            line[Z[u][v]]=source[u][v];
        }
    }
}

inline unsigned int encodelength(int val) {
    unsigned int res=0;
    for(val=abs(val);val;val>>=1) res++;
    return res;
}

inline int encodeval(int val) {
    int len=encodelength(val);
    return val+(val<0)*((1<<len)-1);
}


int count[256];
std::pair< std::vector<int> ,std::vector<int> > __huffman() {
    std::priority_queue < std::pair<int,int>, std::vector< std::pair<int,int> > ,std::greater <std::pair<int,int> > > que;
    int Fa[256*2],Len[256*2],cnt=256;
    memset(Fa,-1,sizeof Fa);
    for(int i=0;i<256;i++) if (count[i]) {
        que.push(std::make_pair(count[i],i));
    }
    
    while(que.size()>1) {
        std::pair<int,int> A=que.top();
        que.pop();
        std::pair<int,int> B=que.top();
        que.pop();
        Fa[A.second]=cnt;
        Fa[B.second]=cnt;
        que.push(std::make_pair(A.first+B.first,cnt));
        cnt++;
    }
    // exit(0);
    std::vector<int> A,B;
    std::vector<int> C[17];
    A.resize(17);

    for(int i=0;i<256;i++) if (count[i]) {
        int l=0;
        for(int j=i;j!=-1;j=Fa[j]) {
            l++;
        }
        // l--;
        assert(l<=16);
        if (l==1) l++;
        A[l]++;C[l].push_back(i);
    }
    for(int i=0;i<=16;i++) {
        for(auto j:C[i]) {
            B.push_back(j);
        }
    }
    return std::make_pair(A,B);
}

std::pair< std::vector<int> ,std::vector<int> > Huffman(std::vector< encode > source) {
    memset(count,0,256*4);
    for(auto i:source) {
        // assert(i.length<256);
        count[i.length]++;
    }
    return __huffman();
}

std::pair< std::vector<int> ,std::vector<int> > Huffman(std::vector< encode > source1,std::vector< encode > source2) {
    memset(count,0,256*4);
    for(auto i:source1) count[i.length]++;
    for(auto i:source2) count[i.length]++;
    return __huffman();
}

void printmap(std::pair< std::vector<int> ,std::vector<int> > source) {
    bool tag=0;
    // int ccc=0;
    for(int i=1;i<=16;i++) {
        Putchar(source.first[i]);   // 各长度数量
        // ccc+=source.first[i];
    }
    // int ddd=0;
    for(auto i:source.second) Putchar(i);    //  范式哈夫曼编码与原值对应关系
    // assert(ccc==ddd);
}
struct huffmanmap {
    int iter=0,cnt=0;
    int c[256],Len[256],huffmanval[256];
    huffmanmap(std::pair< std::vector<int> ,std::vector<int> > table) {
        c[0]=0;
        for(auto v:table.first) {
            for(int j=1;j<=v;j++) {
                if (iter) {
                    if (j!=1) c[iter]=c[iter-1]+1;
                    else c[iter]=((c[iter-1]+1)<<(cnt-Len[iter-1]));
                }
                Len[iter]=cnt;
                // assert(Len[iter]>=encodelength(c[iter]));
                iter++;
            }
            cnt++;
        }
        iter=0;
        memset(huffmanval,-1,256*4);
        for(auto it:table.second) {
            #ifdef DEBUG
            std::cerr << Len[iter] <<" " << c[iter] << " " << it << std::endl;
            #endif
            huffmanval[it]=iter++;
        }
        #ifdef DEBUG
        std::cerr << std::endl;
        #endif
    }
    std::pair<int,int> query(int v) {
        if (huffmanval[v]==-1) {
            std::cerr << "error " << v << std::endl;
            exit(0);
        }
        return std::make_pair(Len[huffmanval[v]],c[huffmanval[v]]);
    }
};

int Cnt=0;

void outputblock(huffmanmap &mapDC,huffmanmap &mapAC,std::vector<encode>::iterator &DC,std::vector<encode>::iterator &AC) {
    
    push(mapDC.query(DC->length).first,mapDC.query(DC->length).second);
    // std::cerr << mapDC.query(itdc->length).first << std::endl;
    #ifdef DEBUG
    // std::cerr << mapDC.query(DC->length).first << " " << mapDC.query(DC->length).second << " " << DC->length << " " << DC->v << std::endl;
    #endif
    push(DC->length,DC->v);
    int total=0;
    for(;;++AC) {
        // Cnt++;
        // std::cerr << Cnt << std::endl;
        #ifndef DEBUG
        push(mapAC.query(AC->length).first,mapAC.query(AC->length).second);
        #else
        Putchar(AC->length);
        #endif
        // std::cerr << mapAC.query(AC->length).first << " " << mapAC.query(AC->length).second << " " << AC->length << " " << AC->v << std::endl;
        // std::cerr << itac -> length << std::endl;
        push(AC->length&15,AC->v);
        if (AC->length==0x00) break;
        total+=AC->length>>4;
        total++;
    }
    assert(total<=63);
    ++DC;++AC;
}

void printhuffman(std::pair< std::vector<int> ,std::vector<int> > table1DC,
                    std::pair< std::vector<int> ,std::vector<int> > table1AC,
                    std::pair< std::vector<int> ,std::vector<int> > table2DC,
                    std::pair< std::vector<int> ,std::vector<int> > table2AC,
                    std::pair< std::vector<encode> ,std::vector< encode > >  dataY,
                    std::pair< std::vector<encode> ,std::vector< encode > >  dataCb,
                    std::pair< std::vector<encode> ,std::vector< encode > >  dataCr
                    ) {
        huffmanmap mapYDC(table1DC),mapYAC(table1AC),mapCDC(table2DC),mapCAC(table2AC);
        std::vector<encode>::iterator YDC=dataY.first.begin();
        std::vector<encode>::iterator CbDC=dataCb.first.begin();
        std::vector<encode>::iterator CrDC=dataCr.first.begin();
        std::vector<encode>::iterator YAC=dataY.second.begin();
        std::vector<encode>::iterator CbAC=dataCb.second.begin();
        std::vector<encode>::iterator CrAC=dataCr.second.begin();
        
        for(;YDC!=dataY.first.end();) {
            Cnt++;
            outputblock(mapYDC,mapYAC,YDC,YAC);
            outputblock(mapCDC,mapCAC,CrDC,CrAC);
            outputblock(mapCDC,mapCAC,CbDC,CbAC);
        }
    }
}
#endif