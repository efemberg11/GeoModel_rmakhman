#ifndef CONFIGREGIONS_H
#define CONFIGREGIONS_H

#include <QDialog>

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

signals:
    void send_config(std::string  region_name, std::string frootLV_names
                     ,double electron_cut , double proton_cut
                     ,double positron_cut , double gamma_cut);

private:
    Ui::ConfigRegions *ui;
};

#endif // CONFIGREGIONS_H
