#include "pacman.h"

int main() {
	GameInit();

	while (1) {
		GameDraw();
		GameUpdate();
	}
	return 0;
}