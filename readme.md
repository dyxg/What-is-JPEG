支持Linux

### 编译命令
```
g++ main.cpp -O2
```
### 如何运行

```
main input.bin output.jpeg X Y
```
demo.bin 表示输入文件名，每个像素点以RGB顺序，各1字节，连续存放为一个3字节的段，然后按照从左到右，从上到下顺序排列每个像素点

demo.jpeg 表示输出文件名

X表示纵向像素数。
Y表示横向像素数。
（X，Y须为8的倍数）

### 成品效果

![image](https://raw.githubusercontent.com/dyxg/What-is-JPEG/main/demo.jpeg)

