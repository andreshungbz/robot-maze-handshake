package config

const (
	TELLO_CMD_RESP_ADDRESS     = "192.168.10.1:8889"    // UDP address for two-way communication between drone and computer
	TELLO_STATE_RX_ADDRESS     = "0.0.0.0:8890"         // UDP address for one-way communication with drone sending state data to computer
	TELLO_VIDEO_STREAM_ADDRESS = "udp://@0.0.0.0:11111" // UDP address for one-way communication with drone sending video data to computer
)
