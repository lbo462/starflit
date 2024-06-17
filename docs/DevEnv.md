# Install the development environment

## Arduino

Install the [Arduino IDE](https://www.arduino.cc/en/software) for your platform and add the libraries folder to your environment.

To do so, copy paste the libraries into a folder named `libraries` in the Arduino folder of your computer.
This folder is probably located under `Documents\` on Windows or `~/Arduino` on linux.
Otherwise, check or change its location in the preferences in the Arduino IDE.

`cp arduino/libraries/* ~/Arduino/libraries/` on linux to install the libraries.

> [!note]
> If you're using Linux, you'll have to add your user to the `dialout` group :
>
> `sudo usermod -a -G dialout <user>`

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

## Raspberry PI 3 - Python

In terms of IDE, I recommend Pycharm but VSCode does the trick.

Working with python is easy, but it's required to be mindful!

> We use `black` for the linting.
> If you're using Pycharm, you should configure it to run on save.
> Otherwise, you're highly encouraged to run `python -m black .` before commiting changes!

### Install Pycharm Professional Edition

As said previously, VSCode is ok for python development, but Pycharm is awesome.

To install Pycharm, go to their website at
[https://www.jetbrains.com/pycharm/download/](https://www.jetbrains.com/pycharm/download/).
You can download their toolbox, but I'd rather install Pycharm standalone since I won't use the other IDEs.

Follow the steps to install it for your OS at
[https://www.jetbrains.com/help/pycharm/installation-guide.html#standalone](https://www.jetbrains.com/help/pycharm/installation-guide.html#standalone).

Now, note that as you're probably a student, you have a __free Pycharm Pro license__ !
To activate it, create an account with your student email address and voilà !

When asked to activate the license, just log to your account and you now have access to Pycharm Professional Edition.

### Create a python virtual env

For that purpose, make sure that you have python installed on your computer.

> On Linux, make sure that you have the python-venv package corresponding to your python version
> (such as `python3.10-venv`).

Then, just follow this tutorial: [https://docs.python.org/3/library/venv.html](https://docs.python.org/3/library/venv.html)

> [!important]
> Since you're installing your virtual environment on a raspberry PI and not and
> on your personal computer, you have to pass an extra argument when creating it :
> 
> `python -m venv venv --system-site-packages`
>
> _If you name your venv other than `venv`, make sure to add it to the `.gitignore` file !_

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

### Actually executing the code

#### Environment variables

Are you here because you didn't create the `.env` file ?

The `.env` file contains your environment variables for the project.
It's ignored by git since everyone can have a different environment and that some private data
might lie in there.

In order to be able to execute the code, one has to create this file in the `python/` folder.

Hopefully, this doc is up-to-date and here's the list of the variables you'll have to declare in this file:
- `SERIAL_PORT` : The full path to the port to which the Arduino is connected ;
- `SERIAL_BAUD` : The speed of the serial transmission.

Here's an example of this file for my Linux setup:

`.env`
```
SERIAL_PORT=/dev/ttyUSB0
SERIAL_BAUD=115200
```

#### Run the code

If you came down to here, you should be ready to execute the code.

Be sure to have read all the prerequisites above beforehand!

In your python venv, run the command : `python main.py`.

Voilà!

### Remote developing on the Raspberry PI 3

While VSCode and Pycharm allow you to open a remote IDE through SSH,
you'll soon find out that RPi 3 are pretty slow, hence remote developing becomes complicated.
And thus, the title was clickbait, and we won't be doing remote developing ^^

The workaround to that issue lies in [rsync](https://linux.die.net/man/1/rsync).
_Please make sure that it's installed for you._

For those unfamiliar with rsync, rsync is a tool that allows file synchronization through SSH.

Since only the python code will run on the RPi, one only need to copy its content on the remote RPi.

Here's the "marche à suivre":

- Log through SSH to your remote RPi.
If you have no idea on how to do so, check [NetworkLab.md](NetworkLab.md) and [InitializeRPis.md](InitializeRPis.md).

- Create a folder named `starflit`:
```sh
cd ~  # go to your home directory
mkdir starflit
```

- Now, log out from SSH and copy the content of the python folder from your computer:
```
cd python/  # enter the python folder (if not already the working dir)
rsync -Pru ./ strandbeest1:/home/<user>/starflit/
```

- Log back through SSH and go the `starflit` folder created and check that the files are present:
```
cd starflit/
ls -lacv  # should display the content of your python folder
```

#### Fastening the process

The above process is long and does not require to be done at every change.

Most of the time, you do not need to test the code on the RPi since you can just plug
the Arduino card on a port of your computer and run the code.

Still, if you need to test your features on the RPi, keep two terminals open:
- One on your computer in the `python` folder,
- the other on the remote in the `starflit` folder.

Once you made your changes, just run this command on your first terminal:

```sh
rsync -Pru ./ strandbeest1:/home/<user>/starflit/
```

And the code will be updated on the other side, so you just have to run it on the second terminal.