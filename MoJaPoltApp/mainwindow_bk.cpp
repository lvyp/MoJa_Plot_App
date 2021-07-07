#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    int width = this->geometry().width();
    int height = this->geometry().height();
    this->setFixedSize(width,height); //设置窗体固定大小

    this->count = 0;

    //设置时间轴时间验证格式
    QDoubleValidator *pDfValidator = new QDoubleValidator(0, 600, 3, ui->lineEdit);
    pDfValidator->setNotation(QDoubleValidator::StandardNotation);
    QDoubleValidator *pDfValidator2 = new QDoubleValidator(0, 600, 3, ui->lineEdit_2);
    pDfValidator2->setNotation(QDoubleValidator::StandardNotation);
    QDoubleValidator *pDfValidator3 = new QDoubleValidator(0, 600, 3, ui->lineEdit_3);
    pDfValidator3->setNotation(QDoubleValidator::StandardNotation);
    ui->lineEdit->setValidator(pDfValidator);
    ui->lineEdit_2->setValidator(pDfValidator2);
    ui->lineEdit_3->setValidator(pDfValidator3);
}

MainWindow::~MainWindow()
{
    delete ui;
}


int MainWindow::TTSVedio(QString tts_str)
{
    this->count += 1;
    this->point_Index = this->count;
    char audioName[256] = {0};
    strcpy(audioName, QString::number(this->count).toStdString().data());
    strcat(audioName, ".mp3");


    //初始化python模块
    Py_SetPythonHome((const wchar_t *)(L"C:\\Users\\98375\\AppData\\Local\\Programs\\Python\\Python36-32"));
    Py_Initialize();
    if ( !Py_IsInitialized() )
    {
        return -1;
    }
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    qDebug() << QDir::currentPath();
    //导入tts.py模块
    PyObject* pModule = PyImport_ImportModule("main");
    if (!pModule) {
            printf("Cant open python file!\n");
            return -1;
        }
    //获取tts模块中的tts_generate函数
   PyObject* pFuntts= PyObject_GetAttrString(pModule,"ttsGenerate");

   if(!pFuntts){
       printf("Get function tts failed");
       return -1;
   }
   PyObject* args = PyTuple_New(2);
   PyObject* param1 = Py_BuildValue("s", tts_str.toStdString().data());
   PyObject* param2 = Py_BuildValue("s", audioName);
   PyTuple_SetItem(args, 0, param1);
   PyTuple_SetItem(args, 1, param2);
   //调用tts函数
   PyObject* pReturn = PyObject_CallObject(pFuntts, args);
   if (pReturn == NULL)
   {
     return -1;
   }
   //获取py返回值
   char* szBuffer[1000] = {0};
   PyArg_Parse(pReturn, "s", &szBuffer);
   //结束，释放python
   Py_Finalize();

   return 0;
}

void MainWindow::initLabel()
{
    ui->label_16->setText("无");
    ui->label_17->setText("无");
    ui->label_18->setText("无");
    ui->label_19->setText("无");
    ui->label_20->setText("无");
    ui->label_21->setText("无");
    ui->label_22->setText("无");
}

void MainWindow::on_commitButton_clicked()
{

    if (ui->lineEdit->text().isEmpty() && ui->lineEdit_2->text().isEmpty()
            && ui->lineEdit_3->text().isEmpty() && ui->lineEdit_4->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Waring"),
                                     tr("有未填项，请查看!"),
                                     QMessageBox::Yes);
    }
    else
    {
        if (ui->lineEdit->text().isEmpty())
        {
            ui->lineEdit->setText("0.000");
        }

        if (ui->lineEdit_2->text().isEmpty())
        {
            ui->lineEdit_2->setText("0.000");
        }

        if (ui->lineEdit_3->text().isEmpty())
        {
            ui->lineEdit_3->setText("0.000");
        }

        TTSVedio(ui->lineEdit_4->text());

        QString time_str = "\"time\":" + ui->lineEdit->text() + ",";
        QString sub_time_child_str = "\"sub_time_child\":" + ui->lineEdit_2->text() + ",";
        QString action_child_str = "\"action_child\":\"" + ui->comboBox->currentText() + "\",";
        QString sub_time_old_str = "\"sub_time_old\":" + ui->lineEdit_3->text() + ",";
        QString action_old_str = "\"action_old\":\"" + ui->comboBox_3->currentText() + "\",";
        QString dialogue_str = "\"dialogue\":\"" + ui->lineEdit_4->text() + "\",";
        QString current_human_str = "\"current_human\":\"" + ui->comboBox_4->currentText() + "\"";

        this->time_str = time_str;
        this->sub_time_child_str = sub_time_child_str;
        this->action_child_str = action_child_str;
        this->sub_time_old_str = sub_time_old_str;
        this->action_old_str = action_old_str;
        this->dialogue_str = dialogue_str;
        this->current_human_str = current_human_str;

        QString sum_str  = "{" + time_str + sub_time_child_str + action_child_str +
                sub_time_old_str + action_old_str + dialogue_str + current_human_str + "}";
        strList.push_back(sum_str);

        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->comboBox->setCurrentIndex(0);
        ui->lineEdit_3->clear();
        ui->comboBox_3->setCurrentIndex(0);
        ui->lineEdit_4->clear();
        ui->comboBox_4->setCurrentIndex(0);

        //显示数据
        ui->label_16->setText(this->time_str);
        ui->label_17->setText(this->sub_time_child_str);
        ui->label_18->setText(this->action_child_str);
        ui->label_19->setText(this->sub_time_old_str);
        ui->label_20->setText(this->action_old_str);
        ui->label_21->setText(this->dialogue_str);
        ui->label_22->setText(this->current_human_str);
    }
}

void MainWindow::on_saveButton_clicked()
{
    QString json_str = "";
    QString var;
    foreach (var, strList) {
        json_str  = json_str + var;
        json_str = json_str + ",";
    }

    if (json_str.length() > 0)
    {
            json_str  = json_str.left(json_str.length() - 1);
    }

    json_str = "{\"MoJa\":[" + json_str + "]}";

    QFile file;
    file.setFileName(QFileDialog::getSaveFileName(this, QString("保存路径"), QString("/"),QString("TEXT(*.txt)")));
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(json_str.toUtf8());
    file.close();
    strList.clear();
    this->count = 0;
    this->point_Index = this->count;
    initLabel();
}

QList<QString> MainWindow::parseString(QString str)
{
    QList<QString> tempList;
    str = str.replace("{", "").replace("}", "");
    tempList = str.split(",");
    return tempList;
}

void MainWindow::on_deleteButton_clicked()
{
    if (this->count > 0)
    {
        char audioName[256] = {"C:\\Users\\98375\\Desktop\\tts\\"};
        strcat(audioName, QString::number(this->count).toStdString().data());
        strcat(audioName, ".mp3");

        this->count -= 1;
        this->point_Index = this->count;

        strList.removeLast();

        //删除音频文件
        QFile::remove(audioName);

        if (this->count != 0)
        {
            QList<QString> tempList;
            tempList = parseString(strList[this->count - 1]);

            ui->label_16->setText(tempList[0]);
            ui->label_17->setText(tempList[1]);
            ui->label_18->setText(tempList[2]);
            ui->label_19->setText(tempList[3]);
            ui->label_20->setText(tempList[4]);
            ui->label_21->setText(tempList[5]);
            ui->label_22->setText(tempList[6]);
        }
        else
        {
            initLabel();
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Waring"),
                                     tr("没有数据，你删什么啊？！！"),
                                     QMessageBox::Yes);
    }

}

void MainWindow::on_upButton_clicked()
{

}

void MainWindow::on_downButton_clicked()
{

}
