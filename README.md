# nautical
nautical game engine v0.2



CONTROLS:

- pause with 'P'

- move player with 'WASD'

- shoot grapple with mouse, release to retract

- zoom with ',' and '.'

- turn on darkness with 'O'

- change pixel resolution with '1', '2', '3', and '4'

- to switch between DEBUG_MODE, press enter

in DEBUG_MODE:

- show bumpers and normals with 'B'



TODOs:

- change ptrs to shared_ptrs where appropriate

- change pass by value to pass by const reference where appropriate

- fix bugs in movement in map

- edit World::generatePath() to allow for MapObject interuption between vector adjustments

- preload all images to then be used by classes

- saving feature

- various TODOs in project (search for "TODO" in project files)



KNOWN BUGS:

- sometimes object will jump to other end of map edge when passing to map vertex

- in DarknessOverlay subtracted shapes cause issues when going off-screen
