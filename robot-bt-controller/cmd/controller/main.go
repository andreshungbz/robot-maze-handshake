package main

import (
	"flag"
	"log"
	"log/slog"
	"os"
	"time"

	"github.com/andreshungbz/robot-maze-handshake/robot-bt-controller/internal/ble"
	"github.com/andreshungbz/robot-maze-handshake/robot-bt-controller/internal/config"
	"github.com/andreshungbz/robot-maze-handshake/robot-bt-controller/internal/controller"
)

func main() {
	// parse mode flag
	mode := flag.String("mode", "keyboard", "Program input mode (keyboard|gamepad)")
	flag.Parse()

	// create logger and BLE Module client
	logger := slog.New(slog.NewTextHandler(os.Stdout, nil))
	client, err := ble.ConnectToBLEDevice(
		config.BLE_MODULE_LOCAL_NAME,
		10*time.Second,
		logger,
	)
	if err != nil {
		log.Fatal(err)
	}
	defer client.Close()

	// determine controller input based on flag and create Controller with that input
	var input controller.Input
	switch *mode {
	case "keyboard":
		input = &controller.Keyboard{}
	default:
		log.Fatal("Unsupported Mode")
	}
	ctrl := controller.New(client, input, logger)

	// run controller program, which runs controller.Input.Start
	if err := ctrl.Run(); err != nil {
		log.Fatal(err)
	}
}
