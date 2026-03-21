package controller

import (
	"bufio"
	"fmt"
	"log"
	"os"

	"github.com/andreshungbz/robot-maze-handshake/robot-bt-controller/internal/commands"
)

// Keyboard represents the keyboard mode of the program and implements controller.Input.
type Keyboard struct{}

// Start continuously reads commands from standard input and sends appropriate commands to Controller.
func (k *Keyboard) Start(out chan<- byte) {
	reader := bufio.NewReader(os.Stdin)
	for {
		// display mode and available commands
		k.printModeAndCommands()

		// read input
		log.Printf("[KEYBOARD] Command: ")
		text, err := reader.ReadString('\n')
		if err != nil {
			log.Fatalf("[KEYBOARD] Error Reading Input: %v", err)
			continue
		}

		// continue on empty input
		if len(text) == 0 {
			continue
		}

		// get first character only
		inputKey := text[0]

		// check if character is a valid command
		cmd, ok := commands.CommandMap[inputKey]
		if !ok {
			log.Printf("[KEYBOARD] Unknown Key: %s\n", string(inputKey))
			continue
		}

		// send command to Controller channel
		out <- cmd.Code
	}
}

// printModeAndCommands displays the mode and available commands.
func (k *Keyboard) printModeAndCommands() {
	fmt.Println("[KEYBOARD MODE] - [Available Commands]")
	for _, key := range commands.KeyboardOrder {
		cmd := commands.CommandMap[key]
		log.Printf("\t%c : %s\n", key, cmd.Action)
	}
}
