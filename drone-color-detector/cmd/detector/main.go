package main

import (
	"fmt"
	"log"
	"runtime"
	"time"

	"github.com/andreshungbz/robot-maze-handshake/drone-color-detector/internal/config"
	"github.com/andreshungbz/robot-maze-handshake/drone-color-detector/internal/drone"
	"github.com/andreshungbz/robot-maze-handshake/drone-color-detector/internal/vision"
)

func main() {
	// lock thread for GoCV windows
	runtime.LockOSThread()

	// display program introduction
	introduction()

	// connect to the drone and handle interrupts
	d, err := drone.Connect(config.TELLO_CMD_RESP_ADDRESS)
	if err != nil {
		log.Fatal(err)
	}
	defer d.Close()
	drone.HandleInterrupt(d)

	// check battery percentage
	if err := d.SendCommand("battery?"); err != nil {
		log.Printf("[HOST] Battery check failed: %v", err)
	}

	// start the video stream on the drone
	if err := d.StreamOn(); err != nil {
		log.Fatalf("[HOST] Failed to start video stream: %v", err)
	}
	defer d.StreamOff()

	// create a vision.VideoWindow to monitor the drone video stream
	vw, err := vision.NewVideoWindow(config.TELLO_VIDEO_STREAM_ADDRESS, "Tello Video")
	if err != nil {
		log.Fatalf("[HOST] Failed to open video window: %v", err)
	}
	defer vw.Close()

	// MAIN PROGRAM LOOP
	for {
		// read video frame
		img, ok := vw.ReadFrame()
		if !ok {
			continue
		}

		// process frame add detection rectangle and return green threshold passed
		greenDetected := vision.DetectGreen(&img)

		// render window and process GUI events
		vw.Display(img)

		// on detection of green, execute preprogrammed commands
		if greenDetected {
			log.Println("[DRONE] Green detected, executing command sequence!")
			time.Sleep(500 * time.Millisecond)
			drone.ExecuteSequence(d)

			// stop stream cleanly
			if err := d.StreamOff(); err != nil {
				log.Printf("[HOST] Failed to stop stream: %v", err)
			}

			break
		}
	}
}

// introduction displays project information and links.
func introduction() {
	const (
		YELLOW = "\u001B[33m"
		RESET  = "\u001B[0m"
	)

	fmt.Printf("%-35s %s\n", YELLOW+"[drone-color-detector]"+RESET,
		"Program for communicating with the drone and detecting a color signal")
	fmt.Printf("%-35s %s\n", YELLOW+"[GitHub Repository]"+RESET,
		"https://github.com/andreshungbz/robot-maze-handshake")
	fmt.Printf("%-35s %s\n", YELLOW+"[Project Subfolder]"+RESET,
		"https://github.com/andreshungbz/robot-maze-handshake/tree/main/drone-color-detector")
	fmt.Println()
}
