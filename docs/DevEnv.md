# Install the development environment

Install the [Arduino IDE](https://www.arduino.cc/en/software) for your platform and add the libraries folder to your environment.

To do so, copy paste the libraries into a folder named `libraries` in the Arduino folder of your computer.
This folder is probably located under `Documents\` on Windows or `~/Arduino` on linux.
Otherwise, check or change its location in the preferences in the Arduino IDE.

`cp arduino/libraries/* ~/Arduino/libraries/` on linux to install the libraries.

## Better use VSCode

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