## Game rules

· Four players, each of them controlled by a program, play on a square board with 60× 60 cells. The goal in the game is to conquer the maximum number of control points (henceforth called posts for the sake of brevity). There are 16 such posts distributed on the board.
· Each cell of the board is identified with a pair (i,j), where 0 ≤ i, j < 60. The i axis goes from north to south, and the j axis from west to east.
· Each player starts in a quadrant of the board: players 0, 1, 2 and 3 are initially in the north-west, north-east, south-east and south-west quadrants, respectively.
· There are three kinds of units: soldiers, helicopters and parachuters. (In fact, a parachuter is only the possibility of throwing a new soldier, who still does not exist, from a helicopter. So strictly speaking parachuters as such never exist.) When a match starts, each player has 20 soldiers, 2 helicopters and 0 parachuters. Soldiers start in their quadrant, randomly distributed. Helicopters of player 0 start oriented eastwards and southwards, respectively. Helicopters of the other players are oriented symmetrically.
· There are four kinds of land: forest, grass, water and mountain. All the perimeter of the board is surrounded by mountains. Soldiers can only be in forests or grass. Helicopters can be in any kind of land except for mountains (we can imagine they are too high). Initially, the land on which soldiers and helicopters are is consistent with these rules. Soldiers and helicopters move in different planes (heights), and therefore never interfere.
· A match lasts 200 rounds. At each round, the four players give instructions to their units. If a soldier or helicopter gets more than one instruction in a round, only the first one is considered for execution; the rest are ignored.
· At the end of each round all instructions of all players are considered for execution, in the following order: first those of parachuters, then those of helicopters, and finally those of soldiers. Inside each of these groups, instructions are executed in a random order, without giving priority to any of the players.
· Soldiers do not move in a round if they do not get any instruction, if the place they should move to is not allowed (there is water or mountain), or if in the moment of trying to execute the instruction, another soldier (of the same player or an enemy) occupies the place they should move to.
· Similarly, a helicopter does not move in a round if it does not get any instruction, if the place it should move to is not allowed (there is mountain), or if in the moment of trying to execute the instruction, another helicopter (of the same player or an enemy) occupies the place they should move to.
· Soldiers occupy one cell. Soldiers can try to move to any of the adjacent cells, be it horizontally, vertically, or in diagonal. If the cell they try to move to is occupied by a soldier of another player, the enemy gets a random damage depending on the land he is: between 20 and 40 points if he is in a forest (and so it is relatively protected), and between 50 and 100 points if he is on grass (and so it is relatively unprotected). Each soldier initially has 100 life points. When all are lost, the soldier dies. Soldiers of the same player never attack each other.
· Helicopters occupy a square of 5 × 5 cells of sky. At each round, each helicopter can try to move one or two cells forward (in the direction and sense in which it is oriented). If any of the 5 (or 10) new cells it should move to is occupied by another helicopter, or if any is mountain, the helicopter does not move (or moves only one of the two steps, if the first step is possible but the second is not). Helicopters never attack each other, and cannot get any kind of damage. So each player has 2 helicopters all along the match.
· Helicopters can also turn around 90 degrees clockwise, turn around 90 degrees counter clockwise, or throw napalm. Turns are always valid. On the other hand, napalm can only be thrown if it is available: each helicopter has to wait a minimum of 30 rounds from the last throw of napalm (or from the beginning of the match).
· When a helicopter throws napalm, the square 5 × 5 of land which is below it is set on fire, and all soldiers on it, no matter of which player, die. Moreover, fire burns for 5 rounds on grass and water, and 10 rounds on forest. After burning, forests become grass.
· At the end of each round, fire propagates according to the following rule: any cell with forest and without fire that is horizontally or vertically adjacent to one or more cells with fire, has a probability of 10 per cent to start burning. If that is the case, fire lasts 10 rounds (at least: when napalm is thrown on a cell that is already burning, the counter of duration of fire is reset to the maximum).
· A soldier dies when he moves to a cell of grass or forest with fire, or when he is on a cell of forest on which fire is propagated.
· Each soldier or helicopter has a positive integer that uniquely identifies it. When a soldier dies, he disappears (included the identifier), and a new parachuter of another player appears (according to what is explained below), who can later become a soldier. Therefore, the sum of soldiers and parachuters along the match is always 80.
· Each helicopter has a certain number of parachuters available. Each of these allows making a healthy soldier with a new identifier appear below the respective helicopter. Even though a player can have many parachuters available, at each round at most 4 parachuters can be thrown. Moreover, parachuters may expire: an unused parachuter in 20 rounds dies. Additionally, a parachuter who is thrown on a cell which is not covered by the 5 × 5 square of the corresponding helicopter, or on a cell with water, or with fire, or with a soldier (own or enemy) also dies.
· When an enemy soldier is killed, be it by attacking him with an own soldier or by throwing napalm, the attacking player gets a new parachuter.
· Any soldier that dies due to moving into a fire, or by the propagation of a fire, or by napalm thrown by the own player, becomes a parachuter of one of the other three players, chosen uniformly at random.
· Expired parachuters and those that die when they are thrown also become parachuters of another player. The new player is chosen always at random, with an exception: if a parachuter is thrown over a soldier of another enemy player, the new parachuter is added to this player.
· When a new parachuter is added to a player, the helicopter it becomes assigned to is determined randomly.
· When a soldier moves to a post, or when a parachuter falls on it (if the post does not have any soldier), the post is conquered. Posts can be conquered and reconquered by several players as many times as wished. Initially, posts do not belong to any player.
· According to their strategic location, posts may have a high value (100 points) or a low value (50 points). At each quadrant there are 2 posts of 100 points, and 2 other posts of 50 points. The points achieved by a player in a round are computed as follows: each post that the player controls at the end of the round gives as many points to that player as its value, and each soldier (no parachuter) of the player gives 1 additional point. These points are accumulated to the score of the player in the previous round.

## PROGRAMMING

The first thing you should do is to download the source code. This source code includes a C++ program that runs the matches and also an HTML viewer to watch them in a nice animated format. Also, a “Null” player and a “Demo” player are provided to make it easier to start coding your own player.

# Running your first match

Here we will explain how to run the game under Linux, but a similar procedure should work as well under Windows, Mac, FreeBSD, OpenSolaris, …The only requirements on your system are g++, make and a modern browser like Mozilla Firefox or Google Chrome.

To run your first match, follow the next steps:

    Open a console and cd to the directory where you extracted the source code.

    Run:
    
    ```
    make all
    ```

    to build the game and all the players. Note that Makefile identifies any file matching AI*.cc as a player.
    This creates an executable file called Game. This executable allows you to run a match using a command like:
    ```
    ./Game Demo Demo Demo Demo -s 30 -i default.cnf -o default.out
    ```

    In this case, this runs a match with random seed 30 where four instances of the player “Demo” play with the parameters defined in default.cnf (the default parameters). The output of this match is redirected to the file default.out.
    To watch a match, open the viewer file viewer.html with your browser and load the file default.out. Or alternatively use the script viewer.sh, e.g. viewer.sh default.out.



