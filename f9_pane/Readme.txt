0906
优化程序结构 考虑开源设计  继电器功能
RGB灯功能正常
按键修改引脚，测试功能正确
拨码修改引脚，测试功能正确
485修改引脚，测试功能正确

根据D9S 框架 重构F9框架   BUFF存放名字  HEX存放指令  cmd存放长度
增加LOGIC业务
红外感应功能无有效信号

多灯操作 和协议延时差一个字节
协议7E 0F 04 11 01 00 01 03 00 00 00 05 00 00 F0 AA 7D
实际7E 0F 04 10 01 00 01 03 00 00 00 05 00  F0 AA 7D
多灯写 多灯查询工作正确

内部指令说明：
按键-> HEX0 通道
拨码-> HEX0 地址
RGB -> HEX0 单/多通道模式/待机模块   HEX1 单通道/多通道状态   HEX2   单通道数据
通讯->HEX0 单/多通道模式/查询        HEX1 单通道/多通道状态   HEX2   单通道数据
OUTPUT->HEX0 单/多通道模式           HEX1 单通道/多通道状态   HEX2   单通道数据

串口协议
//HEX0 1单灯    2多灯模式  3单输出    4输出  0f查询 
HEX0 1单灯    2多灯模式    0f查询   6个输出与前6个灯绑定
HEX1 通道值/多通道状态   
HEX2 单通道数据


0907
红外感应引脚不能配置成上拉
颜色配置功能正确
IO输出正确

0907-1
灯光对红外感应有干扰
整理LOGIC业务结构 整理 程序文件结构
通讯协议将接收 和发送分开
增加输出映射功能
映射配置
7E 00 11 10  01 02 03 04 05 06 07 08  55  01  AA 7D
完成通讯发送数据优化结构

0908
通讯发送接收封装成函数
发送数据完成封装
接收数据完成封装
修改数据保存BUG
增加按键操作类型  取反 闪烁
增加配置参数立即生效
优化程序排版

0909
优化串口接收结构 将操作函数放在其他文件 与客户文件分开
将OUTPUT改为轻量级协议


