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
        Mesh *mesh, *julietteMesh;

        Game( Renderer &renderer) : Application(renderer)
        {
            mesh = new Mesh(&verts[0], &colors[0], &normals[0], 3);
            getInput().grabCursor(true);

            std::ifstream julietteBin("juliette.blend.bin");
	        julietteMesh = new Mesh(julietteBin);
	        julietteBin.close();
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
            else
            if (input.isKeyDown(Input::KEY_W))
            {
                Camera &camera = getRenderer().getCamera();
                camera.move(glm::vec3(1, 0, 0));
            }
            else
            if (input.isKeyDown(Input::KEY_S))
            {
                Camera &camera = getRenderer().getCamera();
                camera.move(glm::vec3(-1, 0, 0));
            }
            else
            if (input.isKeyDown(Input::KEY_A))
            {
                Camera &camera = getRenderer().getCamera();
                camera.move(glm::vec3(0, 0, 1));
            }
            else
            if (input.isKeyDown(Input::KEY_D))
            {
                Camera &camera = getRenderer().getCamera();
                camera.move(glm::vec3(0, 0, -1));
            }

            if (input.isMouseMoved())
            {
                static const float AMOUNT = .05F;
                glm::vec2 delta = input.getMouseDelta();
                glm::vec3 displace(0);
                if (delta.x < 0) displace.y = AMOUNT;
                if (delta.x > 0) displace.y = -AMOUNT;
                Camera &camera = getRenderer().getCamera();
                camera.rotateTarget(displace);
                //std::cout << "Rotating target" << std::endl;
            }
        }

        void update() {}

        void draw()
        {
            Vertex bla(0, 0, 0);
            getRenderer().draw(*julietteMesh, bla);
        }

};



int main( int argc, char **argv )
{
    Camera camera(glm::vec3(3, 3, 3), glm::vec3(50, 3, 50), 60.0F, Camera::AR_16x9);
    Light light(glm::vec3(5, 3, 5));
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
