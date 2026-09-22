
#include <Engine/App.h>
#include "scenes/MainScene.h"

class Game : public App {
public:
    Game() : App("Game") {}

    void Start() override {
        LOGINF.Output("My game start!").Output("\n");
        SetScene(MakeGlobalPtr<MainScene>());
    }
};

int main() {
    Game game;
    return game.Run();
}

