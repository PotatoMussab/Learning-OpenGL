#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class Camera
{
    public:
        vec3 pos;
        float pitch, yaw; //Pitch is up/down, yaw is left right
        float speed, turnRate;

        //Constructors
        Camera()
        {
            pos = vec3(0.0f, 0.0f, -1.0f);
            up =  vec3(0.0f, 1.0f, 0.0f);
            dir = vec3(0.0f, 0.0f, 1.0f);
            speed = 1.0f;
        }
        Camera(vec3 setPos, vec3 newUp, vec3 newDir)
        {
            pos = setPos;
            up = glm::normalize(newUp);
            dir = glm::normalize(newDir);
        }

        //Set and get functions
        inline vec3 getDir()
            {return dir;}
        inline vec3 getUp()
            {return up;}
        inline void setDir(vec3 newDir)
            {dir = normalize(newDir);}
        inline void setUp(vec3 newUp)
            {up = normalize(newUp);}

        //Camera movement functions
        void moveForward(float dt)
            {pos = pos + speed * dt * dir;}
        void moveBackward(float dt)
            {pos = pos - speed * dt * dir;}
        void moveUp(float dt)
            {pos = pos + speed * dt * up;}
        void moveDown(float dt)
            {pos = pos - speed * dt * up;}
        void strafeRight(float dt)
        {
            vec3 rightDir = cross(dir,up);
            pos = pos + speed * dt * rightDir;
        }
        void strafeLeft(float dt)
        {
            vec3 leftDir = -cross(dir,up);
            pos = pos + speed * dt * leftDir;
        }
        void turn(float dx, float dy, float dt)
        {
            
        }
    private:
        vec3  up, dir;
};