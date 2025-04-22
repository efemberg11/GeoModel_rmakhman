# The 'HelloDummyMaterial' GeoModel example

The `HelloDummyMaterial` example shows you how to create a preliminary version of your geometry, by using GeoModel nodes and a "dummy" material. That way, you will not have to define your set of materials while working on the geometry.

The "Dummy" material will be rendered in a special way in the visualization program GeoModelExplorer, so you will recognize at a glance the volumes for which the material still has to be properly defined.

The example program:

 1. Builds an example geometry:
 2. Writes the geometry to an SQLite file
 3. It accesses the persistent copy of the geometry, as a test


## Run

Now, you can **run the example** by typing:

```bash
./HelloDummyMaterial/helloDummyMaterial
```

When the program terminates, you will find a `geometry.db` file in the build folder. This is a SQLite file containing the dump of the geometry defined in the example C++ code.
