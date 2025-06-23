import serial
import threading
import os
import time

# --- Configuration ---
SERIAL_PORT = 'COM4'
BAUD_RATE = 115200
SAVE_PATH = 'D:\Learning\HabibUniversity\STRP\Tehqiq_1_IoT\LocationsData\CommLabFinal'
FILE_NAME = 'serial_output.txt'  # Change this if needed



file_path = os.path.join(SAVE_PATH, FILE_NAME)

# --- Ensure save directory exists ---
os.makedirs(SAVE_PATH, exist_ok=True)

# --- Variables ---
start_saving = False
data_buffer = []

def wait_for_enter():
    global start_saving
    input("Press ENTER to save printed serial data to file...\n")
    start_saving = True
    with open(file_path, 'w') as f:
        for line in data_buffer:
            f.write(line + '\n')
        print(f"\n[✔] Data saved to: {file_path}")

def read_serial():
    try:
        with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser:
            print(f"Reading from {SERIAL_PORT} at {BAUD_RATE} baud...\n")
            while True:
                line = ser.readline().decode(errors='replace').strip()
                if line:
                    print(line)
                    data_buffer.append(line)
    except serial.SerialException as e:
        print(f"[✘] Serial error: {e}")
        time.sleep(1)
        read_serial()
    except KeyboardInterrupt:
        print("\n[!] Exiting program...")

if __name__ == "__main__":
    # Start thread to wait for ENTER
    threading.Thread(target=wait_for_enter, daemon=True).start()
    read_serial()