/*****************************************************************************
* | File      	:   Readme_CN.txt
* | Author      :   Waveshare team
* | Function    :   Help with use
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-09-24
* | Info        :   在这里提供一个中文版本的使用文档，以便你的快速使用
******************************************************************************/
这个文件是帮助您使用本例程。
在这里简略的描述本工程的使用：

1.基本信息：
本例程均在Raspberry Pi 3B+ 上进行了验证;
本例程使用2inch LCD Module模块进行了验证，你可以在工程的examples\中查看对应的测试例程;

2.管脚连接：
管脚连接你可以在\lib\waveshare_2inch_LCD\config.py中查看，这里也再重述一次：
LCD         =>    Jetson Nano/RPI(BCM)
VCC         ->    3.3V
GND         ->    GND
DIN         ->    MOSI(10)
CLK         ->    SCLK(11)
CS          ->    8
DC          ->    25
RST         ->    27
BL          ->    18

3.安装库：
	sudo apt-get update
	sudo apt-get install python-pip
	sudo pip install RPi.GPIO
	sudo pip install spidev
	sudo apt-get install python-imaging
或

	sudo apt-get update
	sudo apt-get install python3-pip
	sudo pip3 install RPi.GPIO
	sudo pip3 install spidev
	sudo apt-get install python3-imaging
	
4.基本使用：
测试程序在examples\目录中查看
python2
    sudo python main.py

python3
    sudo python3 main.py