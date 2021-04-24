#include "Camera.h"
#include <iostream>
#include <cmath>

#define velocity 0.2f
#define rotation_velocity 0.002f

static Eigen::Vector3f s2c(float phi, float theta) {
    return Eigen::Vector3f(cos(phi) * sin(theta), sin(phi), cos(phi) * cos(theta));
}

Camera::Camera(Eigen::Vector3f thePosition) {
    position = thePosition;

    vz = Eigen::Vector3f(0,0,1).normalized();
    vx = Eigen::Vector3f (0,1,0).cross(vz).normalized();
    vy = vz.cross(vx).normalized();

    std::cout << vz << std::endl;
    std::cout << vx << std::endl;
    std::cout << vy << std::endl;

}

Eigen::Matrix4f Camera::view() {

    Eigen::Affine3f cam = Eigen::Affine3f::Identity();
    cam.translate(-position);

    Eigen::Matrix4f rotation = Eigen::Matrix4f::Identity();
    rotation.row(0).head(3) = vx;
    rotation.row(1).head(3) = vy;
    rotation.row(2).head(3) = vz;

    return rotation * cam.matrix();

}

void Camera::rotate(double x, double y) {
    static double oldX = x;
    static double oldY = y;
    double dx = x - oldX;
    double dy = y - oldY;
    oldX = x;
    oldY = y;

    float new_pitch = pitch + (dy*rotation_velocity);
    if (abs(new_pitch) < M_PI_2)
        pitch = new_pitch;

    yaw -= dx*rotation_velocity;

    vz = s2c(pitch, yaw).normalized();

    vx = Eigen::Vector3f(1,0,0).cross(vz).normalized();
    vy = vz.cross(vx).normalized();

}

void Camera::moveFoward() {
    position -= vz * velocity;
}

void Camera::moveBack() {
    position += vz * velocity;
}

void Camera::moveLeft() {
    position += vx * velocity;
}

void Camera::moveRight() {
    position -= vx * velocity;
}

void Camera::moveUp() {
    position -= vy * velocity;
}

void Camera::moveDown() {
    position += vy * velocity;
}