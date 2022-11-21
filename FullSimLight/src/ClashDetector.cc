#include "ClashDetector.hh"
#include "GeoModelKernel/GeoVolumeCursor.h"

#include "G4AffineTransform.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Version.hh"
#include "G4VisExtent.hh"

#include <queue>
#include <set>

namespace clashdet
{
  enum typeOfClash { withMother = 0, withSister, fullyEncapsSister, invalidSolid };

  struct clash // a simple struct to model a clash detection error
  {
    typeOfClash clashType;
    std::string volume1Name;
    G4int       volume1CopyNo;
    std::string volume1EntityType;
    std::string volume2Name;
    G4int       volume2CopyNo = -1;
    std::string volume2EntityType;
    double x,y,z;
    G4double distance;
  };

  void to_json(json& j, const clash& p)
  {
    j = json{{"typeOfClash", p.clashType},
             {"volume1Name", p.volume1Name},
             {"volume1CopyNo", p.volume1CopyNo},
             {"volume1EntityType", p.volume1EntityType},
             {"volume2Name", p.volume2Name},
             {"volume2CopyNo", p.volume2CopyNo},
             {"volume2EntityType", p.volume2EntityType},
             {"x", p.x},{"y", p.y},{"z", p.z},
             {"distance[mm]", p.distance}};
  }

  void from_json(const json& j, clash& p)
  {
    p.clashType=j.at("typeOfClash").get<typeOfClash>();
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
} // namespace clashdet

ClashDetector::ClashDetector(const G4VPhysicalVolume* world) : fWorld(world) {}

//////////////////////////////////////////////////////////////////////////
//
// Write out the clashes report to a file

void ClashDetector::PrintOutReport(const G4String& reportFileName) const
{
  if (reportFileName == "") return;
  json jReport = {{"ClashesReport", jlist}};
  std::ofstream outJsonFile(reportFileName);
  outJsonFile << std::setw(4) << jReport << std::endl;
  outJsonFile.close();
}

//////////////////////////////////////////////////////////////////////////
//
// Check overlaps in the tree of volumes

void ClashDetector::CheckOverlapsInTree(const G4VPhysicalVolume* root)
{
  std::queue<const G4VPhysicalVolume*> volumes;
  std::set<const G4LogicalVolume*> checked;

  volumes.push(((root == nullptr) ? fWorld : root));
  CheckOverlaps(volumes.front());

  while (!volumes.empty())
  {
    const G4VPhysicalVolume* current = volumes.front();
    volumes.pop();

    // check overlaps for daughters
    const G4LogicalVolume* logical = current->GetLogicalVolume();
    G4int ndaughters = logical->GetNoDaughters();
    for (G4int i=0; i<ndaughters; ++i)
    {
      const G4VPhysicalVolume* daughter = logical->GetDaughter(i);
      CheckOverlaps(daughter);
    }

    // append the queue of volumes
    const G4LogicalVolume* previousLogical = nullptr;
    for (G4int i=0; i<ndaughters; ++i)
    {
      const G4VPhysicalVolume* daughter = logical->GetDaughter(i);
      const G4LogicalVolume* daughterLogical = daughter->GetLogicalVolume();
      if (daughterLogical->GetNoDaughters() == 0) continue;
      G4bool found = (daughterLogical == previousLogical);
      if (!found) found = (checked.find(daughterLogical) != checked.cend());
      if (!found)
      {
        checked.emplace(daughterLogical);
        previousLogical = daughterLogical;
        volumes.push(daughter);
      }
      else
      {
        if (fVerbosity)
        {
          G4cout << "************* Checking overlaps in tree of volume "
                 << daughter->GetName() << ':' << daughter->GetCopyNo()
                 << " is omitted, to avoid duplication" << G4endl;
        }
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
// Find a path from World volume to target volume

void ClashDetector::setPath(const G4VPhysicalVolume* target)
{
  fPath.clear();
  fPath.push_back(fWorld);
  G4bool found = fWorld->GetLogicalVolume()->IsAncestor(target);

  while (found)
  {
    if (fPath.back() == target) break;
    const G4LogicalVolume* logical = fPath.back()->GetLogicalVolume();
    G4int ndaughters = logical->GetNoDaughters();
    for (G4int i = 0; i < ndaughters; ++i)
    {
      const G4VPhysicalVolume* daughter = logical->GetDaughter(i);
      found = (daughter->GetLogicalVolume()->IsAncestor(target) || daughter == target);
      if (found) { fPath.push_back(daughter); break; }
    }
  }
  // Just in case, check that everything is in order
  if (!found)
  {
    std::cout << "ERROR: failure in setPath(), target volume: "
              << target->GetName() << ":" << target->GetCopyNo() << std::endl;
    exit(-1);
  }
}

//////////////////////////////////////////////////////////////////////////
//
// Transform local point to the global coordinates

G4ThreeVector ClashDetector::localToGlobal(const G4ThreeVector& local)
{
  G4ThreeVector point = local;
  for (auto it = fPath.rbegin(); it != fPath.rend(); ++it)
  {
    const G4AffineTransform Tm((*it)->GetRotation(), (*it)->GetTranslation());
    Tm.ApplyPointTransform(point);
  }
  return point;
}

//////////////////////////////////////////////////////////////////////////
//
// Append the overlap to the json list

void ClashDetector::reportOverlap(G4int overlapType,
                                  const G4VPhysicalVolume* volume1,
                                  const G4VPhysicalVolume* volume2,
                                  const G4ThreeVector& localPoint,
                                  G4double overlapSize)
{
  // Set clash type
  clashdet::typeOfClash clashType = clashdet::typeOfClash(overlapType);

  // Print out a message
  if (clashType == clashdet::withMother)
  {
    std::cout << "**** GMClash detected a clash ::withMother, at volume" << std::endl;
  }
  else if (clashType == clashdet::withSister)
  {
    std::cout << "**** GMClash detected a clash ::withSister, at volume" << std::endl;
  }
  else if (clashType == clashdet::fullyEncapsSister)
  {
    std::cout << "**** GMClash detected a clash ::fullyEncapsSister, at volume" << std::endl;
  }
  else if (clashType == clashdet::invalidSolid)
  {
    std::cout << "**** GMClash cannot generate a point on the surface of the volume!" << std::endl;
  }
  else
  {
    std::cout << "Unknown clash type!" << std::endl;
    exit(-1);
  }

  // Print local and global points
  setPath(volume1);
  G4ThreeVector globalPoint = localToGlobal(localPoint);
  std::cout << "local point:\t"  << localPoint  << "\n"
            << "global point:\t" << globalPoint << "\n" << std::endl;

  // Set volumes, take mother physical volume from fPath
  const G4VPhysicalVolume* v1 = volume1;
  const G4VPhysicalVolume* v2 = (volume2) ? volume2 : fPath[fPath.size() - 2];

  // Fill the singleClash struct
  clashdet::clash singleClash;
  singleClash.clashType = clashType;
  singleClash.volume1Name = v1->GetName();
  singleClash.volume1CopyNo = v1->GetCopyNo();
  singleClash.volume1EntityType = v1->GetLogicalVolume()->GetSolid()->GetEntityType();
  singleClash.volume2Name = v2->GetName();
  singleClash.volume2CopyNo = v2->GetCopyNo();
  singleClash.volume2EntityType = v2->GetLogicalVolume()->GetSolid()->GetEntityType();
  singleClash.x = globalPoint.x();
  singleClash.y = globalPoint.y();
  singleClash.z = globalPoint.z();
  singleClash.distance = overlapSize;

  // Append the singleClash to the json list
  json jSingleClash;
  to_json(jSingleClash, singleClash);
  jlist.push_back(jSingleClash);
}

//////////////////////////////////////////////////////////////////////////
//
// Check the physical volume on overlap with the sister volumes and
// the mother volume

bool ClashDetector::CheckOverlaps(const G4VPhysicalVolume* volume)
{
  fNumberOfChecks++;
  const G4int res = fResolution;
  const G4double tol = fTolerance;

  std::cout.precision(8);
  clashdet::clash singleClash;
  json jSingleClash;
  if (res <= 0) { return false; }

  G4VSolid* solid = volume->GetLogicalVolume()->GetSolid();
  G4LogicalVolume* motherLog = volume->GetMotherLogical();
  if (motherLog == nullptr) { return false; } // world physical volume

  if (fVerbosity)
  {
    G4cout << "************* Checking overlaps for volume "
           << volume->GetName() << ':' << volume->GetCopyNo() << " ... ";
  }

  // Check that random points are gererated correctly
  //
  G4ThreeVector localPoint = solid->GetPointOnSurface();
  if (solid->Inside(localPoint) != kSurface)
  {
    G4String position[3] = { "outside", "surface", "inside" };
    std::ostringstream message;
    message << "Sample point is not on the surface !" << G4endl
            << "          The issue is detected for volume "
            << volume->GetName() << ':' << volume->GetCopyNo()
            << " (" << solid->GetEntityType() << ")" << G4endl
            << "          generated point " << localPoint
            << " is " << position[solid->Inside(localPoint)];
    G4Exception("ClashDetector::CheckOverlaps()",
                "GeomVol1002", JustWarning, message);

    localPoint.set(0.,0.,0.); // origin
    reportOverlap(clashdet::invalidSolid, volume, nullptr, localPoint, -999);
    return false;
  }

  // Generate random points on the surface of the solid,
  // transform them into the mother volume coordinate system
  // and find the bonding box
  //
  std::vector<G4ThreeVector> points(res);
  G4double xmin =  kInfinity, ymin =  kInfinity, zmin =  kInfinity;
  G4double xmax = -kInfinity, ymax = -kInfinity, zmax = -kInfinity;
  G4AffineTransform Tvolume_mother(volume->GetRotation(), volume->GetTranslation());
  for (G4int i = 0; i < res; ++i)
  {
    points[i] = Tvolume_mother.TransformPoint(solid->GetPointOnSurface());
    xmin = std::min(xmin, points[i].x());
    ymin = std::min(ymin, points[i].y());
    zmin = std::min(zmin, points[i].z());
    xmax = std::max(xmax, points[i].x());
    ymax = std::max(ymax, points[i].y());
    zmax = std::max(zmax, points[i].z());
  }
  G4ThreeVector scenter(0.5*(xmax+xmin), 0.5*(ymax+ymin), 0.5*(zmax+zmin));
  G4double sradius = 0.5*G4ThreeVector(xmax-xmin, ymax-ymin, zmax-zmin).mag();

  // Check overlap with the mother volume
  //
  G4int overlapCount = 0;
  G4double overlapSize = -kInfinity;
  G4ThreeVector overlapPoint;
  G4VSolid* motherSolid = motherLog->GetSolid();
  for (G4int i = 0; i < res; ++i)
  {
    G4ThreeVector p = points[i];
    if (motherSolid->Inside(p) != kOutside) continue;
    G4double distin = motherSolid->DistanceToIn(p);
    if (distin < tol) continue; // too small overlap
    ++overlapCount;
    if (distin <= overlapSize) continue;
    overlapSize = distin;
    overlapPoint = p;
  }

  // Transform 'overlapPoint' back to the 'volume' local coordinates
  // Print information on the overlap
  //
  if (overlapCount > 0)
  {
#if G4VERSION_NUMBER>1050
    localPoint = Tvolume_mother.InverseTransformPoint(overlapPoint);
#else
    localPoint = Tvolume_mother.Inverse().TransformPoint(overlapPoint);
#endif
    std::ostringstream message;
    message << "Overlap with mother volume !" << G4endl
            << "          Overlap is detected for volume "
            << volume->GetName() << ':' << volume->GetCopyNo()
            << " (" << solid->GetEntityType() << ")"
            << " with its mother volume " << motherLog->GetName()
            << " (" << motherSolid->GetEntityType() << ")" << G4endl
            << "          protrusion at mother local point " << overlapPoint
            << " by " << G4BestUnit(overlapSize, "Length")
            << " (max of " << overlapCount << " cases)";
    G4Exception("ClashDetector::CheckOverlaps()",
                "GeomVol1002", JustWarning, message);

    reportOverlap(clashdet::withMother, volume, nullptr, localPoint, overlapSize);
    return true;
  }

  // Checking overlaps with 'sister' volumes
  //
  G4VSolid* previous = nullptr;
  G4ThreeVector pmin_local(0.,0.,0.), pmax_local(0.,0.,0.);

  for (size_t k = 0; k < motherLog->GetNoDaughters(); ++k)
  {
    G4VPhysicalVolume* sister = motherLog->GetDaughter(k);
    if (sister == volume) continue;
    G4bool check_encapsulation = true;

    G4AffineTransform Tsister_mother(sister->GetRotation(), sister->GetTranslation());
    G4VSolid* sisterSolid = sister->GetLogicalVolume()->GetSolid();
    if (previous != sisterSolid)
    {
#if G4VERSION_NUMBER>=1040
      sisterSolid->BoundingLimits(pmin_local, pmax_local);
#else
      G4VisExtent extent = sisterSolid->GetExtent();
      pmin_local.set(extent.GetXmin(),extent.GetYmin(),extent.GetZmin());
      pmax_local.set(extent.GetXmax(),extent.GetYmax(),extent.GetZmax());
#endif
      previous = sisterSolid;
    }
    overlapCount = 0;
    overlapSize = -kInfinity;
    if (!Tsister_mother.IsRotated()) { // no rotation, only translation
      G4ThreeVector offset = Tsister_mother.NetTranslation();
      G4ThreeVector pmin(pmin_local + offset);
      G4ThreeVector pmax(pmax_local + offset);
      if (pmin.x() >= xmax) continue;
      if (pmin.y() >= ymax) continue;
      if (pmin.z() >= zmax) continue;
      if (pmax.x() <= xmin) continue;
      if (pmax.y() <= ymin) continue;
      if (pmax.z() <= zmin) continue;
      for (G4int i = 0; i < res; ++i)
      {
        G4ThreeVector p = points[i];
        if (p.x() <= pmin.x()) continue;
        if (p.x() >= pmax.x()) continue;
        if (p.y() <= pmin.y()) continue;
        if (p.y() >= pmax.y()) continue;
        if (p.z() <= pmin.z()) continue;
        if (p.z() >= pmax.z()) continue;
        G4ThreeVector sisterPoint = p - offset;
        if (sisterSolid->Inside(sisterPoint) == kInside)
        {
          check_encapsulation = false;
          G4double distout = sisterSolid->DistanceToOut(sisterPoint);
          if (distout < tol) continue; // too small overlap
          ++overlapCount;
          if (distout <= overlapSize) continue;
          overlapSize = distout;
          overlapPoint = sisterPoint;
        }
      }
    }
    else // transformation with rotation
    {
      G4ThreeVector pmin(pmin_local), pmax(pmax_local);
      G4ThreeVector dcenter = Tsister_mother.TransformPoint(0.5*(pmin + pmax));
      G4double dradius = 0.5*((pmax - pmin).mag());
      if ((scenter - dcenter).mag2() >= (sradius + dradius)*(sradius + dradius)) continue;
      if (dcenter.x() - dradius >= xmax) continue;
      if (dcenter.y() - dradius >= ymax) continue;
      if (dcenter.z() - dradius >= zmax) continue;
      if (dcenter.x() + dradius <= xmin) continue;
      if (dcenter.y() + dradius <= ymin) continue;
      if (dcenter.z() + dradius <= zmin) continue;

      G4ThreeVector pbox[8] = {
        G4ThreeVector(pmin.x(), pmin.y(), pmin.z()),
        G4ThreeVector(pmax.x(), pmin.y(), pmin.z()),
        G4ThreeVector(pmin.x(), pmax.y(), pmin.z()),
        G4ThreeVector(pmax.x(), pmax.y(), pmin.z()),
        G4ThreeVector(pmin.x(), pmin.y(), pmax.z()),
        G4ThreeVector(pmax.x(), pmin.y(), pmax.z()),
        G4ThreeVector(pmin.x(), pmax.y(), pmax.z()),
        G4ThreeVector(pmax.x(), pmax.y(), pmax.z())
      };
      G4double dxmin =  kInfinity, dymin =  kInfinity, dzmin =  kInfinity;
      G4double dxmax = -kInfinity, dymax = -kInfinity, dzmax = -kInfinity;
      for (G4int i = 0; i < 8; ++i)
      {
        G4ThreeVector p = Tsister_mother.TransformPoint(pbox[i]);
        dxmin = std::min(dxmin, p.x());
        dymin = std::min(dymin, p.y());
        dzmin = std::min(dzmin, p.z());
        dxmax = std::max(dxmax, p.x());
        dymax = std::max(dymax, p.y());
        dzmax = std::max(dzmax, p.z());
      }
      if (dxmin >= xmax) continue;
      if (dymin >= ymax) continue;
      if (dzmin >= zmax) continue;
      if (dxmax <= xmin) continue;
      if (dymax <= ymin) continue;
      if (dzmax <= zmin) continue;
      for (G4int i = 0; i < res; ++i)
      {
        G4ThreeVector p = points[i];
        if (p.x() >= dxmax) continue;
        if (p.x() <= dxmin) continue;
        if (p.y() >= dymax) continue;
        if (p.y() <= dymin) continue;
        if (p.z() >= dzmax) continue;
        if (p.z() <= dzmin) continue;
#if G4VERSION_NUMBER>1050
        G4ThreeVector sisterPoint = Tsister_mother.InverseTransformPoint(p);
#else
        G4ThreeVector sisterPoint = Tsister_mother.Inverse().TransformPoint(p);
#endif
        if (sisterSolid->Inside(sisterPoint) == kInside)
        {
          check_encapsulation = false;
          G4double distout = sisterSolid->DistanceToOut(sisterPoint);
          if (distout < tol) continue; // too small overlap
          ++overlapCount;
          if (distout <= overlapSize) continue;
          overlapSize = distout;
          overlapPoint = sisterPoint;
        }
      }
    }

    // Transform the 'overlapPoint' to the mother coordinate system
    // and then to the 'volume' local coordinates
    // Print information on overlap
    //
    if (overlapCount > 0)
    {
      G4ThreeVector motherPoint = Tsister_mother.TransformPoint(overlapPoint);
#if G4VERSION_NUMBER>1050
      localPoint = Tvolume_mother.InverseTransformPoint(motherPoint);
#else
      localPoint = Tvolume_mother.Inverse().TransformPoint(motherPoint);
#endif
      std::ostringstream message;
      message << "Overlap with already placed volume !" << G4endl
              << "          Overlap is detected for volume "
              << volume->GetName() << ':' << volume->GetCopyNo()
              << " (" << solid->GetEntityType() << ") with "
              << sister->GetName() << ':' << sister->GetCopyNo()
              << " (" << sisterSolid->GetEntityType() << ")" << G4endl
              << "          overlap at local point " << localPoint
              << " by " << G4BestUnit(overlapSize, "Length")
              << " (max of " << overlapCount << " cases)";
      G4Exception("ClashDetector::CheckOverlaps()",
                  "GeomVol1002", JustWarning, message);

      reportOverlap(clashdet::withSister, volume, sister, localPoint, overlapSize);
      return true;
    }
    else if (check_encapsulation)
    {
      // Now checking that 'sister' volume is not totally included
      // and overlapping. Generate a single point inside of
      // the 'sister' volume and verify that the point in NOT inside
      // the current volume
      //
      G4ThreeVector pointSurface = sisterSolid->GetPointOnSurface();
      G4ThreeVector normal = sisterSolid->SurfaceNormal(pointSurface);
      G4ThreeVector pointInside = pointSurface - normal*1.e-4; // move point to inside
      G4ThreeVector sisterPoint =
        (sisterSolid->Inside(pointInside) == kInside) ? pointInside : pointSurface;

      // Transform the generated point to the mother's coordinate system
      // and then to current volume's coordinate system
      //
      G4ThreeVector motherPoint = Tsister_mother.TransformPoint(sisterPoint);
#if G4VERSION_NUMBER>1050
      localPoint = Tvolume_mother.InverseTransformPoint(motherPoint);
#else
      localPoint = Tvolume_mother.Inverse().TransformPoint(motherPoint);
#endif

      if (solid->Inside(localPoint) == kInside)
      {
        std::ostringstream message;
        message << "Overlap with already placed volume !" << G4endl
                << "          Overlap is detected for volume "
                << volume->GetName() << ':' << volume->GetCopyNo()
                << " (" << solid->GetEntityType() << ")" << G4endl
                << "          apparently fully encapsulating volume "
                << sister->GetName() << ':' << sister->GetCopyNo()
                << " (" << sisterSolid->GetEntityType() << ")"
                << " at the same level!";
        G4Exception("ClashDetector::CheckOverlaps()",
                    "GeomVol1002", JustWarning, message);

        reportOverlap(clashdet::fullyEncapsSister, volume, sister, localPoint, -1);
        return true;
      }
    }
  }

  if (fVerbosity)
  {
    G4cout << "OK, done! " << G4endl;
  }
  return false;
}
