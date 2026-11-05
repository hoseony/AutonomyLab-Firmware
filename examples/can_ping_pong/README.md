# can_ping_pong example
Have 2 picos: 
- `node_a.c`: One sending "u there?" (ID: 200) every second
- `node_b.c`: One sending "i there " (ID: 201) every time it sees a message on CAN bus. Has 10% chance of sending "shut up " too.

## Building and Flashing
```
cmake -B build
cmake --build build
```

#### Connect USB to node B's debugger pico
```
sudo openocd                    # in one terminal
gdb build/node_b.elf -ex flash  # in another terminal
```

#### Connect USB to node A's debugger pico
```
sudo openocd                    # in one terminal
gdb build/node_a.elf -ex flash  # in another terminal
```

## How to Use
### In one terminal:
`sudo openocd`

### In second terminal:
```
sudo ip link set can0 down
sudo ip link set can0 type can bitrate 500000
sudo ip link set can0 up
candump can0
```

### In third terminal:
If you followed the "Building and Flashing" section, both target picos should already be flashed. In this case, you can just plug in a usb to one of the two debuggers (assuming VSYS connects every single pico) and every pico will get power and you don't have to do anything. The commands for debugging or flashing are below though.

flash: 
`gdb build/<node_a.elf or node_b.elf> -ex flash`

debug:
`gdb build/<node_a.elf or node_b.elf> -ex debug`
