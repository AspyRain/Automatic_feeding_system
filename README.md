# Automatic_head_and_tail_system
 自动投喂系统

# 一、 管脚配置

| 模块   | stm32管脚 | 管脚命名   | 模块对应管脚  |
| ------ | --------- | ---------- | ------------- |
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

