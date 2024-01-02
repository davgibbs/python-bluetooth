"""
An application to get the values of the Arduino Nano via BLE
Run with: (venv) davidg@davidg:~/python-bluetooth $ python app.py
Python 3.10.12
"""
import asyncio
import datetime
import os
import struct

from bleak import BleakClient, BleakScanner


# todo make sure it connects again automatically
# https://bleak.readthedocs.io/en/latest/api/scanner.html#starting-and-stopping

async def run():
    """
    Run the the loop to search for devices, in particular an Arduino sensor
    """
    found = False
    # await keyword must be inside an async function
    devices = await BleakScanner.discover(timeout=4.0)
    print(f'Devices: {devices}')
    for d in devices:
        # Maybe rename to "Knee Sleeve Arduino Nano 33 BLE" to be specific. and find by name
        if 'Arduino Nano 33 BLE' in d.name:
            print('Found Arduino Nano 33 BLE Sense Peripheral')
            found = True
            print(f'{d.name=}')  # The advertised name of the device
            print(f'{d.address=}')  # Address, can change when Arduino code edited, so use name to find
            print(f'{d.rssi=}')  # Signal strength

            async with BleakClient(d.address) as client:
                print(f'Connected to {d.address}')


if __name__ == "__main__":
    loop = asyncio.get_event_loop()
    try:
        loop.run_until_complete(run())
    except KeyboardInterrupt:
        print('\nReceived Keyboard Interrupt')
    finally:
        print('Program finished')

