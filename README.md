# HomeEnvMonitor
A Raspberry Pi and Arduino environmental sensor project

Currently using Raspberry Pi 4B (Raspi OS, June 2020) and Arduino Uno rev 3

wlog_cont.py continuously reads BMP180 DS18B20 DHT22 sensors direct over Raspberry Pi GPIO and writes to log file

env_sensor/ contains Arduino sketch to read PPD42, MQ9, MQ135 sensors and print values to serial

TODO: write Arduino serial out to log file, combine log files and adjust timing (1/hour?) for long runs
