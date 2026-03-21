package controller

import (
	"bufio"
	"fmt"
	"os"
	"sort"

	"github.com/andreshungbz/robot-maze-handshake/robot-bt-controller/internal/commands"
)

// Keyboard implements controller.Input and represents the keyboard mode
// of the program.
type Keyboard struct{}

// Start continuously reads commands from standard input and sends the commands
// to the channel in Controller.
func (k *Keyboard) Start(out chan<- byte) {
	reader := bufio.NewReader(os.Stdin)

	// get command map keys
	keys := make([]byte, 0, len(commands.CommandMap))
	for k := range commands.CommandMap {
		keys = append(keys, k)
	}
	sort.Slice(keys, func(i, j int) bool { return keys[i] < keys[j] })

	for {
		// display prompt with available commands
		fmt.Println("[KEYBOARD MODE - Available Commands]")
		for _, k := range keys {
			cmd := commands.CommandMap[k]
			fmt.Printf("\t%c : %s\n", k, cmd.Action)
		}

		// read input
		fmt.Print("[COMMAND]: ")
		text, err := reader.ReadString('\n')
		if err != nil {
			fmt.Fprintln(os.Stderr, "Error Reading input:", err)
			continue
		}

		// continue on empty input
		if len(text) == 0 {
			continue
		}

		// get first character
		inputKey := text[0]

		// check if character is a valid command
		cmd, ok := commands.CommandMap[inputKey]
		if !ok {
			fmt.Println("[UNKNOWN KEY]:", string(inputKey))
			continue
		}

		// send command to Controller channel
		out <- cmd.Code
	}
}
