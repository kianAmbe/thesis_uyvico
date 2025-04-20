import serial
import time
import smtplib
from email.mime.text import MIMEText


ser = serial.Serial('COM5', 9600, timeout=1)
time.sleep(2)

# Email configuration
EMAIL = 'kristoffeambefb@gmail.com'
PASSWORD = 'wtkhgpniroqgsxrl'  # Use App Password if using Gmail
TO_EMAIL = 'uyvico.thomasjhae.bio@gmail.com'

def send_email(subject, body):
    msg = MIMEText(body)
    msg['Subject'] = subject
    msg['From'] = EMAIL
    msg['To'] = TO_EMAIL

    with smtplib.SMTP_SSL('smtp.gmail.com', 465) as server:
        server.login(EMAIL, PASSWORD)
        server.send_message(msg)

# Main loop
buffer = []
start_time = time.time()
interval = 60  # 1 minute

while True:
    line = ser.readline().decode('utf-8').strip()
    if line.startswith("Avg PM1.0:"):
        print(line)
        buffer.append(line)

    # Send every 1 minute
    if time.time() - start_time > interval:
        if buffer:
            full_report = "\n".join(buffer)
            send_email("1-Minute PM Sensor Report", full_report)
            print("Email sent!")
            buffer = []  # Clear buffer
        start_time = time.time()
