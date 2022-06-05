#include "configregions.h"
#include "ui_configregions.h"
#include <iostream>
#include <QDoubleValidator>
#include <sstream>

ConfigRegions::ConfigRegions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigRegions)
{
    ui->setupUi(this);
    this->setWindowTitle("Regions Configuration");
    connect(ui->pB_add_region, &QPushButton::released, this, &ConfigRegions::add_region_config);


    ui->lE_electron->setValidator(new QDoubleValidator(-100000.0,100000.0,8));
    ui->lE_proton->setValidator(new QDoubleValidator(-100000.0,100000.0,8));
    ui->lE_positron->setValidator(new QDoubleValidator(-100000.0,100000.0,8));
    ui->lE_gamma->setValidator(new QDoubleValidator(-100000.0,100000.0,8));

}

ConfigRegions::~ConfigRegions()
{
    delete ui;
}

void ConfigRegions::add_region_config()
{


    region_name = (ui->lE_reg_name->text()).toStdString();
    frootLV_names = (ui->lE_froot->text()).toStdString();
    electron_cut = (ui->lE_electron->text()).toDouble();
    proton_cut = (ui->lE_proton->text()).toDouble();
    positron_cut = (ui->lE_positron->text()).toDouble();
    gamma_cut = (ui->lE_gamma->text()).toDouble();



    emit send_config(region_name,frootLV_names,electron_cut,proton_cut,positron_cut,gamma_cut);

    ui->lE_reg_name->clear();
    ui->lE_froot->clear();
    ui->lE_electron->clear();
    ui->lE_proton->clear();
    ui->lE_positron->clear();
    ui->lE_gamma->clear();

}
