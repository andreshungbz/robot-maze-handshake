package ble

import (
	"errors"
	"fmt"
	"log"
	"time"

	"github.com/andreshungbz/robot-maze-handshake/robot-bt-controller/internal/config"
	"tinygo.org/x/bluetooth"
)

var adapter = bluetooth.DefaultAdapter

// ConnectToBLEDevice uses Bluetooth to connect to a BLE device and checks for the
// required service and characteristics.
func ConnectToBLEDevice(name string, timeout time.Duration) (*Client, error) {
	// 1. Enable Bluetooth Adapter

	log.Println("[BLUETOOTH] Starting BT Adapter...")
	if err := adapter.Enable(); err != nil {
		return nil, fmt.Errorf("[BLUETOOTH] Failed to enable adapter: %w", err)
	}

	// 2. Scan Bluetooth Devices With Timeout

	var deviceAddr bluetooth.Address

	log.Printf("[BLUETOOTH] Scanning for %s...\n", name)
	done := make(chan struct{})
	err := adapter.Scan(func(adapter *bluetooth.Adapter, result bluetooth.ScanResult) {
		if result.LocalName() == name {
			deviceAddr = result.Address
			log.Printf("[BLUETOOTH] Found %s (%s)!\n", name, deviceAddr.String())
			adapter.StopScan()
			close(done)
		}
	})
	if err != nil {
		return nil, err
	}

	select {
	case <-done:
	case <-time.After(timeout):
		return nil, fmt.Errorf("[BLUETOOTH] %s Not Found — Timeout", name)
	}

	// 3. Connect to the BLE Module

	log.Printf("[BLUETOOTH] Connecting to %s...\n", name)
	device, err := adapter.Connect(deviceAddr, bluetooth.ConnectionParams{})
	if err != nil {
		return nil, fmt.Errorf("[BLUETOOTH] Failed to connect to device: %w", err)
	}
	log.Printf("[BLUETOOTH] Connected to %s!\n", name)

	// 4. Find Correct Service & Device Characteristics

	log.Println("[BLUETOOTH] Finding FFE1 Service and FFE2/FFE3 Characteristics")

	services, err := device.DiscoverServices(nil)
	if err != nil {
		return nil, err
	}

	var txChar, rxChar bluetooth.DeviceCharacteristic
	for _, svc := range services {
		// find correct service UUID
		if svc.UUID().String() == config.BLE_SERVICE_UUID {
			chars, err := svc.DiscoverCharacteristics(nil)
			if err != nil {
				return nil, err
			}

			// find correct characteristics
			for _, c := range chars {
				uuid := c.UUID().String()

				switch uuid {
				case config.BLE_RX_UUID:
					rxChar = c
				case config.BLE_TX_UUID:
					txChar = c
				}
			}
		}
	}
	if txChar.UUID() == (bluetooth.UUID{}) || rxChar.UUID() == (bluetooth.UUID{}) {
		return nil, errors.New("[BLUETOOTH] BLE UART characteristics FFE2/FFE3 Not Found")
	}

	log.Println("[BLUETOOTH] Found FFE1 Service and FFE2/FFE3 Characteristics!")

	// 5. Return new Client

	return &Client{device: device, txChar: txChar, rxChar: rxChar}, nil
}
