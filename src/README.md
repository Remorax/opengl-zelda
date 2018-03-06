# INSTRUCTIONS
The game consists of a boat, and three types of enemies and a boss who appears when more than half of enemies of each type have been destroyed.
 
## Enemies
1. The first type of enemy moves around randomly and deals damage in health in case of collision. It doesn't drop any collectibles on being killed.
2. The second type of enemy is stationary and shoots small-sized bullets at short range. It also has only one cannon. Drops coins on being killed.
3. The third type of enemy has 2 cannons, so fires 2 bullets simultaneously and has medium-sized bullets which it fires at long range. These bullets deal much more damage than the bullets fired by the type 2 enemy. Drops health on being killed.
4. On killing half of type 2 and type 3 enemies, a boss appears. He moves around randomly and shoots large sized bullets at large range, which cause instant death to the boat on collision. Drops speedups on being killed.

## Boat
1. The boat also has a cannon with a target which depends on the mouse position. Move the mouse around to adjust the aim and press f to fire.
2. Use the arrow keys for moving around. Using the up and down arrow keys moves the boat up and down, using left and right arrow key rotates the boat in that direction. 
3. Use the spacebar for jumping up and down.
4. Boat at rest bobs up and down.

## Views
1. Boat cam: Attached to front of boat. Activate by pressing b.
2. Sky cam: Top view of boat. Activate by pressing s.
3. Tower cam: Tower view of boat. Activate by pressing t.
4. Follow cam: Cam which follows the boat. Default view. Revert by pressing n.
5. Man cam: Cam which follows man when he walks. Activate by pressing m. Activating another view while in man view takes the man back to the boat.  
6. Helicopter cam: 
    a. Click and drag: Activate by pressing LMB. Gives a target view of the environment. Deactivate by pressing RMB.
    b. Scroll: Scroll up and down to zoom in and out.

## Collectibles
1. Coins: Fills the coin bar by one. Dropped by level 2 enemy. Also available over barrels and on the island.
2. Health: Restores one health bar for the player. Dropped by level 3 enemy. Also available over barrels and on the island.
3. Speedup: Speeds up the boat movement in up and down directions 10x times. Dropped by boss. Also available over barrels and on the island.
4. Green Health: Restores health completely. Found on the island after collecting all other powerups. After collecting the island's green heart, the island disappears and reappears with the original collectibles elsewhere. 

## Bars
1. Health Bar: Indicates health of boat. Contains 20 units.
2. Coins Bar: Indicates coins collected. On collecting 10 coins, we get 1 health unit recharge, and the coin bar becomes empty.

## Environment
1. Wind starts blowing at random angles in random intervals, influencing the boat's direction. Rotating against the wind becomes slower and in the direction becomes faster.
2. 3 Waves are there around the boat, front, middle and rear when it is moving.
3. An island which contains coins, hearts and boosters. Dissapears after green health is collected and reappears elsewhere/

## Man
1. Man can move up, down, left, right, ahead and behind using the keys z, x, a, d, z, w and s respectively.
2. Man cannot collect collectibles that are not on the island. He cannot shoot either, but he does take damage like the boat.
3. Damage to the man affects the boat's health too.
4. Switch to man view using 'm' .
5. After collecting all collectibles on the island, a green heart appears which restores health of man/boat completely.