正点原子IMX6ULL-MINI开发板裸机代码实验

安装编译器流程:

sudo vim /etc/apt/sources.list

添加以下内容:
deb http://dk.archive.ubuntu.com/ubuntu/ xenial main
deb http://dk.archive.ubuntu.com/ubuntu/ xenial universe

sudo apt update
sudo apt install gcc-4.9-arm-linux-gnueabihf