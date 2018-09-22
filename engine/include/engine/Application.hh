#ifndef ENGINE_APPLICATION_HH
#define ENGINE_APPLICATION_HH


#include <engine/Renderer.hh>
#include <engine/Input.hh>


enum class ApplicationState
{
    ST_IDLE,
    ST_STARTING,
    ST_RUNNING,
    ST_STOPPING
};


class Application
{
    public:
        Application(
            Renderer &renderer );

        virtual ~Application();

        virtual void input(
            Input &input ) = 0;

        virtual void update() = 0;

        virtual void draw() = 0;

        virtual void run();

        void stop();

        Renderer &getRenderer();

        Input &getInput();

    private:
        ApplicationState state_;
        Renderer &renderer_;
        Input input_;
};


#endif // ENGINE_APPLICATION_HH