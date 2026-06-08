import sys
import serial
import curses
import time

PORT = "/dev/cu.usbmodem11403"
BAUD = 115200

SCREEN_W = 80
SCREEN_H = 21

START_BYTE = 0xAA
END_BYTE   = 0x55


def read_frame(ser):
    # wait for start
    while True:
        b = ser.read(1)
        if not b:
            return None
        if b[0] == START_BYTE:
            break

    frame = ser.read(SCREEN_W)
    if len(frame) != SCREEN_W:
        return None

    end = ser.read(1)
    if not end or end[0] != END_BYTE:
        return None

    return frame


def draw(stdscr, frame):
    stdscr.clear()

    for col in range(SCREEN_W):
        h = frame[col]

        if h > SCREEN_H:
            h = SCREEN_H

        top = (SCREEN_H - h) // 2
        bottom = top + h

        for row in range(SCREEN_H):
            x = col
            y = row

            if top <= row < bottom:
                stdscr.addch(y, x, ord('#'))
            else:
                stdscr.addch(y, x, ord(' '))

    stdscr.refresh()


def main(stdscr):
    ser = serial.Serial(PORT, BAUD, timeout=0.1)

    curses.curs_set(0)
    stdscr.nodelay(True)
    stdscr.timeout(0)

    last_send = time.time()

    while True:
        # -------- input ----------
        key = stdscr.getch()

        if key == ord('q'):
            break

        if key == ord('a'):
            ser.write(b'a')

        if key == ord('d'):
            ser.write(b'd')

        # -------- frame ----------
        frame = read_frame(ser)

        if frame:
            draw(stdscr, frame)

        time.sleep(0.005)


if __name__ == "__main__":
    curses.wrapper(main)
