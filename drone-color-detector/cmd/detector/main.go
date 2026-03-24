package main

import (
	"fmt"
	"log"
	"net"
	"os"
	"os/signal"
	"runtime"
	"time"

	"gocv.io/x/gocv"
)

func main() {
	runtime.LockOSThread()
	introduction()

	log.Printf("gocv version: %s\n", gocv.Version())
	log.Printf("opencv lib version: %s\n", gocv.OpenCVVersion())

	droneIP := "192.168.10.1:8889"

	// Resolve the UDP address
	addr, err := net.ResolveUDPAddr("udp", droneIP)
	if err != nil {
		log.Fatalf("Failed to resolve address: %v", err)
	}

	// Open a UDP connection
	conn, err := net.DialUDP("udp", nil, addr)
	if err != nil {
		log.Fatalf("Failed to dial UDP: %v", err)
	}
	defer conn.Close()

	// Enter SDK mode
	sendCommand(conn, "command")

	c := make(chan os.Signal, 1)
	signal.Notify(c, os.Interrupt)

	go func() {
		<-c
		fmt.Println("\nInterrupt received, stopping stream...")
		sendCommand(conn, "streamoff")
		os.Exit(0)
	}()

	// send battery check
	_, err = conn.Write([]byte("battery?"))
	if err != nil {
		log.Fatalf("Failed to send battery check: %v", err)
	}

	// read response
	buffer := make([]byte, 1024)
	conn.SetReadDeadline(time.Now().Add(5 * time.Second))
	n, _, err := conn.ReadFromUDP(buffer)
	if err != nil {
		log.Fatalf("Failed to read battery response: %v", err)
	}

	fmt.Printf("Drone battery: %s%%\n", string(buffer[:n]))

	// Start video stream
	sendCommand(conn, "streamon")

	// Open the video stream on UDP port 11111
	videoURL := "udp://@0.0.0.0:11111"
	video, err := gocv.VideoCaptureFile(videoURL)
	if err != nil {
		log.Fatalf("Error opening video capture: %v", err)
	}
	defer video.Close()

	window := gocv.NewWindow("Tello Video")
	defer window.Close()

	img := gocv.NewMat()
	defer img.Close()

	fmt.Println("Press any key in the video window to exit.")

	for {
		if ok := video.Read(&img); !ok {
			log.Println("Cannot read video frame")
			break
		}
		if img.Empty() {
			continue
		}

		window.IMShow(img)
		if window.WaitKey(1) >= 0 {
			break
		}
	}
}

// sendCommand sends a UDP command and waits for a response
func sendCommand(conn *net.UDPConn, cmd string) {
	fmt.Printf("Sending '%s'...\n", cmd)
	_, err := conn.Write([]byte(cmd))
	if err != nil {
		log.Fatalf("Failed to send %s: %v", cmd, err)
	}

	conn.SetReadDeadline(time.Now().Add(5 * time.Second))
	buffer := make([]byte, 1024)
	n, _, err := conn.ReadFromUDP(buffer)
	if err != nil {
		log.Fatalf("No response from drone: %v", err)
	}
	fmt.Printf("Drone response: %s\n", string(buffer[:n]))
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
