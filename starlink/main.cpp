#include "mainwindow.h"
#include <QApplication>
#include "adminlogindialog.h"
#include "adminmain.h"
#include "checkdialog.h"
#include "checklist.h"
#include "debuginfo.h"
#include "entrydialog.h"
#include "reportdialog.h"
#include "user.h"
#include "userlogindialog.h"
#include "usermain.h"

#include <iostream>

void populateCheckLists(CheckList &dieselOffCheckList, CheckList &dieselOnChecklist, CheckList &motorOffCheckList, CheckList &motorOnCheckList);

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    MainWindow mainWindow;
    AdminLoginDialog adminLoginDialog;
    AdminMain adminMain;
    CheckDialog checkDialog;
    EntryDialog entryDialog;
    UserLoginDialog userLoginDialog;
    UserMain userMain;
    User user;
    CheckList dieselOffCheckList;
    CheckList dieselOnCheckList;
    CheckList motorOffCheckList;
    CheckList motorOnCheckList;
    ReportDialog reportDialog;

    std::cout << "main(int, char*[]) Line 1" << std::endl;
    adminLoginDialog.registerAdminMainDialog(&adminMain);
    std::cout << "main(int, char*[]) Line 2" << std::endl;
    adminMain.registerDialogs(&reportDialog);
    std::cout << "main(int, char*[]) Line 3" << std::endl;
    userLoginDialog.registerUserMainDialog(&userMain);
    std::cout << "main(int, char*[]) Line 4" << std::endl;
    userMain.registerDialogs(&checkDialog, &reportDialog);
    std::cout << "main(int, char*[]) Line 5" << std::endl;
    checkDialog.registerUser(&user);
    //mainWindow.show();
    std::cout << "main(int, char*[]) Line 6" << std::endl;
    populateCheckLists(dieselOffCheckList, dieselOnCheckList, motorOffCheckList, motorOnCheckList);
    std::cout << "main(int, char*[]) Line 7" << std::endl;
    checkDialog.registerCheckList(&motorOffCheckList, &motorOnCheckList);
    std::cout << "main(int, char*[]) Line 8" << std::endl;
    entryDialog.setModal(true);
    std::cout << "main(int, char*[]) Line 9" << std::endl;
    entryDialog.registerLoginDialog(&adminLoginDialog, &userLoginDialog, &user);
    std::cout << "main(int, char*[]) Line 10" << std::endl;
    entryDialog.setWindowFlags(Qt::Window);
    std::cout << "main(int, char*[]) Line 11" << std::endl;
#ifdef WINDOWS_IMPLEMENTATION
    entryDialog.show();
#else
    entryDialog.showFullScreen();
#endif
    entryDialog.exec();

    return application.exec();
}

void populateCheckLists(CheckList &dieselOffCheckList, CheckList &dieselOnCheckList, CheckList &motorOffCheckList, CheckList &motorOnCheckList)
{
    dieselOffCheckList.addListItem("Leaks: fuel, hydraulic oil, engine oil, or radiator coolant");
    dieselOffCheckList.addListItem("Tires: condition, pressure");
    dieselOffCheckList.addListItem("Forks, Top Clip Retaining Pin and Heel: condition");
    dieselOffCheckList.addListItem("Load backrest: securely attached");
    dieselOffCheckList.addListItem("Hydraulic hoses, mast chains, cables and stops: check visually");
    dieselOffCheckList.addListItem("Overhead guard: attached");
    dieselOffCheckList.addListItem("Finger guards: attached");
    dieselOffCheckList.addListItem("Propane tank (LP gas truck): rust corrosion, damage");
    dieselOffCheckList.addListItem("Safety warnings: attached");
    dieselOffCheckList.addListItem("Battery: check water/electrolyte level and charge");
    dieselOffCheckList.addListItem("All engine belts: check visually");
    dieselOffCheckList.addListItem("Hydraulic fluid level: check level");
    dieselOffCheckList.addListItem("Engine oil level: dipstick");
    dieselOffCheckList.addListItem("Transmission fluid level: dipstick");
    dieselOffCheckList.addListItem("Engine air cleaner: squeeze rubber dirt trap or check the restriction alarm");
    dieselOffCheckList.addListItem("Fuel sedimentor (diesel)");
    dieselOffCheckList.addListItem("Radiator coolant: check level");
    dieselOffCheckList.addListItem("Operator's manual: in container");
    dieselOffCheckList.addListItem("Nameplate: attached and information matches model, serial number and attachments");
    dieselOffCheckList.addListItem("Seat belt: functioning smoothly");
    dieselOffCheckList.addListItem("Hood latch: adjusted and securely fastened");
    dieselOffCheckList.addListItem("Brake fluid: check level");

    dieselOnCheckList.addListItem("Warning: Unusual noises must be investigated immediately");
    dieselOnCheckList.addListItem("Accelerator or Direction Control Pedal - functioning smoothly");
    dieselOnCheckList.addListItem("Service Brake - functioning smoothly");
    dieselOnCheckList.addListItem("Parking brake: functioning smoothly");
    dieselOnCheckList.addListItem("Steering operation: functioning smoothly");
    dieselOnCheckList.addListItem("Drive control: forward/reverse, functioning smoothly");
    dieselOnCheckList.addListItem("Tilt control: forward and back, functioning smoothly");
    dieselOnCheckList.addListItem("Hoist and lowering control: functioning smoothly");
    dieselOnCheckList.addListItem("Attachment control: operation");
    dieselOnCheckList.addListItem("Horn and lights: functioning");
    dieselOnCheckList.addListItem("Cab: heater, defroster, wipers, functioning");
    dieselOnCheckList.addListItem("Gauges: ammeter, engine oil pressure, hour meter, fuel level, temperature, instrument monitors, functioning");

    motorOffCheckList.addListItem("Leaks: hydraulic oil, battery");
    motorOffCheckList.addListItem("Tires: condition and pressure");
    motorOffCheckList.addListItem("Forks, top clip retaining pin and heel: condition");
    motorOffCheckList.addListItem("Load backrest extension: attached");
    motorOffCheckList.addListItem("Hydraulic hoses, mast chains, cables and stops: check visually");
    motorOffCheckList.addListItem("Finger guards: attached");
    motorOffCheckList.addListItem("Overhead guard: attached");
    motorOffCheckList.addListItem("Safety warnings: attached");
    motorOffCheckList.addListItem("Battery: water/electrolyte level and charge");
    motorOffCheckList.addListItem("Hydraulic fluid level: dipstick");
    motorOffCheckList.addListItem("Transmission fluid level: dipstick");
    motorOffCheckList.addListItem("Operator's manual in container");
    motorOffCheckList.addListItem("Capacity plate attached: information matches model, serial number and attachments");
    motorOffCheckList.addListItem("Battery restraint system: adjust and fasten");
    motorOffCheckList.addListItem("Brake fluid: check level");
    motorOffCheckList.addListItem("Sitdown truck: seat belt - functioning smoothly");
    motorOffCheckList.addListItem("Man-up truck: fall protection/restraining means - functioning");

    motorOnCheckList.addListItem("Warning: unusual noises must be investigated immediately");
    motorOnCheckList.addListItem("Accelerator linkage: functioning smoothly");
    motorOnCheckList.addListItem("Parking brake: functioning smoothly");
    motorOnCheckList.addListItem("Service brake: functioning smoothly");
    motorOnCheckList.addListItem("Steering operation: functioning smoothly");
    motorOnCheckList.addListItem("Drive control: forward/reverse, functioning smoothly");
    motorOnCheckList.addListItem("Tilt control: forward and back, functioning smoothly");
    motorOnCheckList.addListItem("Hoist and lowering control: functioning smoothly");
    motorOnCheckList.addListItem("Attachment control: operation");
    motorOnCheckList.addListItem("Horn: functioning");
    motorOnCheckList.addListItem("Lights and alarms (where present): functioning");
    motorOnCheckList.addListItem("Hour meter: functioning");
    motorOnCheckList.addListItem("Battery discharge indicator: functioning");
    motorOnCheckList.addListItem("Instrument monitors: functioning");
}

