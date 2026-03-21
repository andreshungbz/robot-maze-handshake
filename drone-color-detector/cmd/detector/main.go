package main

import (
	"fmt"
	"log"

	"gocv.io/x/gocv"
)

func main() {
	introduction()

	log.Printf("gocv version: %s\n", gocv.Version())
	log.Printf("opencv lib version: %s\n", gocv.OpenCVVersion())
}

// introduction displays project information and links.
func introduction() {
	const (
		YELLOW = "\u001B[33m"
		RESET  = "\u001B[0m"
	)

	fmt.Printf("%-35s %s\n", YELLOW+"[drone-color-detector]"+RESET,
		"Program for communicating with the drone and detecting a color signal")
	fmt.Printf("%-35s %s\n", YELLOW+"[GitHub Repository]"+RESET,
		"https://github.com/andreshungbz/robot-maze-handshake")
	fmt.Printf("%-35s %s\n", YELLOW+"[Project Subfolder]"+RESET,
		"https://github.com/andreshungbz/robot-maze-handshake/tree/main/drone-color-detector")
	fmt.Println()
}
