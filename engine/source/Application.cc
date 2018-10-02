#include <engine/Application.hh>
#include <engine/Timer.hh>

Application::Application(
            Renderer &renderer ) :
    state_(ApplicationState::ST_IDLE),
    renderer_(renderer)
{
    Timer::update();
}


Application::~Application()
{

}


void Application::run()
{
    state_ = ApplicationState::ST_RUNNING;
    while (state_ == ApplicationState::ST_RUNNING)
    {
        Timer::update();
        input_.update();
        input(input_);
        update();
        renderer_.prepare();
        draw();
        renderer_.refresh();
    }

    state_ = ApplicationState::ST_IDLE;
}


void Application::stop()
{
    state_ = ApplicationState::ST_STOPPING;
}


Renderer &Application::getRenderer()
{
    return renderer_;
}


Input &Application::getInput()
{
    return input_;
}
