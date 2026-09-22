
#include <Engine/App.h>

class Game : public App {
public:
    Game() : App("Game") {}

    void Start() override {
        LOGINF.Output("My game start!").Output("\n");
    }
};

int main() {
    Game game;
    return game.Run();
}

