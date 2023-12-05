mcu.md

# GENERAL

## FUNCTION
- Use state machine to prevent the following
    1. Delay
    2. Takes Long Time

- Entry Point
    1. MainLoop
        - Non-Timing Critical Service
    2. Timer
        - Fixed Cycle
    3. Interrupt
        - IO, ex. UART RX
        - Try not to use unless timing critical or overrun 
    4. Callback
        - Register, ex. ISR callback

<pre>
Moudle Design

    module_init
    module_loop
    module_tisr
    module_deinit

System Design

    main_init       module1_init
                    module2_init
                    ...
                    moduleN_init

    main_loop       module1_loop
                    module2_loop
                    ...
                    moduleN_loop

    timer_isr       module1_tisr
                    module2_tisr
                    ...
                    moduleN_tisr

</pre>

## Thread Safe
- 雖然MCU裡無Thread, 但可以類比主迴圈&Interrupt之間的行為模式
- 避免在主迴圈與INT存取相同的變數或I/O操作
- 如不得以該如何繞過? Mutex? 如何避免死鎖?


## Block/Non-Block RW
1. Block: Polling
2. Non-Block: INT/DMA + Callback
3. 時用情境
    1. 交握
        1. Block R/W: 簡單但耗時，須估算資料量取得CPU占時，也可能因高優先權中斷漏包
        2. Nonblock + 狀態機: 省時但複雜，連續性的交握 (寫->讀->寫->讀…) 會是場災難
    2. Read Only
        - 如觸控座標 or ADC 使用 DMA 讀取
    3. 主機
        - 如I2C Master Read: 使用 Block Write + NonBlock Read
    4. 從機
        - 如I2C Slave 都滿適合 DMA Read/Write 
    5. UART
        - UART 的資料讀取是不曉得什麼時侯來，也不知道資料長度。要用DMA讀取須IC可支援非固定長度的功能


## RAM
1. Heap, Stack, Variable
2. 禁用 Heap = 禁用動態記憶體
3. 評估 Stack使用量 after Compile
4. 監測 Stack使用量 during Run Time
5. Passing pointers instead of values to save Stack
6. Use const declarations for constant values to save Variable


## ISR
1. ISR越短越好，如收到資料就存Buffer，在主迴圈裡再處理。
2. ISR裡禁止長時間的IO操作，如 UART RX裡叫 UART TX，除非使用 DMA TX
3. ISR裡禁止會觸發同一個ISR的操作。如 UART ERR ISR 又去叫 printf
4. ISR可設定優先級別，如可設定UART RX中斷高於ADC中斷避免漏包


## ADC
1. Reference
   - http://wiki.csie.ncku.edu.tw/embedded/ADC
2. Parameter
    1. Resolution: 12bit
    2. Conversion Time: us
    3. Polling, INT, DMA
    4. Auto Scan
3. 使用者在讀取ADC時通常會進行濾皮處理避免雜訊干擾
4. 濾波方法: TBC

## GPIO
1. Reference
    - http://wiki.csie.ncku.edu.tw/embedded/GPIO
2. STM32F4xx
   1. 所有 GPIO 具有容許較高的電流能力與四種速度選擇，根據不同設定管理雜訊、電耗與電磁波
   2. 1. 最大 I/O 切換速度可以支援到 90 MHz
3. 當進入省電模式時須依硬體設計重設GPIO屬性，降低暗電流損耗
4. 開機預設值須依硬體設計設定GPIO屬性，避免開機過程中 誤開 或 誤關 或 開開關關 或 誤判斷

### GPI
1. floating vs. pull-up/pull-down
    1. floating
        - 在沒有外部訊號驅動的情況下是呈現高阻抗狀態(無法確定電位狀態=>不能明確表示現在值是0或1)
    2. pull-up 讓pin維持在高電壓狀態
    3. pull-down 讓pin維持在低電壓狀態
2. 使用者在讀取GPI時通常會進行De-Bounce處理避免雜訊干擾
3. De-Bounce 邏輯: 取樣5ms, 當轉態超過20ms後才承認

### GPO
1. push-pull vs open-drain mode
    1. push-pull模式
        - “0”時輸出低電位
        - “1”時輸出高電位
    2. open-drain模式
        - “0”輸出低電位
        - “1”時為高阻抗(Z)狀態，電位無法確定
2. 此兩種模式下，pin皆可pull-up/pull-down

## PWM
- Pulse-Width Modulation, 又稱pulse-duration modulation(PDM),
  是將脈波轉為類比信號的“一種技術”,利用在頻率不變的狀態下, 改變工作週期大小, 
  使整體平均電壓值上升或下降, 藉此間歇性電壓及功率切換以節省能源及控制等效果.
- duty cycle 在一個信號週期裡，代表1的正脈衝的持續時間與脈衝總周期的比值。
  舉例來說，發出訊號1秒鐘，之後99秒沒有訊號，這是一個週期；
  之後又是發出一秒鐘的訊號，如此循環下去。而該訊號的工作週期就是1/(1+99)=1%。
- 使用PWM控制LED亮度
    1. 設定60Hz避免閃爍
    2. 可調亮度為256階
    3. PWM的頻率 = 60Hz x 256 = 15.36KHz

1. Reference
    - http://wiki.csie.ncku.edu.tw/embedded/PWM
2. Parameter
    1. Frequency
    2. Duty
    3. Clock = Frequency * Duty Resolution

## I2C
1. Reference
    - http://wiki.csie.ncku.edu.tw/embedded/I2C
2. Parameter
    1. Master/Slave
    2. Speed
        - 標準模式 (Standard-mode), 速率可高達 100 kbit/s
        - 快速模式 (Fast-mode), 速率可高達 400 kbit/s
        - 快速模式PLUS (Fast-mode Plus), 速率可高達 1 Mbit/s
        - 高速模式 (High-speed mode), 速率可高達 3.4 Mbit/s
3. I2C Timeout 的數值設定邏輯
    400KBps = 1ms 傳送 40 Bytes
    若要處理 40 Bytes，建議 Timeout 設為10ms
4. I2C R/W Error 時須視狀況重新初始化 I2C
5. I2C Slave 的 Error Handling 須處理 Bus Error 陷入死迴圈的機制
6. 錯誤注入
    - SCL/SDA 對地短路
    - SCL/SDA 斷路
    - SCL/SDA 雜訊注入


# MCU STELLANTIS 13"

# Reference
1. C130HAA010_SYS_PCBA_M02_20221108_909_3004136.pdf

# Hardware Block Diagram

## DIAGRAM

@startuml
skinparam componentStyle rectangle

component CONSOLE

component POWER
component WAKEUP
component POWER_HOLD

component PCBA_NTC
component PCBA_VER1
component PCBA_VER2

component FPC_DET

component PMIC
component MAX96752

component MCU

component MPQ3364_1
component MPQ3364_2
component NT51926



MCU -U-  CONSOLE : UART

MCU -U-  POWER : ADC
MCU -U-  WAKEUP : ADC
MCU -U-  POWER_HOLD: GPO

MCU -U-  PCBA_NTC : ADC
MCU -U-  PCBA_VER1 : ADC
MCU -U-  PCBA_VER2 : ADC

MCU -U-  FPC_DET : ADC

MCU -- PMIC : GPI_PG3V3\n GPI_PG1V8\n GPI_PG1V3
MCU -- MAX96752 : SERDES\n\n UART_SERDES\n UART_NETWORK\n GPO_RST\n GPI_LOCK\n GPI_ERR

MCU -- MPQ3364_1 : LEDDRV\n\n I2C\n GPI_FAULT\n ADC_NTC\n GPO_EN\n PWMO_BRI
MCU -- MPQ3364_2 : LEDDRV\n\n I2C\n GPI_FAULT\n ADC_NTC\n GPO_EN\n PWMO_BRI
MCU -- NT51926: TDDI\n\n GPO_RST\n GPO_TP_EN\n GPI_TP_INT\n GPI_FAULT\n I2C\n SPI\n GPO_SPI_CS\n GPI_DEBUG

@enduml

## POWER
1. HARDWARE
    1. NET: +PERM_ADC
    2. +PERM_ADC ~= (+PERM - 0.7V (VF) ) * (2.2K/(18K+2.2K))
        <pre>
        +PERM   +PERM_ADC
        5.0V    0.468V
        7.0V    0.686V
        9.0V    0.904V
        10.0V   1.013V
        11.0V   1.122V
        12.0V   1.231V
        13.0V   1.340V
        14.0V   1.449V
        15.0V   1.557V
        16.0V   1.666V
        18.0V   1.884V
        24.0V   2.538V
        27.0V   2.864V
        31.0V   3.300V
        35.6V   3.801V
        </pre>
2. SPEC
    1. REQ-0668501 (A) 
        <pre>
        Input : Upower_R		
        Min : 5.6	
        Typ : /	
        Max : 16		
        Unit : V
        </pre>
3. DESIGN
    1. ADC
    2. Resolution: 12bit
    3. Conversion Time: < 1ms
    4. Scan Mode (AUTO/INT): INT + SW (Need Change to AUTO)
    5. DMA: Off (Need Change to AUTO)

## WAKEUP
1. HARDWARE
    1. NET: WAKEUP_ADC
    2. WAKEUP_ADC ~= (WAKEUP - 0.22V (VF) ) * (2.2K/(18K+2.2K))
    <pre> 
    WAKEUP  WAKEUP_ADC
    5.0V    0.521V
    7.0V    0.738V
    9.0V    0.956V
    10.0V   1.065V
    11.0V   1.174V
    12.0V   1.283V
    13.0V   1.392V
    14.0V   1.501V
    15.0V   1.610V
    16.0V   1.719V
    18.0V   1.936V
    24.0V   2.154V
    27.0V   2.917V
    31.0V   3.352V
    35.1V   3.799V
    </pre>
2. SPEC
    1. REQ-0668502 (A)
        <pre>
        Input : UIH		
        Min : 5	
        Typ : /	
        Max : 16		
        Unit : V
        </pre>
    2. REQ-0668504 (A)
        <pre>
        Input : UIL		
        Min : -0.5	
        Typ : /	
        Max : 1.5		
        Unit : V
        </pre>
3. DESIGN
    1. ADC
    2. Sample Rate: 5ms ?
    3. Range: 1 ~ 24V


## POWER_HOLD
1. HARDWARE
   1. NET: PWR_HOLD
   2. PH/PL: N/A
   3. HVBUCK_EN ~= (WAKEUP - 0.22V (VF) ) * (140K/(360K+140K))
        <pre>
        When PWR_HOLD = LOW
        WAKEUP  HVBUCK_EN
        1.5V    0.358V
        4.0V    1.058V
        5.0V    1.338V

        When PWR_HOLD = HIGH
        WAKEUP  HVBUCK_EN
        5.0V    3.088V
        11.0V   3.088V
        12.0V   3.298V
        13.0V   3.578V
        14.0V   3.858V
        16.0V   4.418V
        18.0V   4.978V
        </pre>
2. SPEC
    1. N/A
3. DESIGN
    1. GPO, PushPull
    2. WAKEUP from LOW to HIGH: POWER_HOLD set HIGH before power on sequence
    3. WAKEUP from HIGH to LOW: POWER_HOLD set LOW  after power off sequence

## PCBA_NTC
1. HARDWARE
   1. NET: PCB_NTC_ADC
    <pre>
    PCB_NTC_ADC : (Typ)
    Temperature Vout
    -40         3.147
    -30         3.040
    -20         2.885
    -10         2.678
    0           2.419
    10          2.123
    20          1.810
    30          1.502
    40          1.219
    50          0.972
    60          0.766
    70          0.603
    80          0.473
    90          0.372
    </pre>
2. SPEC
    1. NCU15XH103F6SRC operating temperature range is -40~150 °C
    2. According to the hardware design, the reference temperature range is -40~90 °C
    3. over ? °C -> Derating
3. DESIGN
    1. ADC
    2. Sample Rate: 5ms ?

## BLU_NTC
1. HARDWARE
    1. NET: LED0_NTC_ADC / LED1_NTC_ADC
    <pre>
    BLU_NTC_ADC : (Typ)
    Temperature Vout
    -40         3.291
    -30         3.279
    -20         3.260
    -10         3.232
    0           3.191
    10          3.133
    20          3.055
    30          2.953
    40          2.824
    50          2.667
    60          2.484
    70          2.283
    80          2.068
    90          1.849
    100         1.632
    110         1.426
    120         1.236
    </pre> 
2. SPEC
    1. NCU15XH103F6SRC operating temperature range is -40~150 °C
    2. According to the hardware design, the reference temperature range is -40~120 °C
    3. over ? °C -> Derating
3. DESIGN
    1. ADC
    2. Sample Rate: 5ms ?

## PCBA_VER1
1. HARDWARE
   1. NET: PCBA_VER0_ADC
    <pre>
    Rtop    Rbom    Vout    REV.
    10K     NC      3.308   NotUsed
    1K      10K     3.007   [ M01 ]
    2.2K    10K     2.711   [ M02 ]
    10K     27K     2.414   [ ]
    27K     47K     2.101   [ ]
    10K     12K     1.804   [ ]
    12K     10K     1.504   [ ]
    47K     27K     1.207   [ ]
    27K     10K     0.894   [ ]
    10K     2.2K    0.597   [ ]
    10K     1K      0.301   [ ]
    NC      10K     0.000   NotUsed
    </pre>
2. SPEC
    1. N/A
3. DESIGN
    1. TBC

## PCBA_VER2
1. HARDWARE
   1. NET: PCBA_VER1_ADC
    <pre>
    Rtop    Rbom    Vout    REV.
    10K     NC      3.308   NotUsed
    1K      10K     3.007   [ M01 ]
    2.2K    10K     2.711   [ M02 ]
    10K     27K     2.414   [ ]
    27K     47K     2.101   [ ]
    10K     12K     1.804   [ ]
    12K     10K     1.504   [ ]
    47K     27K     1.207   [ ]
    27K     10K     0.894   [ ]
    10K     2.2K    0.597   [ ]
    10K     1K      0.301   [ ]
    NC      10K     0.000   NotUsed
    </pre>
2. SPEC
    1. N/A
3. DESIGN
    1. TBC
 

## FPC_DET
LCD FPC Connection Detector
<pre>
ADC     FPC_DET     ERR     < 2 V
</pre>   

## PMIC
<pre>
GPI     PG3V3       ERR     L
                    GOOD    H
GPI     PG1V8       ERR     L
                    GOOD    H
</pre>


## MAX96752
1. SERDES - Deserilizer  
 @startuml

title

Hardware Interface
| MCU | MAX96752 | Brief of MAX96752 |
| UART | SDA_RX | Main-Control |
| UART | SCL_TX | Main-Control |
| UART | SDA_RX1/SDA_RX2 | Pass-Through |
| UART | SCL_TX1/SCL_TX2 | Pass-Through |
| GPO | PWDNB | High = normal run, Low = power-down |
| GPI | LOCK | Open-Drain, Lock indication output |
| GPI | ERRB | Open-Drain, Error indication output |
| GPIO | GPIO 10 | NC |

end title

@enduml

### COMM    
1. SPEC
    1. UART/I2C
    2. SDA_RX1/SDA_RX2
        1. Pass-Through Serial Data/Receive. 
        2. UART receive with internal 40kΩ (typ) pullup to IOVDD. 
        3. I2C serial data input/open-drain output
        4. In UART mode, SDA_RX1/SDA_RX2 is the Rx input of the MAX96752’s UART.
        5. In I2C mode, SDA_RX1/SDA_RX2 is the SDA input/output of the MAX96752’s I2C
    3. SCL_TX1/SCL_TX2
        1. Pass-Through Serial Clock/Transmit. 
        2. UART transmit with internal 40kΩ (typ) pullup to IOVDD. 
        3. I2C clock input/open-drain output
        4. In UART mode, SCL_TX1/SCL_TX2 is the Tx output of the MAX96752’s UART.
        5. In I2C mode, SCL_TX1/SCL_TX2 is the SCL input/output of the MAX96752’s I2C
2. DESIGN
    1. UART
      1. 500KB, 8,1,E
      2. RX INT: Receive HOST CMD and reply immediately 
      3. TX DMA: Response in RX INT, so need TX DMA

### SERDES
1. SPEC
    1. UART/I2C
    2. SDA_RX
        1. Main-Control Serial Data/Receive
        2. UART receive with internal 40kΩ (typ) pullup to IOVDD. 
        3. I2C serial data input/open-drain output
        4. In UART mode, SDA_RX is the Rx input of the MAX96752’s UART.
        5. In I2C mode, SDA_RX is the SDA input/output of the MAX96752’s I2C
    3. SCL_TX
        1. Main-Control Serial Clock/Transmit. 
        2. UART transmit with internal 40kΩ (typ) pullup to IOVDD. 
        3. I2C clock input/open-drain output
        4. In UART mode, SCL_TX is the Tx output of the MAX96752’s UART.
        5. In I2C mode, SCL_TX is the SCL input/output of the MAX96752’s I2C
2. DESIGN
    1. UART
        1. 500Kbit, 8,1,E
        2. TX DMA Transmit CMD to MAX96752
        3. RX DMA Receive MAX96752 response    

### PWDNB   
1. SEPC
    1. GPI
        1. Set low to enter PowerDown mode.
        2. Active-low, Input with a 1MΩ Pulldown to Ground. 
        3. Attach pullup resistor to VDDIO for normal operation.
2. DESIGN
    1. GPO
        1. Set GPIO to output mode
        2. Set latch to low
        3. Set Drive Strength to normal

### ERRB     
1. SPEC    
    1. GPO
        1. Open-Drain Error Indication Output with an Internal 40kΩ Pullup to VDDIO. 
        2. When ERRB is low, indicates that a data error or interrupt was detected. 
        3. ERRB is high when PWDNB is low.
2. DESIGN
    1. GPI
        1. Set GPIO to input mode
        2. Set Pull-up
        3. Set Drive Strength to normal
                 
### LOCK    
1. SPEC    
    1. GPO
        1. LOCK = high indicates that PLLs are locked with correct 
           serial-word-boundary alignment. 
        2. LOCK = low indicates that PLLs are not locked or 
           an incorrect serial-word-boundary alignment. 
        3. LOCK remains low when the configuration link is active or 
           during PRBS test. 
        4. Open-Drain Lock Output with Internal 40kΩ Pullup to IOVDD.
        5. LOCK is high impedance when PWDN = low. 
        6. LOCK is an open-drain driver and requires a pullup resistor.
2. DESIGN
    1. GPI
        1. Set GPIO to input mode
        2. Set Pull-up
        3. Set Drive Strength to normal





## MPQ3364 X2

@startuml

title

MPQ3364#0 Hardware Interface
I2C Address: 0x38
| MCU | MPQ3364 | Brief of MPQ3364 |
| I2C | SCL/PSE | I2C clock input pin |
| I2C | SDA/PSE | I2C data input pin |
| PWM | DIM | PWM signal input pin |
| GPO | EN | IC enable pin |
| GPI | FF | Fault flag pin |

MPQ3364#1 Hardware Interface
I2C Address: 0x3A
| MCU | MPQ3364 | Brief of MPQ3364 |
| I2C | SCL/PSE | I2C clock input pin |
| I2C | SDA/PSE | I2C data input pin |
| PWM | DIM | PWM signal input pin |
| GPO | EN | IC enable pin |
| GPI | FF | Fault flag pin |

end title

@enduml

### I2C    
1. SPEC
    1. I2C slave
    2. SCL/PSE
        1. Clock frequency max value is 400kHz
        2. SCL rise time: min:20ns, max:300ns
    3. SDA/PSE
        1. Data hold time min:0us, max:0.9us
        2. SDA rise time: min:20ns, max:300ns
2. DESIGN
    1. I2C master
        1. Speed mode: STANDARD_AND_FAST_MODE
        2. Clock frequency 400kHz
        3. SDA hold time 50-100 ns
        4. I2C rise time(Trise) in nano second: 100
        5. Disable 10-bit addressing

### DIM
1. SPEC
    1. PWM input
        1. Apply a PWM signal on the DIM pin for brightness control
        2. DIM is pulled low internally
        3. A 100Hz to 20kHz PWM signal is recommended
2. DESIGN
    1. PWM output
        1. Set TCC2 into PWM operating mode
        2. Set PWM frequency to 20kHz (PWM freq = clock source / (period value+1))

### EN
1. SPEC
    1. GPI
        1. Pull EN high to enable the IC
        2. Pull EN low to shut down the IC
2. DESIGN
    1. GPO
        1. Set GPIO to output mode
        2. Set latch to low
        3. Set Drive Strength to normal

### FF
1. SPEC
    1. GPO
        1. Open drain during normal operation
        2. FF is pulled low if a fault occurs
2. DESIGN
    1. GPI
        1. Set GPIO to input mode
        2. Set Drive Strength to normal

## NT51926

@startuml

title

Hardware Interface
| MCU | NT51926 | Brief of NT51926 |
| SPI | TCH_IO_SCK | Clock pin |
| SPI | TCH_IO_MOSI | data output pin |
| SPI | TCH_IO_MISO | data input pin |
| GPO | TCH_IO_CSN | chip select pin |
| I2C | SCL | I2C clock input pin |
| I2C | SDI | I2C data input pin |
| GPO | GRB | Global reset pin |
| GPO | TP_EN | Touch enable pin |
| GPI | TCH_ATTN | Touch interrupt pin |
| GPI | IND_OUT | Abnormal indicator pin |
| GPI | TDDI_DEBUG | TDDI debug cable detect |
| GPIO | FTE1 | Synchronize signal output to the system |
| GPIO | FTE2 | Synchronize signal output to the system |

end title

@enduml

### SPI    
1. SPEC
    1. SPI slave
    2. TCH_IO_SCK
        1. Speed in Hz: 5.1MHz
        2. Touch serial communication clock input 
        3. Normally pulled low
    3. TCH_IO_MOSI
        1. Touch serial communication data input pin 
        2. Normally pulled low
    3. TCH_IO_MISO
        1. Touch serial communication data output pin
    3. TCH_IO_CSN
        1. Touch serial communication chip selection
        2. Normally pulled high
2. DESIGN
    1. SPI master
        1. Speed in Hz: 5.1MHz
        2. SPI Transfer Mode 0
    2. GPO
        1. Set GPIO to output mode for controll TCH_IO_CSN
        2. Set latch to high
        3. Set Drive Strength to normal

### I2C    
1. SPEC
    1. I2C slave
    2. SCL
        1. Clock frequency max value is 1MHz
        2. Serial communication clock input
    3. SDI
        1. Data hold time min value is 0ns
        2. Serial communication data input pin
2. DESIGN
    1. I2C master
        1. Speed mode: STANDARD_AND_FAST_MODE
        2. Clock frequency 400kHz
        3. SDA hold time 50-100 ns
        4. I2C rise time(Trise) in nano second: 100
        5. Disable 10-bit addressing

### GRB
1. SPEC
    1. GPI
        1. Global reset pin
        2. GRB = “H”, normal operation
        3. GRB = “L”, The controller is in reset state
2. DESIGN
    1. GPO
        1. Set GPIO to output mode
        2. Set latch to low
        3. Set Drive Strength to normal

### TP_EN
1. SPEC
    1. GPI
        1. Touch enable pin
        2. TP_EN = “H”, normal operation
        3. TP_EN = “L”, The Touch controller is into reset state
        4. Order of priority GRB > TP_EN
2. DESIGN
    1. GPO
        1. Set GPIO to output mode
        2. Set latch to low
        3. Set Drive Strength to normal

### TCH_ATTN
1. SPEC
    1. GPO
        1. Touch interrupt pin
        2. According to practical experience, when the NT51926 reports position data, 
           the TCH_ATTN will be pulled HIGH until the MCU retrieves the position data.
2. DESIGN
    1. GPI
        1. Set GPIO to input mode
        2. Set pull up 
        3. Set Drive Strength to normal

### IND_OUT
1. SPEC
    1. GPO
        1. Abnormal indicator pin
        2. Abnormal indicator pin is connected to a pulled high resistor
        3. This pin output low when abnormal
2. DESIGN
    1. GPI
        1. Set GPIO to input mode
        2. Set Drive Strength to normal

### TDDI_DEBUG
1. SPEC
    1. GPO
        1. TDDI debug cable detect
        2. High : cable connected to TDDI debug/test      
2. DESIGN
    1. GPI
        1. Set GPIO to input mode
        2. Set Drive Strength to normal

### FTE1
1. SPEC
    1. GPO
        1. Synchronize signal output to the system
        2. The FTE pin can select to output control signals via register setting
        3. please refer to the register table (CMD1 Page0, 10h)
2. DESIGN
    1. GPIO
        1. Set to HIGH Impedance
        2. Not use

### FTE2
1. SPEC
    1. GPO
        1. Synchronize signal output to the system
        2. The FTE pin can select to output control signals via register setting
        3. please refer to the register table (CMD1 Page0, 10h)      
2. DESIGN
    1. GPIO
        1. Set to HIGH Impedance
        2. Not use

# Software Block Diagram
@startuml
skinparam componentStyle rectangle

component IO {

    component uart
    component i2c
    component spi

    component gpi
    component gpo

    component pwm
    component adc
}

component UTL {

    component stdio
    component clocker
}

component CORE {

    component cpu
    component flash
    component ram
    component watchdog
    component stack
    component timer
}

@enduml

## cpu

## flash

## ram

## watchdog
  counter: 50ms

## stack

## timer

## stdio

## clocker


