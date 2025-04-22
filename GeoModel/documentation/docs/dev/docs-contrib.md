
# How to contribute to the documentation

## Get the documentation code

The GeoModel documentation is stored within the main GeoModel code repository.

To get the latest documentation, simply clone the repository:

```
git clone https://gitlab.cern.ch/GeoModelDev/GeoModel.git
```

Then, navigate to the documentation directory:

```
cd GeoModel/documentation
```

## How to update the documentation

The documentation uses the documentation engine [MkDocs](https://www.mkdocs.org/) to transform documentation written in Markdown to HTML static files.

It also uses the [Material](https://squidfunk.github.io/mkdocs-material/) theme for MkDocs, which offers many tools to handle additional documentation metadata.

The actual documentation files are stored under the folder `documentation/docs/`.
The `MkDocs` configuration files are stored in the `documentation` root folder, besides the `docs/` folder.

In order to update the documentation, you don't need to install `MkDocs` locally, because its build is performed within the GitLab CI (Continuos Integration) pipelines and then the HTML outcome is installed on the GeoModel server at CERN. 

Thus, if you only want to update the documentation, you can merely:

1. fork the GeoModel repository
2. edit the documentation files you want to update
3. push your changes
4. create a Merge Request to get your changes merged into the GeoModel `main` branch

However, if you want to preview your changes on your machine---and we recommend that---, you need to install `MkDocs` on your machine.


## Dependencies - MkDocs + Material

In order to build the documentation and visualize the ouput on your machine, you have to install MkDocs and the needed packages.
The easiest way to do that is using the script we provide to create a standalone, encapsulated Python *virtualenv* directly in the documentation folder. 

First of all, you need Python3 installed on your machine (you should have it already if your OS is enough recent). Then, navigate to the `documentation` folder and source the script:

```bash
cd GeoModel/documentation
source create_activate_virtualenv_p3.sh
```

The script will create a Python3 virtual environment, it will activate it, it will install MkDocs and all the needed packages, and it will start the virtual env. *[If you want to install everything by hand ---* **not reccommended!** *---, please look into the script which packages you would need to install]*


## Build and serve

From the `documentation` folder, you can now run

```
mkdocs serve
```

This will build the documentation and will start a local web server to serve it on a local port.

You will get a message like this, at the end of the process:

```
[I 201201 19:05:55 server:296] Serving on http://127.0.0.1:8000
```

Open a web browser tab and enter the address you got, including the port number:

```
http://127.0.0.1:8000
```

You will be able to preview the documentation web site, and that will be automatically rebuilt and reloaded in the browser as soon as you change a documentation file.

To stop the local test server, just push `Ctrl+c` in the shell where the server is running. This will stop the test server.

## Closing the *virtualenv*

When you are done with editing and testing the documentation site, you can close the Python3 *virtualenv* by running in the shell:

```bash
deactivate
```

That will close the virtual environment and it will return to the standard Python interpreter and packages, the ones installed on your machine besides your main Python installation.


## Adding items to the Navigation Menu

The navigation menu configuration is stored in the `nav` section of the `documentation/mkdocs.yml` configuration file. If you want to add pages that should be directly accessible from the main navigation menu, you have to add them in that file. A detailed example follows here below.

!!! note

    you do not need to edit that configuration file if you only add pages or sections that should be accessible and/or linked only from other pages. You only need to edit that configuration file when adding items to the main navigation menu.

For example, in the following example, we are creating a `Home` menu button and a `Getting Started` menu section with some buttons in it, then we are linking:

* the `Home` button to the `documentation/docs/index.md` page
* the `Getting Started`-->`Intro` button to the `documentation/docs/start/index.md` page
* the `Getting Started`-->`Install Libraries & Tools` to the `documentation/docs/start/install.md` page


```yaml
nav:                                                                                                         
    - Home: 'index.md'                                                                                       
    - Getting Started:                                                                                       
        - 'Intro': 'start/index.md'                                                                           │  - dev/contributors.md
        - 'Install Libraries & Tools': 'start/install.md' 
```

!!! note

    Please notice that in the `mkdocs.yml` configuration file all paths are relative to the `documentation/docs` folder, which is the main container folder of all the documentation content.

You can find more information on the configuration file and on the navigation menu in the [official MkDocs documentation](https://www.mkdocs.org/user-guide/writing-your-docs/).




