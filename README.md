# ESP32 Precision Event Counter

A high-precision dual-input counter built around the ESP32, featuring a premium Cherry MX Red linear switch and real-time wireless monitoring. With over 40,000 verified actuations, this device delivers reliable event tracking for both professional and recreational applications.

## Core Features
- Dual-input counting system (ESP32 BOOT + Cherry MX Red)
- Real-time wireless monitoring via web interface
- Persistent count storage across power cycles
- Smart power management with auto-WiFi shutdown
- Professional-grade debouncing (20ms)
- Smooth LED transitions with status indication
- Multi-client web interface support

## Applications
- Production line quality control
- Event attendance tracking
- Scientific data collection
- Sports performance metrics
- Rhythm training
- Focus enhancement

## Hardware Requirements
### Basic Setup
- ESP32 DevKit (Any variant with GPIO2 LED)
- USB Cable

### Enhanced Setup
- Cherry MX Red Switch
- 102050 Li-Po Battery + TP4056 Charger
- 0.96" OLED Display (SSD1315)
- 3D Printed Case

## Quick Start Guide
1. Upload code to ESP32
2. Press either button to count
3. Hold any button 5s to enable WiFi
4. Connect to "ClickCounter" network (password: 12345678)
5. Visit 192.168.4.1 in your browser

## LED Indicators
- Smooth fade to 50% on button press
- 2/255 constant when WiFi active
- Two blinks: WiFi enabling
- Three blinks: WiFi disabling

## Technical Specifications
- Input Response: 20ms debounce
- Web Update Rate: 15Hz
- WiFi Auto-shutdown: 5 minutes
- Storage: Non-volatile flash memory
- Power Draw: Optimized for battery operation
- Switch Rating: 50M+ actuations

## Pin Configuration
- GPIO0: Built-in BOOT button
- GPIO15: Cherry MX input
- GPIO2: Status LED

## Power Features
- Auto WiFi shutdown after 5min inactivity
- LED brightness optimization
- Efficient sleep modes
- USB-C charging support

## Web Interface
- Real-time counter updates
- Large, clear display
- Mobile-responsive design
- Automatic number formatting
- Multiple client support

## Performance
- Current Record: 40,000+ actuations
- Expected Lifetime: 50M+ clicks
- Memory Usage: ~30%
- Flash Usage: ~25%

## Future Enhancements
- OLED Display Integration
- Battery Monitoring
- Multiple Counter Modes
- Data Export Features
- Network Time Sync

## Serial Monitor
Baud Rate: 115200
Outputs:
- Counter values
- WiFi status changes
- System events

## Known Issues
None reported - stable and reliable operation

## Version History
- v1.0.0: Initial Release
  - Dual input support
  - WiFi monitoring
  - Persistent storage
  - Power management

## Community
Join our growing community of precision tracking enthusiasts. Share your applications and achievements!

## License
GNU General Public License v3.0 - See LICENSE file for details

## Project Status
Active Development - Regular updates and enhancements planned

---
Built with precision, maintained with passion. Join the counting revolution! 🎯