#!/usr/bin/python
# -*- coding: utf-8 -*-
#
# Program to receive packets from the radio link of two
# Arduinos and display the packet contents.
#

# Set up our libraries and GPIO modes.
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
from lib_nrf24 import NRF24
import time
import spidev

# Set up addresses for sending and receiving data.
pipes = [[0xe7, 0xe7, 0xe7, 0xe7, 0xe7], [0xc2, 0xc2, 0xc2, 0xc2, 0xc2]]

#Set up our radio using the NRF24 portion of the lib_nrf24 library.
radio2 = NRF24(GPIO, spidev.SpiDev())
radio2.begin(0, 25)

radio2.setRetries(15,15)

radio2.setPayloadSize(32)
radio2.setChannel(0x6f)
radio2.setDataRate(NRF24.BR_2MBPS)
radio2.setPALevel(NRF24.PA_MIN)

radio2.setAutoAck(False)
radio2.enableDynamicPayloads()
radio2.enableAckPayload()

radio2.openWritingPipe(pipes[0])
radio2.openReadingPipe(1, pipes[1])

radio2.startListening()
radio2.stopListening()
# Output the results of our setup to the screen.
radio2.printDetails()
# Restart the radio.
radio2.startListening()
# This loop will run indefinitely to listen for packets on the radio.
while True:
    pipe = [0]
    # If there is not data from the radio, wait breifly and then check again.
    while not radio2.available(pipe):
        time.sleep(10000/1000000.0)
    # We have data from the radio and will put it in a buffer.
    recv_buffer = []
    radio2.read(recv_buffer, radio2.getDynamicPayloadSize())
    # If our buffer is actually empty let's print ... to the screen.
    if not recv_buffer:
        print ("...")
    # Our buffer has something so let's process it.
    else:
        # Let's see the raw data from the buffer.
        print ("Received: {}".format(recv_buffer))
        # That raw data isn't very usefule to us as humans so let's convert it.
        print("Translating our message into unicode characters...")
        string = ""
        for n in recv_buffer:
            if (n >= 32 and n <= 126):
                string += chr(n)
        print("Our message decodes to: {}".format(string))