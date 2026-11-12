import threading
import time
import serial

PORT = 'COM9'  # Update to your serial port (e.g., '/dev/ttyUSB0' on Linux)
BAUD_RATE = 9600


def calculate_nmea_checksum(sentence_data: str) -> str:
  """Calculates the 8-bit XOR checksum for NMEA sentences."""
  calc = 0
  for char in sentence_data:
    calc ^= ord(char)
  return f'{calc:02X}'


# Base NMEA payload for Manching, Germany (without $ and checksum)
nmea_payload = 'GNGGA,123519.00,4842.90,N,01129.40,E,1,08,0.9,371.2,M,47.0,M,,'

# Automatically generate the full sentence with the correct checksum
checksum_hex = calculate_nmea_checksum(nmea_payload)
gnss_sentence = f'${nmea_payload}*{checksum_hex}\r\n'.encode('ascii')


def read_from_port(ser):
  """Continuously reads incoming data from the serial port."""
  try:
    while ser.is_open:
      line = ser.readline()
      if line:
        try:
          decoded = line.decode('ascii', errors='ignore').strip()
          if decoded:
            print(f'Received: {decoded}')
        except Exception:
          pass
  except serial.SerialException:
    pass


try:
  with serial.Serial(PORT, BAUD_RATE, timeout=0.1) as ser:
    print(
        f'Streaming Manching GNGGA data (Checksum: *{checksum_hex}) on {PORT}'
        f' at {BAUD_RATE} baud. Press Ctrl+C to exit.'
    )

    # Start a background thread to handle incoming messages
    read_thread = threading.Thread(target=read_from_port, args=(ser,), daemon=True)
    read_thread.start()

    # Main loop sends the GNSS sentence every second
    while True:
      ser.write(gnss_sentence)
      print(f'Sent: {gnss_sentence.decode("ascii").strip()}')
      time.sleep(1.0)

except serial.SerialException as e:
  print(f'Error opening or writing to serial port: {e}')
except KeyboardInterrupt:
  print('\nStream stopped by user.')