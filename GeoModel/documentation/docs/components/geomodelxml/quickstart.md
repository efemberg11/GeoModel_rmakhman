## Quick Start

To define a geometry using `GeoModelXML`, a top-level file should be provided which gives an entry point to the geometry description, and contains several things which allow it to be interpreted by `GeoModelXML`:

- The `DOCTYPE` should be set to `geomodel`, providing the path to a valid `.dtd` file (for example the latest version of `geomodel.dtd` found in `GeoModelXML\GeoModelXml\data`) defining the expected and allowed properties of the various elements.
- While it is perfectly valid to write the full geometry within a single file (and in fact _production_ geometries will be automatically combined into a single file before use) it can often be more readable to break up the geometry into multiple files. This can be done by defining a named `ENTITY` with a path to another `.gmx` file to be included
- A `geomodel` element should then be defined, which contains the actual geometry definition; this includes defining materials and shapes which can then be used to create logical volumes (`logvols`) which can then be built up into larger `logvols` or `assemblies`.
- The entities defined can be included in the geometry by adding them with `&` in front of their name. NB that entities should be included in an appropriate order to avoid issues (e.g. entities defining parameters should be included before the parameters are used).
- An `addbranch` element should then be defined which includes the [geometry elements to build](buildinggeometry.md). These should be of type `logvol` or `assembly` (or a reference thereto). Anything prior to the `addbranch` can be considered as defining _possible_ contents of the geometry, since only things pulled in by the elements included in `addbranch` will finally be built as part of the geometry.

You can see an example below:

```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE geomodel SYSTEM "PATH/TO/FILE/geomodel.dtd" [
<!ENTITY materialFile SYSTEM "PATH/TO/FILE/materials.gmx">
]>
<geomodel name="Hello World" version="1.0"
  xmlns="http://www.nikhef.nl/%7Er29/gmx">

  &materialFile
  
  <shapes>
    <tube name="ShapeOneMeterCylinder" rmin="200" rmax="500" zhalflength="500"/>
  </shapes>

  <addbranch>
    <logvol name="DiamondCylinder" shape="ShapeOneMeterCylinder" material="Diamond"/>
  </addbranch>

</geomodel>
```
```
<!-This is the content of materials.gmx->
  <materials>
    <element name="Carbon" shortname="C" Z="6" A="12.011" />
    <material name="Diamond" density="3.515">
      <elementref fraction="1.0" ref="Carbon"/>
    </material>
  </materials>
```

The way in which these elements can be defined, have shapes, materials, and transformations (to position them) defined, as well as how they can be combined and replicated will be covered in [Building a geometry](buildinggeometry.md).

Once you have a valid geometry defined, you can use it as an input to the various `GeoModel` applications such as `gmcat`, `gmclash`, `gmex`, etc, via using the `GMXPlugin` to process the input. The input file to use in the `GMXPlugin` can be set one of two ways:

- By default, the plugin will looks for a file called `gmx.xml` so you can e.g. simply create a symbolic link to your desired input file with this name. This works well if you have just a single file, or you are working in the directory where all your geometry files reside. However, this can cause problems with relative paths to entities you wish to include. Therefore, a further more robust option is to simply concatenate your multiple files into a single file, e.g:
```
xmllint --noent --valid PATH/TO/FILE/MyTopFile.gmx > gmx.xml
```

- Alternatively, you can define an environment variable `GMX_FILES` which provides the path to the file (or multiple files, if you have several top-level files defined e.g. for dirrferent detector components) to be read in, e.g:
```
export GMX_FILES='PATH/TO/FILES/file1.gmx PATH/TO/FILES/file2.gmx'
```
(also note potential issues with relative paths to entities)

Once you have your inputs defined, you can launch the applications with a command like:

```
install/bin/gmcat install/lib64/libGMXPlugin.so -o mySQLiteFile.db
```
(details will depend on your installation paths, etc).

Note that the `GMXPlugin` is effectively a simple code wrapper around a call to [`gmx2geo`](components/gmx2geo/), within the general plugin framework defined by `GeoVGeometryPlugin`, and so new plugins which implment slighty different access patterns and behaviour are easily added. 

## Good practise guidelines

As with any software framework, following sensible rules when developing in `GeoModelXML` can help greatly in ensuring readability and maintainability for future, so that other developers can pick up your files and easily understand how they work, and how they could be modified as necessary. Some important rules to think about are:

- Avoid magic numbers: use of named variables (if sensibly named) will be far clearer - for example defining `<var name="bulkhead_z_pos" value="300" />` rather than simply placing a structure at `z=300`
- Use descriptive names: E.g. `bulkhead_z_pos` is better than `BHZ`
- Use relative rather than absolute positions: This allows elements to be moved together rather by updating a single parameter, and thus avoiding either potential misplacements (and clashes) or needing to update multiple parameter coherently. E.g if we have:
```
<var name="bulkhead_z_pos" value="300" />
<var name="bulkhead_halflength_z" value="5" />
<var name="myStruct_halflength_z" value="5" />
```
Then it is preferable to place `myStruct` via
```
<var name="myStruct_z_pos" value="bulkhead_z_pos + bulkhead_halflength_z + myStruct_halflength_z" />
```
than simply by doing
```
<var name="myStruct_z_pos" value="310" />
```
since in the former case changing either the dimensions of `myStruct` or the position of `bulkhead` allow a consistent location of `myStruct` relative to `bulkhead`.
-  Components should ideally be built up in a modular way, such that individual components can be visualized and debugged individually: e.g. thought should be given so that it is straightforward to build a geometry containing a single instance of each fundamental structure on its own. 
