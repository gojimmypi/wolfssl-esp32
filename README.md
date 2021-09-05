
# Getting Started

Terminology:

wolfSSL (with the `SSL` in upper case) is the name of the organization
wolfssl (with the `ssl` in lower case) is the name of the tls encrption libraries
wolfssh (with the `ssh` in lower case)is the name of the ssh libraries

Despite `SSL` being part of the name, wolfSSL has implemented [TLS 1.3](https://www.wolfssl.com/docs/tls13/), as [SSL deprecated](https://datatracker.ietf.org/doc/html/rfc7568). 

The wolfSSL libraries are distributed in two parts: the [wolfSSL/wolfssl](https://github.com/wolfSSL/wolfssl) and [wolfSSL/wolfssh](https://github.com/wolfSSL/wolfssh). 
The SSH code is typically copied into the wolfssl directory.

There's a [Get Started in 2021 with wolfSSL](https://www.youtube.com/watch?v=H_0AORo6ZKg) that is probably a good place to start.

Two sets of examples exist on the wolfSSL GitHub account: [wolfssh examples](https://github.com/wolfSSL/wolfssh/tree/master/examples)  `ssh` on port `22222` with the [wolfssl examples](https://github.com/wolfSSL/wolfssl/tree/master/examples) `tls` on port `11111`. 
They _are_ different and they _don't_ work with each other. `TLS` and `SSH` are different, and `SSH` does not occur over a `TLS` connection.

# Settings Configuration

The proper file to edit is the one in the `~/esp/esp-idf/components` directory, _not_ the one in the GitHub repo! (see [how to configure TLS 1.3 DH key size? #11](https://github.com/espressif/esp-wolfssl/issues/11#issuecomment-908805153))

```
~/esp/esp-idf/components/wolfssl/wolfssl/wolfcrypt/settings.h
```

# Installation

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

# Install esp-idf



# Install wolfssl and wolfssh




```
```

# System Include Libraries

The esp-idf does *not* look at the normal `/usr/local/include`, and the current wolfssl-ssh does *not* copy the `SSH` libraries to the correct location.

wolfssh include directory needs to be manually copied to the components/wolfssl

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

See also:

- [espressif/esp-wolfssl](https://github.com/espressif/esp-wolfssl) (note repository now uses upstream wolfSSL GitHub pointer as submodule)
- [wolfSSL Frequently Asked Questions (FAQ)](https://www.wolfssl.com/docs/frequently-asked-questions-faq/#How_do_I_manage_the_build_configuration_for_wolfSSL) (beware code samples contain invalid characters such as unicode instead of `-`, use care when copy/pasting)
- [Where to find libwolfssl ? #4272](https://github.com/wolfSSL/wolfssl/issues/4272#issuecomment-891203208)

