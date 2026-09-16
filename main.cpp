#include <GL/gl.h>

#include "Engine/App.h"
#include "Engine/Render2D.h"
#include "Toolkit/Debug/Logger.h"

class MyGame : public App {
public:
    MyGame()
        : App("My Game") {
    }

    void Start() override {

    }

    void Update() override {
        ExecuteInRenderThread([](auto self) {
            Render2D::DrawLine(0.5, -1, 1, {{1, 0, 0, 1}});
        });
    }
};

int main() {

    MyGame game;

    return game.Run();
}

