#ifndef FSLMAINWINDOW_H
#define FSLMAINWINDOW_H

#include <QMainWindow>
#include <nlohmann/json.hpp>
#include <QStringListModel>
#include <QProcess>
#include <QStandardItemModel>
#include "configregions.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FSLMainWindow; }
QT_END_NAMESPACE


struct Region {
   std::string Region_name;
   std::vector<std::string> fRootLVnames;
   double electron_cut;
   double proton_cut;
   double positron_cut;
   double gamma_cut;
};

inline void to_json(nlohmann::json& j, const Region& r) {
   j = nlohmann::json{{"RegionName", r.Region_name},
   {"RootLVNames", r.fRootLVnames},
   {"GammaCut", r.gamma_cut},
   {"ElectronCut", r.electron_cut},
   {"PositronCut", r.positron_cut},
   {"ProtonCut", r.proton_cut}};
}

inline void from_json(const nlohmann::json& j, Region& r) {
   r.Region_name=j.at("RegionName").get<std::string>();
   r.fRootLVnames=j.at("RootLVNames").get<std::vector<std::string>>();
   r.gamma_cut=j.at("GammaCut").get<double>();
   r.electron_cut=j.at("ElectronCut").get<double>();
   r.positron_cut=j.at("PositronCut").get<double>();
   r.proton_cut=j.at("ProtonCut").get<double>();

}


class FSLMainWindow : public QMainWindow
{
    Q_OBJECT

public:



    //Parameters associated with the main tab
    nlohmann::json j;
    QProcess process;
    std::string geom_file_address = "";
    std::string physics_list_name = "";
    int number_of_threads = 0;
    int number_of_events = 0;
    std::string config_file_name = "";
    std::string save_directory = "";

    //Parameters associated with the Generator tab
    std::string generator = "";
    std::string particle = "";
    std::string particle_energy = "";
    std::string particle_direction = "";
    double p_x;
    double p_y;
    double p_z;
    std::string x_dir;
    std::string y_dir;
    std::string z_dir;
    int number_of_primaries_per_event;
    std::string pythia_input_file = "";
    std::string pythia_type_of_event = "";

    //Parameters associated with the Sensitive Detectors tab
    std::vector<std::string> sensitive_detector_extensions;
    int sens_det_number;
    std::string hits_file = "";
    std::string histo_file = "";

    //Parameters associated with the Magnetic field tab
    std::string magnetic_field_type = "";
    std::string magnetic_field = "";
    std::string magnetic_field_plugin_file = "";
    std::string magnetic_field_map = "";

    //Parameters associated with the User Actions tab
    std::vector<std::string> run_actions;
    std::vector<std::string> event_actions;
    std::vector<std::string> stepping_actions;
    std::vector<std::string> stacking_actions;
    std::vector<std::string> tracking_actions;
    std::vector<std::string> user_action_extensions;

    //Parameters associated with the g4ui commands tab
    std::vector<std::string> g4ui_commands;
    int g4ui_number;

    //Parameters associated with the shape commands tab
    int shape_number;
    std::vector<std::string> shape_extensions;

    //Parameters associated with the Region tab
    std::vector<Region> regions;


    std::string load_file_name;


    //Function headers
    FSLMainWindow(QWidget *parent = nullptr);
    ~FSLMainWindow();

    //Functions used in Configuration
    void save_configuration();
    void create_configuration();
    void view_configuration();
    void run_configuration();
    void load_configuration();
    std::vector<std::string> display_configuration(const std::string &s);

    void clear_main_status();
    void configure_generator();
    void configure_energy_direction();


    void add_sens_det();
    void del_sens_det();
    void configure_sens_det_actions();

    void add_g4ui();
    void del_g4ui();
    void configure_g4ui_command();

    void pop_up_regions();
    void del_region();
    std::vector<std::string> parse_froot_string(std::string input_froot_string);
    void configure_regions();

    void del_user_action();
    void configure_actions();


    void del_shape_ext();
    void add_shape_ext();
    void configure_shape_ext();

    void assign_geom_file();
    void assign_pythia_file();
    void assign_magnetic_field_plugin_file();
    void assign_shape_ext_file();

    void assign_run_actions_file();
    void assign_event_actions_file();
    void assign_stepping_actions_file();
    void assign_stacking_actions_file();
    void assign_tracking_actions_file();

    void run_actions_file();
    void event_actions_file();
    void stepping_actions_file();
    void stacking_actions_file();
    void tracking_actions_file();

    void configure_magnetic_field();

    std::string get_file_name();
    std::string get_directory();


//Custom signals
signals:
    void send_error_message(std::string info);

private slots:
    void catch_error_message(std::string info);
    void get_sens_det_index(QModelIndex region_index);
    void get_g4ui_index(QModelIndex g4ui_index);
    void get_shape_index(QModelIndex g4ui_index);
    void add_region(std::string  region_name, std::string frootLV_names
                    ,double electron_cut , double proton_cut
                    ,double positron_cut , double gamma_cut);
    void readyReadStandardOutput();
    void readyReadStandardError();


private:
    Ui::FSLMainWindow *ui;
    QStringListModel *sens_det_model;
    QStringListModel *g4ui_model;
    QStringListModel *shape_model;
    ConfigRegions *region;
    QStandardItemModel *region_model;
    QStandardItemModel *user_action_model;
    QStringList region_horizontalHeader;
    QStringList user_action_horizontalHeader;




};
#endif // FSLMAINWINDOW_H
