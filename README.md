

git clone ...
cd wolfssl-esp32
./makeall.sh

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