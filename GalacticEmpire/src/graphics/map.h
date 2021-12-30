#ifndef GALACTIC_EMPIRE_PI_MAP_H
#define GALACTIC_EMPIRE_PI_MAP_H

/**
 * Updates the map with the current galaxy state.
 */
void updateMap(world **galaxy, unsigned numWorlds);

/**
 * Clears the map completely of entries.
 */
void clearMap();

#endif
