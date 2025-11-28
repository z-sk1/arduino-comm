package arduino

import (
	"fmt"

	"strings"

	"github.com/tarm/serial"
)

type Device struct {
	Port *serial.Port
}

func New(port *serial.Port) *Device {
	return &Device{Port: port}
}

func (d *Device) Close() error {
	return d.Port.Close()
}

func (d *Device) Exec(cmd string) error {
	cmd = strings.TrimSpace(cmd) + "\n"
	_, err := d.Port.Write([]byte(cmd))
	if err != nil {
		return fmt.Errorf("failed to send command: %w", err)
	}
	return nil
}

func (d *Device) Execf(format string, a ...interface{}) error {
	cmd := strings.TrimSpace(fmt.Sprintf(format, a...)) + "\n"
	_, err := d.Port.Write([]byte(cmd))
	if err != nil {
		return fmt.Errorf("failed to send command: %w", err)
	}
	return nil
}
