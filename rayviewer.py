import serial
import curses
import time

PORT = "/dev/cu.usbmodem11403"
BAUD = 115200

SCREEN_W = 80
SCREEN_H = 21

START_BYTE = 0xAA
END_BYTE   = 0x55

SHADE_CHARS = [
    '#',  # close
    '*',  # medium
    '+',  # far
    '.'   # very far
]


def read_frame(ser):
    while True:
        b = ser.read(1)

        if not b:
            return None

        if b[0] == START_BYTE:
            break

    raw = ser.read(SCREEN_W * 2)

    if len(raw) != SCREEN_W * 2:
        return None

    end = ser.read(1)

    if not end:
        return None

    if end[0] != END_BYTE:
        return None

    frame = []

    for i in range(SCREEN_W):
        height = raw[i * 2]
        shade = raw[i * 2 + 1]

        frame.append((height, shade))

    return frame


def draw(stdscr, frame):
    stdscr.clear()

    for col in range(SCREEN_W):

        height, shade = frame[col]

        if height > SCREEN_H:
            height = SCREEN_H

        if shade > 3:
            shade = 3

        wall_char = SHADE_CHARS[shade]

        top = (SCREEN_H - height) // 2
        bottom = top + height

        for row in range(SCREEN_H):

            if top <= row < bottom:
                stdscr.addch(row, col, wall_char)
            else:
                stdscr.addch(row, col, ' ')

    stdscr.refresh()


def main(stdscr):
    ser = serial.Serial(
        PORT,
        BAUD,
        timeout=0.05
    )

    curses.curs_set(0)

    stdscr.nodelay(True)
    stdscr.timeout(0)

    while True:

        key = stdscr.getch()

        if key == ord('q'):
            break

        if key == ord('l'):
            ser.write(b'l')

        if key == ord('k'):
            ser.write(b'k')

        frame = read_frame(ser)

        if frame is not None:
            draw(stdscr, frame)

        time.sleep(0.002)

    ser.close()


if __name__ == "__main__":
    curses.wrapper(main)
