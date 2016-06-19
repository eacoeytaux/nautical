# nautical
nautical game engine v0.2


CONTROLS:

pause with 'P'

move player with 'WASD'

shoot grapple with mouse, release to retract

zoom with ',' and '.'

turn on darkness with 'O'

to switch between DEBUG_MODE, press enter

in DEBUG_MODE:

- show bumpers and normals with 'B'


TODOs:


1) fix bugs in movement in map

2) edit World::generatePath() to allow for MapObject interuption between vector adjustments

3) various TODOs in project (search for "TODO" in project files)

4) saving


KNOWN BUGS:

- sometimes object will jump to other end of edge when passing to vertex

- in DarknessOverlay subtracted shapes cause issues when going off-screen
