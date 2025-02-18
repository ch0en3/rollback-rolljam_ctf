一道用于验证rollback与rolljam攻击原理的小题目
题目部署需要至少两块esp32+cc1101模块

以下是esp32与cc1101的连接方法:

    电源连接：
        VCC (CC1101) 连接到 3.3V (ESP32)
        GND (CC1101) 连接到 GND (ESP32)
    SPI 接口连接：
        MOSI (CC1101) 连接到 MOSI (ESP32 GPIO 23)
        MISO (CC1101) 连接到 MISO (ESP32 GPIO 19)
        SCK (CC1101) 连接到 SCK (ESP32 GPIO 18)
        CSN (CS/SS) (CC1101) 连接到 CS/SS (ESP32 GPIO 5)
    GDO 引脚连接：
        GDO0 (CC1101) 连接到 ESP32 GPIO 4 (或任意可用 GPIO)
        GDO2 (CC1101) 连接到 ESP32 GPIO 15 (或任意可用 GPIO)

剩下还需要一个可以收发信号的设备,即需满足rolljam攻击的要求
可以使用SDR收发信号+一个能够干扰的模块.但是如果只是想尝试的话建议手动干扰屏蔽接收端信号
(rolljam攻击在现实成功率很低,本题目只是出于教学原理,建议各位想尝试的师傅没必要为了追求攻击效果坐牢..)
另外在实验过程中如果存在信号干扰的话可以尝试调整一些参数,以下是一些示例

    优化接收带宽 (setRxBW)：
        如果你发现接收信号中有很多噪声，可以尝试减小接收带宽来提高信号选择性。
        如果信号质量不稳定或经常丢失信号，可以尝试增大带宽以提高信号的接收能力。
    调整数据率 (setDRate)：
        在信号质量较好的情况下，可以尝试增高数据率以提高传输速度。
        在信号质量较差或有干扰的环境中，尝试降低数据率以提高信号的可靠性。
    设置功率放大器 (setPA)：
        如果信号的传输距离不足，可以尝试增加PA的级别以提高信号强度。
        如果信号过强导致干扰，可以尝试降低PA的级别。
    示例设置
    ELECHOUSE_cc1101.setRxBW(20000);  // 初始设置为20 kHz的接收带宽
    ELECHOUSE_cc1101.setDRate(2000);  // 初始设置为2 kbps的数据率
    ELECHOUSE_cc1101.setPA(10);       // 初始设置为功率放大器等级10



除了SDR以外还有一种低成本的收发方式,详情参考https://github.com/mcore1976/cc1101-tool

