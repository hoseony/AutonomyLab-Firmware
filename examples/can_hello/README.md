# can_hello example
makes a pico spam "hello world" every once in a while and blinks an led if its successful

## Building
``` 
mkdir build
cd build
cmake ..
make
```

## How to Flash or Debug
#### In one terminal:
`sudo openocd`

#### In second terminal:
```
sudo ip link set can0 down
sudo ip link set can0 type can bitrate 500000
sudo ip link set can0 up
candump can0
```

#### In third terminal:
flash:
`gdb -ex flash`

debug:
`gdb -x Debug`

