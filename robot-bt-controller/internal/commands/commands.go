package commands

// Command stores the command code and action description that the
// robot-maze-solver program understands.
type Command struct {
	Code   byte
	Action string
}

// CommandMap holds all the possible commands.
var CommandMap = map[byte]Command{
	'X': {Code: 'X', Action: "Toggle LED"},
}
