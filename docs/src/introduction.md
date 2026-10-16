# Firmware Team Mono Repo
This page covers repo structure and contribution guide.

## Repository Structure

```
AutonomyLab-Firmware/
├── drivers/                    Hardware drivers
│   ├── rp2350/                 RP2350 peripheral drivers
│   │   ├── include/            Register definitions & driver headers
│   │   └── *.c                 Driver implementations
│   └── devices/                External part drivers
├── lib/                        Hardware-independent code
├── examples/                   Small programs that test/demo a driver
├── cmake/                      Shared CMake helpers (pico_sdk_import.cmake)
├── docs/                       This doc
└── .github/workflows/          CI and docs deployment
```

### drivers/
As the name suggests, put drivers here. Keep it baremetal.
- `rp2350/` : the chip's own peripherals (GPIO, PWM, UART, ...)
- `devices/` : for external parts.
- Name registers and fields as the datasheet does (this reduces confusion).
- Note datasheet section/page in comments where helpful.

### lib/
Code that does not touch hardware. It should be able to compile on your machine (not pico) as well.

- If it needs registers -> drivers/
- Only used inside one specific driver -> keep it in that driver
- Shared between drivers or examples -> lib/

### examples/
Small programs that show a driver is working, one folder each. Use them to test a driver and also document example usage of your driver.


## Contributing

### Workflow

Do not directly commit to `main`.

1. Pull `main` and create a branch.
    ```bash
    git switch main && git pull
    git switch -c feature/pwm-driver
    ```
2. Commit your work to your branch.
3. Open a PR into `main`.
4. Get someone else to review and approve.
5. Then merge it.

### Branch Naming Convention


| Prefix      | Use this for |
|-------------|--------------------------|
| `feature/`  | New drivers or features  |
| `fix/`      | Bug fixes                |
| `docs/`     | Documentation only       |

### Code Style
- Format with `clang-format` using the `.clang-format` on the root.

