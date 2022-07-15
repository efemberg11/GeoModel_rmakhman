# fsl

fsl is a GUI for FullSimLight introduced in GeoModel release 4.3.0. It allows users to configure their simulations in FullSimLight easily and without conflicts by providing a series of tabs which offer various configuration options. It can be ran from the command line through the fsl command.

```bash
./fsl
```

 Compatability of the simulation is ensured by the fsl interface by enabling and disabling options based on the selections made. Once the user has configured the simulation to their desire, they can save the configuration in a json file that can be used to run FullSimLight through the -c flag. 
 
```bash
./fullSimLight -c /path/to/config.json
```
 
Alternatively the user can run FullSimLight at any time within fsl itself with the current configuration. fsl also allows users to load in previously saved configurations through the **Open Configuration** option. 
One can also load in a configuration by running fsl with the -c flag.

```bash
./fsl -c /path/to/config.json
```


## Main Tab


{{ imgutils_image_caption('fsl.png', 
   alt='fsl', 
   cap='fsl main tab',
   width ='700px',
   urlFix=False) 
}}

The main tab allows configuration of the following parameters.

- Geometry Input
- Physics List
- Number of Threads
- Number of Events

It also contains the view button which shows the current configuration on the main display. The user can also run FullSimLight in the main display at any time with the current configuration by clicking on the FullSimLight button in the main tab. fsl provides similar buttons to run gmex and gmclash once a geometry input has been selected through the interface.
