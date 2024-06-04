# Install the development environment

## Arduino

Install the [Arduino IDE](https://www.arduino.cc/en/software) for your platform and add the libraries folder to your environment.

To do so, copy paste the libraries into a folder named `libraries` in the Arduino folder of your computer.
This folder is probably located under `Documents\` on Windows or `~/Arduino` on linux.
Otherwise, check or change its location in the preferences in the Arduino IDE.

`cp arduino/libraries/* ~/Arduino/libraries/` on linux to install the libraries.

### Better use VSCode

The Arduino IDE lacks the major features of a professional IDE.
Instead, you're encouraged to work with VSCode and the appropriate extension.

You'll find the extension by typing `Arduino` in the VSCode search extension field 
(`Ctrl+Shift+X`) and clicking the one provided by Microsoft.

__Before going any further__ ...
Open the `.ino` file and select in your IDE footer:

- the board: `Arduino UNO` (install __Arduino AVR__ board if necessary)
- the port: the one on which you're connecting the robot
- the programmer: `AVR ISP`

In the file header, you should see new buttons.
Here's the actions you'll need to do often while developing:

- `Verify` (aka `Compile`) is accessible via a button that appears
in the header of the opened `.ino` file.
__Also accessible through `Ctrl+Shift+i`__.

- `Upload` is accessible the same way via a button that appear
in the header of the opened `.ino` file.
__Also accessible through `Ctrl+Shift+u`__.

- `Serial Monitoring` is accessible via the shortcut `Ctrl+Shift+M` 
and clicking the `Serial Monitor` tab.
Make sure to select the right port and speed (which is `115200` or `9600`) and click
`Start Monitoring`.

## Python

In terms of IDE, I recommend Pycharm but VSCode does the trick.

Working with python is easy but it's required to be mindful!

### Create a python virtual env

For that purpose, make sure that you have python installed on your computer.

> On Linux, make sure that you have the python-venv package corresponding to your python version (such as `python3.10-venv`).

Then, just follow this tutorial: [https://docs.python.org/3/library/venv.html](https://docs.python.org/3/library/venv.html)

> If you name your venv other than `venv`, make sure to add it to the `.gitignore` file !

You're ready to install dependencies now!

### About dependencies ...

We're working with [pip](https://pypi.org/project/pip/),
hence we keep a file called `requirements.txt` in the python folder.

__Install dependencies__

In your virtual env, call `pip install -r requirements.txt`.

You should be ready to work now, but let's see how to add a dependency ...

__Add a dependency__

To add a dependency:

```sh
pip install <your-dep>         # install the dependency in your env
pip freeze > requirements.txt  # add it to the list of dependencies
```

If you're on Windows, copy-paste the output of `pip freeze` into `requirements.txt`.
