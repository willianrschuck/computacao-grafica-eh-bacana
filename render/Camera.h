#ifndef LEARN_OPENGL_CAMERA_H
#define LEARN_OPENGL_CAMERA_H

#include <Eigen/Dense>

class Camera {
public:
    Camera(Eigen::Vector3f position);

    Eigen::Matrix4f view();
    Eigen::Matrix4f projection();

    Eigen::Vector3f getPosition();

    void zoomIn();
    void zoomOut();

    void rotate(double x, double y);
    void moveFoward();
    void moveBack();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

private:
    Eigen::Vector3f position;
    Eigen::Matrix4f projectionMatrix;

    Eigen::Vector3f vx;
    Eigen::Vector3f vy;
    Eigen::Vector3f vz;

    float zFar = 500.0f;
    float zNear = 0.1f;
    float fov = 1.134f;

    float yaw;
    float pitch;

};


#endif //LEARN_OPENGL_CAMERA_H
