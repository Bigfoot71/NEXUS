#include <nexus.hpp>

using namespace nexus;

class Main : public gl::State
{
  private:
    gl::Font font;

  public:
    Main(gl::App& app) : gl::State(app)
    , font(app.window, RESOURCES_PATH "fonts/Futurette.otf", 32)
    {
        font->SetStyle(sr::Font::Style::Bold);
    }

    void Draw()
    {
        app.window.Clear();
        font->DrawCentered("This is App", app.window.GetSize() / 2);
    }
};

int main(int argv, char** args)
{
    gl::App app("GL - App", 800, 600);
    app.AddState<Main>("main");
    return app.Run("main");
}
