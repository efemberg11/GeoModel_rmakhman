virtualenv -p python3 venvp3
. venvp3/bin/activate
python -m pip install --upgrade pip

echo "You can now build and serve the web iste locally by running: 'brew serve'"
echo "Then, when you finished working with mkdocs, you can close the virtual environment by running: 'deactivate'"
