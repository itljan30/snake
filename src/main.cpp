#include "game.h"

#include <RedGir/engine.h>

int main() {
    // maybe add loading custom screen width and height into constants.h

    Game game = Game();
    game.run();

    // save current screen dimensions to a file or something
}
