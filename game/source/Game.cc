#include <engine/Application.hh>
#include <engine/Camera.hh>
#include <engine/Renderer.hh>
#include <engine/Mesh.hh>
#include <engine/Shader.hh>
#include <engine/Vector2.hh>
#include <engine/Vector3.hh>
#include <engine/Matrix4.hh>
#include <engine/Quaternion.hh>
#include <engine/Object.hh>

#include "Level.hh"
#include <fstream>


static const Vector3f verts[]  = { {0, 0, 0}, {5, 0, 0}, {0, 5, 0} };
static const Vector3f colors[] = { {1, 1, 1}, {1, 0, 1}, {0, 1, 1} };
static const Vector3f normals[]  = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };


std::ostream &operator<<( std::ostream &out, const Vector3f &object )
{
    out << "[x: " << object.x << ", y: " << object.y << ", z: " << object.z << "]";
    return out;
}


class Game : public Application
{
    public:
        Mesh *mesh;
        Light &light;
        Object *object;
        float turnDegree = 0;

        Game( Renderer &renderer, Light &light) : Application(renderer), light(light)
        {
            /*mesh = new Mesh(&verts[0], &colors[0], &normals[0], 3);*/
            getInput().grabCursor(true);

            /*std::ifstream julietteBin("juliette.blend.bin");
	        julietteMesh = new Mesh(julietteBin);
	        julietteBin.close();*/
            std::ifstream input("untitled.mesher");
	        mesh = new Mesh(input);
	        input.close();

            object = new Object(*mesh);
            //object->getTransform().rotate({90, 0, 0});
            object->getTransform().scale({30, 30, 30});
            object->getTransform().update();
        }

        ~Game()
        {
            delete object;
            delete mesh;
        }

        void input(
            Input &input )
        {
            if (input.isKeyDown(Input::KEY_ESCAPE))
            {
                stop();
                return;
            }

            if (input.isKeyDown(Input::KEY_S) || input.isKeyDown(Input::KEY_W))
            {
                Camera &camera = getRenderer().getCamera();
                Vector3f dir = (camera.getTarget() - camera.getPosition());
                dir.normalize() / .3F;
                if (input.isKeyDown(Input::KEY_S)) dir = -dir;
                std::cout << "Step of " << dir << std::endl;
                camera.translate(dir);
                std::cout << "Player: " << camera.getPosition().x << " x " << camera.getPosition().z << std::endl;
                light.setPosition(camera.getPosition());
            }

            if (input.isKeyDown(Input::KEY_Q))
            {
                Camera &camera = getRenderer().getCamera();
                Vector3f dir = Vector3f(0, .2, 0);
                camera.translate(dir);
                light.setPosition(camera.getPosition());
            }
            else
            if (input.isKeyDown(Input::KEY_Z))
            {
                Camera &camera = getRenderer().getCamera();
                Vector3f dir = Vector3f(0, -.2, 0);
                camera.translate(dir);
                light.setPosition(camera.getPosition());
            }

            if (input.isKeyDown(Input::KEY_D))
            {
                turnDegree += 1.5F;
                if (turnDegree > 359) turnDegree = 0;
                object->getTransform().rotate(NAN, turnDegree, NAN);
                object->getTransform().update();
            }
            else
            if (input.isKeyDown(Input::KEY_A))
            {
                turnDegree -= 1.5F;
                if (turnDegree < 0) turnDegree = 359;
                object->getTransform().rotate(NAN, turnDegree, NAN);
                object->getTransform().update();
            }

            if (input.isMouseMoved())
            {
                static const float AMOUNT = 1.5F;
                Vector2i delta = input.getMouseDelta();
                Vector3f displace(0);
                if (delta.x < 0) displace.y = AMOUNT;
                if (delta.x > 0) displace.y = -AMOUNT;
                Camera &camera = getRenderer().getCamera();
                camera.rotateTarget(displace);
                //std::cout << "Rotating target" << std::endl;
            }
        }

        void update()
        {
        }

        void draw()
        {
            //getRenderer().draw(object->getMesh(), object->getTransform());
            object->draw(getRenderer());
        }

};


int main( int argc, char **argv )
{
    Level *level = Level::load("maps/sample.png");
    if (level == nullptr) return 1;

    Camera camera(Vector3f(0, 0, -10), Vector3f(0, 0, 0), 50.0F, Camera::AR_16x9);
    Light light(Vector3f(0, 0, 10));
    Renderer renderer(camera, light, 1280, 720);

    std::ifstream sf("./shaders/colored-vertex.glsl");
    Shader vertex(ShaderType::SHADER_VERTEX, sf);
    sf.close();

    sf.open("./shaders/colored-fragments.glsl");
    Shader frags(ShaderType::SHADER_FRAGMENT, sf);
    sf.close();

    ShaderProgram program(vertex, frags);

    renderer.setActiveShader(program);
    Game instance(renderer, light);
    instance.run();
    return 0;
}
