#include <engine/Application.hh>
#include <engine/Camera.hh>
#include <engine/Renderer.hh>
#include <engine/Mesh.hh>
#include <engine/Shader.hh>
#include <fstream>


static const Vertex verts[]  = { {0, 0, 0}, {5, 0, 0}, {0, 5, 0} };
static const Vector3 colors[] = { {1, 1, 1}, {1, 0, 1}, {0, 1, 1} };
static const Vector3 normals[]  = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };


class Game : public Application
{
    public:
        Mesh *mesh;

        Game( Renderer &renderer) : Application(renderer)
        {
            mesh = new Mesh(&verts[0], &colors[0], &normals[0], 3);
            getInput().grabCursor(true);
        }

        ~Game() {}

        void input(
            Input &input )
        {
            if (input.isKeyDown(Input::KEY_Q))
            {
                stop();
                return;
            }

            if (input.isKeyDown(Input::KEY_LEFT))
            {
                Camera &camera = getRenderer().getCamera();
                camera.rotate(glm::vec3(2, 0, 0), glm::vec3(0, 1, 0));
                std::cout << "Rotating to left" << std::endl;
            }
            else
            if (input.isKeyDown(Input::KEY_RIGHT))
            {
                Camera &camera = getRenderer().getCamera();
                camera.rotate(glm::vec3(-2, 0, 0), glm::vec3(0, 1, 0));
                std::cout << "Rotating to right" << std::endl;
            }
        }

        void update() {}

        void draw()
        {
            Vertex bla(0, 0, 0);
            getRenderer().draw(*mesh, bla);
        }

};



int main( int argc, char **argv )
{
    Camera camera(glm::vec3(-1, 0, -1), glm::vec3(0), 90.0F, Camera::AR_16x9);
    Light light(glm::vec3(0));
    Renderer renderer(camera, light, 1280, 720);

    std::ifstream sf("./shaders/colored-vertex.glsl");
    Shader vertex(ShaderType::SHADER_VERTEX, sf);
    sf.close();

    sf.open("./shaders/colored-fragments.glsl");
    Shader frags(ShaderType::SHADER_FRAGMENT, sf);
    sf.close();

    ShaderProgram program(vertex, frags);

    renderer.setActiveShader(program);
    Game instance(renderer);
    instance.run();
    return 0;
}
