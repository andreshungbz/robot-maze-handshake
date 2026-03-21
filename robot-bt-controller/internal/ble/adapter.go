package ble

import (
	"errors"
	"fmt"
	"log/slog"
	"time"

	"github.com/andreshungbz/robot-maze-handshake/robot-bt-controller/internal/config"
	"tinygo.org/x/bluetooth"
)

var adapter = bluetooth.DefaultAdapter

// ConnectToBLEDevice uses Bluetooth to connect to a BLE device and checks for the
// required service and characteristics.
func ConnectToBLEDevice(name string, timeout time.Duration, logger *slog.Logger) (*Client, error) {
	// 1. Enable Bluetooth Adapter

	logger.Info("Starting BT Adapter")

	if err := adapter.Enable(); err != nil {
		return nil, fmt.Errorf("Failed to enable BT Adapter: %w", err)
	}

	// 2. Scan Bluetooth Devices With Timeout

	logger.Info("Scanning for BT Device", "name", name)

	var deviceAddr bluetooth.Address

	done := make(chan struct{})
	err := adapter.Scan(func(adapter *bluetooth.Adapter, result bluetooth.ScanResult) {
		if result.LocalName() == name {
			deviceAddr = result.Address
			logger.Info("Found BT Device", "name", name, "device", deviceAddr.String())
			adapter.StopScan()
			close(done)
		}
	})
	if err != nil {
		return nil, err
	}

	// block until done or after timeout
	select {
	case <-done:
	case <-time.After(timeout):
		return nil, fmt.Errorf("BT Device %s Not Found — Timeout", name)
	}

	// 3. Connect to the BLE Module

	logger.Info("Connecting to BT Device", "name", name)

	device, err := adapter.Connect(deviceAddr, bluetooth.ConnectionParams{})
	if err != nil {
		return nil, fmt.Errorf("Failed to connect to BT Device: %w", err)
	}

	logger.Info("Connected to BT Device!", "name", name)

	// 4. Find Correct Service & Device Characteristics

	logger.Info("Finding FFE1 Service and FFE2/FFE3 Characteristics")

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

	// validate characteristics
	if txChar.UUID() == (bluetooth.UUID{}) || rxChar.UUID() == (bluetooth.UUID{}) {
		return nil, errors.New("BLE UART characteristics FFE2/FFE3 Not Found")
	}

	logger.Info("Found FFE1 Service and FFE2/FFE3 Characteristics!")

	// 5. Return new Client

	return &Client{
		device: device,
		txChar: txChar,
		rxChar: rxChar,
	}, nil
}
