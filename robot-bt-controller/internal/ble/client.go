package ble

import "tinygo.org/x/bluetooth"

// Client holds the BLE device and its transmit and receive characteristics.
type Client struct {
	device bluetooth.Device
	txChar bluetooth.DeviceCharacteristic
	rxChar bluetooth.DeviceCharacteristic
}

// Write sends data to the transmit characteristic of the BLE device.
func (c *Client) Write(data []byte) (int, error) {
	return c.txChar.WriteWithoutResponse(data)
}

// Subscribe reads data from the receive characteristic of the BLE device.
func (c *Client) Subscribe(handler func([]byte)) error {
	return c.rxChar.EnableNotifications(func(buf []byte) {
		handler(buf)
	})
}

// Close disconnects the BT device.
func (c *Client) Close() error {
	return c.device.Disconnect()
}
