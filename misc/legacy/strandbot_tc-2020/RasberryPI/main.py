"""
@author :: HBA
communcation between the RasberryPI and the Arduino on the standbeest
Rasberry POV
"""
import turtle
import serial
import json
import numpy as np  # pip install numpy
import math
import logging

from serial import Serial

USB = "/dev/cu.usbserial-DN02LIM2"  # TODO :: change this on RasberryPI

DIRECTIONS = ["EAST", "NORTH", "WEST", "SOUTH"]
TILE_SIZE = 35  # in cm

# Keys
KEY_REACHABILITY = "reachability"
KEY_LEFT = "left"
KEY_HELLO = "*"
KEY_ACK = "ACK"
KEY_FORWARD = "?"
KEY_TURN_LEFT = "."
KEY_TURN_RIGHT = "/"
ENCODING = "ascii"


def getLeftDirection(Direction):
    """HBA :: wng doesnt cover edge cases """
    return DIRECTIONS[DIRECTIONS.index(Direction) + 1] if DIRECTIONS.index(Direction) + 1 != 4 else DIRECTIONS[0]


def getRightDirection(Direction):
    """HBA :: wng doesnt cover edge cases """
    return DIRECTIONS[DIRECTIONS.index(Direction) - 1]


def isNextTileReachable(data: str):
    try:
        return json.loads(data).get(KEY_REACHABILITY)
    except:
        return False


class Tile:
    """
    @HBA
    Tile is a square on the map that have relative coordinates x, y parent tile where the robot got there from,
    reachability that tells whether or not the robot can reach this tile
    """

    def __init__(self, reachability=True, facing="EAST", parent=None, coordinates=None):
        self.x = 0
        self.y = 0  # coordinates
        self.forward = None  # pointer toward the forward tile
        self.parent = parent  # pointer toward the parent tile
        self.reachability = reachability  # weather or not we can move toward the tile (set to true by default )
        self.facing = facing  # direction of the robot
        self.matrixCoordinates = (None, None)
        if coordinates is not None:
            self.x = coordinates[0]
            self.y = coordinates[1]

    def moveForward(self):
        """
        HBA :: the forward tile is available so the strandbeest mark is as such and move toward it
        """
        self.forward = Tile(facing=self.facing, parent=self)  # by default reachability is set to true
        print("Forward tile available, moving forward...")

        self.forward.x, self.forward.y = self.forwardCordiantes()

        turtle.forward(5)  # draw it
        return self.forward

    def forwardCordiantes(self):
        """
        HBA :: compute the coordiantes of the tile forward
        :return: coordinates of the forward tile
        """
        forwardX = self.x
        forwardY = self.y

        if self.facing == "EAST":
            forwardX += 1

        if self.facing == "NORTH":
            forwardY += 1

        if self.facing == "WEST":
            forwardX -= 1

        if self.facing == "SOUTH":
            forwardY -= 1

        return forwardX, forwardY

    def flagForwardTile(self, turningLeft):
        """
        HBA :: flag forward tile as unreachable 🛑 🛑 🛑 🛑 🛑
        :arg turningLeft true if we are turning left false otherwise
        :return the the unreachable tile
        """
        logging.debug("Forward tile unreachable 🥺")

        NextTile = Tile(reachability=False, parent=self, coordinates=self.forwardCordiantes())  # saving the NextTile
        # coord's before turning

        if turningLeft:
            self.facing = getLeftDirection(self.facing)  # turning left
            turtle.left(90)
        else:
            self.facing = getRightDirection(self.facing)  # turning left
            turtle.right(90)
        return NextTile

    def adjacentTiles(self):
        """HBA
        :return a list of the tiles adjacent to self
        """
        return [Tile(coordinates=(self.x, self.y + 1)), Tile(coordinates=(self.x + 1, self.y)),
                Tile(coordinates=(self.x - 1, self.y)), Tile(coordinates=(self.x, self.y - 1))]

    def diagonalTiles(self):
        """
        :return a list of tiles on the diagonal of self """
        return [Tile(coordinates=(self.x + 1, self.y + 1)), Tile(coordinates=(self.x + 1, self.y - 1)),
                Tile(coordinates=(self.x - 1, self.y + 1)), Tile(coordinates=(self.x - 1, self.y - 1))]

    def __eq__(self, other):
        """HBA :: makes it easier to compare Tiles"""
        return self.x == other.x and self.y == other.y

    def __str__(self):
        """ printable class ❤️"""
        return f"( {self.x}, {self.y}) reachability : {self.reachability}"

    def __repr__(self):
        return str(self)  # ikr 😡

    def __hash__(self):
        return hash(str(self))  # HBA :: hashable class 🤯


class Map:

    def __init__(self):
        self.map = []
        self.borders = [0, 0, 0, 0]
        self.handler = None
        """
             _____borders[1]_____
            |                    |
            |                    |
        borders[0]          borders[2]
            |                    |
            |_____borders[3]_____|

        """
        self.adjacencyMatrix = None
        self.matrix = None  # a Matrix representation of the map
        self.routingTable = {}

    def append(self, Tile: Tile):
        self.map.append(Tile)
        # update borders
        self.borders[0] = min(self.borders[0], Tile.x)
        self.borders[2] = max(self.borders[2], Tile.x)
        self.borders[1] = max(self.borders[1], Tile.y)
        self.borders[3] = min(self.borders[3], Tile.y)

    def draw(self):
        """
        HBA
        :return a string that represent a the
        """
        res = ""
        for i in range(self.borders[3] - 1, self.borders[1] + 1)[::-1]:
            # res += f"\n"
            for j in range(self.borders[0] - 1, self.borders[2] + 1):
                try:
                    res += "| ✅ " if self.map[self.map.index(Tile(coordinates=(j, i)))].reachability else "| ⛔️ "
                except:
                    res += "| ❔ "
            # res += "|\n"
            # res += "-----" * ((self.borders[2] - self.borders[0]))
            res += "|\n"
        return res

    def write(self, filename="map.txt"):
        """HBA ::
        write the map onto a file for easier logging
        """
        with open(filename, "w") as file:
            file.write(self.draw())

    def toAdjacencyMatrix(self):
        """
        set the Matrix of the graph
        :return: adjacency matrix
        """
        colomn = []
        for tile in self.map:
            line = []
            for isAdjacantTile in self.map:
                if isAdjacantTile in tile.adjacentTiles() and isAdjacantTile.reachability:
                    line.append(1)
                else:
                    line.append(math.inf)
            colomn.append(line)

        self.adjacencyMatrix = np.array(colomn)  # change to array to matrix class if needed

    def transitiveClosure(self):
        """
        HBA :: get all the links between the nods by using the transitive closure
        https://en.wikipedia.org/wiki/Transitive_closure
        :return
        """
        raise NotImplementedError  # xD
        self.toAdjacencyMatrix()  # setting up the Matrix
        Matrix = self.adjacencyMatrix
        old_M = Matrix
        i = 2
        while True:
            Matrix = Matrix + self.adjacencyMatrix ** i
            i += 1
            print(i)
            if (Matrix == old_M).all():
                self.adjacencyMatrix = Matrix
                return self.adjacencyMatrix
            old_M = Matrix

    def getTileByCoordinates(self, x, y):
        """ HBA :: this is so we get the right pointer toward the right object
        :return"""
        targetTile = Tile(coordinates=(x, y))
        for tile in self.map:
            if targetTile == tile:
                return tile

    def getTileByMatrixCoordinates(self, x, y):
        coordinates = (x + self.borders[0], self.borders[1] - y)
        return self.getTileByCoordinates(*coordinates)

    def initRoutingTable(self):
        """
        HBA
        THIS MUST BE CALLED AFTER THE WHOLE STRUCTURE IS KNOWN AND STORED IN self.map
        create the routing tables and store them in a dictionnary
        each source-destination couple has a key on this dictionnary and as value the route made out of a list of tiles
        to reach the destination
        example :

        self.routingTable = { (source , destination) : [Tile1 , Tile 2]  }

        both the source and destination are Tile objects

        so to get from the source to destination you will have to follow the route [Tile1, Tile2]

        :return:
        """
        self.toAdjacencyMatrix()
        for source in self.map:
            for destination in self.map:
                self.routingTable[(source, destination)] = [destination] if \
                    self.adjacencyMatrix[self.map.index(source)][
                        self.map.index(
                            destination)] == 1 else []

        return self.routingTable

    def dijkstra(self):
        """HBA ::
        fill the routing table by applying the dijkstra algo
        :return the routing table
        """
        self.initRoutingTable()  # setting up the routingTabe
        for j in range(len(self.adjacencyMatrix)):
            for i in range(len(self.adjacencyMatrix[j])):  # computing potential path from map[j] 2 map[i]
                hop = self.adjacencyMatrix[j][i]  # a potential hop
                hopTile = self.map[i]

                targetList = list(map((lambda x: x + hop), self.adjacencyMatrix[i]))

                for index, target in enumerate(targetList):
                    if target < self.adjacencyMatrix[j][index]:
                        self.adjacencyMatrix[j][index] = target
                        self.routingTable[(self.map[j], self.map[index])] = list(dict.fromkeys(
                            self.routingTable[(self.map[j], hopTile)] + self.routingTable[
                                (self.map[i], self.map[index])]))
                        # list(dict.fromkeys removes  duplicates
                        # the path from j -> self.map[index] is j -> hopTile -> self.map[index]
        return self.routingTable

    def route(self, TileA: Tile, TileB: Tile):
        """ HBA
        call this after dijkstra
        :return a list of tiles that are represent the route we can take to move from TileA  inculded to TileB inculded also
        :param TileA to
        :param TileB
        """
        if TileA not in self.map or TileB not in self.map:
            return None  # come on ...
        # TileA and TileB are in the map
        try:
            print(f"route from {TileA.x}, {TileA.y} to {TileB.x}, {TileB.y}")
            res = self.routingTable[(TileA, TileB)]
            res = [TileA] + res
            emojiRoute = ""
            for index, Tile in enumerate(res):
                if index != len(res) - 1:
                    # not last element
                    x = res[index + 1].x - Tile.x
                    y = res[index + 1].y - Tile.y
                    if x == 1:
                        Tile.facing = "EAST"
                        emojiRoute += "➡️"
                    if x == -1:
                        Tile.facing = "WEST"
                        emojiRoute += "⬅️"
                    if y == 1:
                        Tile.facing = "NORTH"
                        emojiRoute += "⬆️"
                    if y == -1:
                        Tile.facing = "SOUTH"
                        emojiRoute += "⬇️️"
            print(emojiRoute)
            return res
        except KeyError:
            return []

    def getMatrix(self):
        """
        HBA
        get a matrix representaiton of the current map """
        realtiveZero = (self.borders[1] - self.borders[3] + 1, self.borders[2] - self.borders[0] + 1)
        self.matrix = np.zeros(realtiveZero)  # init
        for tile in self.map:
            self.matrix[self.borders[1] - tile.y, tile.x - self.borders[0]] = 1 if tile.reachability else math.inf
        return self.matrix

    def fillInTheGaps(self):
        """
        HBA :: when we have an unknown Tile surrounded by unreachable tile like in the example bellow we assume it is an
        unreachable tile too
        | ⛔️ | ❔ |
        | ✅ | ⛔️ |
        :return
        """
        for tile in self.map:
            if not tile.reachability:
                diagonals = [self.map[index] for index in range(len(self.map)) if
                             (self.map[index] in tile.diagonalTiles() and not self.map[index].reachability)]
                for diagonal in diagonals:
                    unknownTile = list(set(diagonal.adjacentTiles()).intersection(set(tile.adjacentTiles())))[0]
                    unknownTile.reachability = False
                    if unknownTile not in self.map:
                        self.append(unknownTile)

    def isMapLimited(self):
        """HBA
        there gotta be a cleaner way to code this
        :return bool that tells if the map has been completed or no """

        self.getMatrix()
        for line in range(0, len(self.matrix)):
            for colomn in range(0, len(self.matrix[0])):
                if self.matrix[line, colomn] == math.inf:
                    break
                if self.matrix[line, colomn] == 1:
                    return False
        for colomn in range(0, len(self.matrix[0])):
            for line in range(0, len(self.matrix)):
                if self.matrix[line, colomn] == math.inf:
                    break
                if self.matrix[line, colomn] == 1:
                    return False

        for line in range(0, len(self.matrix)):
            for colomn in range(0, len(self.matrix[0]))[::-1]:
                if self.matrix[line, colomn] == math.inf:
                    break
                if self.matrix[line, colomn] == 1:
                    return False
        for colomn in range(0, len(self.matrix[0])):
            for line in range(0, len(self.matrix))[::-1]:
                if self.matrix[line, colomn] == math.inf:
                    break
                if self.matrix[line, colomn] == 1:
                    return False

        return True

    def getBreachs(self) -> list:
        """HBA
        breaches are reachable Tiles that lead to unknown tiles
        :return a list of all the breaches in the map if there is none returns an empty list
        """
        result = []

        self.getMatrix()
        for line in range(0, len(self.matrix)):
            for colomn in range(0, len(self.matrix[0])):
                if self.matrix[line, colomn] == math.inf:
                    break
                if self.matrix[line, colomn] == 1:
                    result.append(self.getTileByMatrixCoordinates(colomn, line))
                    break
        for colomn in range(0, len(self.matrix[0])):
            for line in range(0, len(self.matrix)):
                if self.matrix[line, colomn] == math.inf:
                    break
                if self.matrix[line, colomn] == 1:
                    result.append(self.getTileByMatrixCoordinates(colomn, line))
                    break

        for line in range(0, len(self.matrix)):
            for colomn in range(0, len(self.matrix[0]))[::-1]:
                if self.matrix[line, colomn] == math.inf:
                    break
                if self.matrix[line, colomn] == 1:
                    result.append(self.getTileByMatrixCoordinates(colomn, line))
                    break
        for colomn in range(0, len(self.matrix[0])):
            for line in range(0, len(self.matrix))[::-1]:
                if self.matrix[line, colomn] == math.inf:
                    break
                if self.matrix[line, colomn] == 1:
                    result.append(self.getTileByMatrixCoordinates(colomn, line))
                    break

        return list(set(result))


class Arduino(Serial):
    def cycle(self, cmd: str):
        """
        most our communcaction goes into a send_command-receive_response cycle
        this method makes it easier to send in a command and wait for the response to come back from that command
        :param cmd:
        :return:
        """
        if cmd not in [KEY_FORWARD, KEY_HELLO, KEY_TURN_LEFT, KEY_TURN_RIGHT]:
            raise AttributeError("cmd not recognized")
        self.write(cmd.encode(ENCODING))
        while True:
            try:
                resp = self.readline().decode(ENCODING)
                return json.loads(resp)
            except:
                logging.exception("error parsing :" + resp)

    def handshake(self):
        """HBA
        send a Hello to the Arduino and wait for an ACK
        :param ARDUINO:
        :return:
        """
        while True:
            self.write(KEY_HELLO.encode(encoding=ENCODING))
            logging.debug("sent Hello to arduino ")
            rcv = self.readline().decode(encoding=ENCODING)
            # logging.debug("received data from Arduino :" + rcv)
            if KEY_ACK in rcv:
                logging.debug("Received ACK ")
                break

    def discoverAdjacantTiles(self, currentTile: Tile, Map: Map):
        """
        HBA discover all 4 adjacant tiles to the currentTile and add them to the map
        :param currentTile:
        :param Map:
        :return:
        """
        logging.info("begin discovery of adjacant tiles")
        for _ in range(4):
            forwardTileInfo = self.cycle(KEY_TURN_LEFT)
            currentTile.facing = getLeftDirection(currentTile.facing)
            forwardTile = currentTile.moveForward()
            forwardTile.reachability = forwardTileInfo[KEY_REACHABILITY]
            logging.debug("forwardTile :" + str(forwardTile))
            Map.append(forwardTile)

    def go2(self, currentTile, route):
        """HBA
        send the necessary commands to follow a route
        :param route:
        :return:
        """
        if currentTile != route[0]:
            logging.exception("currentTile != route[0]")
            raise ValueError("currentTile must be the same as the starting tile of the route")
        cmds = []
        for tile in route:
            pass


def handler(data: dict, CurrentTile: Tile, Map: Map):
    """ HBA
    :return nextTile we ought to visit and the corresponding command
    :param Map:
    :param CurrentTile:
    :arg data from the Arduino and the list of the Tiles
    """
    if len(data) == 0 or data[KEY_REACHABILITY]:
        NextTile = CurrentTile.moveForward()
        Map.append(NextTile)
        return NextTile, KEY_FORWARD

    else:  # forward Tile unreachable
        breaches = Map.getBreachs()
        turningLeft = True  # TODO :: find an algo to do this
        Map.append(CurrentTile.flagForwardTile(turningLeft))
        cmd = KEY_TURN_LEFT if turningLeft else KEY_TURN_RIGHT
        return CurrentTile, cmd


def drawMap(Tiles: Tile, zoom: int):
    """
    HBA
    :return a string that represent a the
    """
    res = ""
    for i in range(-zoom, zoom):
        # res += f"\n"
        for j in range(-zoom, zoom):
            try:
                res += "| ✅ " if Tiles[Map.map.index(Tile(coordinates=(-j, i)))].reachability else "| ⛔️ "
            except:
                res += "| ❔ "
        res += "|\n"
        res += "-----" * (zoom * 2 - 1)
        res += "|\n"

    return res


def move(startingTile, finishTile):
    """
    the rasberry can tell the arduino to either move forward or turn left or right
    :param direction must be a keyword
    :return"""
    pass


def discoverAdjacantTiles(MAP: Map, currentTile: Tile, ARDUINO):
    """
    turn left 4 time to get reachability data from all 4 adjacant data
    :param MAP:
    :param ARDUINO:
    :return:
    """
    raise NotImplementedError


def route2cmd(route):
    """
    :param route: route from a Tile to another
    :return: list of cmds to reach the end point
    """
    raise NotImplementedError


def discover(MAP: Map, ARDUINO):
    """
    HBA :: a small algo to make the strandbeest discover the TC hall
    :return
    """

    CurrentTile = Tile()  # initializing the first Tile
    MAP.append(CurrentTile)

    # ARDUINO.write(KEY_FORWARD.encode(encoding="ascii"))
    logging.debug("MAP.getBreachs() :" + str(MAP.getBreachs()))

    while MAP.getBreachs():
        ARDUINO.discoverAdjacantTiles(CurrentTile, MAP)  # discover
        Breaches = MAP.getBreachs()
        MAP.dijkstra()
        route = MAP.route(CurrentTile, Breaches[0])
        ARDUINO.go2(CurrentTile, route)

        CurrentTile = route[-1]

        # try:
        #     parsedData = json.loads(rcv)
        #     nextTile, cmd = handler(parsedData, CurrentTile, MAP)
        #     logging.debug("command sent to Arduino :" + cmd)
        #     ARDUINO.write(cmd.encode(encoding="ascii"))
        #
        #     CurrentTile = nextTile
        #     logging.debug("the map :" + str(MAP.map))
        # except:
        #     logging.exception("Exception when parsing data from arduino")


    return 0


if __name__ == '__main__':
    logging.basicConfig(format='%(asctime)s--%(levelname)s :%(message)s',
                        level=logging.DEBUG)  # TODO :: change the logging level to info upon implmentation
    MAP = Map()
    # open connection
    ARDUINO = Arduino(USB)

    ARDUINO.flush()

    logging.info("attempting handshake")
    ARDUINO.handshake()
    logging.info("Handshake with Arduino completed 😷, beginning discovery protocol")

    discover(MAP, ARDUINO)
    logging.info("discovery finished")
