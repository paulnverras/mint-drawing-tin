#!/usr/bin/python
# -*- coding:utf-8 -*-

from . import config
import RPi.GPIO as GPIO
import time
import numpy as np


class ST7789(object):
    def __init__(self):
        self.width = 240
        self.height = 320
        
        self._dc = config.DC_PIN
        self._rst = config.RST_PIN
        self._bl = config.BL_PIN
        self._cs = config.CS_PIN



    """    Write register address and data     """
    def command(self, cmd):
        config.digital_write(self._cs, GPIO.LOW)
        config.digital_write(self._dc, GPIO.LOW)
        config.spi_writebyte([cmd])
        config.digital_write(self._cs, GPIO.HIGH)

    def data(self, val):
        config.digital_write(self._cs, GPIO.LOW)
        config.digital_write(self._dc, GPIO.HIGH)
        config.spi_writebyte([val])
        config.digital_write(self._cs, GPIO.HIGH)

    def Init(self):
        """Initialize dispaly""" 
        config.module_init()
        
        self.reset()
        
        self.command(0x36)
        self.data(0x00) 

        self.command(0x3A) 
        self.data(0x05)

        self.command(0x21) 

        self.command(0x2A)
        self.data(0x00)
        self.data(0x00)
        self.data(0x01)
        self.data(0x3F)

        self.command(0x2B)
        self.data(0x00)
        self.data(0x00)
        self.data(0x00)
        self.data(0xEF)

        self.command(0xB2)
        self.data(0x0C)
        self.data(0x0C)
        self.data(0x00)
        self.data(0x33)
        self.data(0x33)

        self.command(0xB7)
        self.data(0x35) 

        self.command(0xBB)
        self.data(0x1F)

        self.command(0xC0)
        self.data(0x2C)

        self.command(0xC2)
        self.data(0x01)

        self.command(0xC3)
        self.data(0x12)   

        self.command(0xC4)
        self.data(0x20)

        self.command(0xC6)
        self.data(0x0F) 

        self.command(0xD0)
        self.data(0xA4)
        self.data(0xA1)

        self.command(0xE0)
        self.data(0xD0)
        self.data(0x08)
        self.data(0x11)
        self.data(0x08)
        self.data(0x0C)
        self.data(0x15)
        self.data(0x39)
        self.data(0x33)
        self.data(0x50)
        self.data(0x36)
        self.data(0x13)
        self.data(0x14)
        self.data(0x29)
        self.data(0x2D)

        self.command(0xE1)
        self.data(0xD0)
        self.data(0x08)
        self.data(0x10)
        self.data(0x08)
        self.data(0x06)
        self.data(0x06)
        self.data(0x39)
        self.data(0x44)
        self.data(0x51)
        self.data(0x0B)
        self.data(0x16)
        self.data(0x14)
        self.data(0x2F)
        self.data(0x31)
        self.command(0x21)

        self.command(0x11)

        self.command(0x29)

    def reset(self):
        """Reset the display"""
        config.digital_write(self._rst,GPIO.HIGH)
        time.sleep(0.01)
        config.digital_write(self._rst,GPIO.LOW)
        time.sleep(0.01)
        config.digital_write(self._rst,GPIO.HIGH)
        time.sleep(0.01)
        
    def SetWindows(self, Xstart, Ystart, Xend, Yend):
        #set the X coordinates
        self.command(0x2A)
        self.data(Xstart>>8)               #Set the horizontal starting point to the high octet
        self.data(Xstart & 0xff)      #Set the horizontal starting point to the low octet
        self.data(Xend>>8)               #Set the horizontal end to the high octet
        self.data((Xend - 1) & 0xff) #Set the horizontal end to the low octet 
        
        #set the Y coordinates
        self.command(0x2B)
        self.data(Ystart>>8)
        self.data((Ystart & 0xff))
        self.data(Yend>>8)
        self.data((Yend - 1) & 0xff )

        self.command(0x2C)    

    def ShowImage(self, Image):
        """Set buffer to value of Python Imaging Library image."""
        """Write display buffer to physical display"""
        imwidth, imheight = Image.size
        if imwidth == self.height and imheight ==  self.width:
            img = np.asarray(Image)
            pix = np.zeros((self.width, self.height,2), dtype = np.uint8)
            #RGB888 >> RGB565
            pix[...,[0]] = np.add(np.bitwise_and(img[...,[0]],0xF8),np.right_shift(img[...,[1]],5))
            pix[...,[1]] = np.add(np.bitwise_and(np.left_shift(img[...,[1]],3),0xE0), np.right_shift(img[...,[2]],3))
            pix = pix.flatten().tolist()
            
            self.command(0x36)
            self.data(0x70) 
            self.SetWindows ( 0, 0, self.height,self.width)
            config.digital_write(self._dc,GPIO.HIGH)
            for i in range(0,len(pix),4096):
                config.spi_writebyte(pix[i:i+4096])		
            
        else :
            img = np.asarray(Image)
            pix = np.zeros((imheight,imwidth , 2), dtype = np.uint8)
            
            pix[...,[0]] = np.add(np.bitwise_and(img[...,[0]],0xF8),np.right_shift(img[...,[1]],5))
            pix[...,[1]] = np.add(np.bitwise_and(np.left_shift(img[...,[1]],3),0xE0), np.right_shift(img[...,[2]],3))

            pix = pix.flatten().tolist()
            
            self.command(0x36)
            self.data(0x00) 
            self.SetWindows ( 0, 0, self.width, self.height)
            config.digital_write(self._dc,GPIO.HIGH)
            config.digital_write(self._cs, GPIO.LOW)
            for i in range(0,len(pix),4096):
                config.spi_writebyte(pix[i:i+4096])		
        
    def clear(self):
        """Clear contents of image buffer"""
        _buffer = [0xff]*(self.width * self.height * 2)
        self.SetWindows ( 0, 0, self.width, self.height)
        config.digital_write(self._dc,GPIO.HIGH)
        for i in range(0,len(_buffer),4096):
            config.spi_writebyte(_buffer[i:i+4096])		
