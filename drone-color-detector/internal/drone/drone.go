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

// Drone interfaces with the Ryze Tello drone, with methods for sending common commands.
type Drone struct {
	conn *net.UDPConn
}

// Connect establishes a UDP connection with the drone's IP address for command/response communication.
func Connect(ip string) (*Drone, error) {
	// resolve IP address string to UDP address
	addr, err := net.ResolveUDPAddr("udp", ip)
	if err != nil {
		return nil, err
	}

	// establish connection to UDP address
	conn, err := net.DialUDP("udp", nil, addr)
	if err != nil {
		return nil, err
	}

	// create Drone object and send 'command' to initiate Tello's SDK mode
	d := &Drone{conn: conn}
	if err := d.SendCommand("command"); err != nil {
		return nil, err
	}

	return d, nil
}

// SendCommand sends a command string to the drone via UDP and logs its response.
func (d *Drone) SendCommand(cmd string) error {
	// send command string via UDP connection
	log.Printf("[HOST] Sending '%s'...\n", cmd)
	_, err := d.conn.Write([]byte(cmd))
	if err != nil {
		return err
	}

	// read response with timeout
	d.conn.SetReadDeadline(time.Now().Add(10 * time.Second))
	buf := make([]byte, 1024)
	n, _, err := d.conn.ReadFromUDP(buf)
	if err != nil {
		return fmt.Errorf("[HOST] No response from drone: %w", err)
	}

	// convert response to string
	resp := strings.TrimSpace(string(buf[:n]))

	// log response after validating for "ok" response and read commands (suffix of "?")
	if resp == "ok" || strings.HasSuffix(cmd, "?") {
		log.Printf("[DRONE] %s\n", resp)
		return nil
	}
	return fmt.Errorf("[HOST] Command '%s' failed with [DRONE] response: %s", cmd, resp)
}

// Close tears down the UDP connection to the drone.
func (d *Drone) Close() error {
	return d.conn.Close()
}

// GENERAL COMMANDS

// Takeoff makes the drone perform auto takeoff by sending the "takeoff" command.
func (d *Drone) Takeoff() error {
	return d.SendCommand("takeoff")
}

// Land makes the drone perform auto land by sending the "land" command.
func (d *Drone) Land() error {
	return d.SendCommand("land")
}

// StreamOn starts the video stream from the drone by sending the "streamon" command.
func (d *Drone) StreamOn() error {
	return d.SendCommand("streamon")
}

// StreamOff stops the video stream from the drone by sending the "streamoff" command.
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

// Flip makes the drone flip in a particular direction of the set {"l", "r", "f", "b"}.
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

// validateDistance ensures the distance is within 20 to 500 cm (constraints from Tello SDK 1.3.0).
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

// HELPER METHODS

// HandleInterrupt ensures "streamoff" command is sent and UDP connection to drone is closed
// on interrupt signal (Ctrl + C).
func HandleInterrupt(d *Drone) {
	c := make(chan os.Signal, 1)
	signal.Notify(c, os.Interrupt)

	go func() {
		<-c
		log.Println("[HOST] Interrupt received, stopping stream...")

		// send "streamoff" command
		if err := d.StreamOff(); err != nil {
			log.Printf("[HOST] Failed to stop stream: %v", err)
		}

		// close UDP connection
		d.conn.Close()

		os.Exit(0)
	}()
}

// ExecuteSequence runs a preprogrammed movement sequence (delayed between commands).
func ExecuteSequence(d *Drone) {
	if err := d.Takeoff(); err != nil {
		log.Println(err)
		return
	}

	if err := d.Flip("f"); err != nil {
		log.Println(err)
		return
	}

	if err := d.Forward(20); err != nil {
		log.Println(err)
		return
	}

	if err := d.Land(); err != nil {
		log.Println(err)
		return
	}
}
