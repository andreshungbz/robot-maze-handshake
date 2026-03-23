package main

import (
	"flag"
	"fmt"
	"log"
	"time"

	"github.com/andreshungbz/robot-maze-handshake/robot-bt-controller/internal/ble"
	"github.com/andreshungbz/robot-maze-handshake/robot-bt-controller/internal/config"
	"github.com/andreshungbz/robot-maze-handshake/robot-bt-controller/internal/controller"
)

func main() {
	introduction()

	// parse mode flag
	mode := flag.String("mode", "keyboard", "Program mode (keyboard|gamepad)")
	flag.Parse()

	// create BLE module client
	client, err := ble.ConnectToBLEDevice(
		config.BLE_MODULE_LOCAL_NAME,
		10*time.Second,
	)
	if err != nil {
		log.Fatal(err)
	}
	defer client.Close()

	// create Controller based on mode
	var input controller.Input
	switch *mode {
	case "keyboard":
		input = &controller.Keyboard{}
	case "gamepad":
		input = &controller.Gamepad{}
	default:
		log.Fatal("Unsupported Mode")
	}
	ctrl := controller.New(client, input)

	// run program
	if err := ctrl.Run(); err != nil {
		log.Fatal(err)
	}
}

// introduction displays project information and links.
func introduction() {
	const (
		BLUE  = "\u001B[94m"
		RESET = "\u001B[0m"
	)

	fmt.Printf("%-35s %s\n", BLUE+"[robot-bt-controller]"+RESET,
		"Program for using a keyboard or game controller to send commands to the robot via Bluetooth Low Energy (BLE)")
	fmt.Printf("%-35s %s\n", BLUE+"[GitHub Repository]"+RESET,
		"https://github.com/andreshungbz/robot-maze-handshake")
	fmt.Printf("%-35s %s\n", BLUE+"[Project Subfolder]"+RESET,
		"https://github.com/andreshungbz/robot-maze-handshake/tree/main/robot-bt-controller")
	fmt.Println()
}
