"""
HBA
to make the project efficient and easier to test and modify we made a bunch of tests to make sure everything works just
fine
note some testes aren't perfect and dont test edge cases or typeerror
"""

import unittest
import turtle
import random

from RasberryPI.main import *

class MyTestCase(unittest.TestCase):
    def test_getLeftDirection(self):
        self.assertEqual(getLeftDirection("EAST"), "NORTH")
        self.assertEqual(getLeftDirection("NORTH"), "WEST")
        self.assertEqual(getLeftDirection("WEST"), "SOUTH")
        self.assertEqual(getLeftDirection("SOUTH"), "EAST")

    def test_getRightDirection(self):
        self.assertEqual(getRightDirection("EAST"), "SOUTH")
        self.assertEqual(getRightDirection("SOUTH"), "WEST")

    def test_isNextTileReachable(self):
        self.assertEqual(isNextTileReachable("{\"reachability\" : false }"), False)
        self.assertEqual(isNextTileReachable("{\"reachability\" : true }"), True)
        self.assertEqual(isNextTileReachable("some random stuff "), False)
        self.assertEqual(isNextTileReachable(3), False)

    def test_Tiles(self):
        CurrentTile = Tile()
        for _ in range(10):
            CurrentTile = CurrentTile.moveForward()
        self.assertEqual(CurrentTile.x, 10, "x should be equal to 1 when u move ")
        self.assertEqual(CurrentTile.y, 0)

    def test_flag(self):
        CurrentTile = Tile()
        CurrentTile.flagForwardTile(True)
        self.assertEqual(CurrentTile.facing, "NORTH")
        CurrentTile = CurrentTile.moveForward()
        self.assertEqual(CurrentTile.x, 0)
        self.assertEqual(CurrentTile.y, 1)
        CurrentTile.flagForwardTile(True)
        CurrentTile = CurrentTile.moveForward()
        self.assertEqual(CurrentTile.x, -1)
        self.assertEqual(CurrentTile.y, 1)
        self.assertEqual(CurrentTile.facing, "WEST")
        CurrentTile.flagForwardTile(True)
        CurrentTile = CurrentTile.moveForward()
        self.assertEqual(CurrentTile.x, -1)
        self.assertEqual(CurrentTile.y, 0)
        self.assertEqual(CurrentTile.facing, "SOUTH")
        CurrentTile.flagForwardTile(True)
        CurrentTile = CurrentTile.moveForward()
        self.assertEqual(CurrentTile.x, 0)
        self.assertEqual(CurrentTile.y, 0)
        self.assertEqual(CurrentTile.facing, "EAST")
        CurrentTile.flagForwardTile(False)
        CurrentTile = CurrentTile.moveForward()
        self.assertEqual(CurrentTile.x, 0)
        self.assertEqual(CurrentTile.y, -1)
        self.assertEqual(CurrentTile.facing, "SOUTH")

    def test_isAdjacant(self):
        self.assertTrue(Tile(coordinates=(1, 0)) in Tile(coordinates=(0, 0)).adjacentTiles())
        self.assertFalse(Tile(coordinates=(5, 0)) in Tile(coordinates=(0, 0)).adjacentTiles())

    def test_diagonals(self):
        tile0 = Tile()
        self.assertTrue(Tile(coordinates=(1, 1)) in tile0.diagonalTiles())
        self.assertTrue(Tile(coordinates=(-1, 1)) in tile0.diagonalTiles())
        self.assertFalse(Tile(coordinates=(1, 0)) in tile0.diagonalTiles())

    def test_toMatrix(self):
        MAP = Map()
        CurrentTile = Tile()  # initializing the first Tile
        MAP.append(CurrentTile)

        for _ in range(5):
            CurrentTile = handler(json.loads("{\"reachability\" : true }"), CurrentTile, MAP)[0]

        CurrentTile = handler(json.loads("{\"reachability\" : false ,\"left\" : false }"), CurrentTile, MAP)[0]

        for _ in range(5):
            CurrentTile = handler(json.loads("{\"reachability\" : true }"), CurrentTile, MAP)[0]

        print(MAP.draw())
        MAP.toAdjacencyMatrix()

        self.assertEqual(MAP.adjacencyMatrix[0, 1], 1)
        self.assertEqual(MAP.adjacencyMatrix[6, 5], 1)
        self.assertEqual(MAP.adjacencyMatrix[8, 11], math.inf)

    def test_transitiveClosure(self):
        # we don't see how this can be useful
        # MAP = Map()
        # CurrentTile = Tile()  # initializing the first Tile
        # MAP.append(CurrentTile)
        #
        # for _ in range(5):
        #     CurrentTile = handler("{\"reachability\" : true }", CurrentTile, MAP)
        #
        # MAP.toAdjacencyMatrix()
        # print(MAP.matrix)
        # print(MAP.transitiveClosure())

        # self.assertEqual(MAP.transitiveClosure().tolist(),[1,1,1,1,1])
        pass

    def test_fillGaps(self):
        """
        before fillInTheGaps()                  after fillInTheGaps()
        | ❔ | ⛔️ | ❔ |                          | ❔ | ⛔️ | ⛔️ |
        | ❔ | ✅ | ⛔️ |                          | ❔ | ✅ | ⛔️ |
        | ❔ | ❔ | ❔ |                          | ❔ | ❔ | ❔ |

        """
        MAP4 = Map()
        CurrentTile = Tile()  # initializing the first Tile
        T1 = Tile(reachability=False, coordinates=(1, 0))
        T2 = Tile(reachability=False, coordinates=(0, 1))

        MAP4.append(CurrentTile)
        MAP4.append(T1)
        MAP4.append(T2)

        MAP4.fillInTheGaps()  # should make the tile (1,1) with a reachability set to false

        self.assertFalse(MAP4.getTileByCoordinates(1, 1).reachability)
        self.assertTrue(MAP4.getTileByCoordinates(0, 0).reachability)
        # print(MAP.draw())

    def test_initRoutingTable(self):
        MAP = Map()
        CurrentTile = Tile()  # initializing the first Tile
        MAP.append(CurrentTile)

        for _ in range(2):
            CurrentTile = handler(json.loads("{\"reachability\" : true }"), CurrentTile, MAP)[0]

        MAP.initRoutingTable()

        self.assertEqual(MAP.routingTable[(MAP.map[0], MAP.map[2])], [])
        self.assertEqual(MAP.routingTable[(MAP.map[0], MAP.map[1])], [MAP.map[1]])

    def test_dijkstra(self):
        MAP = Map()
        CurrentTile = Tile()  # initializing the first Tile
        MAP.append(CurrentTile)

        for _ in range(5):
            CurrentTile = handler(json.loads("{\"reachability\" : true }"), CurrentTile, MAP)[0]

        CurrentTile = handler(json.loads("{\"reachability\" : false ,\"left\" : false }"), CurrentTile, MAP)[0]

        for _ in range(5):
            CurrentTile = handler(json.loads("{\"reachability\" : true }"), CurrentTile, MAP)[0]

        MAP.dijkstra()
        rt = MAP.routingTable[(MAP.map[0], MAP.map[5])]
        print(rt)

        self.assertListEqual(rt, [Tile(coordinates=(1, 0)), Tile(coordinates=(2, 0)), Tile(coordinates=(3, 0)),
                                  Tile(coordinates=(4, 0)), Tile(coordinates=(5, 0))], "buddy seek help")
        self.assertListEqual(MAP.routingTable[(MAP.map[4], MAP.map[6])], [], "lol")

        # self.assertEqual(MAP.matrix[3][4], 1)
        # self.assertEqual(MAP.matrix[5][2], 3)

    def test_getTileByMatrixCoordinates(self):
        MAP2 = Map()
        for line in range(4):
            for colomn in range(10):
                if colomn in range(2, 8) and line in [1, 2]:
                    theTile = Tile(reachability=True, coordinates=(colomn, line))
                else:
                    theTile = Tile(reachability=False, coordinates=(colomn, line))
                MAP2.append(theTile)

        MAP2.getMatrix()
        tile = MAP2.getTileByMatrixCoordinates(5, 3)
        self.assertEqual(tile, Tile(coordinates=(5, 0)))

    def test_isMapLimited_getBreaches(self):
        MAP1 = Map()
        CurrentTile = Tile()  # initializing the first Tile
        MAP1.append(CurrentTile)

        for _ in range(2):
            CurrentTile = handler(json.loads("{\"reachability\" : true }"), CurrentTile, MAP1)[0]

        CurrentTile = handler(json.loads("{\"reachability\" : false }"), CurrentTile, MAP1)[0]

        for _ in range(3):
            CurrentTile = handler(json.loads("{\"reachability\" : true }"), CurrentTile, MAP1)[0]

        CurrentTile = handler(json.loads("{\"reachability\" : false }"), CurrentTile, MAP1)[0]

        self.assertFalse(MAP1.isMapLimited())

        """MAP2 :
        | ❔ | ⛔️ | ⛔️ | ⛔️ | ⛔️ | ⛔️ | ⛔️ | ⛔️ | ⛔️ | ⛔️ | ⛔️ |
        | ❔ | ⛔️ | ⛔️ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ⛔️ | ⛔️ |
        | ❔ | ⛔️ | ⛔️ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ⛔️ | ⛔️ |
        | ❔ | ⛔️ | ⛔️ | ⛔️ | ⛔️ | ⛔️ | ⛔️ | ⛔️ | ⛔️ | ⛔️ | ⛔️ |
        
        """
        MAP2 = Map()
        for line in range(4):
            for colomn in range(10):
                if colomn in range(2, 8) and line in [1, 2]:
                    theTile = Tile(reachability=True, coordinates=(colomn, line))
                else:
                    theTile = Tile(reachability=False, coordinates=(colomn, line))
                MAP2.append(theTile)

        self.assertTrue(MAP2.isMapLimited())
        self.assertListEqual(MAP2.getBreachs(), [])

        MAP2.getTileByCoordinates(5, 0).reachability = True  # add a breachs
        MAP2.getTileByCoordinates(8, 1).reachability = True
        MAP2.getTileByCoordinates(9, 1).reachability = True

        self.assertFalse(MAP2.isMapLimited())
        self.assertIn(MAP2.getTileByCoordinates(5, 0), MAP2.getBreachs())
        self.assertIn(MAP2.getTileByCoordinates(9, 1), MAP2.getBreachs())

    def test_route(self):
        MAP = Map()
        CurrentTile = Tile()  # initializing the first Tile
        MAP.append(CurrentTile)

        for _ in range(2):
            CurrentTile = handler(json.loads("{\"reachability\" : true }"), CurrentTile, MAP)[0]

        CurrentTile = handler(json.loads("{\"reachability\" : false ,\"left\" : false }"), CurrentTile, MAP)[0]

        for _ in range(3):
            CurrentTile = handler(json.loads("{\"reachability\" : true }"), CurrentTile, MAP)[0]

        CurrentTile = handler(json.loads("{\"reachability\" : false ,\"left\" : false }"), CurrentTile, MAP)[0]

        for _ in range(2):
            CurrentTile = handler(json.loads("{\"reachability\" : true }"), CurrentTile, MAP)[0]

        CurrentTile = handler(json.loads("{\"reachability\" : false ,\"left\" : false }"), CurrentTile, MAP)[0]

        for _ in range(3):
            CurrentTile = handler(json.loads("{\"reachability\" : true }"), CurrentTile, MAP)[0]

        """
        the map we drew : 
            | ❔ | ❔ | ❔ | ❔ | ⛔️ | ❔ |
            | ❔ | ⛔️ | ✅ | ✅ | ✅ | ❔ |
            | ❔ | ❔ | ✅ | ❔ | ✅ | ❔ |
            | ❔ | ❔ | ✅ | ❔ | ✅ | ❔ |
       0    | ❔ | ❔ | ✅ | ✅ | ✅ | ⛔️ |
            | ❔ | ❔ | ❔ | ❔ | ❔ | ❔ |
                        0
        """

        MAP.dijkstra()
        T1 = Tile(coordinates=(0, 0))
        T2 = Tile(coordinates=(0, 3))
        unreachableTile = Tile(coordinates=(3, 0))
        T3 = Tile(coordinates=(2, 1))
        print(MAP.getMatrix())
        self.assertListEqual(MAP.route(T1, unreachableTile), [])
        self.assertListEqual(MAP.route(T1, T3),
                             [T1, Tile(coordinates=(1, 0)), Tile(coordinates=(2, 0)), Tile(coordinates=(2, 1))])
        self.assertListEqual(MAP.route(T1, T2),
                             [T1, Tile(coordinates=(0, 1)), Tile(coordinates=(0, 2)), Tile(coordinates=(0, 3))])

    def test_simualtion(self):
        """
        a little simulation to make sure everything works
        :return:
        """
        MAP = Map()
        CurrentTile = Tile()  # initializing the first Tile
        MAP.append(CurrentTile)

        CurrentTile = handler(json.loads("{\"reachability\" : false ,\"left\" : false }"), CurrentTile, MAP)[0]

        for _ in range(20):
            CurrentTile = handler(json.loads("{\"reachability\" : true }"), CurrentTile, MAP)[0]

        CurrentTile = handler(json.loads("{\"reachability\" : false ,\"left\" : false }"), CurrentTile, MAP)[0]

        for _ in range(20):
            CurrentTile = handler(json.loads("{\"reachability\" : true }"), CurrentTile, MAP)[0]

        handler(json.loads("{\"reachability\" : false ,\"left\" : false }"), CurrentTile, MAP)

        self.assertEqual(MAP.map[43].x, -21)
        self.assertEqual(MAP.map[37].y, 20)
        self.assertFalse(MAP.map[22].reachability)
        self.assertTrue(MAP.map[30].reachability)

        print(MAP.draw())
        # writeMap(Tiles,21)

        print("""
        PASSED ALL TESTS    （　ﾟДﾟ） 
        🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉
        ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣬⡛⣿⣿⣿⣯⢻ 
        ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⢻⣿⣿⢟⣻⣿⣿⣿⣿⣿⣿⣮⡻⣿⣿⣧ 
        ⣿⣿⣿⣿⣿⢻⣿⣿⣿⣿⣿⣿⣆⠻⡫⣢⠿⣿⣿⣿⣿⣿⣿⣿⣷⣜⢻⣿ 
        ⣿⣿⡏⣿⣿⣨⣝⠿⣿⣿⣿⣿⣿⢕⠸⣛⣩⣥⣄⣩⢝⣛⡿⠿⣿⣿⣆⢝ 
        ⣿⣿⢡⣸⣿⣏⣿⣿⣶⣯⣙⠫⢺⣿⣷⡈⣿⣿⣿⣿⡿⠿⢿⣟⣒⣋⣙⠊ 
        ⣿⡏⡿⣛⣍⢿⣮⣿⣿⣿⣿⣿⣿⣿⣶⣶⣶⣶⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿ 
        ⣿⢱⣾⣿⣿⣿⣝⡮⡻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠛⣋⣻⣿⣿⣿⣿ 
        ⢿⢸⣿⣿⣿⣿⣿⣿⣷⣽⣿⣿⣿⣿⣿⣿⣿⡕⣡⣴⣶⣿⣿⣿⡟⣿⣿⣿ 
        ⣦⡸⣿⣿⣿⣿⣿⣿⡛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⣿⣿⣿ 
        ⢛⠷⡹⣿⠋⣉⣠⣤⣶⣶⣿⣿⣿⣿⣿⣿⡿⠿⢿⣿⣿⣿⣿⣿⣷⢹⣿⣿ 
        ⣷⡝⣿⡞⣿⣿⣿⣿⣿⣿⣿⣿⡟⠋⠁⣠⣤⣤⣦⣽⣿⣿⣿⡿⠋⠘⣿⣿ 
        ⣿⣿⡹⣿⡼⣿⣿⣿⣿⣿⣿⣿⣧⡰⣿⣿⣿⣿⣿⣹⡿⠟⠉⡀⠄⠄⢿⣿ 
        ⣿⣿⣿⣽⣿⣼⣛⠿⠿⣿⣿⣿⣿⣿⣯⣿⠿⢟⣻⡽⢚⣤⡞⠄⠄⠄⢸⣿
        """)


class Arduino_Simulation(object):
    def __init__(self):
        self.helloStatus = False
        self.index = 0

    def readline(self):
        if not self.helloStatus:
            res = random.choice([b"ACK", b""])
            if res == b"ACK":
                self.helloStatus = True
            return res

        else:
            self.index += 1
            if self.index % 5 == 0:
                return b"{\"reachability\" : false }"
            return b"{\"reachability\" : true }"


class closed_Arduino(object):
    def readline(self):
        return b"{\"reachability\" : false }"

    def write(self, data: bytes):
        print(f"Arduino received : {data.decode()}")


class func(unittest.TestCase):
    # def __init__(self):
    #     super().__init__()
    #     self.SimulatedArduino = Arduino_Simulation()


    def test_handler(self):
        MAP = Map()
        CurrentTile = Tile()
        MAP.append(CurrentTile)
        NextTile, cmd = handler(json.loads("{\"reachability\" : true }"), CurrentTile, MAP)
        self.assertEqual(cmd, KEY_FORWARD)
        self.assertEqual(NextTile, Tile(coordinates=(1, 0)))
        self.assertIn(NextTile, MAP.map)
        NextTile, cmd = handler(json.loads("{\"reachability\" : false }"), NextTile, MAP)
        self.assertNotEqual(cmd, KEY_FORWARD)
        self.assertEqual(NextTile, Tile(coordinates=(1, 0)))
        self.assertIn(Tile(coordinates=(2, 0)), MAP.map)
        self.assertFalse(MAP.getTileByCoordinates(2, 0).reachability)

    def test_discover(self):
        MAP = Map()
        arduino = Arduino_Simulation()
        arduino.helloStatus = True

        # discover(MAP, arduino)
        discover(MAP, closed_Arduino())
        res = """| ❔ | ❔ | ⛔️ | ❔ |
| ❔ | ⛔️ | ✅ | ⛔️ |
| ❔ | ❔ | ⛔️ | ❔ |
| ❔ | ❔ | ❔ | ❔ |
"""
        self.assertEqual(MAP.draw(), res)


if __name__ == '__main__':
    unittest.main()
