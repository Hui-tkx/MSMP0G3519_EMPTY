# MSPM0G3519 嵌入式开发项目

## 项目简介

这是一个基于 TI MSPM0G3519 微控制器的嵌入式开发项目，使用 Keil MDK 作为开发环境。项目包含完整的板级支持包（BSP），集成了丰富的外设驱动和测试功能，方便快速进行嵌入式应用开发。

## 硬件平台

- **微控制器**: Texas Instruments MSPM0G3519
- **内核**: ARM Cortex-M0+
- **开发工具**: Keil MDK v6.24 (ARMCLANG)
- **SDK**: MSPM0 SDK 2.08.00.03
- **编译器**: ARM Compiler 6.24

## 目录结构

```
empty/
├── BSP/                    # 板级支持包
│   ├── adc/               # ADC模数转换驱动及测试
│   ├── audio_task/        # 音频任务处理
│   ├── buzzer/            # 蜂鸣器控制
│   ├── dac/               # DAC数模转换驱动及测试
│   ├── delay/             # 延时函数
│   ├── flash/             # Flash读写驱动
│   ├── key/               # 按键输入驱动
│   ├── led/               # LED控制驱动
│   ├── oled/              # OLED显示驱动
│   ├── pwm/               # PWM脉宽调制
│   ├── ringbuffer/        # 环形缓冲区实现
│   ├── temperature/       # 温度传感器驱动及测试
│   ├── uart/              # UART通信驱动及测试
│   ├── w25q64/            # W25Q64 Flash芯片驱动
│   └── test/              # 板级综合测试
├── Project/               # Keil项目文件
│   ├── empty.uvprojx      # 项目配置文件
│   ├── empty.uvoptx       # 工程选项配置
│   └── startup_mspm0g351x_uvision.s  # 启动文件
├── Source/                # TI官方驱动库
│   └── ti/driverlib/      # MSPM0外设驱动库
├── User/                  # 用户代码
│   ├── main.c             # 主程序入口
│   └── ti_msp_dl_config.c # 系统配置代码
└── Output/               # 编译输出目录
```

## 功能模块

### 基础外设驱动

| 模块 | 功能描述 |
|------|----------|
| LED | LED灯控制，支持亮灭操作 |
| KEY | 按键输入检测与读取 |
| BUZZER | 蜂鸣器控制，提供提示音功能 |
| PWM | 脉宽调制输出控制 |
| DELAY | 延时函数实现 |

### 通信接口

| 模块 | 功能描述 |
|------|----------|
| UART | 串口通信，支持数据收发 |
| SPI | SPI总线通信协议实现 |
| RingBuffer | 环形缓冲区，支持数据缓存 |

### 模拟外设

| 模块 | 功能描述 |
|------|----------|
| ADC | 模数转换，支持多通道采样 |
| DAC | 数模转换，输出模拟电压 |
| Temperature | 温度传感器数据采集 |

### 存储与显示

| 模块 | 功能描述 |
|------|----------|
| Flash | 内部Flash读写操作 |
| W25Q64 | 外部SPI Flash芯片驱动（8MB存储空间） |
| OLED | OLED显示屏驱动 |

### 高级功能

| 模块 | 功能描述 |
|------|----------|
| Audio Task | 音频任务处理 |
| Test | 板级综合测试套件 |

## 快速开始

### 编译项目

1. 使用 Keil MDK 打开 `Project/empty.uvprojx` 文件
2. 点击 Build 按钮或使用快捷键 F7 编译项目
3. 编译产物输出到 `Output/` 目录

### 烧录程序

1. 连接 J-Link 或其他调试器
2. 在 Keil 中点击 Download 按钮或使用快捷键 F8 烧录
3. 观察程序运行状态

### 基本使用示例

```c
#include "bsp.h"

int main(void)
{
    // 系统初始化
    SYSCFG_DL_init();
    
    // 初始化外设
    UART_init(UART_0_INST);
    ADC_init();
    DAC_init();
    
    // 使用LED
    LED_On(LED_L1);
    delay_cycles(1000000);
    LED_Off(LED_L1);
    
    // 使用UART发送数据
    UART_sendString(UART_0_INST, "Hello World!\r\n");
    
    // 读取ADC值
    float voltage = ADC_readVoltage(ADC_CHANNEL_0);
    
    // 主循环
    while (1) {
        // 业务逻辑
    }
}
```

## 内存配置

| 区域 | 起始地址 | 大小 | 说明 |
|------|----------|------|------|
| IRAM | 0x20000000 | 64KB | 内部RAM |
| IRAM2 | 0x20100000 | 64KB | 内部RAM2 |
| IROM | 0x00000000 | 512KB | 主Flash |
| IROM2 | 0x00400000 | 512KB | 辅助Flash |
| XRAM | 0x20200000 | 128KB | 外部RAM |
| XRAM2 | 0x20300000 | 64KB | 外部RAM2 |

## 联系方式

如有问题或建议，请通过以下方式联系：
- 开发者：惠
- 创建日期：2026.03.06
