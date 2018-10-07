#include <engine/Application.hh>
#include <engine/Camera.hh>
#include <engine/opengl/Renderer.hh>
#include <engine/opengl/Mesh.hh>
#include <engine/opengl/Texture.hh>
#include <engine/opengl/Shader.hh>
#include <engine/Timer.hh>
#include <engine/Vector.hh>
#include <engine/Matrix4.hh>
#include <engine/Object.hh>
#include <engine/Application.hh>

#include "Level.hh"
#include <fstream>

using quark::Vector3f;
using quark::Vector2i;

static const Vector3f verts[]    = { {0, 0, 0}, {5, 0, 0}, {0, 5, 0} };
static const Vector3f colors[]   = { {1, 1, 1}, {1, 0, 1}, {0, 1, 1} };
static const Vector3f normals[]  = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };


static quark::opengl::ShaderProgram *program = nullptr;


class Game : public quark::Application
{
    public:
        static const int NUM_OBJECTS = 1;
        Mesh *mesh;
        quark::Light &light;
        quark::Object *object[NUM_OBJECTS];
        Texture *texture;
        float turnDegree = 0;
        uint32_t lastFPS = 0;

        Game( quark::Renderer &renderer, quark::Light &light) : Application(renderer), light(light)
        {
            getInput().grabCursor(true);

            // test mesh
            quark::MesherModel model("natasha.mesher");
            if (model.objects.size() == 0) throw "No objects inside mesher model";
            mesh = new quark::opengl::Mesh(model.objects[0]);
            // test texture
            texture = new quark::opengl::Texture(256, 256, "natasha_body_d.data");

            for (size_t i = 0; i < NUM_OBJECTS; ++i)
            {
                float scale = 10;
                object[i] = new quark::Object(*mesh, *texture);
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
            quark::Input &input )
        {
            float speed = 0.4F;
            if (input.getModifiers() & quark::Input::MOD_LSHIFT) speed *= 3.0F;

            if (input.isKeyDown(quark::Input::KEY_ESCAPE))
            {
                stop();
                return;
            }

            if (input.isKeyDown(quark::Input::KEY_S) || input.isKeyDown(quark::Input::KEY_W))
            {
                quark::Camera &camera = getRenderer().getCamera();
                Vector3f dir = camera.frontSide();
                if (input.isKeyDown(quark::Input::KEY_S))
                    dir = -dir;
                //std::cout << "Step of " << dir << std::endl;
                camera.move(dir, speed);
                //std::cout << "Player: " << camera.getPosition().x << " x " << camera.getPosition().z << "\r" << std::flush;
                light.setPosition(camera.getPosition());
            }

            if (input.isKeyDown(quark::Input::KEY_Q))
            {
                quark::Camera &camera = getRenderer().getCamera();
                Vector3f dir = Vector3f(0.0F, .3F, 0.0F);
                camera.move(dir, speed);
                light.setPosition(camera.getPosition());
            }
            else
            if (input.isKeyDown(quark::Input::KEY_Z))
            {
                quark::Camera &camera = getRenderer().getCamera();
                Vector3f dir = Vector3f(0.0F, -.3F, 0.0F);
                camera.move(dir, speed);
                light.setPosition(camera.getPosition());
            }

            if (input.isKeyDown(quark::Input::KEY_D))
            {
                quark::Camera &camera = getRenderer().getCamera();
                camera.move(camera.leftSide(), speed);
                light.setPosition(camera.getPosition());
            }
            else
            if (input.isKeyDown(quark::Input::KEY_A))
            {
                quark::Camera &camera = getRenderer().getCamera();
                camera.move(camera.rightSide(), speed);
                light.setPosition(camera.getPosition());
            }

            if (input.isKeyDown(quark::Input::KEY_E))
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
            if (input.isKeyDown(quark::Input::KEY_C))
            {
                turnDegree -= 1.5F;
                if (turnDegree < 0) turnDegree = 359;
                for (size_t i = 0; i < NUM_OBJECTS; ++i)
                {
                    object[i]->getTransform().rotate(NAN, turnDegree, NAN);
                    object[i]->getTransform().update();
                }
            }

            if (input.isKeyDown(quark::Input::KEY_R))
            {
                quark::Camera &camera = getRenderer().getCamera();
                camera.frontSide() = Vector3f(1, 0, 0);
                camera.update();
            }

            if (input.isMouseMoved())
            {
                static const float AMOUNT = 0.3F;
                Vector2i delta = input.getMouseDelta();
                Vector3f displace(0);
                if (!input.isKeyDown(quark::Input::KEY_X)) displace.x = (float) delta.x * -AMOUNT;
                if (!input.isKeyDown(quark::Input::KEY_Y)) displace.y = (float) delta.y * AMOUNT;
                quark::Camera &camera = getRenderer().getCamera();

                /*angles.x += displace.y;
                if (angles.x < 0) angles.x = 259;
                if (angles.x >= 260) angles.x = 0;*/
                //std::cout << "From " << camera.frontSide() << std::endl;
                camera.pan(displace.x);
                camera.tilt(displace.y);
                //std::cout << "Front " << camera.frontSide() << std::endl << std::endl;
                //std::cout << "Left " << camera.leftSide() << std::endl << std::endl;
            }
        }

        void update()
        {
            uint32_t currentFPS = quark::Timer::getFPS();
            if (currentFPS != lastFPS)
            {
                lastFPS = currentFPS;
                std::cout << "FPS: " << currentFPS << std::endl;
            }
        }

        void draw()
        {
            ((quark::opengl::Renderer&)getRenderer()).drawGrid();
            //getRenderer().setActiveShader(*program);
            for (size_t i = 0; i < NUM_OBJECTS; ++i)
                object[i]->draw(getRenderer());
        }

};


int main( int argc, char **argv )
{
    (void) argc;
    (void) argv;

    quark::Camera camera(Vector3f(0, 15, -30), Vector3f(0, 1, 0), Vector3f(0, 0, 1), 50.0F, quark::Camera::AR_16x9);

    quark::Light light(camera.getPosition());
    quark::opengl::Renderer renderer(camera, light, 1280, 720);

    std::ifstream sf("./shaders/vertex.glsl");
    quark::opengl::Shader vertex(quark::ShaderType::SHADER_VERTEX, sf);
    sf.close();

    sf.open("./shaders/fragments.glsl");
    quark::opengl::Shader frags(quark::ShaderType::SHADER_FRAGMENT, sf);
    sf.close();

    program = new quark::opengl::ShaderProgram(vertex, frags);

    renderer.setActiveShader(*program);
    Game instance(renderer, light);
    instance.run();
    return 0;
}
