package commands

// Command stores the command code and action description that the
// robot-maze-solver program understands.
type Command struct {
	Code   byte   // code that is sent to the robot
	Action string // description of command
}

// CommandMap holds all the possible commands.
var CommandMap = map[byte]Command{
	'X': {Code: 'X', Action: "Toggle RGB LED"},
	'F': {Code: 'F', Action: "Drive robot forward"},
	'B': {Code: 'B', Action: "Drive robot backwards"},
	'L': {Code: 'L', Action: "Turn robot 90 degrees to the left"},
	'R': {Code: 'R', Action: "Turn robot 90 degrees to the right"},
	'1': {Code: '1', Action: "Pivot robot to the left"},
	'2': {Code: '2', Action: "Pivot robot to the right"},
	'A': {Code: 'A', Action: "Enable AUTONOMOUS mode on robot"},
	'M': {Code: 'M', Action: "Switch to MANUAL mode on robot"},
}

// KeyboardOrder defines the display order of commands for keyboard mode.
var KeyboardOrder = []byte{
	'X',
	'F',
	'B',
	'L',
	'R',
	'1',
	'2',
	'A',
	'M',
}

// GamepadMap maps a controller input to a command key.
var GamepadMap = map[string]byte{
	"x":             'X',
	"dpup":          'F',
	"dpdown":        'B',
	"leftshoulder":  'L',
	"rightshoulder": 'R',
	"lefttrigger":   '1',
	"righttrigger":  '2',
	"start":         'A',
	"back":          'M',
}

// GamepadDisplay maps the event button/axis name to a prettier name.
var GamepadDisplay = map[string]string{
	"x":             "X Button",
	"dpup":          "D-Pad Up",
	"dpdown":        "D-Pad Down",
	"leftshoulder":  "Left Shoulder",
	"rightshoulder": "Right Shoulder",
	"lefttrigger":   "Left Trigger",
	"righttrigger":  "Right Trigger",
	"start":         "Start Button",
	"back":          "Back Button",
}

// GamepadOrder defines the display order of commands for gamepad mode.
var GamepadOrder = []string{
	"x",
	"dpup",
	"dpdown",
	"leftshoulder",
	"rightshoulder",
	"lefttrigger",
	"righttrigger",
	"start",
	"back",
}
