package controller

import (
	"log"

	"github.com/andreshungbz/robot-maze-handshake/robot-bt-controller/internal/ble"
)

// Input creates a common interface for programs that send commands to the BLE device.
type Input interface {
	Start(chan<- byte)
}

// Controller combines ble.Client and Input.
type Controller struct {
	client *ble.Client
	input  Input
}

// New is a "constructor" function for Controller.
func New(client *ble.Client, input Input) *Controller {
	return &Controller{
		client: client,
		input:  input,
	}
}

// Run starts a buffered byte channel where Input.Start sends commands to.
// Those commands are then sent to the BLE device via ble.Client.Write.
func (c *Controller) Run() error {
	cmdCh := make(chan byte, 10)

	// run Start in a goroutine
	go c.input.Start(cmdCh)

	// continuously read command values from the channel and send
	// them to the BLE device
	for cmd := range cmdCh {
		n, err := c.client.Write([]byte{cmd})
		if err != nil {
			log.Fatalf("[CONTROLLER] Write Failed: %v", err)
			continue
		}

		log.Printf("[CONTROLLER] Command Sent: %s (%d bytes)", string(cmd), n)
	}

	return nil
}
