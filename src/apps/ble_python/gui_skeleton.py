#!/usr/bin/env python3

import tkinter as tk
import enum
import os
import atexit

os.system('xset r off')
def set_repeat_on():
    os.system('xset r on')
atexit.register(set_repeat_on)

class BLEAction(enum.Enum):
    NONE = 0
    RLEFT = 1
    RRIGHT = 2
    PFWD = 3
    PBACK = 4
    YLEFT = 5
    YRIGHT = 6
    ALTUP = 7
    ALTDOWN = 8
    ARM = 9
    INIT = 10
    KILL = 11

current_state = BLEAction.NONE

def start_rleft(_=None):
    print("start rleft")
    global current_state
    current_state = BLEAction.RLEFT
def end_rleft(_=None):
    print("end rleft")
    global current_state
    current_state = BLEAction.NONE
def start_rright(_=None):
    print("start rright")
    global current_state
    current_state = BLEAction.RRIGHT
def end_rright(_=None):
    print("end rright")
    global current_state
    current_state = BLEAction.NONE

def start_pfwd(_=None):
    print("start pfwd")
    global current_state
    current_state = BLEAction.PFWD
def end_pfwd(_=None):
    print("end pfwd")
    global current_state
    current_state = BLEAction.NONE
def start_pback(_=None):
    print("start pback")
    global current_state
    current_state = BLEAction.PBACK
def end_pback(_=None):
    print("end pback")
    global current_state
    current_state = BLEAction.NONE

def start_yright(_=None):
    print("start yright")
    global current_state
    current_state = BLEAction.YRIGHT
def end_yright(_=None):
    print("end yright")
    global current_state
    current_state = BLEAction.NONE
def start_yleft(_=None):
    print("start yleft")
    global current_state
    current_state = BLEAction.YLEFT
def end_yleft(_=None):
    print("end yleft")
    global current_state
    current_state = BLEAction.NONE

def start_altup(_=None):
    print("start altup")
    global current_state
    current_state = BLEAction.ALTUP
def end_altup(_=None):
    print("end altup")
    global current_state
    current_state = BLEAction.NONE
def start_altdown(_=None):
    print("start altdown")
    global current_state
    current_state = BLEAction.ALTDOWN
def end_altdown(_=None):
    print("end altdown")
    global current_state
    current_state = BLEAction.NONE

def start_arm(_=None):
    print("start arm")
    global current_state
    current_state = BLEAction.ARM
def end_arm(_=None):
    print("end arm")
    global current_state
    current_state = BLEAction.NONE
def start_init(_=None):
    print("start init")
    global current_state
    current_state = BLEAction.INIT
def end_init(_=None):
    print("end init")
    global current_state
    current_state = BLEAction.NONE
def start_kill(_=None):
    print("start kill")
    global current_state
    current_state = BLEAction.KILL
def end_kill(_=None):
    print("end kill")
    global current_state
    current_state = BLEAction.NONE

root = tk.Tk()



updown = tk.Frame(root)

altup = tk.Button(updown)
altup["text"] = "Up (q)"
altup.bind("<ButtonPress>", start_altup)
altup.bind("<ButtonRelease>", end_altup)
altup.grid(row=0, column=0)
root.bind("<KeyPress-q>", start_altup)
root.bind("<KeyRelease-q>", end_altup)
altdown = tk.Button(updown)
altdown["text"] = "Down (a)"
altdown.bind("<ButtonPress>", start_altdown)
altdown.bind("<ButtonRelease>", end_altdown)
altdown.grid(row=1, column=0)
root.bind("<KeyPress-a>", start_altdown)
root.bind("<KeyRelease-a>", end_altdown)

updown.grid(row=0, column=0)


yaw = tk.Frame(root)

yleft = tk.Button(yaw)
yleft["text"] = "Yaw Left (z)"
yleft.bind("<ButtonPress>", start_yleft)
yleft.bind("<ButtonRelease>", end_yleft)
yleft.grid(row=0, column=0)
root.bind("<KeyPress-z>", start_yleft)
root.bind("<KeyRelease-z>", end_yleft)
yright = tk.Button(yaw)
yright["text"] = "Yaw Right (c)"
yright.bind("<ButtonPress>", start_yright)
yright.bind("<ButtonRelease>", end_yright)
yright.grid(row=0, column=1)
root.bind("<KeyPress-c>", start_yright)
root.bind("<KeyRelease-c>", end_yright)

yaw.grid(row=1, column = 1)


dpad = tk.Frame(root)

rleft = tk.Button(dpad)
rleft["text"] = "Roll Left (left)"
rleft.bind("<ButtonPress>", start_rleft)
rleft.bind("<ButtonRelease>", end_rleft)
rleft.grid(row=1, column=0)
root.bind("<KeyPress-Left>", start_rleft)
root.bind("<KeyRelease-Left>", end_rleft)
rright = tk.Button(dpad)
rright["text"] = "Roll Right (right)"
rright.bind("<ButtonPress>", start_rright)
rright.bind("<ButtonRelease>", end_rright)
rright.grid(row=1, column=2)
root.bind("<KeyPress-Right>", start_rright)
root.bind("<KeyRelease-Right>", end_rright)
pfwd = tk.Button(dpad)
pfwd["text"] = "Pitch Forward (up)"
pfwd.bind("<ButtonPress>", start_pfwd)
pfwd.bind("<ButtonRelease>", end_pfwd)
pfwd.grid(row=0, column=1)
root.bind("<KeyPress-Up>", start_pfwd)
root.bind("<KeyRelease-Up>", end_pfwd)
pback = tk.Button(dpad)
pback["text"] = "Pitch Backward (down)"
pback.bind("<ButtonPress>", start_pback)
pback.bind("<ButtonRelease>", end_pback)
pback.grid(row=2, column=1)
root.bind("<KeyPress-Down>", start_pback)
root.bind("<KeyRelease-Down>", end_pback)

dpad.grid(row=0, column=1)


othercontrols = tk.Frame(root)

init = tk.Button(othercontrols)
init["text"] = "Init"
init.bind("<ButtonPress>", start_init)
init.bind("<ButtonRelease>", end_init)
init.grid(row=0, column=0)
arm = tk.Button(othercontrols)
arm["text"] = "Arm"
arm.bind("<ButtonPress>", start_arm)
arm.bind("<ButtonRelease>", end_arm)
arm.grid(row=1, column=0)
kill = tk.Button(othercontrols)
kill["text"] = "Kill"
kill.bind("<ButtonPress>", start_kill)
kill.bind("<ButtonRelease>", end_kill)
kill.grid(row=2, column=0)

othercontrols.grid(row=0, column=2)

def send_current_state():
    print(current_state)
    root.after(100, send_current_state)

send_current_state()
root.mainloop()
