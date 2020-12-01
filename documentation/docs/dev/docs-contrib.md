
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

In order to update the documentation, you don't need to install `MkDocs` locally, because its build is performed within the GitLab CI (Continuos Integration) pipelines and then the HTML outcome is installed on the GeoModel server at CERN. 

Thus, if you only want to update the documentation, you can merely:

1. fork the GeoModel repository
2. edit the documentation files you want to update
3. push your changes
4. create a Merge Request to get your changes merged into the GeoModel `master` branch

However, if you want to preview your changes on your machine---and we recommend that---, you need to install `MkDocs` on your machine.


## Dependencies - MkDocs

On macOS, you can install `MkDocs` with [Homebrew](https://brew.sh):

```
brew install mkdocs
```

## Build and serve

From the `documentation` folder, you can run

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



