# findings - charlieplex current draw

Sun 16 Jul 19:17:11 UTC 2023

  https://www.renesas.com/us/en/document/dst/isl854102-datasheet

Datasheet for the Uno R4 WiFi buck converter - surprise - a renesas product. ;) 

Okay so the Uno R4 WiFi fails over to the bench PSU (5 vdc 90 mA here) but prefers USB (may as not well have the bench supply connected if the USB is plugged in, at least initially).

  (Kinda like the Due, maybe) it'll prefer the bench supply when dialed up higher (7.5v 80 mA presently). 

At 5.41v in from the bench supply it is still measuring 83 mA with USB attached.  But rises to 95 mA when the USB cable is removed. ;)

  At 5.37v in from the bench supply, the current dropped to 70 mA .. the Due also does this and I don't like it, so I want one or the other, not something shared between them. 

Unlike the Due this thing isn't tripping overcurrent protection (135 mA set-point for 90 mA steady load). 

With pins 29 and 30 high, all others OUPUT low, 108 mA I_Total (96 mA baseline at 5.57v from the bench supply).

16 charlieplexed LEDs are lit.

So that's 12 mA spread over 16 LEDs somewhere near as bright as one another. 

One claim is 8 mA per port pin so I probaby was worried about nothing.

Now I have to do it without VBUS present on the system, to verify the numbers are real ;)

Good news (I will close with this) .. stayed right on 108 mA and the (static) charlieplexed array pattern was not disturbed by unplugging VBUS.

Not bad, man. 

END
