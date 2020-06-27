#!/usr/bin/python
# Copyright (c) 2020 Mark E. Fuller (mark.e.fuller@gmx.de)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

from time import sleep
import math
import logging

#pull in sensors
import Adafruit_BMP.BMP085 as BMP085
from ds18b20_therm import DS18B20
import Adafruit_DHT

#BMP180: pressure/temperature/altitude
sensor = BMP085.BMP085(mode=BMP085.BMP085_ULTRALOWPOWER)

#DS18B20 temperature probe
probe = DS18B20()

#DHT22 temperature and relative humidity
DHT_SENSOR = Adafruit_DHT.DHT22
DHT_PIN = 17

#logfile name
logfilename='env_cont.log'

#write header if file doesn't exist
try:
	with open(logfilename,'x') as logfile:
		logfile.write('Wohnzimmer Wetter\tDruck_BMP180 (Pa)\tTemperatur_BMP180 (K)\tTemperatur_DS18B20 (K)\tTemperatur_DHT22 (K)\tFeuchtigkeit_DHT22 (%)\tTaupunkt (K)\n')
except:
	pass

#logfile
logging.basicConfig(filename=logfilename,
                            filemode='a',
                            format='%(asctime)s,%(msecs)d %(name)s %(levelname)s %(message)s',
                            datefmt='%H:%M:%S',
                            level=logging.INFO)

print('Wohnzimmer Wetter\tDruck_BMP180 (Pa)\tTemperatur_BMP180 (K)\tTemperatur_DS18B20 (K)\tTemperatur_DHT22 (K)\tFeuchtigkeit_DHT22(%)\tTaupunkt (K)')

logger = logging.getLogger('wlog')

#report values
while True:
	#read BMP180
	try:
		T180 = sensor.read_temperature()
		p180 = sensor.read_pressure()
	except:
		logger.error('Cannot read BMP180')
		p180 = 0.0
		T180 = 0.0
	
	#read DS18B20
	try:
		Tdsb = probe.read_temp()
	except:
		logger.error('Cannot read DS18B20')
		Tdsb = 0.0
	
	#read DHT22
	try:
		RHdht, Tdht = Adafruit_DHT.read_retry(DHT_SENSOR, DHT_PIN)
	except:
		logger.error('Cannot read DHT22')
		RHdht = 0.0
		Tdht = 0.0	
	
	#calculate dew point (Magnus parameters; 'a' not needed here)
	a = 611.2 #Pa
	b = 17.62
	c = 243.12 #degC
	d = math.log(RHdht/100.)
	T = Tdht #choose source for temperature
	f = d + ((b*T)/(c+T))
	DP = (c*f)/(b-f)

	message = f'{p180:0.2f}\t{(273.15 + T180):0.2f}\t{(273.15 + Tdsb):0.2f}\t{(273.15 + Tdht):0.2f}\t{RHdht:0.2f}\t{(273.15 + DP):0.2f}'
	print(message)
	logger.info(message)
	sleep(10)
