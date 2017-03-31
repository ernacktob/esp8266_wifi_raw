This is an experiment attempting to send and receive raw IEEE-802.11 packets from the ESP8266.

This is currently a dirty hack, and not quite perfect.

DISCLAIMER: THIS IS A PROOF OF CONCEPT ONLY. USE AT YOUR OWN RISK!

The code in this project was compiled and linked against the esp_iot_sdk_v1.2.0. and also works with esp_iot_sdk_v0.9.3.

SENDING
--------

Currently, one can set the bytes 0, 1, and 4, 5, ..., etc of the frame to any arbitrary value.
But the bytes 2 and 3 (Duration ID in a normal IEEE-802.11 frame) are somehow overwritten
by the lower level driver functions, and appear to be always zero.

Also, the length of the packet as sent over the air does not always correspond
to the length of the buffer given as input. The lower layer functions appear to be allocating
enough memory for the whole IEEE-802.11 frame, and these additional bytes will be sent even if
our actual buffer is shorter... But we do not yet understand what goes on below.

Another issue is that the method only works when the ESP is in SOFTAP_MODE.
In STATION_MODE, the eagle_lwip_getif function may crash, or the ieee80211_output_pbuf function
may return an error. We don't know why.


RECEIVING
---------

~~The method currently allows the device to enter in monitor mode, and sniff every packet
that can be found in the air, including control and management frames. There are no
limitations on the packet size as in the official sniffer APIs. You have access to
the complete frame.~~

Make sure you are listening in the correct channel and PHY mode.
Note that the ESP8266 does not seem to support the 5GHz wifi.

Update: It turns out the method only sniffs frames which are broadcast or directed to
	the device's own MAC address. It is not in promiscuous mode, so it can't receive
	all packets sent over the air. It does receive the complete frames for packets
	that are actually processed by the driver. If we set the device to promiscuous
	mode (using wifi_enable_promiscuous(1)) the callback we set up does not get called anymore.
	It seems promiscuous mode is dealt with differently, and the hardware may not be able
	to give the complete frames in this mode...

This can be thought of as a proof of concept, but there is still
a lot to work on. We don't know if we'll have the patience or ability to do so...

REVERSE ENGINEERING
-------------------

I have started some work on reverse-engineering the wifi stack of the ESP8266.
Currently I have been focusing on the libpp.a library, and have decompiled
most of the important functions from wdev.c, and started a little bit on lmac.c

This is a tedious and error-prone process, so there might be mistakes creeping
here and there. Please let me know if you find something interesting.


NOTES
-----

Update: Better method for hooking into ppEnqueueRxq and ppTxPkt https://github.com/ernacktob/esp8266_wifi_raw/issues/1

<strike>This program requires a modified libnet80211 and libpp library, which we called libnet80211_2.a and libpp2.a.
The modified libraries can be seen in the lib/ folder.

The modified net80211 library has its symbol table changed so that all references to ppTxPkt
are replaced with a dummy function called 'aaTxPkt' defined in user_main.c.
A simple way to do this is to go in the `/opt/esp_open_sdk/sdk/lib/` folder, and do

`sed s/ppTxPkt/aaTxPkt/g libnet80211.a > libnet80211_2.a`

The libpp2.a library is a modified libpp.a library where the *undefined* references to ppEnqueueRxq
are replaced to references to aaEnqueueRxq. There will be one ppEnqueueRxq symbol corresponding to
the actual function definition, and this one should not be changed. Follow these steps:

`sed s/ppTxPkt/aaTxPkt/g2 libpp.a > libpp2.a`

The Makefile must also be modified to link to net80211_2 and pp2 instead of net80211 and pp.
Use `make -f Makefile2` to build, or run the `./compile_and_flash` script (pass the port as a command line argument).</strike>

The makefile is modified in order to wrap the ppTxPkt and ppEnqueueRxq functions (see LDFLAGS).