#!/usr/bin/bash
#
# Create a Python3 virtual environment 
# and install all needed packages 
# for the MkDocs-based documentation
#

# create the Python3 virtualenv
python3 -m venv venvp3
# activate the newly created virtual env
source venvp3/bin/activate

# update 'pip' to latest version
python -m pip install --upgrade pip

# install all the needed MkDocs-related packages
pip install mkdocs-material        # this installs 'mkdocs' too
pip install markdown-include       # adds a LaTeX-style `include` directive to include .md files
pip install mkdocs-minify-plugin   # ???
pip install mkdocs-macros-plugin   # adds the possibility of defining macros to extend MkDocs
pip install mkdocs-mermaid2-plugin # adds support for mermaid diagrams / flowcharts



echo "\n\n\nSuccess!!\nYou can now build and serve the web iste locally by running: 'mkdocs serve'"
echo "\n[Note: if you have mkdocs installed on your system folders already, to avoid errors about missing packages, you might have to run the one in the virtualenv explicitely, by running: './venvp3/bin/mkdocs serve']"

echo "\nThen, when you finished working with mkdocs, you can close the virtual environment by running: 'deactivate'\n\n"
echo "Happy documenting! :-)\n\n"

