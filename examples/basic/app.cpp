#include <nexus.hpp>

using namespace nexus;

class Main : public core::State
{
  private:
    gfx::Surface text;
    gfx::BasicFont font;

  public:
    Main(core::App& app) : core::State(app)
    , font(RESOURCES_PATH "fonts/Futurette.otf", 32)
    {
        text = font.RenderTextBlended("This is App", gfx::White);
    }

    void Draw()
    {
        app.window.GetSurface().Lock();
            app.window.GetSurface().Fill(gfx::Black);
            app.window.GetSurface().DrawImage(text, (app.window.GetSize() - text.GetSize()) / 2);
        app.window.GetSurface().Unlock();

        app.window.UpdateSurface();
    }
};

int main(int argv, char** args)
{
    core::App app("Basic - App", 800, 600);
    app.AddState<Main>("main");
    return app.Run("main");
}
