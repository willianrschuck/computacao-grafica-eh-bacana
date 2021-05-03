#ifndef LEARN_OPENGL_CAMERA_H
#define LEARN_OPENGL_CAMERA_H

#include <Eigen/Dense>

class Camera {
public:
    Camera(Eigen::Vector3f position,  float theAspectRatio);

    Eigen::Matrix4f view();
    Eigen::Matrix4f projection();

    Eigen::Vector3f getPosition();
    Eigen::Vector3f getDirection();

    void zoomIn();
    void zoomOut();

    void rotate(double x, double y);
    void moveFoward();
    void moveBack();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    float walkSpeed = 5.0f;
    float rotationSpeed = 1.0f;

    void newViewAspectRatio(float i);

private:
    Eigen::Vector3f position;
    Eigen::Matrix4f projectionMatrix;

    Eigen::Vector3f vx;
    Eigen::Vector3f vy;
    Eigen::Vector3f vz;

    float zFar = 500.0f;
    float zNear = 0.1f;
    float fov = 1.134f;

    float yaw{};
    float pitch{};

    float aspectRatio = 1.0f;
};


#endif //LEARN_OPENGL_CAMERA_H
