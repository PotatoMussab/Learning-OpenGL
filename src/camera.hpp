#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class Camera
{
    public:
        vec3 pos, up, dir;
        float speed, turnRate;
        Camera()
        {
            pos = vec3(0.0f, 0.0f, -1.0f);
            up =  vec3(0.0f, 1.0f, 0.0f);
            dir = vec3(0.0f, 0.0f, 1.0f);
            speed = 1.0f;
        }
        Camera(vec3 setPos, vec3 setUp, vec3 setDir)
        {
            pos = setPos;
            up = setUp;
            dir = setDir;
        }
        void moveForward(float dt)
            {pos = pos + speed * dt * normalize(dir);}
        void moveBackward(float dt)
            {pos = pos - speed * dt * normalize(dir);}
        void moveUp(float dt)
            {pos = pos + speed * dt * normalize(up);}
        void moveDown(float dt)
            {pos = pos - speed * dt * normalize(up);}
        void strafeRight(float dt)
        {
            vec3 rightDir = normalize(cross(dir,up));
            pos = pos + speed * dt * rightDir;
        }
        void strafeLeft(float dt)
        {
            vec3 leftDir = -normalize(cross(dir,up));
            pos = pos + speed * dt * leftDir;
        }
};