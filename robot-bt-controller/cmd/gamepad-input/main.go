package main

import (
	"log"

	"github.com/veandco/go-sdl2/sdl"
)

func main() {
	// initialize SDL library and controller subsystem
	if err := sdl.Init(sdl.INIT_GAMECONTROLLER); err != nil {
		log.Fatal(err)
	}
	defer sdl.Quit()

	if sdl.NumJoysticks() < 1 {
		log.Fatal("No game controllers connected")
	}

	// open first controller
	controller := sdl.GameControllerOpen(0)
	if controller == nil {
		log.Fatal("Failed to open game controller")
	}
	defer controller.Close()

	log.Printf("[MONITORING CONTROLLER -  %s]\n", controller.Name())
	log.Printf("%-15s %-18s %-12s\n", "Input Type", "Control", "Value")

	for {
		for event := sdl.PollEvent(); event != nil; event = sdl.PollEvent() {
			switch e := event.(type) {
			// controller button events (e.g. d-pad, a, x)
			case *sdl.ControllerButtonEvent:
				state := "Released"
				if e.State == sdl.PRESSED {
					state = "Pressed"
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
