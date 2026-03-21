package main

import (
	"flag"
	"log"
	"time"

	"github.com/andreshungbz/robot-maze-handshake/robot-bt-controller/internal/ble"
	"github.com/andreshungbz/robot-maze-handshake/robot-bt-controller/internal/config"
	"github.com/andreshungbz/robot-maze-handshake/robot-bt-controller/internal/controller"
)

func main() {
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
