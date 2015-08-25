#!/usr/bin/python
"""Update a stoplight with Naglite3/nagios info.

Read from a Naglite3 server and parse for warnings or critical messages.

Return a color code ("R", "Y", "G") based on if the status is good/green,
warning/yellow, or critical/red.
"""

import serial
import time
import requests
from HTMLParser import HTMLParser


# Change this to match your USB identifier:
arduino_usb = '/dev/tty.usbserial-A6026U8D'

# URLs of the Naglite3 servers to collect the data from:
naglite_ops = 'http://nagios.fulcrum.net/Naglite3/'
naglite_prod = 'http://nagios.fulcrumhosted.com/Naglite3/'


class checkWarningParser(HTMLParser):

    """Look for warnings and critical events from the HTML."""

    returnLevel = 3

    def handle_starttag(self, tag, attrs):
        """Look for <tr> tags that have warnings or criticals attached."""
        if tag == 'tr':
            for name, crit in attrs:
                if name == 'class' and crit == 'warning':
                    if self.returnLevel < 2:
                        self.returnLevel = 1
                if name == 'class' and crit == 'critical':
                    self.returnLevel = 2
                if name == 'class' and crit == 'down':
                    self.returnLevel = 2

warningParser = checkWarningParser()

ser = serial.Serial(arduino_usb, 9600)

ser.setDTR(False)

time.sleep(2)
ser.write(b'B')

while True:
    warningParser.returnLevel = 0
    r = requests.get(naglite_ops)
    warningParser.feed(r.text)
    ops = warningParser.returnLevel

    warningParser.returnLevel = 0
    r = requests.get(naglite_prod)
    warningParser.feed(r.text)
    prod = warningParser.returnLevel

    value = max(ops, prod)

    if value == 0:
        ser.write(b'G')
    elif value == 1:
        ser.write(b'Y')
    elif value == 2:
        ser.write(b'R')

    time.sleep(30)
