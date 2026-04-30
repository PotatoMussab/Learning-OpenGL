#define _USE_MATH_DEFINES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
using namespace glm;

class Camera
{
    public:
        const float MAX_PITCH =  89.9f;
        const float MIN_PITCH = -89.9f;
        vec3 pos;
        float pitch, yaw; //Yaw is defined as the angle from the positive z-axis CCW in degrees
        float speed, turnRate; //Turnrate in degrees per units moved by mouse

        //Constructors
        Camera()
        {
            pos = vec3(0.0f, 0.0f, -1.0f);
            up =  vec3(0.0f, 1.0f, 0.0f);
            dir = vec3(0.0f, 0.0f, 1.0f);
            speed = 1.0f;
            pitch = 0.0f;
            yaw = 0.0f;
            turnRate = 0.1f;
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
        void setDir(vec3 newDir)
        {
            dir = normalize(newDir);
            pitch = asinf(radians(dir.y));
            yaw = atanf(radians(dir.x/dir.z));
        }
        void setUp(vec3 newUp)
        {
            up = normalize(newUp);
        }

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
        void turn(float dx, float dy)
        {
            yaw   -= dx * turnRate;
            pitch += dy * turnRate;
            if(pitch > MAX_PITCH)
                pitch = MAX_PITCH;
            else if(pitch < MIN_PITCH)
                pitch = MIN_PITCH;
        }
        void computeDirFromAngles()
        {
            float pitchRad = radians(pitch);
            float yawRad = radians(yaw);
            dir = normalize(vec3(cosf(pitchRad)*sinf(yawRad),
                                 sinf(pitchRad),
                                 cosf(pitchRad)*cosf(yawRad)));
        }
    private:
        vec3  up, dir;
};