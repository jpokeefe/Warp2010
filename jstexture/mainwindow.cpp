#include "mainwindow.h"
#include <texturebuilder/texturebuilder.h>
#include <QFileDialog>
#include <QSplitter>
#include <QVBoxLayout>
#include <QTextStream>
#include <QMessageBox>
#include <apppaths.h>
#include <QStringList>
#include <QStringListModel>
#include <createmoduledescriptorjson.h>
#include <QtGradientEditor/qtgradientdialog.h>
#include <heightmapbuilderdialog.h>
#include <rendererdescdialog.h>

MainWindow::MainWindow(QWidget *parent) :
    imageLabel(new QLabel),
    scrollArea(new QScrollArea),
    QMainWindow(parent),
    _currentTextureFile("")
{
    this->createActions();
    this->createMenus();
    this->createWidgets();
    this->layoutWidgets();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createActions() {
    action_newTexture = new QAction(QIcon(":/icons/new2.png"),"Create new texture",this),
    action_Load_Texture = new QAction(QIcon(":/icons/open.png"),"Load Texture",this);
    action_Save_Texture = new QAction(QIcon(":/icons/save.png"),"Save Texture", this);
    actionSave_As = new QAction(QIcon(":/icons/save.png"),"Save as",this);
    action_Exit = new QAction(QIcon(":/icons/quit.png"),"Exit",this);


    connect(action_newTexture,SIGNAL(triggered(bool)),this, SLOT(on_action_new_texture_triggered()));
    connect(action_Load_Texture,SIGNAL(triggered(bool)),this, SLOT(on_action_Load_Texture_triggered()));
    connect(action_Save_Texture,SIGNAL(triggered(bool)),this, SLOT(on_action_Save_Texture_triggered()));
    connect(actionSave_As,SIGNAL(triggered(bool)),this, SLOT(on_actionSave_As_triggered()));
    connect(action_Exit, SIGNAL(triggered(bool)),this, SLOT(on_action_Exit_triggered()));

    action_Generate_Texture = new QAction(QIcon(":/icons/generate2.png"),"Generate Texture",this);
    action_CreateModuleDescJson = new QAction(QIcon(":/icons/module.png"),"Create Module descriptor",this);
    action_CreateHeightmapBuilder = new QAction(QIcon(":/icons/renderermap.png"),"Create Heightmap Builder",this);
    action_CreateRendererDesc = new QAction(QIcon(":/icons/renderer.png"),"Create renderer descriptor",this);

    connect(action_Generate_Texture,SIGNAL(triggered(bool)),this, SLOT(on_action_Generate_Texture_triggered()));
    connect(action_CreateModuleDescJson,SIGNAL(triggered(bool)),this, SLOT(on_action_CreateModuleDescJson()));
    connect(action_CreateHeightmapBuilder,SIGNAL(triggered(bool)),this, SLOT(on_action_CreateHeightmapBuilder()));
    connect(action_CreateRendererDesc,SIGNAL(triggered(bool)),this,SLOT(on_action_create_rend_desc()));
}

void MainWindow::createMenus() {
    mainToolBar = new QToolBar(this);
    mainToolBar->setIconSize(QSize(32,32));
    statusBar = new QStatusBar(this);
    mainToolBar->addAction(action_newTexture);
    mainToolBar->addAction(action_Load_Texture);
    mainToolBar->addAction(action_Save_Texture);
    mainToolBar->addAction(actionSave_As);
    mainToolBar->addSeparator();
    mainToolBar->addAction(action_Generate_Texture);
    mainToolBar->addAction(action_CreateModuleDescJson);
    mainToolBar->addAction(action_CreateHeightmapBuilder);
    mainToolBar->addAction(action_CreateRendererDesc);
    mainToolBar->addSeparator();
    mainToolBar->addAction(action_Exit);
}

void MainWindow::createWidgets() {

    //custom setup;
    tabWidget = new QTabWidget (this);
    centralWidget = new QWidget(this);
    _splitter = new QSplitter(this);
    _splitter1 = new QSplitter(this);
    this->plainTextEdit = new QPlainTextEdit();

    scrollArea = new QScrollArea(this);
    imageLabel = new QLabel(this);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    _tex = new TextureBuilderExplorer(this);


    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    this->plainTextEdit->setFont(fixedFont);
    this->_listFiles = new QListWidget(this);
    connect(this->_listFiles,SIGNAL(itemClicked(QListWidgetItem*)) ,this, SLOT(on_listFiles_clicked(QListWidgetItem*)));
}

void MainWindow::layoutWidgets() {
    this->setCentralWidget(tabWidget);
    this->addToolBar(Qt::TopToolBarArea, mainToolBar);
    this->setStatusBar(statusBar);

    _splitter1->addWidget(_tex);
    _splitter1->addWidget(this->plainTextEdit);
    _splitter->setOrientation(Qt::Horizontal);

    _splitter->addWidget(_listFiles);
    _splitter->addWidget(scrollArea);
    _splitter->setOrientation(Qt::Vertical);

    tabWidget->addTab(_splitter1,"JSON Editor");
    tabWidget->addTab(_splitter, "Generated images");
    this->setCentralWidget(tabWidget);
}

void MainWindow::on_action_Load_Texture_triggered()
{
    QString fileName =
                QFileDialog::getOpenFileName(this, tr("Open Texture Json File"),
                                             AppPaths::appDir(),
                               tr("JSON Texture File (*.texjson)"));
        if (!fileName.isEmpty() && !fileName.isNull())
        {
            _currentTextureFile = fileName;
            QFile f(fileName);
            f.open(QIODevice::ReadOnly);
            QString s;
            QTextStream s1(&f);
            s.append(s1.readAll());

            this->plainTextEdit->setPlainText(s);
            QByteArray jsonData(s.toStdString().c_str());
            QJsonParseError parserError;
            QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parserError);
            this->_tb.fromJson(doc.object());
            this->_tex->setTextureBuilder(&_tb);
        }

}

void MainWindow::on_action_Generate_Texture_triggered()
{
    try {
        this->on_action_Save_Texture_triggered();
        TextureBuilder tb;
        tb.buildTextureFromJson(_currentTextureFile);

        if (tb.generatedMaps().count()> 0) {
            QString imgFile = tb.generatedMaps().first();
            QPixmap pixmap(imgFile);
            this->imageLabel->setPixmap(pixmap);
            this->imageLabel->setGeometry(0,0,pixmap.width(),pixmap.height());
            _listFiles->clear();
            for (auto h = 0; h < tb.generatedMaps().count(); ++h) {
                _listFiles->addItem(tb.generatedMaps()[h]);
            }
            //QPixmap pixmap(imgFile);
            //this->imageLabel->setPixmap(pixmap);
            //this->imageLabel->setGeometry(0,0,pixmap.width(),pixmap.height());
        }
    }
    catch (noise::ExceptionInvalidParam &exc) {
        errorBox("Invalid parameter somewhere. Check zeros and negatives ");
    }
    catch (noise::ExceptionNoModule &exc) {
        errorBox("Unknown module referenced as source or control module ");
    }
    catch (noise::ExceptionUnknown &exc) {
        errorBox("Libnoise unknown error ");
    }
    catch (std::exception exc) {
        QString error = exc.what();
        errorBox(error);
    }
    catch (std::string exc) {
        QString error(exc.c_str());
        errorBox(error);
    }
    catch (QString exc) {
        QString error(exc);
        errorBox(error);
    }
    catch (...) {
        std::string err = "Undefined error";
        throw err;
    }
}

void MainWindow::errorBox(QString msg) {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText("<big>"+msg+"</big>");
    msgBox.setInformativeText(msg);
    msgBox.setStandardButtons(QMessageBox::Ok );
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();

}

int MainWindow::questionBox(QString msg) {
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    return ret;
}

void MainWindow::on_action_Save_Texture_triggered()
{
    QString fileName = _currentTextureFile;
    if (_currentTextureFile == "") {
        fileName = QFileDialog::getSaveFileName(this, tr("Save Texture Json File"),
                                                AppPaths::appDir() + "/texture1.texjson",tr("JSON Texture File (*.texjson)"));
        if (!fileName.endsWith(".texjson"))
            fileName += ".texjson";
        setCurrentTextureFile( fileName);
    }
    if (!fileName.isEmpty() && ! fileName.isNull()) {
        QFile f(fileName);
        f.open((QIODevice::ReadWrite | QIODevice::Truncate));
        QTextStream s1(&f);
        auto data = this->plainTextEdit->toPlainText();
        s1 <<  data;
        f.flush();
        f.close();
    }

}

void MainWindow::on_action_Exit_triggered()
{
}

void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Texture Json File"),
                                                    AppPaths::appDir()+ "/texture1.texjson",tr("JSON Texture File (*.texjson)"));
    if (!fileName.isEmpty() && ! fileName.isNull()) {
        if (!fileName.endsWith(".texjson"))
            fileName += ".texjson";
        QFile f(fileName);
        f.open((QIODevice::ReadWrite | QIODevice::Truncate));
        QTextStream s1(&f);
        auto data = this->plainTextEdit->toPlainText();
        s1 << data;
        f.flush();
        f.close();
        setCurrentTextureFile( fileName);
    }


}

void MainWindow::on_listFiles_clicked(QListWidgetItem *idx) {
    //_viewer->loadImage(imgFile);
    QString data = _listFiles->currentItem()->text();
    this->setWindowTitle(data);
    QPixmap pixmap(data);
    this->imageLabel->setPixmap(pixmap);
    this->imageLabel->setGeometry(0,0,pixmap.width(),pixmap.height());
}

void MainWindow::on_action_CreateModuleDescJson()
{
    CreateModuleDescriptorJson dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        QJsonObject o;
        dlg.module()->toJson(o);
        QJsonDocument doc(o);
        QString strJson(doc.toJson(QJsonDocument::Compact));
        strJson.replace("],","],\n");
        this->plainTextEdit->insertPlainText(strJson);
    }
}

void MainWindow::on_action_CreateHeightmapBuilder() {
    HeightMapBuilderDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        auto builder = dlg.builderWidget()->builder();
        QJsonObject o;
        builder->toJson(o);
        QJsonDocument doc(o);
        QString strJson(doc.toJson(QJsonDocument::Compact));
        this->plainTextEdit->insertPlainText(strJson);
    }
}

void MainWindow::on_action_new_texture_triggered()
{
    TextureBuilder tb;
    QJsonObject o;
    tb.toJson(o);
    QJsonDocument doc(o);
    QString strJson(doc.toJson());
    this->plainTextEdit->setPlainText(strJson);

}

void MainWindow::on_action_create_rend_desc() {
    RendererDescDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        QJsonObject oRenderer;
        dlg.rendererDescriptor().toJson(oRenderer);
        QJsonDocument doc(oRenderer);
        QString strJson(doc.toJson());
        this->plainTextEdit->insertPlainText(strJson);
    }
}
