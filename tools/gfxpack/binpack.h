#ifndef BINPACK_H
#define BINPACK_H

#include <vector>

#include "image.h"
#include "chip.h"

// todo, options?
FpImage *packChips( std::vector<Chip*> chips );


#endif
