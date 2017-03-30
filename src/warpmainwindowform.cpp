/*
############################################################################
#
# This file is part of Warp2010, (C) Massimiliano Lambertini - 2009
# Contact: m.lambertini@gmail.com
#
# GNU General Public License Usage
# This file may be used under the terms of the GNU
# General Public License version 3.0 as published by the Free Software
# Foundation and appearing in the file LICENSE.GPL included in the
# packaging of this file.  Please review the following information to
# ensure the GNU General Public License version 3.0 requirements will be
# met: http://www.gnu.org/copyleft/gpl.html.
#
############################################################################
*/

#include "warpmainwindowform.h"
#include <QListWidget>
#include <QtSvg>
#include <math.h>
#include <QPen>
#include <QObject>
#include <stdlib.h>
#include <time.h>
#include "star.h"
#include "starwidgetitem.h"
#include "stargraphicsitem.h"
#include "dialogs/createtraderoutedialog.h"
#include "mainscenehandler.h"
#include "solarsystemcreator.h"
#include "dialogs/traderoutedialog.h"
#include "traderoutewidgetitem.h"
#include "warpexception.h"
#include "traderoutereportwriter.h"
#include "dialogs/newsectordialog.h"
#include "celestiaexporter.h"
#include "dialogs/preferencesdialog.h"
#include "cluster/cluster.h"
#include "cluster/clusteritem.h"
#include "diasporaclusterdialog.h"
#include "cluster/aspect.h"
#include "cluster/aspectlist.h"
#include "sceneimageexporter.h"
#include "appmessage.h"
#include "helpers/progressbarhelper.h"
#include "helpers/preferences.h"
#include "helpers/apppaths.h"
#include "dialogs/printingsetup.h"
#include "helpers/splashscreen.h"
#include "starlist.h"
#include "widgets/planetaryinspector.h"
#include "helpers/pixmaphelper.h"
#include "helpers/startodiasporasummary.h"
#include "dialogs/createnewstardlg.h"
#include "dialogs/createbetweenstarsdlg.h"
#include "helpers/parsecstarlisthelper.h"

WarpMainWindowForm::WarpMainWindowForm(QWidget *parent) :
        QMainWindow(parent),  ui(new Ui::WarpMainWindow)
{
    _currentStar = 0;
    ui->setupUi(this);
    scene = new QGraphicsScene;
    ui->graphicsView->setRenderHint(QPainter::Antialiasing,true);
    _createTradeRouteDlg = new CreateTradeRouteDialog();

    _starList = StarList::StarListPtr();

    _sceneMediator = new SceneMediator(scene);
    _sceneMediator->setDrawMode(SceneMediatorDrawMode::XY);
    _sceneMediator->setShowNeighborsRoute(false);
    _sceneMediator->setShowOptimalPath(true);
    _sceneMediator->setShowTradeRoute(true);
    _sceneMediator->setShowOnlyWorldOnRoutes(false);
    _sceneMediator->setStarList(_starList);
    _sceneMediator->setTradeRoute(_tradeRoutes);
    _sceneMediator->setShowHexMap(false);
    _sceneMediator->populateScene();

    _tradeRouteMediator = TradeRouteMediator::mediator();
    _tradeRouteMediator->setCreateTradeRouteDialog(_createTradeRouteDlg);
    _tradeRouteMediator->setStarList(_starList);

     QObject::connect(_tradeRouteMediator, SIGNAL(tradeRouteCreated(QString&)), this, SLOT(tradeRouteCreated(QString&)));
     QObject::connect(ui->solsysView ,SIGNAL(starNameChanged(QString&)), this, SLOT(on_star_name_changed(QString&)));

     QStringList colorNames = QColor::colorNames();
     for (int i = 0; i < colorNames.size(); ++i) {
         QColor color(colorNames[i]);
         ui->colorPicker->insertColor(color, colorNames[i]);
     }

     _bUpdateTradeRoute = true;
     this->setUnifiedTitleAndToolBarOnMac(false);

     pixmap.load(AppPaths::appDir()+"/pics/rex_orbiting_horza.jpg");

     AspectList aspects = AspectList::fromAppDir();
     ui->clusterCreator->setAspectList(aspects);
     ui->toolbarStarMap->setVisible(true);
     ui->toolbarSolarSystem->setVisible(false);
     ui->toolbarCluster->setVisible(false);

     progressBar = new QProgressBar(this);
     ui->statusbar->addPermanentWidget(progressBar,0);

     progressBar->setVisible(false);

     QString styleSheet = qApp->styleSheet();
     ui->clusterCreator->setStyleSheet(styleSheet);

     Preferences::loadPreferences();

     SplashScreen::screenPtr()->initialize();

     menuSectorOptions = new QMenu(this);
     menuSectorOptions->addAction(ui->action_map_Show_All_Stars);
     menuSectorOptions->addAction(ui->action_map_Show_Reachable_Stars);
     menuSectorOptions->addAction(ui->action_map_Show_Stars_on_Trade_Routes_only);
     menuSectorOptions->addSeparator();
     menuSectorOptions->addAction(ui->action_map_Show_Hexmap_With_Reachable_Stars);
     menuSectorOptions->addAction(ui->action_map_Show_Hexmap_with_Trade_Route_Only);
     menuSectorOptions->addSeparator();
     menuSectorOptions->addAction(ui->action_map_Show_Metro_Map_With_Reachable_Stars);
     menuSectorOptions->addAction(ui->action_map_Show_Metro_Map_With_Trade_Routes);

     ui->action_map_Sector_Map_Options->setMenu(menuSectorOptions);


     agMapActions = new QActionGroup(this);
     agMapActions->addAction(ui->action_map_Show_All_Stars);
     agMapActions->addAction(ui->action_map_Show_Reachable_Stars);
     agMapActions->addAction(ui->action_map_Show_Stars_on_Trade_Routes_only);
     agMapActions->addAction(ui->action_map_Show_Hexmap_With_Reachable_Stars);
     agMapActions->addAction(ui->action_map_Show_Hexmap_with_Trade_Route_Only);
     agMapActions->addAction(ui->action_map_Show_Metro_Map_With_Reachable_Stars);
     agMapActions->addAction(ui->action_map_Show_Metro_Map_With_Trade_Routes);

     ui->action_map_Show_All_Stars->setCheckable(true);
     ui->action_map_Show_Hexmap_With_Reachable_Stars->setCheckable(true);
     ui->action_map_Show_Hexmap_with_Trade_Route_Only->setCheckable(true);
     ui->action_map_Show_Reachable_Stars->setCheckable(true);
     ui->action_map_Show_Stars_on_Trade_Routes_only->setCheckable(true);
     ui->action_map_Show_Metro_Map_With_Reachable_Stars->setCheckable(true);
     ui->action_map_Show_Metro_Map_With_Trade_Routes->setCheckable(true);

     ui->action_map_Show_All_Stars->setChecked(true);
     agMapActions->setExclusive(true);

     menuProjections = new QMenu(this);
     menuProjections->addAction(ui->action_Set_XY_Mode);
     menuProjections->addAction(ui->action_Set_XZ_Mode);
     menuProjections->addAction(ui->action_Set_YZ_Mode);
     ui->action_Planar_Projection_Modes->setMenu(menuProjections);

     agViews = new QActionGroup(this);
     agViews->addAction(ui->action_Star_Sector_View);
     agViews->addAction(ui->action_Solar_System_View);
     agViews->addAction(ui->action_Diaspora_Cluster_View);
     ui->action_Star_Sector_View->setChecked(true);

     agDrawMode = new QActionGroup(this);
     agDrawMode->addAction(ui->action_Set_XY_Mode);
     agDrawMode->addAction(ui->action_Set_XZ_Mode);
     agDrawMode->addAction(ui->action_Set_YZ_Mode);
     ui->action_Set_XY_Mode->setChecked(true);
     agDrawMode->setExclusive(true);
     
     connect  (agMapActions,SIGNAL(triggered(QAction*)),this,SLOT(agMapActions_triggered(QAction*)));
     connect (ui->action_Export_Solar_System_To_PDF,SIGNAL(triggered()), this, SLOT(on_action_solsys_export_to_pdf()));
     connect (ui->action_Export_Solar_System_To_SVG,SIGNAL(triggered()), this, SLOT(on_action_solsys_export_to_svg()));
     connect (ui->action_Print_Solar_System,SIGNAL(triggered()), this, SLOT(on_action_solsys_print()));

     connect (ui->btnZoomUp, SIGNAL(clicked()),this,SLOT(on_btnZoomUp_clicked()));
     connect (ui->btnZoomDown, SIGNAL(clicked()),this,SLOT(on_btnZoomDown_clicked()));

     connect(ui->action_Set_XY_Mode,SIGNAL(triggered()),this, SLOT(on_action_map_set_xy_mode_triggered()));
     connect(ui->action_Set_XZ_Mode,SIGNAL(triggered()),this, SLOT(on_action_map_set_xz_mode_triggered()));
     connect(ui->action_Set_YZ_Mode,SIGNAL(triggered()),this, SLOT(on_action_map_set_yz_mode_triggered()));

     connect(ui->solsysView,SIGNAL(neighborSelected(int)),this,SLOT(on_neighbor_selected(int)));

     PixmapHelper::instancePtr()->initializeBitmaps();
 }

WarpMainWindowForm::~WarpMainWindowForm()
{
    delete ui;
    //delete _tradeRouteMediator;
    delete _createTradeRouteDlg;
    delete _sceneMediator;
    delete scene;

    TradeRoute *tr;
    foreach (tr, _tradeRoutes)
        delete tr;

    delete agMapActions;
    //// qDebug() << "Deleting window";
}


void WarpMainWindowForm::on_listWidget_itemSelectionChanged()
{
    this->on_listWidget_itemClicked(ui->listWidget->currentItem());
}

void WarpMainWindowForm::on_listWidget_itemDoubleClicked(QListWidgetItem* item)
{
    StarWidgetItem *swi = (StarWidgetItem *)item;
    int idx = swi->indexOnList();
    this->_currentStarListIndex = idx;

    splash_showMessage("Recentering on "+swi->star()->starName);
    ui->listWidget->clear();

    this->rebuildMatrix(idx, this->distance());
    fillListWithCalculatedData(idx);


}

void WarpMainWindowForm::on_listWidget_itemClicked(QListWidgetItem* item)
{
    QTableWidgetItem *w;
    StarWidgetItem* swi = (StarWidgetItem *)item;
    QTableWidget *qwi = ui->routeTable;
    Star *star = swi->star();
    _currentStar = star;

    // qDebug() << "clicked item " << star->starName;

    if (_currentStar != 0) {
        //QString sHtml = _currentStar->toHtml();
        ui->solsysView->setStar(_currentStar);
    }
    else {
        ui->solsysView->setStar(0);
    }

    // qDebug() << "set solar system";


    QString sText = "Route to <b>" + star->starName + "</b>";
    ui->lblRouteTo->setText(sText);
    //ui->dockWidgetGrid->setWindowTitle(sText);
    //this->setWindowTitle(star->starName);
    int nRows = star->path().count();

    // qDebug() << "setting table";


    qwi->clear();
    qwi->setRowCount(nRows);
    qwi->setColumnCount(2);

    QStringList *headers = new QStringList();
    *headers << "Star" << "Distance";

    // qDebug() << "table set";


    qwi->setHorizontalHeaderLabels(*headers);
    Star *sRef = _starList->stars().at(0);

    ParsecStarListHelper pslh;
    pslh.setDrawMode(_sceneMediator->drawMode());


    QPointF ptFrom =  QPointF(pslh.starX(sRef), pslh.starY(sRef));

    // qDebug() << "setting rows...";


    for (int i = 0; i < nRows; i++)
    {

        // qDebug() << "setting row..." << i;


        int idx = star->path().at(i);
        sRef = _starList->stars().at(idx);
        w = new QTableWidgetItem(sRef->starName,0);
        QSize cellSize = w->sizeHint();
        cellSize.setHeight(cellSize.height()-4);
        w->setSizeHint(cellSize);
        qwi->setItem(i,0,w);

        double distance = 0.0;
        if (i != 0) {
            //Coordinates cTo = star->coordinates();
            distance = _starList->calculatePathDistance(idx);
        }
        QString sDist = QString().sprintf("%4.2f",distance);
        w = new QTableWidgetItem(sDist,0);
        cellSize = w->sizeHint();
        cellSize.setHeight(cellSize.height()-4);
        w->setSizeHint(cellSize);
        qwi->setItem(i,1,w);

    }


    // qDebug() << "centering" ;

    if (!_sceneMediator->showHexMap())
    {
        qDebug() << "centering on: " << pslh.starX( star)*_sceneMediator->sizeFactor() <<
                pslh.starY( star)*_sceneMediator->sizeFactor() << star->starName;
        ui->graphicsView->centerOn(
                pslh.starX( star)*_sceneMediator->sizeFactor(),
                pslh.starY( star)*_sceneMediator->sizeFactor());
    }
    else {
        ParsecStar ps= ParsecStar(_currentStar);
        QPointF pf = _sceneMediator->pointFromParsecStar(ps);
        ui->graphicsView->centerOn(pf.x(), pf.y());
    }
    delete headers;

    // qDebug() << "centered" ;


    //se scelgo una stella devo mettere currentTradeRoute = 0;
    _currentTradeRoute =0;
    _currentTwi = 0;   
    QVector<int> path = star->path();

    // qDebug() << "setting trade route info" ;

    if (path.count() > 1) {
        QString from = _starList->stars().at(path.first())->starName;
        QString to = _starList->stars().at(path.last())->starName;
        QString res = "from " + from + " to " + to;
        QColor color = QColor (rand() % 256,rand() % 256,rand() % 256);
        ui->txtRouteName->setText(res);
        ui->colorPicker->setCurrentColor(color);
        _currentStarListIndex = swi->indexOnList();
        _currentPath = swi->star()->path();
    }

    // qDebug() << "set trade route info" ;


}

void WarpMainWindowForm::fillListWithCalculatedData(int idx)
{
    bReloading = true;

    Star *star;

    //qDebug() << "Calling splash ";

    splash_showMessage( "Widgets cleared");

    //qDebug() << "Starting filling list";

    QPixmap ledRed (":/pics/ledred.png");
    QPixmap earth (":/pics/Planets/Terrestrial.png");
    QPixmap remote (":/pics/reachable_earth.png");
    QPixmap marginal (":/pics/unreachable_earth.png");
    QPixmap reference (":/pics/reference.png");
    QPixmap planetx (":/pics/cancel.png");


    QBrush br (QColor(255,255,255));
    for (int n = 0; n < _starList->stars().count(); n++)
    {
        bool bGotImage = false;
        star = _starList->stars().at(n);
        StarWidgetItem *swi = new StarWidgetItem
                              (star, n, ui->listWidget);
        if (star->path().count() > 0 && n != idx) {
            QIcon icon(ledRed);
            swi->setIcon(icon);
            bGotImage = true;
        }
        int numMarginals = star->starValue().numPreGarden+star->starValue().numPostGarden;
        int numGardens = star->starValue().numGlacier+star->starValue().numGarden;

        if (numMarginals > 0) {
            if (star->path().count() > 0 && n != idx)
            {
                QIcon icon(marginal);
                swi->setIcon(icon);
            }
            else
            {
                QIcon icon(remote);
                swi->setIcon(icon);
            }
            bGotImage = true;
        }

        if (numGardens > 0) {
            if (star->path().count() > 0 && n != idx)
            {
                QIcon icon(earth);
                swi->setIcon(icon);
            }
            else
            {
                QIcon icon(remote);
                swi->setIcon(icon);
            }
            bGotImage = true;
        }

        ui->listWidget->addItem(swi);
        if (n == idx) {
            QIcon icon(reference);
            swi->setIcon(icon);
            bGotImage = true;
        }
        if (!bGotImage) {
            QIcon icon(planetx);
            swi->setIcon(icon);
            bGotImage = true;
        }

    }
	
    //qDebug() << "End filling list";
	
    splash_showMessage( "Items filled");


    ui->listWidget->sortItems();
    splash_showMessage( "Items sorted");

    scene->clear();
    splash_showMessage( "Scene cleared");

    ui->sliderRotate->setValue(0);
    ui->sliderZoom->setValue(200);
    splash_showMessage( "Sliders set");



    populateScene();
    splash_showMessage( "Scene populated");

    ui->graphicsView->setScene(this->scene);
    //qDebug()  << "Redrawing Scene...";
    splash_showMessage( "Scene set");

    bReloading = false;
    WarpMainWindowForm::sliderValueChanged(ui->sliderZoom->value());
    //qDebug() << "Scene redrawn!";

}


void WarpMainWindowForm::performCreateSolSysForAllStars()
{


    if (_starList->stars().count() > 0) {
        int nStep = 0;
        int nMax = _starList->stars().count();

        progressBar->setMinimum(nStep);
        progressBar->setValue(nStep);
        progressBar->setMaximum(nMax);
        progressBar->setVisible(true);

        SolarSystemCreator creator;

        splash_showMessage( "Clearing list...");
        ui->listWidget->clear();

        Star *s;
        StarToDiasporaSummary::current()->clearSummaryMap();
        for (int nx = 0; nx < _starList->stars().count(); nx++)
        {
            s = _starList->stars()[nx];
            try {
                s->calcStarValue();
                creator.setStar(s);
                splash_showMessage( "Creating solsys for "+s->starName);
                creator.createSolarSystem();
                s->calcStarValue();

                progressBar->setValue(++nStep);
            }
            catch (WarpException exc) {
                QString errorText = "Error loading sector";
                QString errorInfo = QString(exc.what());
                AppMessage::Error(errorText,errorInfo);
            }
            StarToDiasporaSummary::current()->addToEnvironment(s->diasporaEnvironment(),nx);
            StarToDiasporaSummary::current()->addToResources(s->diasporaResources(),nx);

        }
        fillListWithCalculatedData(0);
        progressBar->setVisible(false);


    }
    else
        AppMessage::Error("<b>There are no stars</b>","You must create or load a star sector\nto create solar systems.");
    //qDebug() << "done with solsys";
}

void WarpMainWindowForm::performMapProcessing(bool bCreateNewMap, QString filename)
{
    try {

        SplashScreen::screenPtr()->show();

        ProgressBarHelper progHelp(this->progressBar);
        progHelp.reset(0,10);
        progHelp.show();
		
        SplashScreen::screenPtr()->setMessage("Clearing list...");
        ui->listWidget->clear();

        SplashScreen::screenPtr()->setMessage( "Clearing trade routes...");
        ui->gridTradeRoutes->clear();
		
        progHelp.nextStep(1);		
        _tradeRoutes.clear();
        SplashScreen::screenPtr()->setMessage("Trade routes are clear");

        progHelp.nextStep(2);
        _tradeRouteMediator->clearTradeRoutes();
        SplashScreen::screenPtr()->setMessage("Trade routes are clearer");

        progHelp.nextStep(3);
        _sceneMediator->setTradeRoute(_tradeRouteMediator->tradeRoutes());
        SplashScreen::screenPtr()->setMessage("Universe emptied, recreating...");


        progHelp.nextStep(4);
        _sceneMediator->clearTradeRoute();
		
        progHelp.nextStep(5);

        this->_referenceIdx= 0;
        SplashScreen::screenPtr()->setMessage("Loading Map");

		
        progHelp.nextStep(6);
        bool mustRebuildMatrix = true;
		
		
        if (!bCreateNewMap) {
            if (!filename.endsWith(".starx")) {
                this->_starList->loadMap(filename);
                SplashScreen::screenPtr()->setMessage("Now building...");
            }
            else {
                this->_starList->loadMatrix(filename);
                SplashScreen::screenPtr()->setMessage("Matrix loaded");
                mustRebuildMatrix = false;
            }
		
            SplashScreen::screenPtr()->setMessage( "Loaded map, rebuilding...");
        }
        else {
            int sectorRadius = _newSectorDialog->sectorRadius();
            QString sectorName = _newSectorDialog->sectorName();
            int sectorDensity = _newSectorDialog->sectorDensity();
            this->_starList->createRandomMap((double)sectorRadius, (double)sectorDensity);
            _starList->setListName(sectorName);
            SplashScreen::screenPtr()->setMessage( "Created map, rebuilding...");
        }

        if (mustRebuildMatrix) {
            progHelp.nextStep(7);
            this->rebuildMatrix(0, this->distance());
            SplashScreen::screenPtr()->setMessage( "Rebuilding paths...");
            progHelp.nextStep(8);
        }
        fillListWithCalculatedData(0);
        SplashScreen::screenPtr()->setMessage( "Paths ok!");
        progHelp.nextStep(9);
        ui->solsysView->setStar(0);
        progHelp.nextStep(10);

        this->setWindowTitle(_starList->listName());

        progHelp.hide();
    }
    catch (WarpException exc) {
        SplashScreen::screenPtr()->hide();
        QString errorText = "Error loading sector";
        QString errorInfo = QString(exc.what());
        AppMessage::Error(errorText,errorInfo);
        _sceneMediator->scene()->clear();
    }
}




void WarpMainWindowForm::on_btnRotateLeft_clicked()
{
    ui->sliderRotate->setValue(
            ui->sliderRotate->value() -
            ui->sliderRotate->pageStep());
}



void WarpMainWindowForm::populateScene()
{
    _sceneMediator->setTradeRoute(_tradeRouteMediator->tradeRoutes());
    _sceneMediator->populateScene();
}

void WarpMainWindowForm::on_sliderRotate_valueChanged(int value)
{
    sliderValueChanged(value);
}

void WarpMainWindowForm::sliderValueChanged(int value)
{
    if (!bReloading) {
        qreal scale = 0.7*qPow(qreal(2), (ui->sliderZoom->value() - 250) / qreal(50));

        QTransform transform;
        transform.scale(scale,scale);
        transform.rotate(ui->sliderRotate->value());
        //QMatrix matrix;
        //matrix.scale(scale, scale);
        //matrix.rotate(ui->sliderRotate->value());

        ui->graphicsView->setTransform(transform);
    }
}

void WarpMainWindowForm::on_btnZoomDown_clicked()
{
    this->on_actionZoomOut_triggered();

}

void WarpMainWindowForm::on_btnZoomUp_clicked()
{
    this->on_actionZoomIn_triggered();
}

void WarpMainWindowForm::on_btnRotateRight_clicked()
{
    ui->sliderRotate->setValue(
            ui->sliderRotate->value() +
            ui->sliderRotate->pageStep());
}


void WarpMainWindowForm::AddToTradeRoute (QString & tradeRouteName, QColor & bgColor, QVector<int> & path, int &indexOnList)
{
    if (_starList->count() > 0) {
        _tradeRouteMediator->setTableWidget(ui->gridTradeRoutes);
        TradeRoute *tr = _tradeRouteMediator->performAddToRouteList(
                tradeRouteName,
                bgColor,
                path);
        //_tradeRouteMediator->addTradeRoute(tr);
        _tradeRouteMediator->performAddToTradeRoute(tr,indexOnList);
        _sceneMediator->setTradeRoute(_tradeRouteMediator->tradeRoutes());
        _sceneMediator->redrawScene();
        return;
    }
    else {
        this->messageCannotCreateTradeRoute();
    }
}


void WarpMainWindowForm::on_gridTradeRoutes_itemClicked(QListWidgetItem* item)
{
    if (_bUpdateTradeRoute) {
        TradeRouteWidgetItem *twi = (TradeRouteWidgetItem *)item;
        ui->webTradeRouteView->setHtml(_tradeRouteMediator->tradeRouteToHTML(twi->tradeRoute()));

        double dDistance = _tradeRouteMediator->tradeRouteDistance(twi->tradeRoute());
        QString sDistance; sDistance.sprintf("<b><big>%4.2f</big></b> ly", dDistance);
        ui->lblRouteDistance->setText(sDistance);

        ui->colorPicker->setCurrentColor(twi->tradeRoute()->routeColor());
        ui->txtRouteName->setText(twi->tradeRoute()->routeName());
        _currentTradeRoute = twi->tradeRoute();
        _currentTwi = twi;
    }

}

void WarpMainWindowForm::on_gridTradeRoutes_itemChanged(QListWidgetItem* item)
{
    if (_bUpdateTradeRoute)
        this->on_gridTradeRoutes_itemClicked(item);
}

void WarpMainWindowForm::on_gridTradeRoutes_itemSelectionChanged()
{
    if (_bUpdateTradeRoute)
        this->on_gridTradeRoutes_itemClicked(ui->gridTradeRoutes->currentItem());
}

void WarpMainWindowForm::on_btnConfirmRoute_clicked()
{
    if (_starList->count() == 0) {
        this->messageCannotCreateTradeRoute();
        return;
    }
    else {
        QString starName = ui->txtRouteName->text();
        QColor starColor = ui->colorPicker->currentColor();
        if (_currentTradeRoute == 0)
        {
            this->AddToTradeRoute(
                    starName,
                    starColor,
                    _currentPath,
                    _currentStarListIndex);
        }
        else
        {
            _currentTwi->setText(starName);
            _currentTwi->tradeRoute()->setRouteColor(starColor);
            _currentTwi->tradeRoute()->setRouteName(starName);
        }
        _sceneMediator->redrawScene();
    }
}

void WarpMainWindowForm::on_sliderZoom_valueChanged(int value)
{
    this->sliderValueChanged(value);
}

void WarpMainWindowForm::on_btnDeleteRoute_clicked()
{
    QListWidgetItem *lwi = ui->gridTradeRoutes->currentItem();
    if (lwi != 0) {
        if (AppMessage::Question(tr("Do you want to remove the route?"),""))
        {
            TradeRouteWidgetItem  *twi = (TradeRouteWidgetItem *)lwi;
            TradeRouteWidgetItem  *twi2 = twi;
            //int idxToRemove = twi->indexOnList();
            int rowToRemove = ui->gridTradeRoutes->currentRow();
            int nList = ui->gridTradeRoutes->count();
            if (nList > 1) {
                ui->gridTradeRoutes->takeItem(rowToRemove);
                //_tradeRouteMediator->removeRoute(idxToRemove);
                _tradeRouteMediator->tradeRoutes().clear();
                for (int x = 0; x <  ui->gridTradeRoutes->count(); x++)
                {
                    twi = (TradeRouteWidgetItem *)ui->gridTradeRoutes->item(x);
                    TradeRoute *tpr = twi->tradeRoute();
                    _tradeRouteMediator->tradeRoutes().append(tpr);
                }
                delete twi2;
                _tradeRoutes.clear();
                QPointer<TradeRoute> ptr;
                foreach (ptr, _tradeRouteMediator->tradeRoutes())
                    _tradeRoutes.append(ptr);
                _sceneMediator->setTradeRoute(_tradeRoutes);
            }
            else {
                ui->gridTradeRoutes->clear();
                _tradeRouteMediator->tradeRoutes().clear();
            }

            _sceneMediator->redrawScene();
        }
    }
}


void WarpMainWindowForm::tradeRouteCreated(QString& routeName) {
    ui->statusbar->showMessage(routeName);
    QApplication::processEvents();
}

void WarpMainWindowForm::on_btnSaveHtml_clicked()
{
    if (_currentStar == 0)
    {
        AppMessage::Error(tr("No star selected","Select a star and create a solar system\nbefore exporting its data to an HTML file"),"");
        return;
    }
    QString defaultFilename = _currentStar->starName+".html";
    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save star to Html file"),
                                         AppPaths::appDir()+"/"+ defaultFilename,
                                         tr("Html file (*.html)"));
    if (!filename.isEmpty()) {
        QString html = _currentStar->toHtml();
        QFile data(filename);
        if (data.open(QFile::WriteOnly | QFile::Truncate)) {
            QTextStream out(&data);
            out << html;
            out.flush();
        }
        data.close();
    }
}

void WarpMainWindowForm::on_btnSaveCelestia_clicked()
{
    if (_currentStar == 0)
    {
        AppMessage::Error("No star selected","Select a star and create a solar system\nbefore exporting its data to Celestia");
        return;
    }

    QString defaultFilename = AppPaths::appDir()+"/"+ _currentStar->starName+".ssc";
    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save star to Celestia Data"),
                                         defaultFilename,
                                         tr("Celestia star file (*.ssc)"));
    if (!filename.isEmpty()) {
        CelestiaExporter celestia(_currentStar);
        celestia.saveCelestiaDataToFile(filename);
    }

}

void WarpMainWindowForm::on_btnDeleteAllRoutes_clicked()
{
    if (AppMessage::Question(tr("Do you want to remove ALL trade routes?"),""))
    {

        _tradeRoutes.clear();
        _tradeRouteMediator->clearTradeRoutes();
        ui->gridTradeRoutes->clear();
        _sceneMediator->setTradeRoute(_tradeRoutes);
        _sceneMediator->redrawScene();


    }
}


void WarpMainWindowForm::on_tabSubprograms_currentChanged(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
    switch (index) {
    case TAB_STAR_SECTOR: //starmap tab
        ui->splitterStarInfo->setVisible(true);
        ui->toolbarStarMap->setVisible(true);
        ui->toolbarSolarSystem->setVisible(false);
        ui->toolbarCluster->setVisible(false);
        break;

    case TAB_SOLAR_SYSTEM: //solsys tab
        ui->splitterStarInfo->setVisible(true);
        ui->toolbarStarMap->setVisible(false);
        ui->toolbarSolarSystem->setVisible(true);
        ui->toolbarCluster->setVisible(false);
        break;

    case TAB_CLUSTER_MAP:
        ui->splitterStarInfo->setVisible(false);
        ui->toolbarStarMap->setVisible(false);
        ui->toolbarSolarSystem->setVisible(false);
        ui->toolbarCluster->setVisible(true);
        break;
    }
}

void WarpMainWindowForm::on_btnRotateLeft_triggered(QAction* )
{
    this->on_actionRotateLeft_triggered();
}


void WarpMainWindowForm::on_stackWidget_currentChanged(int index)
{
    on_tabSubprograms_currentChanged(index);
}

void WarpMainWindowForm::agMapActions_triggered(QAction *action) {
    action->setChecked(true);
}

void WarpMainWindowForm::on_star_name_changed(QString &newStarName) {
    //qDebug() << "Star name changed to " << newStarName;
    _currentStar->starName = newStarName;
    if (ui->listWidget->currentItem() != 0) {
        StarWidgetItem* swi = (StarWidgetItem *)ui->listWidget->currentItem();
        swi->setText(newStarName);
    }
}


void WarpMainWindowForm::clearSectorMap() {

}

void WarpMainWindowForm::clearDiasporaCluster() {

}

void WarpMainWindowForm::clearSolarSystem()
{
    ui->listWidget->clear();
    ui->gridTradeRoutes->clear();
    _tradeRoutes.clear();
    _tradeRouteMediator->clearTradeRoutes();
    _sceneMediator->setTradeRoute(_tradeRouteMediator->tradeRoutes());
    _sceneMediator->clearTradeRoute();
    _sceneMediator->scene()->clear();
    _starList->stars().clear();
    this->ui->graphicsView->show();
}


void WarpMainWindowForm::on_txtSectorName_returnPressed()
{
    QString listName = this->ui->txtSectorName->text();
    StarList::StarListPtr()->setListName(listName);
    this->setWindowTitle(StarList::StarListPtr()->listName() + " Sector");
}

void WarpMainWindowForm::on_neighbor_selected(int i) {
    QString name = StarList::StarListPtr()->stars().at(i)->starName;
    //qDebug() << "neigbor selected, its name is " << name;
    QList<QListWidgetItem *> lst =  ui->listWidget->findItems(name, Qt::MatchExactly);
    if (lst.count() > 0) {
        QListWidgetItem *item = lst.at(0);
        ui->listWidget->setCurrentItem(item);
    }
}


