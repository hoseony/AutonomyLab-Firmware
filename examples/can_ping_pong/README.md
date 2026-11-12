# can_ping_pong example
Have 2 picos: 
- `node_a.c`: One sending "u there?" (ID: 200) every second
- `node_b.c`: One sending "i there " (ID: 201) every time it sees a message on CAN bus. Has 10% chance of sending "shut up " too.

### Connection Setup Picture
<img width="720" height="540" alt="connection_setup" src="https://github.com/user-attachments/assets/4d86fcd6-e2cd-435d-a194-79011f8d46ef" />

### Expected output (From Innomaker USB-CAN Module, after running for 9 seconds)
<img width="735" height="379" alt="can_ping_pong_demo" src="https://github.com/user-attachments/assets/73aa5dc4-10c6-46a3-b665-cb5b872e5097" />

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
`gdb <path-to-build-directory>/<ex_node_a.elf or ex_node_b.elf> -ex flash`

debug:
`gdb <path-to-build-directory>/<ex_node_a.elf or ex_node_b.elf> -ex debug`
