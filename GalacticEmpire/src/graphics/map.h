#ifndef GALACTIC_EMPIRE_PI_MAP_H
#define GALACTIC_EMPIRE_PI_MAP_H

/**
 * Updates the map with the current galaxy state.
 */
void updateMap(Galaxy *empire, unsigned char numWorlds);

/**
 * Clears the map completely of entries.
 */
void clearMap();

#endif
