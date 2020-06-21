#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ViewerWidget * vW = new ViewerWidget("polygon", ui->Canvas->size());
    ui->Canvas->setWidget(vW);
    ViewerWidget* w = getCurrentViewerWidget();
    //labely zobrazujuce farby sa nastavia na bielu
    ui->surroundingColorDisplay->setStyleSheet("QLabel { background-color :white;}");
    ui->Source1ColorDisplay->setStyleSheet("QLabel { background-color :white;}");
    ui->Source2ColorDisplay->setStyleSheet("QLabel { background-color :white;}");

    //na radiobuttonoch sa nastavia prednastavene vybery
    ui->radioButtonWireFrame->setChecked(true);
    ui->radioButtonGorandShading->setChecked(true);
    ui->radioButtonParalellProjection->setChecked(true);

    //na začiatku sa zneviditelni vyber stredu zobrazenia
    ui->labelCentralProjectionCenter->setVisible(false);
    ui->doubleSpinBoxCentralProjectionCenter->setVisible(false);
    this->installEventFilter(this);
    ui->Canvas->setMouseTracking(true);
    w->update();
}

ViewerWidget* MainWindow::getCurrentViewerWidget()
{
    auto vW = static_cast<ViewerWidget*>(ui->Canvas->widget());
    if (vW)
    {
        return vW;
    }
        return nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (obj->objectName() == "Canvas") {
        return ViewerWidgetEventFilter(obj, event);
    }
    return false;
}

bool MainWindow::ViewerWidgetEventFilter(QObject* obj, QEvent* event)
{
    ViewerWidget* w = static_cast<ViewerWidget*>(static_cast<QScrollArea*>(obj)->widget());

    if (!w) {
        return false;
    }
    return false;
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    ViewerWidget* w = getCurrentViewerWidget();
    if (event->buttons() == Qt::LeftButton )
    {
        QPoint point;

        point = mapFromGlobal(event->globalPos());

        My3Dpoint whereToTrans(point.x()-660,point.y()-20,0);

        w->translate(w->findImageCenter(w->renderedPoints),whereToTrans);
        w->renderImage();
    }
}

void MainWindow::on_pushButtonLoadImage_clicked()
{
    ViewerWidget* w = getCurrentViewerWidget();
        QString fileName = QFileDialog::getOpenFileName(this,
                           tr("Otvoriť súbor"), "/home", tr("Image Files (*.txt)"));
       bool imageIsLoaded= w->loadImage(fileName);
       if(!imageIsLoaded)
       {
           QMessageBox::warning(this, tr("Chyba načítania"),
                                tr("Nepodarilo sa načítať súbor.\n"),
                                QMessageBox::Ok);
       }
       else
       {
            w->renderImage();
       }


}

void MainWindow::on_radioButtonWireFrame_clicked()
{
     ViewerWidget* w = getCurrentViewerWidget();
     w->typeOfRendering="wireframe";
     w->renderImage();
}

void MainWindow::on_radioButtonPhong_clicked()
{
     ViewerWidget* w = getCurrentViewerWidget();
     w->typeOfRendering="phong";
     w->renderImage();
}

void MainWindow::on_radioButtonParalellProjection_clicked()
{
     ViewerWidget* w = getCurrentViewerWidget();

     ui->labelCentralProjectionCenter->setVisible(false);
     ui->doubleSpinBoxCentralProjectionCenter->setVisible(false);

     w->typeOfProjection="paralel";
     w->renderImage();
}

void MainWindow::on_radioButtonCenterProjection_clicked()
{
     ViewerWidget* w = getCurrentViewerWidget();
     //zobrazia sa dalsie nastavenia
     ui->labelCentralProjectionCenter->setVisible(true);
     ui->doubleSpinBoxCentralProjectionCenter->setVisible(true);

     w->typeOfProjection="center";
     w->renderImage();
}
void MainWindow::on_radioButtonGorandShading_clicked()
{
    ViewerWidget* w = getCurrentViewerWidget();

    ui->labelCentralProjectionCenter->setVisible(false);
    ui->doubleSpinBoxCentralProjectionCenter->setVisible(false);

    w->typeOfShading="gorand";
    w->renderImage();
}
void MainWindow::on_radioButtonConstantShading_clicked()
{
    ViewerWidget* w = getCurrentViewerWidget();

    ui->labelCentralProjectionCenter->setVisible(false);
    ui->doubleSpinBoxCentralProjectionCenter->setVisible(false);

    w->typeOfShading="constant";
    w->renderImage();
}

void MainWindow::on_doubleSpinBoxCentralProjectionCenter_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->centerOfProjection=ui->doubleSpinBoxCentralProjectionCenter->value();
}

void MainWindow:: on_doubleSpinBoxRotationAngle_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->rotationAngle=ui->doubleSpinBoxRotationAngle->value();
}
void MainWindow::on_doubleSpinBoxScaleCoeficient_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->scaleCoeficient=ui->doubleSpinBoxScaleCoeficient->value();
    w->renderImage();
}
void MainWindow:: on_spinBoxLightSource1_X_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->lightSource1.setX(ui->spinBoxLightSource1_X->value());
    w->renderImage();
}
void MainWindow:: on_spinBoxLightSource1_Y_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->lightSource1.setY(ui->spinBoxLightSource1_Y->value());
    w->renderImage();
}
void MainWindow:: on_spinBoxLightSource1_Z_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->lightSource1.setZ(ui->spinBoxLightSource1_Z->value());
    w->renderImage();
}
void MainWindow:: on_spinBoxLightSource2_X_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->lightSource2.setX(ui->spinBoxLightSource2_X->value());
    w->renderImage();
}
void MainWindow:: on_spinBoxLightSource2_Y_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->lightSource2.setY(ui->spinBoxLightSource2_Y->value());
    w->renderImage();
}
void MainWindow:: on_spinBoxLightSource2_Z_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->lightSource2.setZ(ui->spinBoxLightSource2_Z->value());
    w->renderImage();
}
void MainWindow:: on_doubleSpinBoxMirrorR_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->mirrorPart.setX(ui->doubleSpinBoxMirrorR->value());
    w->renderImage();
}
void MainWindow:: on_doubleSpinBoxMirrorG_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->mirrorPart.setY(ui->doubleSpinBoxMirrorG->value());
    w->renderImage();
}
void MainWindow:: on_doubleSpinBoxMirrorB_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->mirrorPart.setZ(ui->doubleSpinBoxMirrorB->value());
    w->renderImage();
}
void MainWindow:: on_doubleSpinBoxDifusionR_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->difusionPart.setX(ui->doubleSpinBoxDifusionR->value());
    w->renderImage();
}
void MainWindow:: on_doubleSpinBoxDifusionG_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->difusionPart.setY(ui->doubleSpinBoxDifusionG->value());
    w->renderImage();
}
void MainWindow:: on_doubleSpinBoxDifusionB_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->difusionPart.setZ(ui->doubleSpinBoxDifusionB->value());;
    w->renderImage();
}
void MainWindow:: on_doubleSpinBoxAmbientR_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->ambientPart.setX(ui->doubleSpinBoxAmbientR->value());
    w->renderImage();
}
void MainWindow:: on_doubleSpinBoxAmbientG_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->ambientPart.setY(ui->doubleSpinBoxAmbientG->value());
    w->renderImage();
}
void MainWindow:: on_doubleSpinBoxAmbientB_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->ambientPart.setZ(ui->doubleSpinBoxAmbientB->value());
    w->renderImage();
}
void MainWindow:: on_spinBoxCameraPosition_X_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->cameraPosition.setX(ui->spinBoxCameraPosition_X->value());
    w->renderImage();
}
void MainWindow:: on_spinBoxCameraPosition_Y_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->cameraPosition.setY(ui->spinBoxCameraPosition_Y->value());;
    w->renderImage();
}
void MainWindow:: on_spinBoxCameraPosition_Z_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->cameraPosition.setZ(ui->spinBoxCameraPosition_Z->value());
    w->renderImage();
}
void MainWindow:: on_spinBoxSharpnessOfLight_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->sharpnessOfLight=ui->spinBoxSharpnessOfLight->value();
    w->renderImage();
}
void MainWindow:: on_horizontalSliderZenit_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->zenitValue=ui->horizontalSliderZenit->value();
     w->renderImage();
}
void MainWindow:: on_horizontalSliderAzimut_valueChanged()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->azimutValue=ui->horizontalSliderAzimut->value();
    w->renderImage();
}



void MainWindow:: on_pushButtonSurroundingColor_clicked()
{
    ViewerWidget* w = getCurrentViewerWidget();
        QColor color = QColorDialog::getColor(Qt::white, this, "Vyber farbu");
        if (color.isValid()) {
            ui->surroundingColorDisplay->setStyleSheet("QLabel { background-color :"+ color.name()+";}");
            w->colorOfEnviroment = color.rgb();
        };
        w->renderImage();
}
void MainWindow:: on_pushButtonLightSource1Color_clicked()
{
    ViewerWidget* w = getCurrentViewerWidget();
        QColor color = QColorDialog::getColor(Qt::white, this, "Vyber farbu");
        if (color.isValid()) {
            ui->Source1ColorDisplay->setStyleSheet("QLabel { background-color :"+ color.name()+";}");
            w->colorOfSource1 = color.rgb();
        };
        w->renderImage();
}
void MainWindow:: on_pushButtonLightSource2Color_clicked()
{
    ViewerWidget* w = getCurrentViewerWidget();
        QColor color = QColorDialog::getColor(Qt::white, this, "Vyber farbu");
        if (color.isValid()) {
            ui->Source2ColorDisplay->setStyleSheet("QLabel { background-color :"+ color.name()+";}");
            w->colorOfSource2 = color.rgb();
        };
        w->renderImage();
}
void MainWindow::on_pushButtonScaleUp_clicked()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->scale(true);
}
void MainWindow::on_pushButtonScaleDown_clicked()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->scale(false);
}

void MainWindow::on_pushButtonRotateUp_clicked()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->rotate(true);
}
void MainWindow::on_pushButtonRotateLeft_clicked()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->rotate(false);
}
void MainWindow::on_pushButtonRotateRight_clicked()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->rotate(false);
}
void MainWindow::on_pushButtonRotateDown_clicked()
{
    ViewerWidget* w = getCurrentViewerWidget();
    w->rotate(true);
}
