# Automatic_head_and_tail_system
野外飞禽智能投喂系统（嵌入式作业）

​		该项目旨在设计和开发一款野外飞禽智能投食系统，专为云南滇池红嘴鸥的科学投喂而设计。系统基于STM32微控制器，整合了BPC中国电波钟模块和多路开关信号，采用短波电波授时协议，确保时间同步的精准性。通过RTOS多任务架构，包括设备管理、计划设置和投喂器控制任务，系统实现了高效的任务分配和调度。此外，系统支持ESP-01S与手机通信，实现远程监控和操作，提升了系统的灵活性和便利性。整体目标是为红嘴鸥提供一套可靠、智能的投食解决方案，以满足其生态环境下的科学饲养需求。

## 特性

- **多任务调度：** 使用RT-Thread实时操作系统，实现多任务并行运行，确保系统高效稳定。
- **时钟计时：** 通过BPC低频信号进行时钟同步，确保系统时钟的准确性。
- **授时同步：** 系统启动时通过ESP01S与互联网通信，实现系统时钟的授时同步。
- **WiFi通信：** 利用ESP01S模块，通过TCP通信与安卓设备建立稳定的无线连接。

## 硬件与工具

- STM32系列微控制器
- ESP01S WiFi模块
- STM32CubeMX
- MDK（Keil uVision）
- VSCode

## 项目结构

```plaintext
├──	Schematic diagram of the circut #电路原理图
├── feeding_system # KEIL代码
├── mobile #安卓app代码
├── esp数据包格式 数据包json格式
├── README.md   # 项目说明文件
```

# 一、 管脚配置

| 模块   | stm32管脚 | 管脚命名   | 模块对应管脚  |
| ------ | --------- | ---------- | :------------ |
| esp01s | PA10      | wifi_rx    | tx            |
| esp01s | PA9       | wifi_tx    | rx            |
| esp01s | 3.3v      |            | vcc           |
| esp01s | GND       |            | GND           |
| 投喂   | PA4       | feeding_1  | led,蜂鸣器vcc |
| 投喂   | PA5       | feeding_2  | led,蜂鸣器vcc |
| 投喂   | PA6       | feeding_3  | led,蜂鸣器vcc |
| OLED   | PB6       | OLED_SCL   | SCL           |
| OLED   | PB7       | OLED_SDA   | SDA           |
| 电波钟 | PA8       | clock_tccn | tccn          |
| 电波钟 | PB15      | clock_pon  | pon           |

