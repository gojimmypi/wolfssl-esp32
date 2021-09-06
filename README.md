# wolfssl-esp32

Sample project files for using the wolfSSL encrypion libraries with the Espressif ESP32.

## Getting Started

### WARNING:

The CONFIG_EXAMPLE_WIFI_PASSWORD is saved in `sdkconfig`. Exercise caution when saving to GitHub or other public locations.

Terminology:

wolfSSL (with the `SSL` in upper case) is the name of the organization
wolfssl (with the `ssl` in lower case) is the name of the tls encrption libraries
wolfssh (with the `ssh` in lower case) is the name of the ssh libraries

Despite `SSL` being part of the name, wolfSSL has implemented [TLS 1.3](https://www.wolfssl.com/docs/tls13/), as [SSL is deprecated](https://datatracker.ietf.org/doc/html/rfc7568). 

The wolfSSL libraries are distributed in two parts: the [wolfSSL/wolfssl](https://github.com/wolfSSL/wolfssl) and [wolfSSL/wolfssh](https://github.com/wolfSSL/wolfssh), and under the [GPLv2 or commercial license](https://github.com/wolfSSL/wolfssl/blob/master/LICENSING).
The SSH code is typically copied into the wolfssl directory.

There's a [Get Started in 2021 with wolfSSL](https://www.youtube.com/watch?v=H_0AORo6ZKg) that is probably a good place to start.

Two sets of examples exist on the wolfSSL GitHub account: [wolfssh examples](https://github.com/wolfSSL/wolfssh/tree/master/examples)  `ssh` on port `22222` with the [wolfssl examples](https://github.com/wolfSSL/wolfssl/tree/master/examples) `tls` on port `11111`. 
They _are_ different and they _don't_ work with each other. `TLS` and `SSH` are different, and `SSH` does not occur over a `TLS` connection.


## Settings Configuration

The proper file to edit is the one in the `~/esp/esp-idf/components` directory, _not_ the one in the GitHub repo! (see [how to configure TLS 1.3 DH key size? #11](https://github.com/espressif/esp-wolfssl/issues/11#issuecomment-908805153)).



```
ls ~/esp/esp-idf/components/wolfssl/wolfssl/wolfcrypt/settings.h

echo "Ensure these are NOT commented out for esp-idf/components:"
grep "#define WOLFSSL_ESPIDF"     ~/esp/esp-idf/components/wolfssl/wolfssl/wolfcrypt/settings.h
grep "#define WOLFSSL_ESPWROOM32" ~/esp/esp-idf/components/wolfssl/wolfssl/wolfcrypt/settings.h

# set example connection configuration SSID and password (See "Example Connection Congiration" menu item)
# this saves SSID and password to local sdkconfig file! (be caseful if saving to say, GitHub)
idf.py menuconfig

```

## Frequently Used Commands

See [Using the Build System](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/build-system.html#using-the-build-system) `idf.py`:

```
# prepare (only needed once per session)
cd ~/esp/esp-idf

# optionally show version. this repo developed with v4.4-dev-2825-gb63ec47238
git describe

. $HOME/esp/esp-idf/export.sh

# configure for ESP-32 (from each project directory)
if [ -f sdkconfig ]; then rm sdkconfig; fi
idf.py set-target esp32

# optionall adjust any other non-default settings
idf.py menuconfig

# clean
idf.py fullclean

# build
idf.py build

# flash (the -b is optional for a slower programming rate)
idf.py flash -p /dev/ttyS9 -b 115200

# connect to serial port;  Ctrl-] to exit; Ctrl-T, Ctrl-H for help; 
idf.py monitor -p /dev/ttyS9
```


## Installation

```
git clone ...
cd wolfssl-esp32
./makeall.sh
```

# Install Build Prerequisites

```
# optional Python 3.8 install in WSL/Linux
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install python3.8
pip install pyserial
```

## Install esp-idf

HACK TO FIX:

Added this line to `/esp/esp-idf/components/wolfssh/src/internal.c`:
```
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
```

See the [Espressif Get Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html)

```
mkdir -p ~/esp
cd ~/esp
git clone --recursive https://github.com/espressif/esp-idf.git

cd ~/esp/esp-idf
./install.sh esp32
```

## Install wolfssl and wolfssh


```
# edit this next line if you want to you your own fork
# note we are not pulling history with --depth 1 (MB vs MB)
git clone --depth 1  https://github.com/wolfssl/wolfssl.git

cd wolfssl
./autogen.sh
#./configure --enable-openssh --enable-ssh
./configure --enable-ssh
make
sudo make install

# create links and cache to recently added wolfSSH libraries
sudo ldconfig

# wolfSSL SSH
cd $WORKSPACE
git clone https://github.com/wolfSSL/wolfssh.git
./autogen.sh
./configure --enable-ssh

# make builds all of the examples (not including IDE)
make
sudo make install
sudo ldconfig
make check
```

## System Include Libraries

The esp-idf does *not* look at the normal `/usr/include/` and `/usr/local/include`; Further, the current wolfssl-ssh does *not* copy the `SSH` libraries to the correct location to be used by the Espressif IDF.

wolfssh include directory needs to be manually copied to the components/wolfssl, for example `~/esp/esp-idf/components/wolfssl/wolfssh/`.

```
cd ~/esp/esp-idf/components/wolfssl/
cp -r /usr/local/include/wolfssh/ .
```

```
idf.py -p /dev/ttyS9 -b 115200 flash
```

# Tips and Suggestions

Visual Studio users compiling in WSL may wish to turn off the CMake automated features:

~[disable-auto-cmake.png](../images/disable-auto-cmake.png)

Searching for keywords in SSH libraries:
```
grep -rnw ~/esp/esp-idf/components/wolfssl/wolfssh/ -e 'THREAD_RETURN'
```

Searching for filea in `esp-idf`:

```
find . -name "protocol_examples_common.h"
```

See WOLFSSL_USER_SETTINGS [Confusion about include path](https://www.wolfssl.com/forums/topic1517-solved-confusion-about-include-path.html):

> _If you are trying to use a `user_settings.h` with the configure script I would not recommend that approach instead when using the configure script it will generate the file `<wolfssl/options.h>` which is the replacement for `user_settings.h` when building with `./configure.`_

# Troubleshooing

Message: `FAILED: bootloader-prefix/src/bootloader-stamp/bootloader-configure`, after `idf.py fullclean`, try `idf.py build` a second time. If that fails, try `rm sdkconfig; idf.py menuconfig`. (the defaults are typically acceptable)

```
*******************************************************************************
# ESP-IDF Partition Table
# Name, Type, SubType, Offset, Size, Flags
nvs,data,nvs,0x9000,24K,
phy_init,data,phy,0xf000,4K,
factory,app,factory,0x10000,1M,
*******************************************************************************
[346/1093] Performing configure step for 'bootloader'
FAILED: bootloader-prefix/src/bootloader-stamp/bootloader-configure
```

Message: `Timed out waiting for packet header`, try slowing down the programming speed (the default is 460800) using the `-b` baudrate:  `idf.py -p /dev/ttyS9 -b 115200 flash`

```
Configuring flash size...

A fatal error occurred: Timed out waiting for packet header
CMake Error at run_serial_tool.cmake:56 (message):
  /home/gojimmypi/.espressif/python_env/idf4.4_py3.8_env/bin/python
  /home/gojimmypi/esp/esp-idf/components/esptool_py/esptool/esptool.py --chip
  esp32 failed

```

See also:

- [espressif/esp-wolfssl](https://github.com/espressif/esp-wolfssl) (note repository now uses upstream wolfSSL GitHub pointer as submodule)
- [wolfSSL Frequently Asked Questions (FAQ)](https://www.wolfssl.com/docs/frequently-asked-questions-faq/#How_do_I_manage_the_build_configuration_for_wolfSSL) (beware code samples contain invalid characters such as unicode instead of `-`, use care when copy/pasting)
- [Where to find libwolfssl ? #4272](https://github.com/wolfSSL/wolfssl/issues/4272#issuecomment-891203208)

