#include<cstdio>

#ifndef myIO
#define myIO

#define inMAXBUF 1024

unsigned char inS[inMAXBUF];   // 读入缓冲区
unsigned int inbuf=inMAXBUF;


unsigned char Getchar() {
    if (inbuf==inMAXBUF) {
        inbuf=0;
        fread(inS,1,inMAXBUF,stdin);
    }
    return inS[inbuf++];
}

#define outMAXBUF 65536

unsigned char outS[outMAXBUF];
unsigned int outbuf=0;
int CNT;
void Putchar(unsigned char ch,bool option = 0) {
    #ifdef DEBUG
    int x=ch>>4,y=ch&15;
    if (x>=10) putchar(x-10+'A');
    else putchar('0'+x);
    if (y>=10) putchar(y-10+'A');
    else putchar('0'+y);
    putchar(' ');
    CNT++;
    if (CNT==20) {
        CNT=0;
        putchar('\n');
    }
    return ;
    #else
    if (outbuf==outMAXBUF) {
        outbuf=0;
        fwrite(outS,1,outMAXBUF,stdout);
    }
    outS[outbuf++]=ch;
    if (outbuf==outMAXBUF) {
        outbuf=0;
        fwrite(outS,1,outMAXBUF,stdout);
    }
    if (option && ch==0xFF) {
        outS[outbuf++]=outMAXBUF;
    }
    #endif
}
void flush() {
    fwrite(outS,1,outbuf,stdout);
}


int L=0,x=0;
void push(int len,int v) {
    #ifdef DEBUG
        if (len==255) return ;
        for(int i=len-1;i>=0;i--) {
            putchar('0'+(bool)(v&(1<<i)));
        }putchar(' ');
    #else
    // std::cerr << len << " " << v << std::endl;
    if (len==255&&v==0) {
        if (!L) return ;
        x<<=8-L;
        Putchar(x,1);
        x=0,L=0;
        return ;
    }
    if (len==255&&v==1) {
        // std::cerr <<(L) << std::endl;
        if (!L) return ;
        while(L<8) x=(x<<1)|1,L++;
        Putchar(x,1);
        x=0,L=0;
        return ;
    }
    // v=rand()%(1<<len); 
    // std::cerr << len << " " << v << std::endl;
    x=(x<<(int)len)|v;
    L+=len;
    // std::cerr << len << " " << v << std::endl;
    // std::cerr << L << " " << x << std::endl;
    while (L>=8) {
        L-=8;
        Putchar(x>>L,1);
        x&=((1<<L)-1);
    }
    #endif  
}


#endif