#include <GL/gl.h>

#include "Engine/App.h"
#include "Engine/Render2D.h"
#include "Toolkit/Debug/Logger.h"

#include "Engine/objects/Rect.h"

class MyGame : public App {
    GLOBAL_PTR<Rect> rect;
public:
    MyGame()
        : App("My Game") {
    }

    void Start() override {
        rect = MakeGlobalPtr<Rect>();
        auto transformHandle = rect->GetComponent<Transform>();
        transformHandle->Size().x = 0.3;
        transformHandle->Size().y = 0.3;

        AddGameObject(rect);
    }

    void Update() override {

    }
};

int main() {

    MyGame game;

    return game.Run();
}

