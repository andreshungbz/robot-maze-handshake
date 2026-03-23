package controller

import (
	"log"

	"github.com/andreshungbz/robot-maze-handshake/robot-bt-controller/internal/commands"
	"github.com/veandco/go-sdl2/sdl"
)

// Gamepad represents the gamepad mode of the program and implements controller.Input.
type Gamepad struct{}

// Start continuously polls controller events and sends appropriate commands to Controller.
func (g *Gamepad) Start(out chan<- byte) {
	// initialize SDL library and controller subsystem
	if err := sdl.Init(sdl.INIT_GAMECONTROLLER); err != nil {
		log.Fatal(err)
	}
	defer sdl.Quit()
	if sdl.NumJoysticks() < 1 {
		log.Fatal("[GAMEPAD] No Game Controllers Connected")
	}

	// open first controller
	ctrl := sdl.GameControllerOpen(0)
	if ctrl == nil {
		log.Fatal("[GAMEPAD] Failed to Open Game Controller")
	}
	defer ctrl.Close()
	log.Println("[GAMEPAD] Successfully Connected to Game Controller!")

	// display mode and available commands
	g.printModeAndCommands(ctrl.Name())

	for {
		for event := sdl.PollEvent(); event != nil; event = sdl.PollEvent() {
			switch e := event.(type) {

			// Button Events
			case *sdl.ControllerButtonEvent:
				// check only for pressed state
				if e.State != sdl.PRESSED {
					continue
				}

				// get button name and send appropriate command
				btn := sdl.GameControllerButton(e.Button)
				name := sdl.GameControllerGetStringForButton(btn)
				switch name {
				case "x":
					out <- commands.CommandMap['X'].Code
				case "dpup":
					out <- commands.CommandMap['F'].Code
				case "dpdown":
					out <- commands.CommandMap['B'].Code
				case "leftshoulder":
					out <- commands.CommandMap['L'].Code
				case "rightshoulder":
					out <- commands.CommandMap['R'].Code
				case "start":
					out <- commands.CommandMap['A'].Code
				case "back":
					out <- commands.CommandMap['M'].Code
				default:
					continue
				}

			// Axis Events
			case *sdl.ControllerAxisEvent:
				// check value threshold for triggers
				if e.Value < 20000 {
					continue
				}

				// get axis name and sent appropriate command
				axis := sdl.GameControllerAxis(e.Axis)
				name := sdl.GameControllerGetStringForAxis(axis)
				switch name {
				case "lefttrigger":
					out <- commands.CommandMap['1'].Code
				case "righttrigger":
					out <- commands.CommandMap['2'].Code
				default:
					continue
				}
			}
		}

		sdl.Delay(20)
	}
}

// printModeAndCommands prints the mode, controller name, and available commands.
func (g *Gamepad) printModeAndCommands(name string) {
	log.Printf("[GAMEPAD MODE - %s] [Available Commands]\n", name)
	for _, input := range commands.GamepadOrder {
		key := commands.GamepadMap[input]
		cmd := commands.CommandMap[key]
		display := commands.GamepadDisplay[input]

		log.Printf("  %-18s : %s\n", display, cmd.Action)
	}
}
