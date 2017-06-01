#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QPlainTextEdit>
#include <QGraphicsScene>
#include <QLabel>
#include <QScrollArea>
#include <viewingdialog.h>
#include <QAction>
#include <QWidget>
#include <QToolBar>
#include <QStatusBar>


class MainWindow : public QMainWindow
{
    Q_OBJECT

    QAction *action_Load_Texture;
    QAction *action_Generate_Texture;
    QAction *action_Save_Texture;
    QAction *action_Exit;
    QAction *actionSave_As;
    QWidget *centralWidget;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    QPlainTextEdit *plainTextEdit;
    QGraphicsScene *scene;
    QFrame *frame;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    QString _currentTextureFile ;
    ViewingDialog *_viewer;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void errorBox(QString msg);
    int questionBox(QString msg);
    void setCurrentTextureFile(const QString& _file) { _currentTextureFile = _file; this->setWindowTitle(_file); }

private slots:
    void on_action_Load_Texture_triggered();
    void on_action_Generate_Texture_triggered();
    void on_action_Save_Texture_triggered();
    void on_action_Exit_triggered();
    void on_actionSave_As_triggered();

private:
    void createActions();
    void createMenus();
    void createWidgets();
    void layoutWidgets();
};

#endif // MAINWINDOW_H
