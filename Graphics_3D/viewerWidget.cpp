#include "viewerWidget.h"


ViewerWidget::ViewerWidget(QString viewerName, QSize imgSize, QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    if (imgSize != QSize(0, 0))
    {
        img = new QImage(imgSize, QImage::Format_ARGB32);
        img->fill(Qt::white);
        resizeWidget(img->size());

        setPainter();
    }
}


ViewerWidget::~ViewerWidget()
{
    delete img;
}


void ViewerWidget::resizeWidget(QSize size)
{
    this->resize(size);
    this->setMinimumSize(size);
    this->setMaximumSize(size);
}


void ViewerWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QRect area = event->rect();
    painter.drawImage(area, *img, area);
}


void ViewerWidget::drawLine(My3Dpoint A, My3Dpoint B,QColor color)
{
    float dx = B.getX() - A.getX();
        float dy = B.getY() - A.getY();
        float steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

        float Xinc = dx / (float)steps;
        float Yinc = dy / (float)steps;

        float X = A.getX();
        float Y = A.getY();

        int i = 0;
        while (i <= steps)
        {
            //pristup k obrazku tutok
            img->setPixel((int)X, (int)Y,color.rgb());
            X += Xinc;
            Y += Yinc;
            i++;
        }
}


void ViewerWidget::clear()
{
    img->fill(Qt::white);
    loadedPoints.clear();
    triangles.clear();
    edges.clear();

}

bool ViewerWidget::loadImage(QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "nepodarilo sa otvorit subor";
        return false;
    }

    else
    {
        clear();
        QString readingType = "Line";
        int cnt,cntMax;
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QList<QString> items = line.split(" ");
            if (readingType == "Line")
            {
                if (items[0] == "POINTS")
                {

                    cnt         = 0;
                    readingType = "Points";
                    cntMax      = items[1].toInt() - 1;

                }
                else if (items[0] == "LINES")
                {
                    cnt         = 0;
                    readingType = "Edges";
                    cntMax      = items[1].toInt()-1;

                }
                else if (items[0] == "POLYGONS")
                {
                    cnt = 0;
                    readingType = "polygon";
                    cntMax = items[1].toInt() - 1;
                }

            }

            else if (readingType == "Points")
            {
               loadedPoints.append(My3Dpoint(items[0].toInt(), items[1].toInt(), items[2].toInt()));

                if (cnt == cntMax)
                {
                readingType = "Line";
                }
                cnt++;

            }

            else if (readingType == "polygon")
            {
                if (items[0].toInt() == 3)
                {

                    edges.append(Edge (items[1].toInt(), items[2].toInt()));
                    edges.append(Edge (items[2].toInt(), items[3].toInt()));
                    edges.append(Edge (items[1].toInt(), items[3].toInt()));
                    triangles.append(Triangle(items[1].toInt(), items[2].toInt(), items[3].toInt()));

                }
                if (cnt == cntMax)
                {
                    readingType = "Line";
                }
                cnt++;
            }
            else if (readingType == "Edges")
            {
                Edge toApp(items[1].toInt(), items[2].toInt());
                edges.append(toApp);


                if (cnt == cntMax)
                {
                    readingType = "Line";
                }
                cnt++;
            }

        }

        return true;
    }

}


void ViewerWidget::renderImage()
{
//funkcia slúži ako rázcestník pre jednotlivé možnosti vykreslovania

    //pokial nemame nacitane ziadne body nebudeme vykreslovat nic
    if( loadedPoints.isEmpty())
        return;

    //vyberiem si medzi stredovým a paralelným zobrazením
    renderedPoints.clear();
    projectPoints();
    img->fill(Qt::white);


    if(typeOfRendering=="wireframe")
    {
       connectEdges();
    }
    else if(typeOfRendering=="phong")
    {
        zBufferClear();
        zBufferInit();

        if(typeOfShading=="gorand")
        {
            shading(true);
        }
        else if(typeOfShading=="constant")
        {
            shading(false);
        }
    }
    this->update();
}

void ViewerWidget::projectPoints()
{
    double alphaRad, betaRad;

        alphaRad = (M_PI * azimutValue) / (double)180;
        betaRad = (M_PI * zenitValue) / (double)180;
        //qDebug() << alphaRad << betaRad;

        double v1, v2, v3;
        v1 = sin(alphaRad) * cos(betaRad);
        v2 = sin(alphaRad) * sin(betaRad);
        v3 = cos(alphaRad);

        double norm = sqrt(v1 * v1 + v2 * v2 + v3 * v3);
        v1 = v1 / (double)norm;
        v2 = v2 / (double)norm;
        v3 = v3 / (double)norm;

        double w1, w2, w3;
        w1 = sin(alphaRad + (M_PI / 2.)) * cos(betaRad);
        w2 = sin(alphaRad + (M_PI / 2.)) * sin(betaRad);
        w3 = cos(alphaRad + (M_PI / 2.));

        norm = sqrt(w1 * w1 + w2 * w2 + w3 * w3);
        w1 = w1 / (double)norm;
        w2 = w2 / (double)norm;
        w3 = w3 / (double)norm;

        double u1, u2, u3;
        u1 = v2 * w3 - v3 * w2;
        u2 = v3 * w1 - v1 * w3;
        u3 = v1 * w2 - v2 * w1;

        for (int i = 0; i <  loadedPoints.length()  ; i++)
        {
            double x, y, z;
            x =  loadedPoints[i].getX() * w1 +  loadedPoints[i].getY() * w2 +  loadedPoints[i].getZ() * w3;
            y =  loadedPoints[i].getX() * u1 +  loadedPoints[i].getY() * u2 +  loadedPoints[i].getZ() * u3;
            z =  loadedPoints[i].getX() * v1 +  loadedPoints[i].getY() * v2 +  loadedPoints[i].getZ() * v3;
            if(typeOfProjection=="paralel")
            {
              renderedPoints.append(My3Dpoint(int(x), int(y),int(z)));
            }
            else if(typeOfProjection=="center")
            {
                renderedPoints.append(My3Dpoint(int(centerOfProjection * x) / (centerOfProjection- z),
                                             (int(centerOfProjection * y) / (centerOfProjection - z)),z));

            }
        }

}
My3Dpoint ViewerWidget::findImageCenter(QList<My3Dpoint> points)
{
    //funkcia zráta ťažisko načítaného telesa
    int x=0,y=0,z=0;
    for(int i=0;i<points.length();i++)
    {
        x=x+points[i].getX();
        y=y+points[i].getY();
        z=z+points[i].getZ();
    }

    return My3Dpoint(x/(double)points.length(),y/(double)points.length(),z/(double)points.length());
}

void ViewerWidget::connectEdges()
{
    for(int i=0;i<edges.length();i++)
    {
        drawLine(renderedPoints[edges[i].getStart()],renderedPoints[edges[i].getEnd()],Qt::black);
    }

}

My3Dpoint ViewerWidget::findCanvasCenter()
{
    My3Dpoint toRet(0,0,0);
    toRet.setX(img->width()/2);
    toRet.setY(img->height()/2);
    return toRet;

}

void ViewerWidget::translate(My3Dpoint imageCenter, My3Dpoint whereToTranslate)
{
   QPoint translateDistance(0,0);
   translateDistance.setX(whereToTranslate.getX()-imageCenter.getX());
   translateDistance.setY(whereToTranslate.getY()-imageCenter.getY());
   for(int i=0;i<loadedPoints.length();i++)
   {
       loadedPoints[i].setX(loadedPoints[i].getX()+translateDistance.x());
       loadedPoints[i].setY(loadedPoints[i].getY()+translateDistance.y());
   }
}

void ViewerWidget::scale(bool scaleUp)
{
    if(loadedPoints.length()==0)
        return;
    My3Dpoint imageCenter=findImageCenter(loadedPoints);
    translate(imageCenter,My3Dpoint(0,0,0));

    if(scaleUp)
    {
        for(int i=0;i<loadedPoints.length();i++)
        {
            loadedPoints[i].setX(loadedPoints[i].getX()*scaleCoeficient);
            loadedPoints[i].setY(loadedPoints[i].getY()*scaleCoeficient);
            loadedPoints[i].setZ(loadedPoints[i].getZ()*scaleCoeficient);
        }
    }
    else
    {
        for(int i=0;i<loadedPoints.length();i++)
        {
           loadedPoints[i].setX(loadedPoints[i].getX()/scaleCoeficient);
            loadedPoints[i].setY(loadedPoints[i].getY()/scaleCoeficient);
            loadedPoints[i].setZ(loadedPoints[i].getZ()/scaleCoeficient);
        }
    }
    translate(My3Dpoint(0,0,0),imageCenter);
    renderImage();

};


void ViewerWidget::rotate(bool Xaxis)
{
    My3Dpoint imageCenter=findImageCenter(loadedPoints);
    translate(imageCenter,My3Dpoint(0,0,0));
    double alpha=M_PI*rotationAngle/180;
    if(Xaxis)
    {
        for(int i=0;i<loadedPoints.length();i++)
        {
            int y=loadedPoints[i].getY();
            int z=loadedPoints[i].getZ();
            loadedPoints[i].setY((int)(cos(alpha)*y-sin(alpha)*z));
            loadedPoints[i].setZ((int)(sin(alpha)*y+cos(alpha)*z));

        }
    }

    else
    {
        for(int i=0;i<loadedPoints.length();i++)
        {
            int x=loadedPoints[i].getX();
            int z=loadedPoints[i].getZ();
            loadedPoints[i].setX((int)(cos(alpha)*x-sin(alpha)*z));
            loadedPoints[i].setZ((int)(sin(alpha)*x+cos(alpha)*z));

        }
    }
    translate(My3Dpoint(0,0,0),imageCenter);
    renderImage();
}



QColor ViewerWidget::phong(My3Dpoint point)
{

/******************************************
* Funkcia dostane na vstup bod a vráti    *
* jeho farbu .Všetky vektory v tejto      *
* funkcií sú pomenované podľa prezentácie *
*******************************************/

            //Vektor N
            My3Dpoint N(point.getX(),point.getY(),point.getZ());
            N = N-findImageCenter(loadedPoints);
            N.normalize();

            //Vektor L pre oba zdroje svetla
            My3Dpoint L1 = lightSource1-point;
            L1.normalize();
            My3Dpoint L2 = lightSource2-point;
            L2.normalize();

            //Vektor R pre oba zdroje svetla
            float SS1 = 2*N.dotproduct(L1);
            float SS2 = 2*N.dotproduct(L2);
            My3Dpoint R1 = N*SS1-L1;
            R1.normalize();
            My3Dpoint R2 = N*SS2-L2;
            R2.normalize();

            //Vektor V
            My3Dpoint V = cameraPosition;
            V = V - point;
            V.normalize();

            //***Difuzna zlozka***//
            float LN1 = N.dotproduct(L1);
            float LN2 = N.dotproduct(L2);
            My3Dpoint IL1((double)colorOfSource1.red(), (double)colorOfSource1.green(), (double)colorOfSource1.blue());
            My3Dpoint IL2((double)colorOfSource2.red(), (double)colorOfSource2.green(), (double)colorOfSource2.blue());

            My3Dpoint difusion;
            if(LN1<=0)
                difusion=IL2*difusionPart*LN2;
            else if(LN2<=0)
                difusion=IL1*difusionPart*LN1;
            else
                difusion = IL1*difusionPart*LN1 + IL2*difusionPart*LN2;

            //***ZRKADLOVA ZLOZKA***//
            double VR1  = V.dotproduct(R1);
            double VRh1 = pow(VR1,sharpnessOfLight);
            double VR2  = V.dotproduct(R2);
            double VRh2 = pow(VR2,sharpnessOfLight);

            My3Dpoint mirror = IL1*mirrorPart*VRh1 + IL2*mirrorPart*VRh2;
            if(VR1<=0)
                mirror = IL2*mirrorPart*VRh2;
            else if (VR2<=0)
                IL1*mirrorPart*VRh1;
            else if(LN1<=0)
                mirror = IL2*mirrorPart*VRh2;
            else if (LN2<=0)
                IL1*mirrorPart*VRh1;
            //***AMBIENTNA ZLOZKA***//
            My3Dpoint IO((double)colorOfEnviroment.red(),(double)colorOfEnviroment.green(),(double)colorOfEnviroment.blue());
            My3Dpoint ambient = IO*ambientPart;

            if(mirror.getR()>255)
                mirror.setR(255);
            if(mirror.getG()>255)
                mirror.setG(255);
            if(mirror.getB()>255)
                mirror.setB(255);

            if(ambient.getR()>255)
               ambient.setR(255);
            if(ambient.getG()>255)
               ambient.setG(255);
            if(ambient.getB()>255)
               ambient.setB(255);

            if(difusion.getR()>255)
               difusion.setR(255);
            if(difusion.getG()>255)
               difusion.setG(255);
            if(difusion.getB()>255)
               difusion.setB(255);
            QColor color;
            //kontrola a nastavenie červenej farby
            if(mirror.getX() + ambient.getX() + difusion.getX() > 255)
                color.setRed(255);
            else if (mirror.getX() + ambient.getX() + difusion.getX() < 0)
                color.setRed(0);
            else
                color.setRed(mirror.getX() + ambient.getX() + difusion.getX());

            //kontrola a nastavenie zelenej farby
            if (mirror.getY() + ambient.getY() + difusion.getY() > 255)
                color.setGreen(255);
            else if (mirror.getY() + ambient.getY() + difusion.getY() < 0)
                color.setGreen(0);
            else
                color.setGreen(mirror.getY() + ambient.getY() + difusion.getY());

            //kontrola a nastavenie modrej farby
            if (mirror.getZ() + ambient.getZ() + difusion.getZ()> 255)
                color.setBlue(255);
            else if (mirror.getZ() + ambient.getZ() + difusion.getZ()<0 )
                color.setBlue(0);
            else
                color.setBlue(mirror.getZ() + ambient.getZ() + difusion.getZ());

            return color;
}

QList<My3Dpoint> ViewerWidget::sortByX(QList<My3Dpoint> points)
{
    QList<My3Dpoint> sorted;
    while (!points.isEmpty())
    {
        int max=0;
        for (int i = 0; i < points.length(); i++)
        {
            if (max < points[i].getX())
                max = points[i].getX();
        }

        for (int i = 0; i < points.length(); i++)
        {
            if (max == points[i].getX())
            {
                sorted.prepend(points[i]);
                points.removeAt(i);
            }
        }
    }
    return sorted;
}
 void ViewerWidget::drawLineZbuffer(My3Dpoint A, My3Dpoint B, QColor color)
 {

     float dx = B.getX() - A.getX();
     float dy = B.getY() - A.getY();
     float steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

     float Xinc = dx / (float)steps;
     float Yinc = dy / (float)steps;

     float X = A.getX();
     float Y = A.getY();

     int i = 0;
     while (i <= steps)
     {
        //priblizny vypocey z-ovej suradnice bodu
       
        int z=createY(A,B,X,Y);
         My3Dpoint toApp(X,Y,z);
        //kontrola ci som v obrazku
        if(X>=img->width()||Y>=img->height()||X<0||Y<0)
            return;
        //kontrola z-ovej hodnoty so Z-bufferom
             if(zBuffer[(int)X][(int)Y]>z)
             {
                zBuffer[(int)X][(int)Y]=z;
                img->setPixel((int)X, (int)Y,color.rgb());
             }

             X += Xinc;
             Y += Yinc;
             i++;
     }
 }
 void ViewerWidget::zBufferInit()
 {
     //inicializacia velkosti Z-buffera
     zBuffer=new int*[img->width()];
     for(int i=0;i<img->width();i++)
     {
         zBuffer[i]=new int[img->height()];
     }
     //vyplnenie zbuffera velkou hodnotou
     for(int i=0;i<img->width();i++)
     {
         for(int j=0;j<img->height();j++)
         {
             zBuffer[i][j]=INT_MAX;
         }
     }
 }

 void ViewerWidget::zBufferClear()
 {
     //postupne sa vycistia jednotlive riadky
     if(zBuffer==nullptr)
         return;
     for(int i=0;i<img->width();i++)
     {
         delete[] zBuffer[i];
     }
     //nakoniec sa vycisti samotny buffer
     delete[] zBuffer;

 }
void ViewerWidget::shading(bool gorand)
{
    for(int i=0;i<triangles.length();i++)
    {
        My3Dpoint A,B,C;
        //trojuholnik sa zoradí tak aby boli body označené zhora písmenami C A B
        int a = renderedPoints[triangles[i].getA()].getY();
        int b = renderedPoints[triangles[i].getB()].getY();
        int c = renderedPoints[triangles[i].getC()].getY();

        if (b <= a && c > a)
        {
            A = renderedPoints[triangles[i].getA()];
            B = renderedPoints[triangles[i].getB()];
            C = renderedPoints[triangles[i].getC()];
        }

        else if (c <= a && b >= a)
        {
                A = renderedPoints[triangles[i].getA()];
                B = renderedPoints[triangles[i].getC()];
                C = renderedPoints[triangles[i].getB()];
        }

        else if (a <= c && b >= c)
        {
                A = renderedPoints[triangles[i].getC()];
                B = renderedPoints[triangles[i].getA()];
                C = renderedPoints[triangles[i].getB()];
        }

        else if (b <= c && a >= c)
        {
            A = renderedPoints[triangles[i].getC()];
            B = renderedPoints[triangles[i].getB()];
            C = renderedPoints[triangles[i].getA()];
        }

        else if (a <= b && c >= b)
        {
            A = renderedPoints[triangles[i].getB()];
            B = renderedPoints[triangles[i].getA()];
            C = renderedPoints[triangles[i].getC()];
        }

        else if (c <= b && a >= b)
        {
            A = renderedPoints[triangles[i].getB()];
            B = renderedPoints[triangles[i].getC()];
            C = renderedPoints[triangles[i].getA()];
        }

        //pre pripad ze trojuholnik netreba rozdelit
        if((int)A.getY()==(int)B.getY()||(int)A.getY()==(int)C.getY()||(int)C.getY()==(int)B.getY())
        {
            //vymenim body tak aby C bolo vždy ten bod ktorý nemá spoločné súradnice
            if((int)C.getY()==(int)B.getY())
            {
                My3Dpoint aux;
                aux=C;
                C=A;
                A=aux;
            }
            else if((int)C.getY()==(int)A.getY())
            {
                My3Dpoint aux;
                aux=C;
                C=B;
                B=aux;
            }
            //Vytvorí sa zoznam farieb
            QList<QColor>colors;
            if(gorand)
            {
               colors.append(phong(A));
               colors.append(phong(B));
               colors.append(phong(C));
            }
            else
            {
             //Vytvorí sa stred
               My3Dpoint center = My3Dpoint::findTriangleCenter(A,B,C);
               colors.append(phong(center));
            }
            int ymin1 = img->height(), ymax1 = 0;
                QList<My3Dpoint> points1;
                points1.append(A);
                points1.append(B);
                points1.append(C);

                if((int)C.getY()>(int)A.getY())
                {
                    ymin1=(int)A.getY();
                    ymax1=(int)C.getY();
                }
                else if((int)C.getY()<(int)A.getY())
                {
                    ymin1=(int)C.getY();
                    ymax1=(int)A.getY();
                }
                //prechádzam celý trojuholník
                for (int ya = ymin1; ya < ymax1; ya++)
                {

                    QList<My3Dpoint> hits;

                    //vypocet priesecnika pre hranu AC
                    double xa=createXa(A,C,ya);
                    double z=createZ(A,C,xa,ya);
                    My3Dpoint toApp((int)xa, ya,z);
                    hits.append(toApp);

                    //vypocet priesecnika pre hranu BC
                    xa=createXa(B,C,ya);
                    z=createZ(B,C,xa,ya);
                    My3Dpoint toApp2((int)xa, ya,z);
                    hits.append(toApp2);
                    hits = sortByX(hits);

                    if (gorand)
                         drawLineBarycentric(A,B,C,hits[0],hits[1],colors);
                    else
                         drawLineZbuffer(hits[0],hits[1],colors[0]);
                }
        }

        else
        {
            //Vytvorí sa zoznam farieb
            QList<QColor>colors;
            if(gorand)
            {
                colors.append(phong(A));
                colors.append(phong(B));
                colors.append(phong(C));
            }
            else
            {
                My3Dpoint center = My3Dpoint::findTriangleCenter(A,B,C);
                colors.append(phong(center));
            }


            My3Dpoint D,dx;
            double s;
            D.setY(A.getY());
            dx = My3Dpoint(C.getX() - B.getX(), C.getY() - B.getY(), C.getZ() - B.getZ());
            s = (D.getY() - B.getY()) / dx.getY();
            D.setX(B.getX() + (dx.getX() * s));
            D.setZ(B.getZ() + (dx.getZ() * s));

                int ymin1 = img->height(), ymax1 = 0;
                    QList<My3Dpoint> points1, points2;
                    points1.append(A);
                    points1.append(D);
                    points1.append(C);

                    points2.append(A);
                    points2.append(B);
                    points2.append(D);
                    for (int i=0;i<points1.length();i++)
                    {
                        if (points1[i].getY() < ymin1)
                            ymin1 = points1[i].getY();
                        if (points1[i].getY() > ymax1)
                            ymax1 = points1[i].getY();
                    }

                    int ymin2 = img->height(), ymax2 = 0;
                    for (int i=0;i<points2.length();i++)
                    {
                        if (points2[i].getY() < ymin2)
                            ymin2 = points2[i].getY();
                        if (points2[i].getY() > ymax2)
                            ymax2 = points2[i].getY();
                    }

                    for (int ya = ymin1; ya < ymax1; ya++)
                        {
                            QList<My3Dpoint> hits;
                            //vypocet priesecnika pre hranu AC
                            double xa=createXa(A,C,ya);
                            double z=createZ(A,C,xa,ya);
                            My3Dpoint toApp((int)xa, ya,z);
                            hits.append(toApp);

                            //vypocet priesecnika pre hranu CD
                            xa=createXa(C,D,ya);
                            z=createZ(C,D,xa,ya);
                            My3Dpoint toApp2((int)xa, ya,z);
                            hits.append(toApp2);

                            hits = sortByX(hits);
                            if(gorand)
                                drawLineBarycentric(A,B,C,hits[0],hits[1],colors);
                            else
                                drawLineZbuffer(hits[0],hits[1],colors[0]);

                        }

                        for (int ya = ymin2; ya < ymax2; ya++)
                        {
                            QList<My3Dpoint> hits;
                            //vypocet priesecnika pre hranu BA
                            double xa=createXa(B,A,ya);
                            double z=createZ(B,A,xa,ya);
                            My3Dpoint toApp((int)xa, ya,z);
                             hits.append(toApp);

                             //vypocet priesecnika pre hranu BD
                             xa=createXa(B,D,ya);
                             z=createZ(B,D,xa,ya);
                             My3Dpoint toApp2((int)xa, ya,z);
                             hits.append(toApp2);

                              hits = sortByX(hits);
                              if(gorand)
                                  drawLineBarycentric(A,B,C,hits[0],hits[1],colors);
                              else
                                  drawLineZbuffer(hits[0],hits[1],colors[0]);
                        }
        }
        }



}

void ViewerWidget::drawLineBarycentric(My3Dpoint AA, My3Dpoint BB, My3Dpoint CC, My3Dpoint A, My3Dpoint B, QList<QColor> color)
{
    float dx = B.getX() - A.getX();
    float dy = B.getY() - A.getY();
    float steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float Xinc = dx / (float)steps;
    float Yinc = dy / (float)steps;

    float X = A.getX();
    float Y = A.getY();

    int i = 0;
    double volumeAll = volume(AA, BB, CC);
    while (i <= steps)
    {
       //priblizny vypocet z-ovej suradnice bodu
       double z=createZ(A,B,X,Y);
       My3Dpoint P((int)X, (int)Y,z);
       //kontrola ci som v obrazku
       if(X>=img->width()||Y>=img->height()||X<0||Y<0)
           continue;
       int r,g,b;
        {
           //vypocet podielov jednotlivych zloziek
           double lbA = (volume(P,BB,CC))/ volumeAll;
           double lbB = (volume(P,AA,CC))/ volumeAll;
           double lbC = 1-lbA -lbB;

           //vytvorenie finalnej farby bodu
           r = color[0].red()*lbA+color[1].red()*lbB+color[2].red()*lbC;
           g = color[0].green() * lbA + color[1].green() * lbB + color[2].green() * lbC;
           b = color[0].blue() * lbA + color[1].blue() * lbB + color[2].blue() * lbC;
       }
        QColor colorToDraw(r, g, b);

       //kontrola z-ovej hodnoty so Z-bufferom
            if(zBuffer[(int)X][(int)Y]>z)
            {

               zBuffer[(int)X][(int)Y]=z;
              // qDebug()<<"VNUTORNY BOD"<<X<<Y<<z;
               img->setPixel((int)X, (int)Y,colorToDraw.rgb());
            }

            X += Xinc;
            Y += Yinc;
            i++;
    }
}

double ViewerWidget::volume(My3Dpoint A, My3Dpoint B, My3Dpoint C)
{
   My3Dpoint AB=A-B;
   My3Dpoint AC=A-C;
   My3Dpoint Z(AB.getY()*AC.getZ()-AB.getZ()*AC.getY(),
               AB.getZ()*AC.getX()-AB.getX()*AC.getZ(),
               AB.getX()*AC.getY()-AB.getY()*AC.getX());

    return 0.5 * Z.length();
}

double ViewerWidget::createZ(My3Dpoint A, My3Dpoint B, double x, double y)
{
    My3Dpoint dxx = My3Dpoint(B.getX() - A.getX(), B.getY() - A.getY(), B.getZ() - A.getZ());
    double s;
    if(dxx.getX()==0 && dxx.getY()==0)
    {
        s=0;
    }
    else if(dxx.getY()==0)
    {
        s = (x - A.getX()) / dxx.getX();
    }
    else
    {
      s = (y - A.getY()) / dxx.getY();
    }
    return (int)(A.getZ() + (dxx.getZ() * s));
}

double ViewerWidget::createXa(My3Dpoint A, My3Dpoint B, double ya)
{
    if(((double)B.getY() - (double)A.getY())==0)
        return A.getX();
    double w = ((double)B.getY() - (double)A.getY()) / ((double)B.getX() - (double)A.getX());
    w = 1.0 / w;
    return (w * ya) - (w * A.getY()) + (A.getX());
}
