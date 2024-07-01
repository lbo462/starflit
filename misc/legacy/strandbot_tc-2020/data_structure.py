import random
import time
import turtle
from turtle import *

DIRECTIONS = ["EAST", "NORTH", "WEST", "SOUTH"]


def getLeftDirection(Direction):
    return DIRECTIONS[DIRECTIONS.index(Direction) + 1] if DIRECTIONS.index(Direction) + 1 != 4 else DIRECTIONS[0]


class Tile:
    def __init__(self):
        self.x = 0
        self.y = 0
        self.forward = None
        self.parent = None
        self.reachability = True
        self.facing = "EAST"

    def moveForward(self):
        pass

    def reachability(self):
        self.forward = Tile()
        self.forward.reachability = random.choice([True, False])

    def move(self):
        self.forward = Tile()
        self.forward.reachability = random.choice([True, False])
        if self.forward.reachability:
            print("Forward tile availible, moving forward...")

            if self.facing == "EAST":
                self.forward.x = self.x + 1
                self.forward.y = self.y

            if self.facing == "NORTH":
                self.forward.x = self.x
                self.forward.y = self.y + 1

            if self.facing == "WEST":
                self.forward.x = self.x - 1
                self.forward.y = self.y

            if self.facing == "SOUTH":
                self.forward.x = self.x
                self.forward.y = self.y - 1

            turtle.forward(20)
        else:
            print("turning left ")

            self.forward.facing = getLeftDirection(self.facing)
            self.forward.x = self.x
            self.forward.y = self.y
            turtle.left(90)


if __name__ == "__main__":
    current_tile = Tile()

    while True:
        # time.sleep(2)
        print(f"Current position ( {current_tile.x} , {current_tile.y} )")
        current_tile.move()
        current_tile.forward.parent = current_tile  # setup the parent of the next child
        current_tile = current_tile.forward  # moving on
