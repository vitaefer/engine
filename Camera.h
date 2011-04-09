#ifndef Camera_H
#define Camera_H

class fpsCamera
{
public:
    fpsCamera();
    void position(float _x, float _y, float _z);
    void move(float _x, float _y, float _z);
    void rotate(float _rx, float _ry, float _rz);
    void update();
    void setUp();
private:
    float x, y, z;
    float rx, ry, rz;
    int mousex, mousey, lastmousex, lastmousey;
};

#endif