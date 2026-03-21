package controller

import (
	"log/slog"

	"github.com/andreshungbz/robot-maze-handshake/robot-bt-controller/internal/ble"
)

// Input creates a common interface for programs that send commands to the BLE device.
type Input interface {
	Start(chan<- byte)
}

// Controller combines ble.Client, Input, and a logger.
type Controller struct {
	client *ble.Client
	input  Input
	logger *slog.Logger
}

// New is a "constructor" function for Controller.
func New(client *ble.Client, input Input, logger *slog.Logger) *Controller {
	return &Controller{
		client: client,
		input:  input,
		logger: logger,
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
			c.logger.Error("Write Failed", "err", err)
			continue
		}

		c.logger.Info("Command Sent",
			"cmd", string(cmd),
			"bytes", n,
		)
	}

	return nil
}
