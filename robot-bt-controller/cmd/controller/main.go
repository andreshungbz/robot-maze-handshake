package main

import (
	"bufio"
	"fmt"
	"os"
	"time"

	"tinygo.org/x/bluetooth"
)

var adapter = bluetooth.DefaultAdapter

func main() {
	fmt.Println("Starting BLE Adapter controller...")
	err := adapter.Enable();
	if  err != nil {
		panic("failed to enable BLE adapter: " + err.Error())
	}

	var robotAddr bluetooth.Address
	bleModuleName := "Makeblock_LE703e97eeb26f"

	done := make(chan struct{})
	fmt.Println("Scanning for robot BLE...")
	adapter.Scan(func(adapter *bluetooth.Adapter, result bluetooth.ScanResult) {
		if result.LocalName() == bleModuleName {
			robotAddr = result.Address
			fmt.Printf("Found robot: %s [%s]\n", bleModuleName, robotAddr.String())
			adapter.StopScan()
			close(done)
		}
	})
	select {
	case <-done:
	case <-time.After(10 * time.Second):
		fmt.Println("Robot not found — exiting")
		return
	}

	fmt.Println("Connecting...")
	device, err := adapter.Connect(robotAddr, bluetooth.ConnectionParams{})
	if err != nil {
		panic("failed to connect: " + err.Error())
	}
	defer device.Disconnect()
	fmt.Println("Connected!")

	// Discover all services and look specifically for FFE1 -> FFE2/FFE3
	var txChar, rxChar bluetooth.DeviceCharacteristic
	services, _ := device.DiscoverServices(nil)
	for _, svc := range services {
		if svc.UUID().String() == "0000ffe1-0000-1000-8000-00805f9b34fb" {
			chars, _ := svc.DiscoverCharacteristics(nil)
			for _, c := range chars {
				uuid := c.UUID().String()
				switch uuid {
				case "0000ffe2-0000-1000-8000-00805f9b34fb":
					rxChar = c // notify from robot
				case "0000ffe3-0000-1000-8000-00805f9b34fb":
					txChar = c // write to robot
				}
			}
		}
	}

	if txChar.UUID() == (bluetooth.UUID{}) || rxChar.UUID() == (bluetooth.UUID{}) {
		panic("BLE UART characteristics FFE2/FFE3 not found")
	}

	fmt.Println("Ready! Type a character to send (will go to FFE3)...")

	reader := bufio.NewReader(os.Stdin)
	for {
		fmt.Print("> ")
		text, _ := reader.ReadString('\n')
		if len(text) == 0 {
			continue
		}
		b := []byte{text[0]}
		n, err := txChar.WriteWithoutResponse(b)
		if err != nil {
			fmt.Println("Write failed:", err)
		} else {
			fmt.Printf("Sent %d bytes to FFE3: %c\n", n, text[0])
		}
	}
}
