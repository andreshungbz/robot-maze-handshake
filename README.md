# robot-maze-handshake

Series of programs that allow a robot to navigate a maze using the right-hand rule, activate a color signal at the designated finish point, then allow a drone to detect the color signal and perform a series of set actions.

The following robots/kits are used in this project:

- [Makeblock Ultimate 2.0 Robot Kit](https://support.makeblock.com/hc/en-us/sections/360001778514-Ultimate-2-0)
- [DJI Tello Drone](https://www.ryzerobotics.com/tello)

## Programs

### `robot-maze-solver` (C++)

The first main program that allows the robot to autonomously navigate the maze using the right-hand rule. The robot uses the ultrasonic sensor on its right to detect the right wall and the line follower sensor on its front to detect front walls. Once it reaches the designated finish point, it activates the RGB LED module to signal the drone. It also manages a manual mode for the program.

This program is compiled and uploaded to the robot through its serial port.

### `drone-color-detector` (Go)

The second main program that communicates with the drone via a UDP server. It continuously checks the video feed from the drone for the specific color signal activated by the robot. Once the color signal is detected, it sends a series of commands to the drone.

This program runs on a separate process on a computer which is connected to the same network as the drone.

### `robot-bt-controller` (Go)

An add-on program that interfaces with the `robot-maze-solver` program through the Makeblock BLE module. Commands are sent via keyboard or a connected game controller to control the robot manually.

This program runs on a separate process on a computer with a game controller paired (if using gamepad mode) and requires the robot to be turned on.
