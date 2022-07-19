#ifndef CONFIGREGIONS_H
#define CONFIGREGIONS_H

#include <QDialog>
#include <QDoubleValidator>
#include <QLocale>

namespace Ui {
class ConfigRegions;
}

class ConfigRegions : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigRegions(QWidget *parent = nullptr);
    ~ConfigRegions();
    std::string region_name;
    std::string frootLV_names;
    double electron_cut;
    double proton_cut;
    double positron_cut;
    double gamma_cut;
    void add_region_config();
    QLocale loc;

signals:
    void send_config(std::string  region_name, std::string frootLV_names
                     ,double electron_cut , double proton_cut
                     ,double positron_cut , double gamma_cut);

private:
    Ui::ConfigRegions *ui;
    QDoubleValidator *electron_cut_validator;
    QDoubleValidator *proton_cut_validator;
    QDoubleValidator *positron_cut_validator;
    QDoubleValidator *gamma_cut_validator;
};

#endif // CONFIGREGIONS_H
