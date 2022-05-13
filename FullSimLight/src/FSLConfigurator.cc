#include "FSLConfigurator.hh"
#include "GeoModelKernel/GeoVolumeCursor.h"

#include "G4AffineTransform.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Version.hh"


namespace fslconf {
    enum typeOfClash{ withMother=0, withSister, fullyEncapsSister, invalidSolid};
    // a simple struct to model a clash detection erro
    struct clash {
        typeOfClash clashType;
        std::string volume1Name;
        G4int       volume1CopyNo;
        std::string volume1EntityType;
        std::string volume2Name;
        G4int       volume2CopyNo;
        std::string volume2EntityType;
        double x,y,z;
        G4double distance;
    };
    
    void to_json(json& j, const clash& p) {
        j = json{{"typeOfClash", p.clashType}, {"volume1Name", p.volume1Name}, {"volume1CopyNo", p.volume1CopyNo}, {"volume1EntityType", p.volume1EntityType},{"volume2Name", p.volume2Name},{"volume2CopyNo", p.volume2CopyNo}, {"volume2EntityType", p.volume2EntityType},{"x", p.x},{"y", p.y},{"z", p.z},{"distance[mm]", p.distance} };
    }
    
    void from_json(const json& j, clash& p) {
        p.clashType=j.at("clashType").get<typeOfClash>();
        p.volume1Name=j.at("volume1Name").get<std::string>();
        p.volume1CopyNo=j.at("volume1CopyNo").get<int>();
        p.volume1EntityType=j.at("volume1EntityType").get<std::string>();
        p.volume2Name=j.at("volume2Name").get<std::string>();
        p.volume2CopyNo=j.at("volume2CopyNo").get<int>();
        p.volume2EntityType=j.at("volume2EntityType").get<std::string>();
        p.x=j.at("x").get<double>();
        p.y=j.at("y").get<double>();
        p.z=j.at("z").get<double>();
        p.distance=j.at("distance[mm]").get<double>();
    }
} // namespace fslconf

FSLConfigurator::FSLConfigurator()
{}
