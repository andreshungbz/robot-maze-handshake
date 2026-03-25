package drone

import (
	"fmt"
	"log"
	"net"
	"os"
	"os/signal"
	"strings"
	"time"
)

// Drone is an interface to the Ryze Tello drone, with methods for sending common commands.
type Drone struct {
	conn *net.UDPConn
}

// Connect uses the drone's IP address to establish a UDP connection and create a Drone.
func Connect(ip string) (*Drone, error) {
	// resolve IP address to UDP
	addr, err := net.ResolveUDPAddr("udp", ip)
	if err != nil {
		return nil, err
	}

	// establish UDP connection
	conn, err := net.DialUDP("udp", nil, addr)
	if err != nil {
		return nil, err
	}

	d := &Drone{conn: conn}
	// the 'command' command initiate's Tello's SDK mode
	if err := d.SendCommand("command"); err != nil {
		return nil, err
	}

	return d, nil
}

// Close closes the UDP connection to the drone.
func (d *Drone) Close() error {
	return d.conn.Close()
}

// SendCommand sends a string command to the drone via UDP and reads its response.
func (d *Drone) SendCommand(cmd string) error {
	// send command
	log.Printf("[HOST] Sending '%s'...\n", cmd)
	_, err := d.conn.Write([]byte(cmd))
	if err != nil {
		return err
	}

	// read its response
	d.conn.SetReadDeadline(time.Now().Add(5 * time.Second))
	buf := make([]byte, 1024)
	n, _, err := d.conn.ReadFromUDP(buf)
	if err != nil {
		return fmt.Errorf("[HOST] No response from drone: %w", err)
	}

	// convert response to string
	resp := strings.TrimSpace(string(buf[:n]))

	// check "ok" response and allow read commands
	if resp == "ok" || strings.HasSuffix(cmd, "?") {
		log.Printf("[DRONE] %s\n", resp)
		return nil
	}

	return fmt.Errorf("[DRONE] Command '%s' failed with response: %s", cmd, resp)
}

// GENERAL COMMANDS

// Takeoff makes the drone perform auto takeoff.
func (d *Drone) Takeoff() error {
	return d.SendCommand("takeoff")
}

// Land makes the drone perform auto land.
func (d *Drone) Land() error {
	return d.SendCommand("land")
}

// StreamOn starts the video stream from the drone.
func (d *Drone) StreamOn() error {
	return d.SendCommand("streamon")
}

// StreamOff stops the video stream from the drone.
func (d *Drone) StreamOff() error {
	return d.SendCommand("streamoff")
}

// BASIC MOVEMENT COMMANDS

// Up flies the drone up a distance.
func (d *Drone) Up(cm int) error {
	return d.SendCommand(fmt.Sprintf("up %d", validateDistance(cm)))
}

// Down flies the drone down a distance.
func (d *Drone) Down(cm int) error {
	return d.SendCommand(fmt.Sprintf("down %d", validateDistance(cm)))
}

// Left flies the drone left a distance.
func (d *Drone) Left(cm int) error {
	return d.SendCommand(fmt.Sprintf("left %d", validateDistance(cm)))
}

// Right flies the drone right a distance.
func (d *Drone) Right(cm int) error {
	return d.SendCommand(fmt.Sprintf("right %d", validateDistance(cm)))
}

// Forward flies the drone forward a distance.
func (d *Drone) Forward(cm int) error {
	return d.SendCommand(fmt.Sprintf("forward %d", validateDistance(cm)))
}

// Back flies the drone back a distance.
func (d *Drone) Back(cm int) error {
	return d.SendCommand(fmt.Sprintf("back %d", validateDistance(cm)))
}

// Flip makes the drone flip in a particular direction {l, r, f, b}.
func (d *Drone) Flip(dir string) error {
	if dir != "l" && dir != "r" && dir != "f" && dir != "b" {
		return fmt.Errorf("[HOST] Invalid Flip Direction")
	}

	return d.SendCommand("flip " + dir)
}

const (
	minDistanceCm = 20
	maxDistanceCm = 500
)

// validateDistance ensures the distance is within 20 to 500 cm.
func validateDistance(cm int) int {
	if cm < minDistanceCm {
		log.Printf("[HOST] Distance %d below minimum, using %d\n", cm, minDistanceCm)
		return minDistanceCm
	}

	if cm > maxDistanceCm {
		log.Printf("[HOST] Distance %d above maximum, using %d\n", cm, maxDistanceCm)
		return maxDistanceCm
	}

	return cm
}

// HELPERS

// HandleInterrupt sets up a signal handler to stop stream and exit cleanly.
func HandleInterrupt(d *Drone) {
	c := make(chan os.Signal, 1)
	signal.Notify(c, os.Interrupt)

	go func() {
		<-c
		log.Println("\n[HOST] Interrupt received, stopping stream...")
		if err := d.StreamOff(); err != nil {
			log.Printf("[HOST] Failed to stop stream: %v", err)
		}
		os.Exit(0)
	}()
}

// ExecuteSequence runs a preprogrammed movement sequence.
func ExecuteSequence(d *Drone) {
	if err := d.Takeoff(); err != nil {
		log.Println(err)
		return
	}

	time.Sleep(2 * time.Second)

	if err := d.Flip("f"); err != nil {
		log.Println(err)
		return
	}

	time.Sleep(2 * time.Second)

	if err := d.Forward(100); err != nil {
		log.Println(err)
		return
	}

	time.Sleep(2 * time.Second)

	if err := d.Land(); err != nil {
		log.Println(err)
	}
}
