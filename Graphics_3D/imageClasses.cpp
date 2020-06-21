#include "imageClasses.h"

My3Dpoint My3Dpoint::operator+(const My3Dpoint& toSum)
{
    //2 3D vektory sa sčítajú zložka po zložke
    My3Dpoint toRet;
    toRet.setX(x+toSum.x);
    toRet.setY(y+toSum.y);
    toRet.setZ(z+toSum.z);
    return toRet;
}

void My3Dpoint::normalize()
{
    //vektor sa znormalizuje štandartným skalárnym súčinom
    double norm = sqrt(x*x + y*y + z*z);
    x=x/norm;
    y=y/norm;
    z=z/norm;
}

My3Dpoint My3Dpoint::operator-(const My3Dpoint& toSum)
{
    //Od jedneho vektora sa odčíta druhý zložku po zložke
    My3Dpoint toRet;
    toRet.setX(x-toSum.x);
    toRet.setY(y-toSum.y);
    toRet.setZ(z-toSum.z);
    return toRet;
}

double My3Dpoint::dotproduct(My3Dpoint A)
{
    //vráti sa skalárny súčin vektora ktorý zavolá metódu s argumentom
    return A.getX()*x+A.getY()*y+A.getZ()*z;
}

My3Dpoint My3Dpoint::operator*(const double & toSum)
{
    //vektor sa vynásobí konštantou na pravej strane
    My3Dpoint toRet;
    toRet.setX(x*toSum);
    toRet.setY(y*toSum);
    toRet.setZ(z*toSum);
    return toRet;
}

My3Dpoint My3Dpoint::operator*(const My3Dpoint& toSum)
{
    //vektory sa vynásobia zložku po zložke
    My3Dpoint toRet;
    toRet.setX(x*toSum.x);
    toRet.setY(x*toSum.y);
    toRet.setZ(x*toSum.z);
    return toRet;
}

My3Dpoint  My3Dpoint::findTriangleCenter(My3Dpoint A, My3Dpoint B, My3Dpoint C)
{
    //funkcia vráti ťažisko trojuholníka
    My3Dpoint toRet(0,0,0);
    toRet.setX((A.getX()+B.getX()+C.getX())/3);
    toRet.setY((A.getY()+B.getY()+C.getY())/3);
    toRet.setZ((A.getZ()+B.getZ()+C.getZ())/3);
    return toRet;
}

double My3Dpoint::length()
{
    return sqrt(x*x+y*y+z*z);
}

bool My3Dpoint::pointLaysOnLine(My3Dpoint A, My3Dpoint B)
{
    My3Dpoint C(x,y,0);
    My3Dpoint AB=B-A;
    My3Dpoint AC=C-A;
    My3Dpoint CB=B-C;

    if(AB.length()==(AC.length()+CB.length()))
            return true;
    return false;

}
