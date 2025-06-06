# Cyber-Physical System: ARM Microcontroller with Grafana Server Integration

A comprehensive cyber-physical system that interfaces a TM4C123GH6PM ARM microcontroller with cloud-based monitoring and control through Telegram bot integration, InfluxDB data storage, and Grafana visualization.

## 🏗️ System Architecture

This project implements a multi-layered cyber-physical system:

```
┌─────────────────┐    ┌──────────────────┐    ┌─────────────────┐
│   Telegram Bot  │◄──►│  Python Server   │◄──►│ ARM Controller  │
│  (Remote UI)    │    │   (Data Hub)     │    │  (Hardware)     │
└─────────────────┘    └──────────────────┘    └─────────────────┘
                              │
                              ▼
                       ┌──────────────────┐
                       │   InfluxDB +     │
                       │    Grafana       │
                       │ (Data Storage &  │
                       │ Visualization)   │
                       └──────────────────┘
```

## 🔧 Hardware Components

### ARM Microcontroller (TM4C123GH6PM)
- **Temperature Sensor**: Internal ADC-based temperature measurement
- **PWM Control**: 4-channel PWM for LEDs and fan control
- **UART Communication**: Serial interface at 115200 baud
- **GPIO Control**: LED control (Red/Blue) and fan speed regulation
- **Command Shell**: Built-in command processor for real-time control

### Supported Hardware Commands
```c
- help          : Display available commands
- led [r|b] [on|off] : Control red/blue LEDs
- readTemp [c|f]     : Read temperature in Celsius/Fahrenheit  
- fanCtrl [0-10]     : Set fan duty cycle (0=off, 10=max)
```

## 🐍 Python Server Components

### Core Classes

#### `class_tiva.py` - Hardware Interface
- **Serial Communication**: UART interface with ARM controller
- **Command Execution**: Send commands and receive responses
- **Auto-detection**: Handles `/dev/ttyACM0` serial port

#### `class_BOT.py` - Telegram Integration  
- **Remote Control**: Execute hardware commands via Telegram
- **Alert Processing**: Automated responses to temperature alerts
- **Command Parser**: Parse user commands with `@` prefix
- **Logging**: Complete conversation logging

#### `class_DB.py` - Database Management
- **InfluxDB Client**: Time-series data storage
- **Auto-configuration**: Database creation and connection management
- **Data Writing**: JSON-formatted sensor data storage

#### `class_debugger.py` - Logging System
- **File Logging**: Comprehensive system logging
- **Timestamp Tracking**: All events timestamped
- **Error Handling**: Robust error logging and debugging

## 🚀 Key Features

### 1. Real-time Monitoring
- **Temperature Sensing**: Continuous temperature monitoring
- **CPU Usage Tracking**: System performance monitoring  
- **Data Logging**: 1-second interval data collection
- **Time-series Storage**: InfluxDB integration for historical data

### 2. Remote Control via Telegram
```
Telegram Commands:
• @ : LED : [r|b] : [on|off]     - Control LEDs
• @ : FANCTRL : [0-10]           - Set fan speed  
• @ : READTEMP : [c|f]           - Read temperature
• help                           - Show command list
```

### 3. Automated Control System
- **PID Control**: Temperature-based fan speed regulation
- **Alert System**: Automatic responses to temperature thresholds
- **Feedback Loop**: Closed-loop control implementation

### 4. Data Visualization
- **InfluxDB Integration**: Time-series database storage
- **Grafana Compatibility**: Ready for dashboard creation
- **Real-time Graphs**: Temperature and CPU usage tracking

## 📁 Project Structure

```
Cyber_pysical_system/
├── src_c/                          # ARM Microcontroller Code
│   ├── main.c                      # Hardware initialization (PWM, ADC, UART)
│   ├── shell.c                     # Command processor and hardware control
│   ├── shell.h                     # Function declarations and definitions
│   ├── startup_rvmdk.S            # ARM startup and interrupt handlers
│   ├── backups_shell/             # Shell implementation backups
│   └── examples_do_not_edit/      # Reference examples
├── src_py/                         # Python Server Components  
│   ├── tiva_if_continous_temp_read.py  # Main application orchestrator
│   ├── class_BOT.py               # Telegram bot implementation
│   ├── class_DB.py                # InfluxDB database client
│   ├── class_tiva.py              # Serial communication interface
│   ├── class_debugger.py          # Logging and debugging system
│   ├── tiva_debug_IF.py           # Simple UART read/write utilities
│   ├── logs/                      # System logs and bot conversations
│   ├── screenshots/               # Documentation screenshots
│   └── email_client/              # Email notification system
├── Keil_ARM_project/               # ARM Development Environment
│   ├── shell.uvprojx              # Keil μVision project file
│   ├── shell.uvoptx               # Project options and settings
│   ├── Objects/                   # Compiled object files
│   └── Listings/                  # Assembly listings and maps
└── README.md                      # This documentation
```

## ⚙️ Installation & Setup

### Prerequisites
```bash
# Python Dependencies
pip install pyserial influxdb telebot psutil pytz

# Hardware Requirements  
- TM4C123GH6PM ARM Microcontroller
- Keil μVision IDE for ARM development
- USB cable for serial communication
```

### Database Setup
```bash
# Install InfluxDB
sudo apt-get install influxdb
sudo systemctl start influxdb

# Install Grafana (optional)
sudo apt-get install grafana
sudo systemctl start grafana-server
```

### Configuration Steps

1. **ARM Controller Setup**:
   - Load `Keil_ARM_project/shell.uvprojx` in Keil μVision
   - Compile and flash to TM4C123GH6PM
   - Connect via USB (appears as `/dev/ttyACM0`)

2. **Python Server Configuration**:
   ```python
   # In tiva_if_continous_temp_read.py
   GLOBAL_bot_token = "YOUR_TELEGRAM_BOT_TOKEN"  # Required!
   ```

3. **Database Configuration**:
   ```python
   # Default settings in class_DB.py
   IFhost = "localhost"
   IFport = 8086  
   IFDbname = 'CPU'
   ```

## 🎯 Usage Examples

### Start the System
```bash
cd src_py/
python3 tiva_if_continous_temp_read.py
```

### Telegram Bot Commands
Send these messages to your Telegram bot:
```
@ : LED : r : on        # Turn on red LED
@ : FANCTRL : 5         # Set fan to 50% speed
@ : READTEMP : c        # Read temperature in Celsius
help                    # Show all commands
```

### Direct Serial Commands
```bash
python3 tiva_debug_IF.py
# Then type: readTemp c, fanCtrl 3, led b on, etc.
```

## 📊 Data Flow

1. **Sensor Reading**: ARM controller reads temperature via ADC
2. **Serial Communication**: Python server requests data every second  
3. **Database Storage**: Data stored in InfluxDB with timestamps
4. **Telegram Integration**: Commands sent via bot, responses returned
5. **Automated Control**: PID controller adjusts fan based on temperature
6. **Logging**: All activities logged for debugging and analysis

## 🎛️ Control Features

### Manual Control
- **LED Control**: Independent red/blue LED control
- **Fan Speed**: 11 levels (0-10) of PWM duty cycle
- **Temperature Reading**: Real-time temperature in C° or F°

### Automatic Control  
- **Temperature Alerts**: Configurable temperature thresholds
- **PID Control**: Proportional-Integral-Derivative fan control
- **Feedback Loop**: Automatic fan speed adjustment

## 📈 Monitoring & Visualization

### Built-in Logging
- **System Logs**: Complete operation logging in `logs/`
- **Bot Conversations**: Telegram chat history
- **Error Tracking**: Comprehensive error logging

### Database Integration
- **Time-series Data**: Temperature and CPU usage over time
- **Grafana Ready**: Direct integration with Grafana dashboards
- **Real-time Updates**: Live data streaming

## 🔧 Development & Customization

### Adding New Commands
1. **ARM Side**: Add command to `CmdTable[]` in `shell.c`
2. **Python Side**: Update command parsing in `class_BOT.py`
3. **Telegram**: Add to help string in main application

### Extending Sensors
1. **Hardware**: Initialize new ADC channels in `main.c`
2. **Shell**: Add new read functions in `shell.c`  
3. **Python**: Update data collection in main loop

## 🐛 Troubleshooting

### Common Issues
- **Serial Port**: Ensure `/dev/ttyACM0` permissions: `sudo chmod 666 /dev/ttyACM0`
- **Bot Token**: Must set `GLOBAL_bot_token` before running
- **Database**: Ensure InfluxDB is running: `sudo systemctl status influxdb`
- **ARM Connection**: Press reset button when prompted during startup

### Debug Mode
```python
# Enable verbose logging
Verbose_GLOBAL = True
```

## 📄 License & Credits

**Author**: Rishabh Dubey  
**Institution**: Indian Institute of Science (IISc)  
**Project Type**: Cyber-Physical Systems Research

## 🔗 Related Technologies

- **ARM Cortex-M4**: TM4C123GH6PM microcontroller
- **TivaWare**: ARM peripheral driver library
- **InfluxDB**: Time-series database
- **Grafana**: Data visualization platform  
- **Telegram Bot API**: Remote control interface
- **Python Serial**: UART communication library

---

*This project demonstrates the integration of embedded systems, real-time control, remote monitoring, and data visualization in a comprehensive cyber-physical system architecture.*
