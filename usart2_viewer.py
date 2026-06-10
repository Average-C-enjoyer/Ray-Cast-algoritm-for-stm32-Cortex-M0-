import serial
import serial.tools.list_ports
import time


def find_stm32_port():
    ports = serial.tools.list_ports.comports()

    for p in ports:
        if "usbmodem" in p.device.lower():
            return p.device

    return None


def main():
    port = find_stm32_port()

    if not port:
        print("STM32 not found")
        return

    print(f"Using port: {port}")

    ser = serial.Serial(port, 115200, timeout=1)

    time.sleep(2)  # reset time for STM32

    while True:
        try:
            data = ser.read(ser.in_waiting or 1)

            if data:
                try:
                    print(data.decode('utf-8', errors='replace'), end='')
                except:
                    print(data)

        except KeyboardInterrupt:
            break

    ser.close()


if __name__ == "__main__":
    main()
