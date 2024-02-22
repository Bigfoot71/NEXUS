#ifndef NEXUS_CORE_STATE_HPP
#define NEXUS_CORE_STATE_HPP

#include "./nxWindow.hpp"
#include "./nxEvent.hpp"

namespace _core_impl {

    /**
     * @brief The State class serves as the base class for different states within the application.
     *
     * This class provides a template for creating different states within an application.
     * States encapsulate different behaviors and functionalities, allowing the application to switch
     * between them seamlessly. It includes functions for entering, exiting, updating, and drawing the state,
     * as well as event callback functions for handling various input events.
     *
     * @tparam T_App The type of app associated with the state.
     */
    template <typename T_App>
    class State
    {
      protected:
        T_App &app;  ///< Pointer to the App instance associated with this state.

      public:
        State(T_App& app)
        : app(app)
        { }

        virtual ~State() = default;

      public:
        /**
         * @brief Called when entering this state.
         */
        virtual void Enter()
        { }

        /**
         * @brief Called when exiting this state.
         */
        virtual void Exit()
        { }

        /**
         * @brief Updates the state logic.
         * @param dt Delta time since the last frame.
         */
        virtual void Update(float dt)
        { }

        /**
         * @brief Draws the state content.
         */
        virtual void Draw()
        { }


        /* Event Callback Functions */

        /**
         * @brief Callback function triggered when the application receives a quit event.
         *
         * This function is called when the user attempts to close the application window.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::Quit containing information about the quit event.
         */
        virtual void OnQuit(const nexus::core::Event::Quit& event)
        {
            app.Finish(); (void)event;
        }

        /**
         * @brief Callback function triggered when a display event occurs.
         *
         * This function is called when there are changes related to display events.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::Display containing information about the display event.
         */
        virtual void OnDisplayEvent(const nexus::core::Event::Display& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a window event occurs.
         *
         * This function is called when there are changes related to the application window.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::Window containing information about the window event.
         */
        virtual void OnWindowEvent(const nexus::core::Event::Window& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a system WM (Window Manager) event occurs.
         *
         * This function is called when there are changes related to the system window manager.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::SysWM containing information about the system WM event.
         */
        virtual void OnSysWMEvent(const nexus::core::Event::SysWM& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a key is pressed.
         *
         * This function is called when a keyboard key is pressed.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::Keyboard containing information about the key press event.
         */
        virtual void OnKeyDown(const nexus::core::Event::Keyboard& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a key is released.
         *
         * This function is called when a keyboard key is released.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::Keyboard containing information about the key release event.
         */
        virtual void OnKeyUp(const nexus::core::Event::Keyboard& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered during text editing.
         *
         * This function is called when the user is editing text.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::TextEditing containing information about the text editing event.
         */
        virtual void OnTextEditing(const nexus::core::Event::TextEditing& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when text input occurs.
         *
         * This function is called when the user inputs text.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::TextInput containing information about the text input event.
         */
        virtual void OnTextInput(const nexus::core::Event::TextInput& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when the key map changes.
         *
         * This function is called when there is a change in the key map.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::Keyboard containing information about the key map change event.
         */
        virtual void OnKeyMapChanged(const nexus::core::Event::Keyboard& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered during extended text editing.
         *
         * This function is called when extended text editing events occur.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::TextEditingExt containing information about the extended text editing event.
         */
        virtual void OnTextEditingExt(const nexus::core::Event::TextEditingExt& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when the mouse is moved.
         *
         * This function is called when the mouse is moved.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::MouseMotion containing information about the mouse motion event.
         */
        virtual void OnMouseMotion(const nexus::core::Event::MouseMotion& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a mouse button is pressed.
         *
         * This function is called when a mouse button is pressed.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::MouseButton containing information about the mouse button press event.
         */
        virtual void OnMouseButtonDown(const nexus::core::Event::MouseButton& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a mouse button is released.
         *
         * This function is called when a mouse button is released.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::MouseButton containing information about the mouse button release event.
         */
        virtual void OnMouseButtonUp(const nexus::core::Event::MouseButton& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when the mouse wheel is scrolled.
         *
         * This function is called when the mouse wheel is scrolled.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::MouseWheel containing information about the mouse wheel scroll event.
         */
        virtual void OnMouseButtonWheel(const nexus::core::Event::MouseWheel& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a joystick axis is moved.
         *
         * This function is called when a joystick axis is moved.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::JoyAxis containing information about the joystick axis motion event.
         */
        virtual void OnJoyAxisMotion(const nexus::core::Event::JoyAxis& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a joystick ball is moved.
         *
         * This function is called when a joystick ball is moved.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::JoyBall containing information about the joystick ball motion event.
         */
        virtual void OnJoyBallMotion(const nexus::core::Event::JoyBall& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a joystick hat is moved.
         *
         * This function is called when a joystick hat is moved.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::JoyHat containing information about the joystick hat motion event.
         */
        virtual void OnJoyHatMotion(const nexus::core::Event::JoyHat& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a joystick button is pressed.
         *
         * This function is called when a joystick button is pressed.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::JoyButton containing information about the joystick button press event.
         */
        virtual void OnJoyButtonDown(const nexus::core::Event::JoyButton& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a joystick button is released.
         *
         * This function is called when a joystick button is released.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::JoyButton containing information about the joystick button release event.
         */
        virtual void OnJoyButtonUp(const nexus::core::Event::JoyButton& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a joystick device is added.
         *
         * This function is called when a joystick device is added.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::JoyDevice containing information about the joystick device addition event.
         */
        virtual void OnJoyDeviceAdded(const nexus::core::Event::JoyDevice& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a joystick device is removed.
         *
         * This function is called when a joystick device is removed.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::JoyDevice containing information about the joystick device removal event.
         */
        virtual void OnJoyDeviceRemoved(const nexus::core::Event::JoyDevice& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when the joystick battery level is updated.
         *
         * This function is called when the joystick battery level is updated.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::JoyBattery containing information about the joystick battery update event.
         */
        virtual void OnJoyBatteryUpdated(const nexus::core::Event::JoyBattery& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a controller axis is moved.
         *
         * This function is called when a controller axis is moved.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::ControllerAxis containing information about the controller axis motion event.
         */
        virtual void OnControllerAxisMotion(const nexus::core::Event::ControllerAxis& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a controller button is pressed.
         *
         * This function is called when a controller button is pressed.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::ControllerButton containing information about the controller button press event.
         */
        virtual void OnControllerButtonDown(const nexus::core::Event::ControllerButton& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a controller button is released.
         *
         * This function is called when a controller button is released.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::ControllerButton containing information about the controller button release event.
         */
        virtual void OnControllerButtonUp(const nexus::core::Event::ControllerButton& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a controller device is added.
         *
         * This function is called when a controller device is added.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::ControllerDevice containing information about the controller device addition event.
         */
        virtual void OnControllerDeviceAdded(const nexus::core::Event::ControllerDevice& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a controller device is removed.
         *
         * This function is called when a controller device is removed.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::ControllerDevice containing information about the controller device removal event.
         */
        virtual void OnControllerDeviceRemoved(const nexus::core::Event::ControllerDevice& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a controller device is remapped.
         *
         * This function is called when a controller device is remapped.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::ControllerDevice containing information about the controller device remapping event.
         */
        virtual void OnControllerDeviceRemapped(const nexus::core::Event::ControllerDevice& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a touchpad on the controller is pressed.
         *
         * This function is called when a touchpad on the controller is pressed.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::ControllerTouchpad containing information about the controller touchpad press event.
         */
        virtual void OnControllerTouchPadDown(const nexus::core::Event::ControllerTouchpad& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when the touchpad on the controller is moved.
         *
         * This function is called when the touchpad on the controller is moved.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::ControllerTouchpad containing information about the controller touchpad motion event.
         */
        virtual void OnControllerTouchPadMotion(const nexus::core::Event::ControllerTouchpad& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a touchpad on the controller is released.
         *
         * This function is called when a touchpad on the controller is released.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::ControllerTouchpad containing information about the controller touchpad release event.
         */
        virtual void OnControllerTouchPadUp(const nexus::core::Event::ControllerTouchpad& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a controller sensor is updated.
         *
         * This function is called when a controller sensor is updated.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::ControllerSensor containing information about the controller sensor update event.
         */
        virtual void OnControllerSensorUpdate(const nexus::core::Event::ControllerSensor& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a finger touches the touch screen.
         *
         * This function is called when a finger touches the touch screen.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::TouchFinger containing information about the finger touch event.
         */
        virtual void OnFingerDown(const nexus::core::Event::TouchFinger& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a finger is lifted from the touch screen.
         *
         * This function is called when a finger is lifted from the touch screen.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::TouchFinger containing information about the finger lift event.
         */
        virtual void OnFingerUp(const nexus::core::Event::TouchFinger& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a finger moves on the touch screen.
         *
         * This function is called when a finger moves on the touch screen.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::TouchFinger containing information about the finger motion event.
         */
        virtual void OnFingerMotion(const nexus::core::Event::TouchFinger& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a dollar gesture is recognized.
         *
         * This function is called when a dollar gesture is recognized.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::DollarGesture containing information about the dollar gesture event.
         */
        virtual void OnDollarGesture(const nexus::core::Event::DollarGesture& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a dollar gesture is being recorded.
         *
         * This function is called when a dollar gesture is being recorded.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::DollarGesture containing information about the dollar gesture recording event.
         */
        virtual void OnDollarRecord(const nexus::core::Event::DollarGesture& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a multi-finger gesture is recognized.
         *
         * This function is called when a multi-finger gesture is recognized.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::MultiGesture containing information about the multi-finger gesture event.
         */
        virtual void OnMultiGesture(const nexus::core::Event::MultiGesture& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when the clipboard content is updated.
         *
         * This function is called when the clipboard content is updated.
         * It can be overridden in derived classes to implement custom behavior.
         */
        virtual void OnClipboardUpdate()
        { }

        /**
         * @brief Callback function triggered when a file is dropped onto the application window.
         *
         * This function is called when a file is dropped onto the application window.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::Drop containing information about the file drop event.
         */
        virtual void OnDropFile(const nexus::core::Event::Drop& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when text is dropped onto the application window.
         *
         * This function is called when text is dropped onto the application window.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::Drop containing information about the text drop event.
         */
        virtual void OnDropText(const nexus::core::Event::Drop& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when the drop operation begins.
         *
         * This function is called when a drop operation begins.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::Drop containing information about the drop operation beginning event.
         */
        virtual void OnDropBegin(const nexus::core::Event::Drop& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when the drop operation is completed.
         *
         * This function is called when a drop operation is completed.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::Drop containing information about the drop operation completion event.
         */
        virtual void OnDropComplete(const nexus::core::Event::Drop& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when an audio device is added.
         *
         * This function is called when an audio device is added.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::AudioDevice containing information about the audio device addition event.
         */
        virtual void OnAudioDeviceAdded(const nexus::core::Event::AudioDevice& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when an audio device is removed.
         *
         * This function is called when an audio device is removed.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::AudioDevice containing information about the audio device removal event.
         */
        virtual void OnAudioDeviceRemoved(const nexus::core::Event::AudioDevice& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when a sensor is updated.
         *
         * This function is called when a sensor is updated.
         * It can be overridden in derived classes to implement custom behavior.
         *
         * @param event The Event::Sensor containing information about the sensor update event.
         */
        virtual void OnSensorUpdate(const nexus::core::Event::Sensor& event)
        {
            (void)event;
        }

        /**
         * @brief Callback function triggered when the render target is reset.
         *
         * This function is called when the render target is reset.
         * It can be overridden in derived classes to implement custom behavior.
         */
        virtual void OnRenderTargetReset()
        { }

        /**
         * @brief Callback function triggered when the render device is reset.
         *
         * This function is called when the render device is reset.
         * It can be overridden in derived classes to implement custom behavior.
         */
        virtual void OnRenderDeviceReset()
        { }

        /**
         * @brief Callback function triggered when the application is terminating.
         *
         * This function is called when the application is terminating.
         * It can be overridden in derived classes to implement custom behavior.
         * (Special meaning on iOS, see sdl/README-ios.md for details.)
         */
        void OnTerminating()
        { }

        /**
         * @brief Callback function triggered when the application is experiencing low memory.
         *
         * This function is called when the application is experiencing low memory.
         * It can be overridden in derived classes to implement custom behavior.
         * (Special meaning on iOS, see sdl/README-ios.md for details.)
         */
        void OnLowMemory()
        { }

        /**
         * @brief Callback function triggered when the application will enter the background.
         *
         * This function is called when the application will enter the background.
         * It can be overridden in derived classes to implement custom behavior.
         * (Special meaning on iOS, see sdl/README-ios.md for details.)
         */
        void OnWillEnterBackground()
        { }

        /**
         * @brief Callback function triggered when the application did enter the background.
         *
         * This function is called when the application did enter the background.
         * It can be overridden in derived classes to implement custom behavior.
         * (Special meaning on iOS, see sdl/README-ios.md for details.)
         */
        void OnDidEnterBackground()
        { }

        /**
         * @brief Callback function triggered when the application will enter the foreground.
         *
         * This function is called when the application will enter the foreground.
         * It can be overridden in derived classes to implement custom behavior.
         * (Special meaning on iOS, see sdl/README-ios.md for details.)
         */
        void OnWillEnterForeground()
        { }

        /**
         * @brief Callback function triggered when the application did enter the foreground.
         *
         * This function is called when the application did enter the foreground.
         * It can be overridden in derived classes to implement custom behavior.
         * (Special meaning on iOS, see sdl/README-ios.md for details.)
         */
        void OnDidEnterForeground()
        { }

        /**
         * @brief Callback function triggered when the locale is changed.
         *
         * This function is called when the locale is changed.
         * It can be overridden in derived classes to implement custom behavior.
         * (Special meaning on iOS, see sdl/README-ios.md for details.)
         */
        void OnLocaleChanged()
        { }
    };

    /**
     * @brief The LoadingState class, derived from State, is designed for handling asynchronous operations
     *        using core::App::Loading().
     *
     * This class provides a framework for creating states that involve asynchronous tasks. It extends the base State
     * class and introduces functionality to execute a main task in a separate thread during the state's execution.
     * Additionally, it allows an optional task to be executed on the main thread after the main task completes.
     *
     * @tparam T_App The type of app class associated with the state.
     */
    template <typename T_App>
    class LoadingState : public State<T_App>
    {
      protected:
        bool isFinished; ///< Flag indicating whether the main task has finished.

      public:
        using State<T_App>::State;

        /**
         * @brief Default virtual destructor for the LoadingState class.
         */
        virtual ~LoadingState() = default;

      public:
        /**
         * @brief The main task to be executed in a separate thread during the state's execution.
         *
         * This pure virtual function must be implemented in derived classes to define the main task
         * that will be executed in a separate thread.
         */
        virtual void Task() = 0;

        /**
         * @brief Optional task executed after the main Task on the main thread.
         *
         * This function can be overridden in derived classes to define an optional task that will be executed
         * on the main thread after the main task completes. The default implementation does nothing.
         */
        virtual void PostTask()
        { }

        /**
         * @brief Check if the main task has finished.
         *
         * @return True if the main task has finished; false otherwise.
         */
        virtual bool IsFinished() const { return isFinished; }

      protected:
        /**
         * @brief Mark the main task as finished.
         *
         * This function sets the isFinished flag to true, indicating that the main task has finished.
         * It is called internally when the main task is considered complete.
         */
        virtual void Finish() { isFinished = true; }
    };

}

namespace nexus { namespace core {

    class App;

    using State = NEXUS_API _core_impl::State<App>;
    using LoadingState = NEXUS_API _core_impl::LoadingState<App>;

}}

#endif //NEXUS_CORE_STATE_HPP
