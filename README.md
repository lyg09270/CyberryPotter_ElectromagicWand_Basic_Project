# 关于项目

此项目是Cyberry Potter Electromagic wand魔杖的基础工程

已经实现的功能有：

1. MPU6050读写数据
2. 按键控制
3. LED状态指示灯
4. 串口通信
5. 在PC端使用的[数据收集](https://github.com/lyg09270/CyberryPotter_ElectromagicWand_Basic_Project/blob/main/CNN/Serial_Read.py)、[模型训练](https://github.com/lyg09270/CyberryPotter_ElectromagicWand_Basic_Project/blob/main/CNN/CNNTrainRaw.py)、[串口模型测试脚本](https://github.com/lyg09270/CyberryPotter_ElectromagicWand_Basic_Project/blob/main/CNN/CNNTestSerialRaw.py)

项目使用的嵌入式机器学习库来自[nnom](https://github.com/majianjia/nnom)

# 如何训练

1. 使用[脚本](https://github.com/lyg09270/CyberryPotter_ElectromagicWand_Basic_Project/blob/main/CNN/Serial_Read.py)收集数据
   - 首先需要将config.h文件中的SYSTEM_MODE_DATA_COLLECT解除注释
   - 完成以上操作后单片机将进入数据打印模式而非推理模式
2. 运行[模型训练](https://github.com/lyg09270/CyberryPotter_ElectromagicWand_Basic_Project/blob/main/CNN/CNNTrainRaw.py)脚本
   - 运行训练脚本你将得到一个.h5的模型文件和一个.h的c头文件
   - .h5文件用于在[串口模型测试脚本](https://github.com/lyg09270/CyberryPotter_ElectromagicWand_Basic_Project/blob/main/CNN/CNNTestSerialRaw.py)对未量化的模型进行测试
   - .h文件是单片机用于编译模型所需要的文件

# 如何继续开发

此项目目前已经实现了动作识别的功能，并没有对其他功能进行开发，你可以在此工程的基础上对魔杖进行进一步开发
