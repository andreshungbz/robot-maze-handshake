package main

import (
	"fmt"
	"log"

	"github.com/veandco/go-sdl2/sdl"
)

func main() {
	introduction()

	// initialize SDL library and controller subsystem
	if err := sdl.Init(sdl.INIT_GAMECONTROLLER); err != nil {
		log.Fatal(err)
	}
	defer sdl.Quit()

	if sdl.NumJoysticks() < 1 {
		log.Fatal("[GAMEPAD-INPUT] No game controllers connected")
	}

	// open first controller
	controller := sdl.GameControllerOpen(0)
	if controller == nil {
		log.Fatal("[GAMEPAD-INPUT] Failed to open game controller")
	}
	defer controller.Close()

	log.Printf("[GAMEPAD-INPUT] [MONITORING CONTROLLER -  %s]\n", controller.Name())
	log.Printf("%-15s %-18s %-12s\n", "[INPUT TYPE]", "[CONTROL]", "[VALUE]")

	for {
		for event := sdl.PollEvent(); event != nil; event = sdl.PollEvent() {
			switch e := event.(type) {
			// controller button events (e.g. d-pad, a, x)
			case *sdl.ControllerButtonEvent:
				state := "released"
				if e.State == sdl.PRESSED {
					state = "pressed"
				}
				log.Printf("%-15s %-18s %-12s\n",
					"[Button]",
					sdl.GameControllerGetStringForButton(sdl.GameControllerButton(e.Button)),
					state,
				)

			// controller axis events (e.g. analog sticks, shoulder triggers)
			case *sdl.ControllerAxisEvent:
				log.Printf("%-15s %-18s %-12d\n",
					"[Axis]",
					sdl.GameControllerGetStringForAxis(sdl.GameControllerAxis(e.Axis)),
					e.Value,
				)

			case *sdl.QuitEvent:
				return
			}
		}

		sdl.Delay(20)
	}
}

func introduction() {
	const (
		PURPLE = "\u001B[95m"
		RESET  = "\u001B[0m"
	)

	fmt.Printf("%-35s %s\n", PURPLE+"[gamepad-input]"+RESET,
		"Program for observing monitoring game controller input values")
	fmt.Printf("%-35s %s\n", PURPLE+"[GitHub Repository]"+RESET,
		"https://github.com/andreshungbz/robot-maze-handshake")
	fmt.Printf("%-35s %s\n", PURPLE+"[Project Subfolder]"+RESET,
		"https://github.com/andreshungbz/robot-maze-handshake/tree/main/robot-bt-controller")
	fmt.Println()
}
