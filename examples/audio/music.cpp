#include <nexus.hpp>

using namespace nexus;

int main(int argv, char** args)
{
    core::Window window("Audio - Music", 800, 600);
    gfx::Surface winCanvas(window.GetSurface());

    core::Clock clock;
    core::Event event;

    gfx::BasicFont font(RESOURCES_PATH "fonts/Futurette.otf", 42);
    gfx::Surface play = font.RenderTextBlended("IS PLAYING", gfx::White);
    gfx::Surface stop = font.RenderTextBlended("NOT PLAYING", gfx::White);

    audio::Device audioDevice;
    audio::Music music(audioDevice, RESOURCES_PATH "audio/music.wav");

    bool isRunning = true;
    bool keyPressed = false;

    while (isRunning)
    {
        // UPDATE

        clock.Begin();

        while (event.Poll() != 0)
        {
            switch (event.GetType())
            {
                case core::Event::Type::Quit:
                    isRunning = false;
                    break;

                case core::Event::Type::KeyDown:
                    if (keyPressed == false)
                    {
                        if (music->IsPlaying()) music->Stop();
                        else music->Play();
                        keyPressed = true;
                    }
                    break;

                case core::Event::Type::KeyUp:
                    keyPressed = false;
                    break;

                default:
                    break;
            }
        }

        // DRAW

        winCanvas.Lock();
            winCanvas.Fill(gfx::Black);
            if (music->IsPlaying()) winCanvas.DrawImage(play, (window.GetSize() - play.GetSize()) / 2);
            else winCanvas.DrawImage(stop, (window.GetSize() - stop.GetSize()) / 2);
        winCanvas.Unlock();

        window.UpdateSurface();

        clock.End();
    }

    return 0;

}
