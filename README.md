# bad-apple
STM32F407ZGT6 90FPS刷bad apple  
bad apple 90fps刷屏程序，未进行底层优化，只是用了一些小技巧，底层优化，可以速度更快  
有问题可联系qq362276794  
本程序只能在正点原子STM32F407探索板2.8寸屏上运行，其他屏需要自行修改程序和重新合成bin播放文件  
本程序使用定时器3进行刷屏测速，串口打印刷屏速度，串口波特率为115200  
全速运行时请注释掉测速函数，不然影响刷屏速度  
请把制作的二进制播放文件放到存储卡apple目录下，并且文件名为apple.bin，完整路径为/apple/apple.bin  
如果出现花屏，请按复位键，有时是SD卡初始化不成功导致  
由于bin文件太大无法上传，请到百度云下载，链接为：
链接：https://pan.baidu.com/s/1xKVRwllRsH8eoV0DFE-x9w 
提取码：38wy 
里面为bin文件和整个工程文件
