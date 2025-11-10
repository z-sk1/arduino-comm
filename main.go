package main

import (
	"fmt"
	"log"

	"time"

	"github.com/ncruces/zenity"
	"github.com/tarm/serial"
	"github.com/z-sk1/arduino/arduino"
)

var Device *arduino.Device

func main() {
	portName := askForPort()
	openPort(portName)

	go readSerialLoop()
	setupTray()
}

func askForPort() string {
	port, err := zenity.Entry("Enter COM Port: (e.g COM3, COM5)")
	if err != nil {
		log.Fatal(err)
	}
	return port
}

func openPort(comPort string) {
	c := &serial.Config{Name: comPort, Baud: 9600}
	port, err := serial.OpenPort(c)
	if err != nil {
		log.Fatal(err)
	}

	Device = arduino.New(port)
}

func readSerialLoop() {
	buf := make([]byte, 128)
	for {
		n, err := Device.Port.Read(buf)
		if err != nil {
			fmt.Println("Read error:", err)
			continue
		}
		if n > 0 {
			fmt.Print(string(buf[:n]))
		} else {
			time.Sleep(10 * time.Millisecond)
		}
	}
}
