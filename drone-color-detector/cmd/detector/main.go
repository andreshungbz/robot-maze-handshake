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
	introduction()

	// lock thread for GoCV window
	runtime.LockOSThread()

	// connect to the drone
	d, err := drone.Connect(config.TELLO_CMD_RESP_ADDRESS)
	if err != nil {
		log.Fatal(err)
	}
	defer d.Close()

	// launch goroutine to handle interrupt signal (Ctrl + C)
	drone.HandleInterrupt(d)

	// check battery percentage
	if err := d.SendCommand("battery?"); err != nil {
		log.Printf("[HOST] Battery check failed: %v", err)
	}

	// start drone video stream
	if err := d.StreamOn(); err != nil {
		log.Fatalf("[HOST] Failed to start video stream: %v", err)
	}
	defer d.StreamOff()

	// create a window to monitor the drone video stream
	vw, err := vision.NewVideoWindow(config.TELLO_VIDEO_STREAM_ADDRESS, "Tello Video")
	if err != nil {
		log.Fatalf("[HOST] Failed to open video window: %v", err)
	}
	defer vw.Close()

	// MAIN PROGRAM LOOP
	for {
		// read video frame, skipping bad frames
		img, ok := vw.ReadFrame()
		if !ok {
			continue
		}

		// process frame to detect presence of bright green color (adds rectangle to frame)
		greenDetected := vision.DetectGreen(&img)

		// render frame to window
		vw.Display(img)

		// on detection of green, execute preprogrammed commands
		if greenDetected {
			log.Println("[VISION] Green detected!")

			// close drone video stream and window
			log.Println("[HOST] Stopping video stream before command sequence...")
			if err := d.StreamOff(); err != nil {
				log.Printf("[HOST] Failed to stop stream: %v", err)
			}
			vw.Close()

			log.Println("[HOST] Executing command sequence!")

			time.Sleep(500 * time.Millisecond)
			drone.ExecuteSequence(d) // comment-able when debugging

			// exit loop to end program
			log.Println("[HOST] Completed program! Exiting...")
			break
		}
	}

	time.Sleep(1 * time.Second)
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
