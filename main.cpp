#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <string>
#include <QMainWindow>
#include "console_testing.h"
#include "SystemInfo.h"
#include <QLineEdit>
#include "kill_processes.h"
#include <QListWidget>
#include <QTimer>
#include "get_processes.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QMainWindow window;

   // main background window
    QWidget *central = new QWidget();
    central->setStyleSheet("background-color: #eaeaea;");
    window.setCentralWidget(central);
    SystemInfo info = SystemInfoProvider::getSystemInfo();

    //CPU NAME LABEL
    QLabel *labelCPU = new QLabel("CPU: " +QString::fromStdString(info.cpuName), central);
    labelCPU->setStyleSheet("font-weight: bold;");
    labelCPU->move(50, 50);

    //CORE NUMBERS
    QLabel *labelCores = new QLabel("Number of Cores: " +QString::number(info.logicalCores), central);
    labelCores->setStyleSheet("font-weight: bold;");
    labelCores->move(50, 70);

    //CPU architecture
    std::string CPUarhitecture;
    if (info.cpuArchitecture==0)
        CPUarhitecture = std::string("x86 (32-bit)");
    if (info.cpuArchitecture==5)
        CPUarhitecture = std::string("ARM");
    if (info.cpuArchitecture==9)
        CPUarhitecture = std::string("x64 (64-bit)");
    if (info.cpuArchitecture==12)
        CPUarhitecture = std::string("ARM64");
    QLabel *labelCPUarh = new QLabel("CPU architecture: " +QString::fromStdString(CPUarhitecture), central);
    labelCPUarh->setStyleSheet("font-weight: bold;");
    labelCPUarh->move(50, 90);

    //Ram memory
    QLabel *labelRamMem =new QLabel("RAM Memory: " +QString::number(info.ramMem)+" MB", central);
    labelRamMem->setStyleSheet("font-weight: bold;");
    labelRamMem->move(50, 110);

     //internal memory
    QLabel *labelInternalMem =new QLabel("Internal Memory: " +QString::number(info.diskMem)+" MB", central);
    labelInternalMem->setStyleSheet("font-weight: bold;");
    labelInternalMem->move(50, 130);

    // command killing processes
    QLineEdit *processInput = new QLineEdit(central);
    processInput->setPlaceholderText("Enter process name (ex: chrome.exe)");
    processInput->setGeometry(50, 160, 250, 30);
    QPushButton *killButton = new QPushButton("Kill Process", central);
    killButton->setGeometry(310, 160, 120, 30);
    QLabel *status = new QLabel("", central);
    status->setGeometry(50, 200, 400, 30);

    QObject::connect(killButton, &QPushButton::clicked, [=]() {

    std::string processName = processInput->text().toStdString();

    killProcess(processName);


});
    QLabel *labellist =new QLabel("Processes list: ", central);
    labellist->setStyleSheet("font-weight: bold;");
    labellist->move(50, 230);

    //processes wiever
    QListWidget *processList = new QListWidget(central);
    processList->setGeometry(50, 250, 400, 300);

    auto updateProcesses = [=]() {
        processList->clear();



        for (const auto &p : getProcesses1()) {
            processList->addItem(QString::fromStdString(p));
        }
    };

    QTimer *timer = new QTimer(central);
    QObject::connect(timer, &QTimer::timeout, updateProcesses);
    timer->start(1000);

    window.resize(800, 600);
    window.show();

    runConsoleTests();//console tests
    return QApplication::exec();
}