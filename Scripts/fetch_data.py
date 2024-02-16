#################################

# Use if C++ program doesn't work

#################################

import subprocess
from time import sleep
import serial
from random import randint

OUTPUT_FILE: str = "../Server/Host/data.txt"
ARDUINO_PORT: str = "/dev/ttyACM0"
ARDUINO_BAUD: int = 9600
MAX_WAIT_TIME: int = 1

def fetch_temperature() -> None:
    # Pipes CPU temperature into output file
    _ = subprocess.getoutput("vcgencmd measure_temp").replace("temp=", "")
    return _.replace("'C", "")

def fetch_Arduino() -> None:
    # Initialize connection with Arduino and return its output
    # If anything crashes, the default return becomes 50
    ser = None

    try:
        while True:
            if ser is None or not ser.is_open:
                try:
                    ser = serial.Serial(ARDUINO_PORT, ARDUINO_BAUD, timeout=1)
                    print(f"Opened serial connection to Arduino on {ARDUINO_PORT}")
                except serial.SerialException as e:
                    print(f"Error opening serial connection: {e}")
                    continue

            try:
                # Read data from Arduino
                data = ser.readline().decode('utf-8').strip()

                # Do something with the received data
                if data:
                    # Save the received data to a file
                    with open(OUTPUT_FILE, 'w') as file:
                        file.write(str(randint(0, 10))); file.write("\n")
                        file.write(data);  file.write("\n")
                        file.write(fetch_temperature())
                    sleep(1)

            except serial.SerialException as e:
                print(f"Error reading from Arduino: {e}")
                continue
        
    finally:
        if ser and ser.is_open:
            ser.close()

def main():
    fetch_Arduino()
    
if __name__ == "__main__":
    main()
