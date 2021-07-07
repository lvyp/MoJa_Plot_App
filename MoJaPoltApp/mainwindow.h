#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "math.h"
#include <Python.h>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QFile>
#include <QCoreApplication>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QList<QString> strList;
    char* TTSVedio(QString tts_str, int human);
    QList<QString> parseString(QString str);
    void initLabel();

private slots:
    void on_commitButton_clicked();

    void on_saveButton_clicked();

    void on_deleteButton_clicked();

    void on_upButton_clicked();

    void on_downButton_clicked();

private:
    Ui::MainWindow *ui;
    int count;
    QString time_str;
    QString sub_time_child_str;
    QString action_child_str;
    QString sub_time_old_str;
    QString action_old_str;
    QString dialogue_str;
    QString current_human_str;
    QString dialogue_content_str;
    int point_Index;
    int delete_offset;
};

#endif // MAINWINDOW_H
