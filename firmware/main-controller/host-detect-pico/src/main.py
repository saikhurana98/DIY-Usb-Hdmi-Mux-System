from machine import mem32, Pin


led = Pin(25, Pin.OUT)
outPin = Pin(4, Pin.OUT)

def isUsbHostPresent():
    SIE_STATUS=const(0x50110000+0x50)
    CONNECTED=const(1<<16)
    SUSPENDED=const(1<<4)
    return mem32[SIE_STATUS] & (CONNECTED | SUSPENDED) == CONNECTED

def outputState(state):
    led.on() if state else led.off()
    outPin.on() if state else outPin.off()


def main():
    while True:
        outputState(isUsbHostPresent())

main()