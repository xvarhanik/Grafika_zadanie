#ifndef IMAGECLASSES_H
#define IMAGECLASSES_H

#include <QtMath>
class My3Dpoint {
private:
    double x;
    double y;
    double z;
public:
    My3Dpoint() {}

    My3Dpoint(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    double getX() { return x; }
    double getY() { return y; }
    double getZ() { return z; }

    double getR() { return x; }
    double getG() { return y; }
    double getB() { return z; }

    void setX(double x) { this->x = x; }
    void setY(double y) { this->y = y; }
    void setZ(double z) { this->z = z; }

    void setR(double x) { this->x = x; }
    void setG(double y) { this->y = y; }
    void setB(double z) { this->z = z; }

    void setPoint(double x,double y,double z){this->x=x;this->y=y;this->z=z;}

    My3Dpoint operator+(const My3Dpoint& toSum);
    My3Dpoint operator-(const My3Dpoint& toSum);
    My3Dpoint operator*(const double& toSum);
    My3Dpoint operator*(const My3Dpoint& toSum);
    //My3Dpoint operator=(const My3Dpoint& toSum);
    My3Dpoint static findTriangleCenter(My3Dpoint A,My3Dpoint B,My3Dpoint C);
    bool pointLaysOnLine(My3Dpoint A,My3Dpoint B);
    void normalize();
    double dotproduct(My3Dpoint A);
    double length();

    My3Dpoint(const My3Dpoint &p2) {x = p2.x; y = p2.y; z = p2.z; }
};


class Triangle {
private:
    int A;
    int B;
    int C;
public:
    Triangle() {}
    Triangle(int A, int B,int C){this->A = A;this->B = B;this->C = C;}

    void setTriangle(int A, int B, int C) { this->A = A; this->B = B; this->C = C; }
    int getA() { return A; }
    int getB() { return B; }
    int getC() { return C; }


};


class Edge{
private:
    int start;
    int end;
public:
    Edge(){}
    Edge(int start, int end){this->start = start;this->end = end;}

    void setStart(int start) { this->start = start; }
    void setEnd(int end) { this->end = end; }
    void setEdge(int start, int end) { this->start = start; this->end = end; }

    int getStart() { return start; }
    int getEnd() { return end; }

};
#endif // IMAGECLASSES_H
