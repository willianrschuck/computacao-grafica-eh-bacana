#include "Camera.h"
#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h>

#define ROTATION_ANGLE 0.001f

static Eigen::Vector3f s2c(float phi, float theta) {
    return Eigen::Vector3f(cos(phi) * sin(theta), sin(phi), cos(phi) * cos(theta));
}

double deltaTime;

void updateDeltaTime() {
    static double oldTime = glfwGetTime();
    double delta = glfwGetTime() - oldTime;
    oldTime = glfwGetTime();
    deltaTime = delta;
}

Eigen::Matrix4f buildPerspectiveMatrix(float fov, float zNear, float zFar, float aspectRatio) {

    Eigen::Matrix4f mat = Eigen::Matrix4f::Zero();
    float halfFOV = tan(fov/2.0f);

    mat(0,0) = 1.0f / (halfFOV * aspectRatio);
    mat(1,1) = 1.0f / halfFOV;
    mat(2,2) = (zNear+zFar) / (-zFar + zNear);
    mat(2,3) = (2.0f * zNear * zFar) / (-zFar + zNear);
    mat(3,2) = -1.0f;

    return mat;

}

Camera::Camera(Eigen::Vector3f thePosition, float theAspectRatio) {
    position = thePosition;
    aspectRatio = theAspectRatio;

    vz = Eigen::Vector3f(0,0,1).normalized();
    vx = Eigen::Vector3f (0,1,0).cross(vz).normalized();
    vy = vz.cross(vx).normalized();

    projectionMatrix = buildPerspectiveMatrix(fov, zNear, zFar, aspectRatio);

}

Eigen::Matrix4f Camera::view() {

    updateDeltaTime();

    Eigen::Affine3f cam = Eigen::Affine3f::Identity();
    cam.translate(-position);

    Eigen::Matrix4f rotation = Eigen::Matrix4f::Identity();
    rotation.row(0).head(3) = vx;
    rotation.row(1).head(3) = vy;
    rotation.row(2).head(3) = vz;

    return rotation * cam.matrix();

}

Eigen::Matrix4f Camera::projection() {
    return projectionMatrix;
}

void Camera::zoomIn() {
    fov -= 0.1f;
    if (fov < 0.224f) fov = 0.224f;
    projectionMatrix = buildPerspectiveMatrix(fov, zNear, zFar, aspectRatio);
}

void Camera::zoomOut() {
    fov += 0.1f;
    if (fov > 1.309f) fov = 1.309f;
    projectionMatrix = buildPerspectiveMatrix(fov, zNear, zFar, aspectRatio);
}

void Camera::rotate(double x, double y) {
    static double oldX = x;
    static double oldY = y;
    double dx = x - oldX;
    double dy = y - oldY;
    oldX = x;
    oldY = y;

    float new_pitch = pitch + (dy * ROTATION_ANGLE * rotationSpeed);
    if (abs(new_pitch) < M_PI_2)
        pitch = new_pitch;

    yaw -= dx * ROTATION_ANGLE * rotationSpeed;

    vz = s2c(pitch, yaw).normalized();
    vx = Eigen::Vector3f(0,1,0).cross(vz).normalized();
    vy = vz.cross(vx).normalized();

}

void Camera::moveFoward() {
    position -= vz * walkSpeed * deltaTime;
}

void Camera::moveBack() {
    position += vz * walkSpeed * deltaTime;
}

void Camera::moveLeft() {
    position -= vx * walkSpeed * deltaTime;
}

void Camera::moveRight() {
    position += vx * walkSpeed * deltaTime;
}

void Camera::moveUp() {
    position += vy * walkSpeed * deltaTime;
}

void Camera::moveDown() {
    position -= vy * walkSpeed * deltaTime;
}

Eigen::Vector3f Camera::getPosition() {
    return position;
}

Eigen::Vector3f Camera::getDirection() {
    return -vz;
}

void Camera::newViewAspectRatio(float i) {
    aspectRatio = i;
    projectionMatrix = buildPerspectiveMatrix(fov, zNear, zFar, aspectRatio);
}


