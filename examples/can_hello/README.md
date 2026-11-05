# can_hello example
makes a pico spam "hello world" every once in a while and blinks an led if its successful

## How to use

### Building
``` 
mkdir build
cd build
cmake ..
make
```

#### In one terminal:
`sudo openocd`

#### In another terminal:
```
sudo ip link set can0 down
sudo ip link set can0 type can bitrate 500000
sudo ip link set can0 up
candump can0
```

#### In a third terminal:
flash:
`gdb -ex flash`

debug:
`gdb -x Debug`

