# robot-maze-handshake

A series of programs that allow a robot to navigate a maze using the right-hand rule, activate a color signal at the designated finish point, then allow a drone to detect the color signal and perform a series of set actions.

The following robots/kits are used in this project:

- [Makeblock Ultimate 2.0 Robot Kit](https://support.makeblock.com/hc/en-us/sections/360001778514-Ultimate-2-0)
- [DJI Tello Drone](https://www.ryzerobotics.com/tello)

## Programs

### `robot-maze-solver` (C++)

The first main program allows the robot to autonomously navigate the maze using the right-hand rule. The robot uses the ultrasonic sensor on its right to detect the right wall, and the line follower sensor on its front to detect the front wall. Once it reaches the designated finish point, it activates the RGB LED module to signal the drone. It also manages a manual mode for the program.

This program is compiled and uploaded to the robot through its serial port. It uses an [updated fork of the official Makeblock libraries by nbourre](https://github.com/nbourre/Makeblock-Libraries), with development being managed through [PlatformIO](https://platformio.org).

### `drone-color-detector` (Go)

The second main program that communicates with the drone via a UDP server. It continuously checks the drone's video feed for the specific color signal emitted by the robot. Once the color signal is detected, it sends a series of commands to the drone.

This program runs in a separate process on a computer connected to the same network as the drone. It uses [GoCV](https://gocv.io), which requires [OpenCV](https://opencv.org) to be separately installed.

### `robot-bt-controller` (Go)

An add-on program that interfaces with the `robot-maze-solver` program through the Makeblock BLE module. Commands are sent via keyboard or a connected game controller to control the robot manually.

This program runs in a separate process on a computer with a game controller paired (if using gamepad mode) and requires the robot to be turned on. It uses [go-sdl2](https://github.com/veandco/go-sdl2), which wraps [SDL2](https://www.libsdl.org) (may need to be separately installed). Support for SDL3 wrappers in Go is currently limited, so the more stable package is used.
