# Tue 11 Jul 16:05:50 UTC 2023

#     pio project init --board uno_r4_wifi

all:
	pio run

projinitAREYOUSURE:
	pio project init --board uno_r4_wifi

monitor:
	pio run -t monitor

# https://docs.platformio.org/en/latest/platforms/espressif32.html#erase-flash

# https://community.platformio.org/t/i-cannot-figure-out-how-to-upload-a-spiffs-image/6172

devlist:
	pio device list

# Hardware ID: USB VID:PID=1A86:55D4 SER=54B0005774 LOCATION=2-1.4:1.0
# Description: USB Single Serial

# https://community.platformio.org/t/i-cannot-figure-out-how-to-upload-a-spiffs-image/6172

nsterm:
	sleep 2; ./cf-fastpico9t /dev/ttyACM0

term:
	sleep 2; ./cf-fastpico9t /dev/ttyACM0

upload:
	pio run -t upload

install: upload term

upgrade:
	pio upgrade

# https://docs.platformio.org/en/latest/core/userguide/pkg/cmd_update.html#cmd-pkg-update

pkgupdate:
	pio pkg update

pkglist:
	pio pkg list

# https://docs.platformio.org/en/latest/core/userguide/pkg/cmd_outdated.html#cmd-pkg-outdated

outdated:
	pio pkg outdated

clean:
	pio run -t clean

distclean: clean
	rm -rf ./.pio
