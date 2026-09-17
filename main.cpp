#include <GL/gl.h>

#include "Engine/App.h"
#include "Engine/Render2D.h"
#include "Toolkit/Debug/Logger.h"

#include "Engine/objects/Rect.h"
#include "Engine/components/Sprite.h"

class MyGame : public App {
    GLOBAL_PTR<Rect> rect;
public:
    MyGame()
        : App("My Game") {
    }

    void Start() override {
        auto tex = GetRenderSystem()->LoadTextureSync("player.png");

        rect = MakeGlobalPtr<Rect>();
        auto sprite = rect->AddComponent<Sprite>();
        rect->SetColor({{0,0,0,0}});

        auto transformHandle = rect->GetComponent<Transform>();
        transformHandle->Size().x = 0.3;
        transformHandle->Size().y = 0.3;
        sprite->SetTexture(tex);
        sprite->SetSourceRect({32, 0, 16, 16});
        AddGameObject(rect);
    }

    void Update() override {

    }
};

int main() {

    MyGame game;

    return game.Run();
}

