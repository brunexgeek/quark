#include <engine/Application.hh>
#include <engine/Camera.hh>
#include <engine/Renderer.hh>
#include <engine/Mesh.hh>
#include <engine/Timer.hh>
#include <engine/Shader.hh>
#include <engine/Vector2.hh>
#include <engine/Vector3.hh>
#include <engine/Matrix4.hh>
#include <engine/Quaternion.hh>
#include <engine/Object.hh>

#include "Level.hh"
#include <fstream>


static const Vector3f verts[]    = { {0, 0, 0}, {5, 0, 0}, {0, 5, 0} };
static const Vector3f colors[]   = { {1, 1, 1}, {1, 0, 1}, {0, 1, 1} };
static const Vector3f normals[]  = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };


class Game : public Application
{
    public:
        static const int NUM_OBJECTS = 3;
        Mesh *mesh;
        Light &light;
        Object *object[NUM_OBJECTS];
        Texture *texture;
        float turnDegree = 0;
        Vector3f angles = { 0, 0, 0 };
        uint32_t lastFPS = 0;

        Game( Renderer &renderer, Light &light) : Application(renderer), light(light)
        {
            getInput().grabCursor(true);

            // test mesh
	        mesh = new Mesh("natasha2.mesher");
            // test texture
            texture = new Texture(256, 256, "natasha_body_d.data");

            for (size_t i = 0; i < NUM_OBJECTS; ++i)
            {
                float scale = 30;
                object[i] = new Object(*mesh, *texture);
                object[i]->getTransform().scale({scale, scale, scale});
                object[i]->getTransform().translate(Vector3f( (float)i * scale, 0.0F, 0.0F));
                object[i]->getTransform().update();
            }
        }

        ~Game()
        {
            for (size_t i = 0; i < NUM_OBJECTS; ++i) delete object[i];
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
                Vector3f dir = (camera.frontSide() - camera.getPosition());
                dir.normalize();
                if (input.isKeyDown(Input::KEY_S)) dir = -dir;
                //std::cout << "Step of " << dir << std::endl;
                camera.move(dir, .3F);
                //std::cout << "Player: " << camera.getPosition().x << " x " << camera.getPosition().z << std::endl;
                light.setPosition(camera.getPosition());
            }

            if (input.isKeyDown(Input::KEY_Q))
            {
                Camera &camera = getRenderer().getCamera();
                Vector3f dir = Vector3f(0.0F, .2F, 0.0F);
                camera.move(dir, .3F);
                light.setPosition(camera.getPosition());
            }
            else
            if (input.isKeyDown(Input::KEY_Z))
            {
                Camera &camera = getRenderer().getCamera();
                camera.move(camera.leftSide(), .3F);
                light.setPosition(camera.getPosition());
            }

            if (input.isKeyDown(Input::KEY_D))
            {
                Camera &camera = getRenderer().getCamera();
                camera.move(camera.leftSide(), .3F);
                light.setPosition(camera.getPosition());
            }
            else
            if (input.isKeyDown(Input::KEY_A))
            {
                Camera &camera = getRenderer().getCamera();
                camera.move(camera.rightSide(), .3F);
                light.setPosition(camera.getPosition());
            }

            if (input.isKeyDown(Input::KEY_E))
            {
                turnDegree += 1.5F;
                if (turnDegree > 359) turnDegree = 0;
                for (size_t i = 0; i < NUM_OBJECTS; ++i)
                {
                    object[i]->getTransform().rotate(NAN, turnDegree, NAN);
                    object[i]->getTransform().update();
                }
            }
            else
            if (input.isKeyDown(Input::KEY_C))
            {
                turnDegree -= 1.5F;
                if (turnDegree < 0) turnDegree = 359;
                for (size_t i = 0; i < NUM_OBJECTS; ++i)
                {
                    object[i]->getTransform().rotate(NAN, turnDegree, NAN);
                    object[i]->getTransform().update();
                }
            }

            if (input.isMouseMoved())
            {
                static const float AMOUNT = 0.2F;
                Vector2i delta = input.getMouseDelta();
                Vector3f displace(0);
                displace.x = (float) delta.x * -AMOUNT;
                displace.y = (float) delta.y * AMOUNT;
                Camera &camera = getRenderer().getCamera();

                angles.x += displace.y;
                if (angles.x < 0) angles.x = 259;
                if (angles.x >= 260) angles.x = 0;
                //std::cout << "From " << camera.frontSide() << std::endl;
                camera.pan(displace.x);
                camera.tilt(displace.y);
                //std::cout << "  To " << camera.frontSide() << std::endl;
            }
        }

        void update()
        {
            uint32_t currentFPS = Timer::getFPS();
            if (currentFPS != lastFPS)
            {
                lastFPS = currentFPS;
                std::cout << "FPS: " << currentFPS << std::endl;
            }
        }

        void draw()
        {
            for (size_t i = 0; i < NUM_OBJECTS; ++i)
                object[i]->draw(getRenderer());
        }

};


int main( int argc, char **argv )
{
    (void) argc;
    (void) argv;

    Camera camera(Vector3f(0, 0, 0), Vector3f(0, 1, 0), Vector3f(0, 0, 1), 50.0F, Camera::AR_16x9);

    Light light(Vector3f(0, 0, 10));
    Renderer renderer(camera, light, 1280, 720);

    std::ifstream sf("./shaders/vertex.glsl");
    Shader vertex(ShaderType::SHADER_VERTEX, sf);
    sf.close();

    sf.open("./shaders/fragments.glsl");
    Shader frags(ShaderType::SHADER_FRAGMENT, sf);
    sf.close();

    ShaderProgram program(vertex, frags);

    renderer.setActiveShader(program);
    Game instance(renderer, light);
    instance.run();
    return 0;
}
