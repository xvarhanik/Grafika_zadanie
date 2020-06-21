#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "viewerWidget.h"
#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <qdebug.h>
#include <qcolordialog.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //Event filtre
    bool eventFilter(QObject* obj, QEvent* event);
    bool ViewerWidgetEventFilter(QObject* obj, QEvent* event);
    void mouseMoveEvent(QMouseEvent *event);

    ViewerWidget* getCurrentViewerWidget();

private:
    Ui::MainWindow *ui;
private slots:
    void on_pushButtonLoadImage_clicked();

    //škálovanie
    void on_pushButtonScaleUp_clicked();
    void on_pushButtonScaleDown_clicked();

    //rotacia
    void on_pushButtonRotateUp_clicked();
    void on_pushButtonRotateLeft_clicked();
    void on_pushButtonRotateRight_clicked();
    void on_pushButtonRotateDown_clicked();

//sloty na nastavenia od uzivatela
    //výber vykreslovacieho režimu pomocou radiobuttonov
    void on_radioButtonWireFrame_clicked();
    void on_radioButtonPhong_clicked();
    void on_radioButtonParalellProjection_clicked();
    void on_radioButtonCenterProjection_clicked();
    void on_radioButtonGorandShading_clicked();
    void on_radioButtonConstantShading_clicked();

    //nastavenia hodnôt pre premietanie
    void on_doubleSpinBoxCentralProjectionCenter_valueChanged();
    void on_doubleSpinBoxRotationAngle_valueChanged();
    void on_doubleSpinBoxScaleCoeficient_valueChanged();
    void on_horizontalSliderZenit_valueChanged();
    void on_horizontalSliderAzimut_valueChanged();
    //nastavenie polohy zdrojov svetla
    void on_spinBoxLightSource1_X_valueChanged();
    void on_spinBoxLightSource1_Y_valueChanged();
    void on_spinBoxLightSource1_Z_valueChanged();
    void on_spinBoxLightSource2_X_valueChanged();
    void on_spinBoxLightSource2_Y_valueChanged();
    void on_spinBoxLightSource2_Z_valueChanged();

    // nastavie koeficientov absorbcie svetla
    void on_doubleSpinBoxMirrorR_valueChanged();
    void on_doubleSpinBoxMirrorG_valueChanged();
    void on_doubleSpinBoxMirrorB_valueChanged();
    void on_doubleSpinBoxDifusionR_valueChanged();
    void on_doubleSpinBoxDifusionG_valueChanged();
    void on_doubleSpinBoxDifusionB_valueChanged();
    void on_doubleSpinBoxAmbientR_valueChanged();
    void on_doubleSpinBoxAmbientG_valueChanged();
    void on_doubleSpinBoxAmbientB_valueChanged();

    //nastavenie polohy kamery
    void on_spinBoxCameraPosition_X_valueChanged();
    void on_spinBoxCameraPosition_Y_valueChanged();
    void on_spinBoxCameraPosition_Z_valueChanged();

    //nastavenie farieb svetla
    void on_spinBoxSharpnessOfLight_valueChanged();
    void on_pushButtonSurroundingColor_clicked();
    void on_pushButtonLightSource1Color_clicked();
    void on_pushButtonLightSource2Color_clicked();

};
#endif // MAINWINDOW_H
