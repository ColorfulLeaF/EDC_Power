# 最大功率点追踪
## 快速上手

- 初始化mppt结构体,而后使用初始化函数
```c++
mppt my_mppt;
mppt_init(&my_mppt,0.4,0.01,0.75,0.3);
```