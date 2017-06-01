#include "mainwindow.h"
#include <texturebuilder/texturebuilder.h>
#include <QFileDialog>
#include <QSplitter>
#include <QVBoxLayout>
#include <QTextStream>
#include <QMessageBox>
#include <apppaths.h>

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
    action_Load_Texture = new QAction("Load Texture",this);
    connect(action_Load_Texture,SIGNAL(triggered(bool)),this, SLOT(on_action_Load_Texture_triggered()));
    action_Generate_Texture = new QAction("Generate Texture",this);
    connect(action_Generate_Texture,SIGNAL(triggered(bool)),this, SLOT(on_action_Generate_Texture_triggered()));
    action_Save_Texture = new QAction("Save Texture", this);
    connect(action_Save_Texture,SIGNAL(triggered(bool)),this, SLOT(on_action_Save_Texture_triggered()));
    action_Exit = new QAction("Exit",this);
    connect(action_Exit, SIGNAL(triggered(bool)),this, SLOT(on_action_Exit_triggered()));
    actionSave_As = new QAction("Save as",this);
    connect(actionSave_As,SIGNAL(triggered(bool)),this, SLOT(on_actionSave_As_triggered()));
}

void MainWindow::createMenus() {
    mainToolBar = new QToolBar(this);
    statusBar = new QStatusBar(this);
    mainToolBar->addAction(action_Load_Texture);
    mainToolBar->addAction(action_Save_Texture);
    mainToolBar->addAction(actionSave_As);
    mainToolBar->addAction(action_Generate_Texture);
    mainToolBar->addAction(action_Exit);
}

void MainWindow::createWidgets() {

    //custom setup;
    centralWidget = new QWidget(this);
    //QSplitter *splitter = new QSplitter(this);
    this->plainTextEdit = new QPlainTextEdit(this);

    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    this->plainTextEdit->setFont(fixedFont);

}

void MainWindow::layoutWidgets() {
    this->setCentralWidget(centralWidget);
    this->addToolBar(Qt::TopToolBarArea, mainToolBar);
    this->setStatusBar(statusBar);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(plainTextEdit,1);
    this->centralWidget->setLayout(layout);
    //this->setLayout(layout);
    this->_viewer = new ViewingDialog(this);
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
            if (_viewer->isHidden()) {
                _viewer->show();
                _viewer->raise();
                _viewer->activateWindow();
            }
            _viewer->loadImage(imgFile);
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
