# ATOMS3Nunchuk

Wii ヌンチャク RVL-004をBluetoothマウスとして使うテスト。  
Arduino IDEでATOMS3を利用する前提のプログラムとなっているが、ほかのESP32系のデバイスでも動く可能性がある。

I2Cのヌンチャクとのやり取りは[DigistumpNunchuk](https://github.com/bigw00d/DigistumpNunchuk)のコードを利用。  
Bluetoothマウスの処理は[ESP32-BLE-Mouse](https://github.com/T-vK/ESP32-BLE-Mouse)を利用。

## License notice

以下のレポジトリのソースコードを一部利用している。  
[bigw00d/DigistumpNunchuk](https://github.com/bigw00d/DigistumpNunchuk) Apache License, Version 2.0