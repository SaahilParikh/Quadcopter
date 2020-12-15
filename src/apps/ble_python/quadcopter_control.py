#!/usr/bin/env python3

import struct
from bluepy.btle import Peripheral, DefaultDelegate
import argparse

parser = argparse.ArgumentParser(description='Print advertisement data from a BLE device')
parser.add_argument('addr', metavar='A', type=str, help='Address of the form XX:XX:XX:XX:XX:XX')
args = parser.parse_args()
addr = args.addr.lower()
if len(addr) != 17:
    raise ValueError("Invalid address supplied")

DISPLAY_SERVICE_UUID = "62295813-af1c-3eb5-4a07-c92f9053c5c5"

DISPLAY_CHAR_UUID    = "6229108a-af1c-3eb5-4a07-c92f9053c5c5"


try:
    print("connecting")
    buckler = Peripheral(addr)

    print("connected")

    # Get service
    sv = buckler.getServiceByUUID(DISPLAY_SERVICE_UUID)
    # Get characteristic
    ch = sv.getCharacteristics(DISPLAY_CHAR_UUID)[0]

    while True:
        pwm_duty = input("Enter a duty cycle: ")
        pwm_duty = int(pwm_duty)
        ch.write(pwm_duty.to_bytes(4, "little"))
finally:
    print("disconnecting...")
    buckler.disconnect()
    buckler.disconnect()
