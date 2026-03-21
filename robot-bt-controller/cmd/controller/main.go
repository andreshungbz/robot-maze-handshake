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

func introduction() {
	const (
		PURPLE = "\u001B[95m"
		RESET  = "\u001B[0m"
	)

	fmt.Printf("%-35s %s\n", PURPLE+"[robot-bl-controller]"+RESET,
		"Program for connecting a game controller and sending commands to the robot.")
	fmt.Printf("%-35s %s\n", PURPLE+"[Project Repository]"+RESET,
		"https://github.com/andreshungbz/robot-maze-handshake")
	fmt.Println()
}
