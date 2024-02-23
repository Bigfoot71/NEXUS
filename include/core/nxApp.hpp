/**
 * Copyright (c) 2023-2024 Le Juez Victor
 *
 * This software is provided "as-is", without any express or implied warranty. In no event 
 * will the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including commercial 
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not claim that you 
 *   wrote the original software. If you use this software in a product, an acknowledgment 
 *   in the product documentation would be appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not be misrepresented
 *   as being the original software.
 *
 *   3. This notice may not be removed or altered from any source distribution.
 */

#ifndef NEXUS_CORE_APP_HPP
#define NEXUS_CORE_APP_HPP

#include "../platform/nxPlatform.hpp"

#include "./nxWindow.hpp"
#include "./nxEvent.hpp"
#include "./nxClock.hpp"
#include "./nxState.hpp"

#ifdef SUPPORT_AUDIO
#   include "../audio/nxDevice.hpp"
#endif

#ifdef EXTENSION_CORE
#   include "./ext_core/nxSaveManager.hpp"
#   include "./ext_core/nxAssetManager.hpp"
#endif

#include <unordered_map>
#include <type_traits>
#include <utility>
#include <memory>
#include <thread>
#include <atomic>

namespace _core_impl {

    /**
     * @brief The App class represents the main application framework.
     *
     * This class facilitates a more object-oriented management of your program but is optional.
     * The basic functionality it provides includes a state system through the State class,
     * SDL event callback system, and the ability to perform actions in parallel with state execution
     * using LoadingState.
     *
     * The App class is quite basic, and you can use it as is or derive a custom class to tailor its behavior.
     * For example, you can customize UpdateAndDraw and Run, add state transition systems, global rendering targets,
     * custom event handling, etc.
     *
     * This class also uses the Curiously Recurring Template Pattern (CRTP) by taking as a template argument the derived class itself.
     * This allows for casting itself to its child class as a parameter in the constructor of the State type in AddState.
     * This is necessary because the State class takes as a template argument derivatives of _core_impl::App,
     * but it is _core_impl::App that will have to construct them by providing the derived App instance.
     */
    template <typename T_App, typename T_Window>
    class NEXUS_API App
    {
        static_assert(std::is_base_of<nexus::core::Window, T_Window>::value,
            "T_Window must be derived from the core::Window class");

      public:
        using MapState = std::unordered_map<std::string, std::unique_ptr<_core_impl::State<T_App>>>;
        using PairState = std::pair<const std::string, std::unique_ptr<_core_impl::State<T_App>>>;

      public:
        T_Window window;                                        ///< Window instance for the application.
        nexus::core::Event event;
        nexus::core::Clock clock;

      public:
    #ifdef SUPPORT_AUDIO
        nexus::audio::Device audio;                                ///< AudioDevice instance for the application.
    #endif

      public:
    #ifdef EXTENSION_CORE
        nexus::core::AssetManager assetManager;                    ///< Basic generic asset manager.
        std::unique_ptr<nexus::core::SaveManager> saveManager;     ///< Basic generic save manager (optional).
    #endif

      protected:
        MapState states;                                        ///< Map of all states.
        PairState *currentState = nullptr;                      ///< Current state of the App.

      protected:
        bool running = false;                                   ///< Flag indicating whether the application is running.
        int retCode = 0;                                        ///< Return code of the application.

      protected:
        virtual void ProcessEvents(_core_impl::State<T_App>& state);
        virtual void UpdateAndDraw(_core_impl::State<T_App>& state);

      public:
        /**
         * @brief Constructs an App instance with specified parameters.
         * @param appTitle The title of the application.
         * @param winWidth The width of the window.
         * @param winHeight The height of the window.
         * @param winFlags Additional flags for window creation.
         */
        App(const std::string& appTitle, int winWidth, int winHeight, nexus::core::WindowFlag winFlags = nexus::core::WindowFlag::Shown);

        /**
         * @brief Virtual default destructor 
         */
        virtual ~App() = default;

    #ifdef EXTENSION_CORE

        /**
         * @brief Initializes the SaveManager with the specified parameters.
         * @tparam _Ts The type of the object to be saved.
         * @param origin The object to be saved.
         * @param version The version of the SaveManager (default is 1).
         * @param directory The directory for saving/loading files (default is empty).
         */
        template <typename _Ts>
        void InitSaveManager(const _Ts& origin, int version = 1, const std::string& directory = "");

    #endif

        /**
         * @brief Initiates a loading screen and executes a loading task.
         * @tparam _Tls The type of LoadingState to be used.
         */
        template<typename _Tls>
        void Loading();

        /**
         * @brief Adds a state of type _Ts to the App.
         * @tparam _Ts The type of state to be added.
         * @param name The name of the state.
         */
        template<typename _Ts>
        void AddState(const std::string& name);

        /**
         * @brief Sets the current state of the App.
         * @param stateName The name of the state to set.
         */
        virtual void SetState(const std::string& stateName);

        /**
         * @brief Runs the application with the specified initial state.
         * @param firstState The name of the first state to run.
         * @param targetFPS The target frames per second for the application.
         * @return The return code of the application.
         */
        virtual int Run(const std::string& firstState, int targetFPS = 60);

        /**
         * @brief Finishes the application with the specified return code.
         * @param ret The return code to set (default is 0).
         */
        virtual void Finish(int ret = 0) { running = false, retCode = ret; }
    };

    /* Protected Implementation */

    template <typename T_App, typename T_Window>
    void App<T_App, T_Window>::ProcessEvents(_core_impl::State<T_App>& state)
    {
        using namespace nexus::core;

        while (event.Poll() != 0)
        {
            switch (event.GetType())
            {
                case Event::Type::Quit:                     state.OnQuit(event->quit);                          break;
                case Event::Type::DisplayEvent:             state.OnDisplayEvent(event->display);               break;
                case Event::Type::WindowEvent:              state.OnWindowEvent(event->window);                 break;
                case Event::Type::SysWMEvent:               state.OnSysWMEvent(event->syswm);                   break;
                case Event::Type::KeyDown:                  state.OnKeyDown(event->key);                        break;
                case Event::Type::KeyUp:                    state.OnKeyUp(event->key);                          break;
                case Event::Type::TextEditing:              state.OnTextEditing(event->edit);                   break;
                case Event::Type::TextInput:                state.OnTextInput(event->text);                     break;
                case Event::Type::KeyMapChanged:            state.OnKeyMapChanged(event->key);                  break;
                case Event::Type::TextEditingExt:           state.OnTextEditingExt(event->editExt);             break;
                case Event::Type::MouseMotion:              state.OnMouseMotion(event->motion);                 break;
                case Event::Type::MouseButtonDown:          state.OnMouseButtonDown(event->button);             break;
                case Event::Type::MouseButtonUp:            state.OnMouseButtonDown(event->button);             break;
                case Event::Type::MouseButtonWheel:         state.OnMouseButtonWheel(event->wheel);             break;
                case Event::Type::JoyAxisMotion:            state.OnJoyAxisMotion(event->jaxis);                break;
                case Event::Type::JoyBallMotion:            state.OnJoyBallMotion(event->jball);                break;
                case Event::Type::JoyHatMotion:             state.OnJoyHatMotion(event->jhat);                  break;
                case Event::Type::JoyButtonDown:            state.OnJoyButtonDown(event->jbutton);              break;
                case Event::Type::JoyButtonUp:              state.OnJoyButtonUp(event->jbutton);                break;
                case Event::Type::JoyDeviceAdded:           state.OnJoyDeviceAdded(event->jdevice);             break;
                case Event::Type::JoyDeviceRemoved:         state.OnJoyDeviceRemoved(event->jdevice);           break;
                case Event::Type::JoyBatteryUpdated:        state.OnJoyBatteryUpdated(event->jbattery);         break;
                case Event::Type::ControllerAxisMotion:     state.OnControllerAxisMotion(event->caxis);         break;
                case Event::Type::ControllerButtonDown:     state.OnControllerButtonDown(event->cbutton);       break;
                case Event::Type::ControllerButtonUp:       state.OnControllerButtonUp(event->cbutton);         break;
                case Event::Type::ControllerDeviceAdded:    state.OnControllerDeviceAdded(event->cdevice);      break;
                case Event::Type::ControllerDeviceRemoved:  state.OnControllerDeviceRemoved(event->cdevice);    break;
                case Event::Type::ControllerDeviceRemapped: state.OnControllerDeviceRemapped(event->cdevice);   break;
                case Event::Type::ControllerTouchPadDown:   state.OnControllerTouchPadDown(event->ctouchpad);   break;
                case Event::Type::ControllerTouchPadMotion: state.OnControllerTouchPadMotion(event->ctouchpad); break;
                case Event::Type::ControllerTouchPadUp:     state.OnControllerTouchPadUp(event->ctouchpad);     break;
                case Event::Type::ControllerSensorUpdate:   state.OnControllerSensorUpdate(event->csensor);     break;
                case Event::Type::FingerDown:               state.OnFingerDown(event->tfinger);                 break;
                case Event::Type::FingerUp:                 state.OnFingerDown(event->tfinger);                 break;
                case Event::Type::FingerMotion:             state.OnFingerMotion(event->tfinger);               break;
                case Event::Type::DollarGesture:            state.OnDollarGesture(event->dgesture);             break;
                case Event::Type::DollarRecord:             state.OnDollarRecord(event->dgesture);              break;
                case Event::Type::MultiGesture:             state.OnMultiGesture(event->mgesture);              break;
                case Event::Type::ClipboardUpdate:          state.OnClipboardUpdate();                          break;
                case Event::Type::DropFile:                 state.OnDropFile(event->drop);                      break;
                case Event::Type::DropText:                 state.OnDropText(event->drop);                      break;
                case Event::Type::DropBegin:                state.OnDropBegin(event->drop);                     break;
                case Event::Type::DropComplete:             state.OnDropComplete(event->drop);                  break;
                case Event::Type::AudioDeviceAdded:         state.OnAudioDeviceAdded(event->adevice);           break;
                case Event::Type::AudioDeviceRemoved:       state.OnAudioDeviceRemoved(event->adevice);         break;
                case Event::Type::SensorUpdate:             state.OnSensorUpdate(event->sensor);                break;
                case Event::Type::RenderTargetReset:        state.OnRenderTargetReset();                        break;
                case Event::Type::RenderDeviceReset:        state.OnRenderDeviceReset();                        break;
                case Event::Type::Teraminating:             state.OnTerminating();                              break;
                case Event::Type::LowMemory:                state.OnLowMemory();                                break;
                case Event::Type::WillEnterBackground:      state.OnWillEnterBackground();                      break;
                case Event::Type::DidEnterBackground:       state.OnDidEnterBackground();                       break;
                case Event::Type::WillEnterForeground:      state.OnWillEnterForeground();                      break;
                case Event::Type::DidEnterForeground:       state.OnDidEnterForeground();                       break;
                case Event::Type::LocaleChanged:            state.OnLocaleChanged();                            break;

                default: break;
            }
        }
    }

    template <typename T_App, typename T_Window>
    void App<T_App, T_Window>::UpdateAndDraw(_core_impl::State<T_App>& state)
    {
        clock.Begin();
            ProcessEvents(state);
            state.Update(clock.GetDelta());
            state.Draw();
        clock.End();
    }


    /* Public Implementation */

    template <typename T_App, typename T_Window>
    App<T_App, T_Window>::App(const std::string& appTitle, int winWidth, int winHeight, nexus::core::WindowFlag winFlags)
    : window(appTitle, winWidth, winHeight, winFlags)
    #ifdef SUPPORT_AUDIO
    , audio()
    #endif
    { }

    #ifdef EXTENSION_CORE
    template <typename T_App, typename T_Window>
    template <typename _Ts>
    void App<T_App, T_Window>::InitSaveManager(const _Ts& origin, int version, const std::string& directory)
    {
        saveManager = std::make_unique<nexus::core::SaveManager>(origin, version, directory);
    }
    #endif

    template <typename T_App, typename T_Window>
    template <typename _Tls>
    void App<T_App, T_Window>::Loading()
    {
        _core_impl::LoadingState<T_Window> *loadingState = new _Tls();
        loadingState->SetApp(this);
        loadingState->Enter();

        std::atomic<bool> onLoading = true;
        std::thread taskThread([&loadingState, &onLoading]() {
            loadingState->Task(); onLoading = false;
        });

        while (onLoading || !loadingState->IsFinished())
        {
            UpdateAndDraw(*loadingState);
        }

        if (taskThread.joinable())
        {
            taskThread.join();
        }

        loadingState->PostTask();
        loadingState->Exit();
        delete loadingState;
    }

    template <typename T_App, typename T_Window>
    template <typename _Ts>
    void App<T_App, T_Window>::AddState(const std::string& name)
    {
        states[name] = std::make_unique<_Ts>(*static_cast<T_App*>(this));
    }

    template <typename T_App, typename T_Window>
    void App<T_App, T_Window>::SetState(const std::string& stateName)
    {
        if (stateName != currentState->first)
        {
            currentState->second->Exit();
            currentState = &(*states.find(stateName));
            currentState->second->Enter();
        }
    }

    template <typename T_App, typename T_Window>
    int App<T_App, T_Window>::Run(const std::string& firstState, int targetFPS)
    {
        currentState = &(*states.find(firstState));
        currentState->second->Enter();

        clock.SetTargetFPS(targetFPS);
        running = true;

        while (running) UpdateAndDraw(*currentState->second);
        currentState->second->Exit();

        return retCode;
    }

}

namespace nexus { namespace core {

    class App : public _core_impl::App<App, Window>
    {
      public:
        using _core_impl::App<App, Window>::App;
    };

}}

#endif //NEXUS_CORE_APP_HPP
