
## 项目说明

本项目为基于Python的晶体管网络类，主要用于读取或输入，并模拟晶体管网络的行为

## 项目结构

- Readme.md: 项目说明文档
- bsd_sim.py:简单模拟 `BSD` 并给出输出，以 `BSD` 文件格式作为输入

- Transistor.py: 定义晶体管类，包括晶体管的基本属性和行为
- Tnet_graph.py: 定义以`图`为基准的晶体管网络基类
- Tnet_union.py: 定义以`并查集`为基准的晶体管网络基类
- TransistorNet.py: 定义晶体管网络类，继承上述二者之一，实现晶体管网络的读取、输出、仿真等

- test.py: 简单的测试文件

- bsd/ :存放 `BSD` 文件
- cdl/ : 存放 `CDL` 文件
- generate_bsd/ : 生成 `BSD` 文件的工具（来自其他组的项目镜像）

- int2float/ :用上述工具生成的BSD，测试用
- Transfer8_1/ : 用上述工具生成的BSD，测试用