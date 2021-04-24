#ifndef LEARN_OPENGL_CAMERA_H
#define LEARN_OPENGL_CAMERA_H

#include <Eigen/Dense>

class Camera {
public:
    Camera(Eigen::Vector3f position);

    Eigen::Matrix4f view();

    void rotate(double x, double y);
    void moveFoward();
    void moveBack();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

private:
    Eigen::Vector3f position;

    Eigen::Vector3f vx;
    Eigen::Vector3f vy;
    Eigen::Vector3f vz;

    float yaw{};
    float pitch{};

};


#endif //LEARN_OPENGL_CAMERA_H
