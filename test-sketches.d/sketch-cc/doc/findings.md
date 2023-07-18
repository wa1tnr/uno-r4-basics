# findings - charlieplex current draw

Tue 18 Jul 01:41:03 UTC 2023

l82 will not light brightly whereas another LED in the
array lights to a nice mid-bright gamma.

No reason has been uncovered yet, as to why this is.

0 acl 1 asb
0 asb 1 acl

Neither of these light dimly as l82 does.

The l82 word sets the direction to OUTPUT for just two port pins,
pin 0 (real pin: D28) and pin 10 (real pin: D38).

```
   0 acl   clears D28 (pin '0' here).
  10 asb   sets   D38 (pin '10' here).
```

mnemonics are:

```
  'acl' for clear an Arduino port pin (OUTPUT, LOW);

  'asb' for set   an Arduino port pin (OUTPUT, HIGH);
```

clr and setb are reserved for other uses and are not implemented.

In Arduino (C++) this is:
```
  pinMode(28, OUTPUT); digitalWrite(28, LOW);
  pinMode(38, OUTPUT); digitalWrite(38, HIGH);
```

In this program, the l82 word was coded as:

```
void l82d() {
    push(0); acld();
    push(10); asbd();
}
```

And other definitions in C++ take care of the details.

```
  5 asb 10 acl   lower left corner - cell 85 - this will be the l85 word
```

.. if the scheme is continued. ;)

That means D38 (pin '10' locally) operates the next five of the LEDs 'to the right'
of LED 85 (through LED 90, where the bus of D38 'ends').

```
  5 acl 10 asb - l86
  1 asb 10 acl - l87
  1 acl 10 asb - l88
```

In forth:

```
  ok
  1 asb 10 acl cls 1 acl 10 asb cls  \ convenient to do twins in one row

```


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
