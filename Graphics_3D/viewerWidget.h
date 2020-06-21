#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QRect>
#include <QPaintEvent>
#include "imageClasses.h"
#include <QDebug>
#include <QtMath>

class ViewerWidget : public QWidget
{
    Q_OBJECT
public:

    //konstruktor, destruktor
    explicit ViewerWidget(QString viewerName, QSize imgSize, QWidget* parent = Q_NULLPTR);
    ~ViewerWidget();

//nevyhnutne funkcie pre widget
    void   resizeWidget(QSize size);
    inline void setPainter() { painter = new QPainter(img); }

//funkcie na vykreslovanie
    //zakladne funkcie
    void renderImage();
    void clear();
    bool loadImage(QString &fileName);
    void projectPoints();
    void connectEdges();

    //funkcie sluziace na samotne kreslenie
    void drawLine(My3Dpoint A,My3Dpoint B,QColor color);
    void drawLineZbuffer(My3Dpoint A,My3Dpoint B,QColor color);
    void drawLineBarycentric(My3Dpoint AA, My3Dpoint BB, My3Dpoint CC, My3Dpoint A,
                             My3Dpoint B, QList<QColor> color);
    //transformacie
    void rotate(bool Xaxis);
    void translate(My3Dpoint imageCenter,My3Dpoint canvasCenter);
    void scale(bool scaleUp);

    //osvetlovacie funkcie
    void   shading(bool gorand);
    QColor phong(My3Dpoint point);

    //pomocne funkcie
    QList<My3Dpoint> sortByX(QList<My3Dpoint> points);
    My3Dpoint        findImageCenter(QList<My3Dpoint> points);
    My3Dpoint        findCanvasCenter();
    void             zBufferInit();
    void             zBufferClear();
    double           volume(My3Dpoint A,My3Dpoint B,My3Dpoint C);
    double           createZ(My3Dpoint A,My3Dpoint B,double x, double y);
    double           createXa(My3Dpoint A,My3Dpoint B,double ya);
//parametre užívatela
    QString typeOfRendering   = "wireframe";
    QString typeOfProjection  = "paralel";
    QString typeOfShading     = "gorand";

    double centerOfProjection = 500;
    double zenitValue         = 0;
    double azimutValue        = 0;
    double scaleCoeficient    = 1.5;
    double sharpnessOfLight   = 15;
    double rotationAngle      = 45;

    QColor colorOfEnviroment  = Qt::white;
    QColor colorOfSource1     = Qt::white;
    QColor colorOfSource2     = Qt::white;

    My3Dpoint lightSource1    = My3Dpoint(500,500,500);
    My3Dpoint lightSource2    = My3Dpoint(400,400,400);
    My3Dpoint cameraPosition  = My3Dpoint(100,100,100);

    My3Dpoint mirrorPart      = My3Dpoint(0.50,0.50,0.50);
    My3Dpoint difusionPart    = My3Dpoint(0.50,0.50,0.50);
    My3Dpoint ambientPart     = My3Dpoint(0.50,0.50,0.50);

    // dáta obrázku
    QList<My3Dpoint> renderedPoints;
    QList<Edge>      edges;
    QList<My3Dpoint> loadedPoints;
    QList<Triangle>  triangles;
    QList<QColor>    colors;
    My3Dpoint        imageCenter;

private:
    //nevyhnutné premenné pre widget
    QImage*   img     = nullptr;
    QPainter* painter = nullptr;
    int**     zBuffer = nullptr;

public slots:
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

};

#endif // VIEWERWIDGET_H
