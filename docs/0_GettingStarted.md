# Getting started 🚀

If you're reading this, it means that you are ready to start your Starflit
dive. Aren't you ? If you're not, keep this reading for later, as it might be
a bit overwhelming at the start. If you are, let's begin the diving!

## Git and Github

The starflit project has its GitHub repository at
[https://github.com/lbo462/starflit](https://github.com/lbo462/starflit).
If you want to start your project where we left ours, you should
[fork](https://en.wikipedia.org/wiki/Fork) this repository into your own
personal space, using the fork button on GitHub.
_See [https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks/fork-a-repo](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks/fork-a-repo)._
After the forking stuff, you should have your own version of the repository,
where you'll be able to work on your version of the Starflit project.

> You are free to rename the project.

But you might not want to fork the project and start your version from
scratch, but that would make us sad. Anyway, you'll find in the appendix a
section about working with Git and GitHub. If you are already used to work
with it, you might do not need to look into it. But if you're not, you're
highly encouraged to! Plus, that will give you an idea of how the starflit
crew worked with git and GitHub.

## Arduino development environment

First things first, you'll have to install some things on your personal
computer. If you have a Linux computer, use it! That will help you a lot for
the rest. If you don't, you will still be able to work with Windows, but with
a bit more pain. If you're on Mac, the process should be similar, but we haven't tested.

> Or one could use a VM.

As you might have guessed, we're working on Arduino. Hopefully, the one
reading this, has already worked with Arduino and is familiar with the
Arduino environment. Otherwise, you'll have trouble reading through this.
We do not aim to write an Arduino tutorial, but a "Starflit tutorial".

Usually while working with Arduino, one should install the Arduino IDE when 
working with Arduino, but we did not, as this IDE lacks the major features of 
a professional IDE. We definitely do not recommend using it while working on 
this project, but you're free not to listen. If so, check it
[here](https://www.arduino.cc/en/software) and install it for your platform.

Instead, we used VSCode. If you do not have VSCode, install it from their
[website](https://code.visualstudio.com/).

> The new version of the Arduino IDE comes with a CLI tool that lead to the
> creation of a VSCode extension for Arduino development. As we're writing
> this, the extension is still in beta version, but worked more than enough 
> for us.

You'll find the extension by typing `Arduino` in the VSCode search extension
field (`Ctrl+Shift+X`) and clicking the one provided by Microsoft.

To configure the extension, open the `.ino` file that you want to compile and 
upload (here, probably `arduino/main/main.ino`), and select in your IDE 
footer:

- the board: `Arduino UNO` (install __Arduino AVR__ board if necessary)
- the port: the one on which you're connecting the robot
- the programmer: `AVR ISP`

### Install the dependencies

Now, one has to install the Arduino dependencies for the project. We kept all 
the library we used in the folder `libraries`, but Arduino won't find them
here. Instead, we have to copy-paste the content of this folder where Arduino
will find them.

To do so, copy and paste the libraries into a folder named `libraries` in the 
Arduino folder of your computer. This folder is probably located under
`Documents\Arduino` on Windows or `~/Arduino` on Linux. Otherwise, check or
change its location in the preferences in the Arduino IDE, if you've
installed it.

`cp arduino/libraries/* ~/Arduino/libraries/` on Linux to install the libraries.

### Add a dependency

Arduino recommends adding libraries using their library manager, in their IDE.
To install a library without the Arduino IDE library manager, you can download
the libraries as a zip file (on the Arduino website, or on GitHub) and unzip it
into the `arduino/libraries` folder, and re-running the command
`cp arduino/libraries/* ~/Arduino/libraries/`. This way, the libraries will be
updated on the repo, for everyone.

<br>

Back to the extension configuration ...

In the file header, you should see new buttons. Here are the actions you'll 
often need to do while developing:

- Verify: `Ctrl+Shift+i` (aka _Compile_) is accessible via a button that appears in the 
header of the opened `.ino` file.

> You can do this action any time to check that your code compiles correctly.
> It also refreshes the VSCode interpreter, so feel free to use it 
> frequently.
>
> __You will need to have installed the dependencies beforehand !__
>
> If you're using Linux, you'll have to add your user to the _dialout_ group
> with `sudo usermod -a -G dialout <user>` to run the code. You might need to 
> restart your computer in order to take effect. Use `groups` to check.

- Upload: `Ctrl+Shift+u` is accessible the same way via a button that appear in the header 
of the opened `.ino` file.

> Before uploading your code, you want to plug the strandbeest to your
> computer on the dev interface (the other interface is used to charge the
> strandbeest). Uploading will also compile the code.
> 
> Note that you'll have to change the main `.ino` program by hand in the 
> footer whenever you want to change the program. Just opening an other 
> `.ino` file and clicking `Upload` will still upload the configured old
> program!

- Serial Monitoring is accessible via the shortcut `Ctrl+Shift+M` 
and clicking the `Serial Monitor` tab.
Make sure to select the right port and speed (which is `115200` or `9600`) and click
`Start Monitoring`.

## Network Lab

### Why?

You'll now notice that we also have a Raspberry PI (RPi) on the strandbeest.
Hence, we need to set-up quite some things to develop on these.

This lab was configured to be able to configure the RPis via SSH, without needing to connect them to a monitor.

To build this lab, you will need:

- a network switch
- a RPi (which you will configure as a DHCP server and router).

### Instructions

#### Format the micro SD card

In order to work with a Raspberry Pi, you first need to format a micro SD card with the Raspbian OS (or any other OS you are comfortable with).

The steps to do so are the following:

- download the [Raspberry Pi Imager](https://www.raspberrypi.com/software/)
- choose the version of the RPi your are using (3 model B in our case)
- choose which OS you want to use (RaspbianOS 64 bit in our case)
- next, we need to set custom settings in order to enable SSH, click on "Edit Settings"
- set a hostname, username and password and locale settings according to your region
- in the "Services" tab, click on "enable SSH" and "Use password authentication"
- in the "Options" tab, disable "Enable telemetry" (we don't want anyone spying on us 🕵️‍♂️)
- click on "Save", "Yes" and "Yes" again. Wait a couple of minutes and everything should be good to go 👍.

<br>

Once the micro SD card is flashed, in the `cmdline.txt` file in the boot partition, add the following line: `ip=192.168.1.254`.

Connect both your computer and the RPi to the switch. You should now be able to connect to it via SSH with the aforementioned IP.

> Once connected, don't forget to `sudo apt-get update && sudo apt-get upgrade`.

For the connection to the internet, we will be connecting to the Eduroam network via Wifi. It is a WPA2 network configured with PEAP / MSCHAPv2 authentication (this is import because this creates many complications...).

#### WPA configuration

1. Edit the `/etc/wpa_supplicant/wpa_supplicant.conf` file.

2. Insert the following configuration:

```bash
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1
ap_scan=1
network={
ssid="eduroam"
scan_ssid=1
key_mgmt=WPA-EAP
eap=PEAP
identity="${EDUROAM_EMAIL}"
phase2="auth=MSCHAPV2"
password="${EDUROAM_PW}"
anonymous_identity="anonymous@insa-lyon.fr"
}  
```

> For the identity and password, you can use your own university email address and password.

It is a good security measure to use environment variables in configuration files when it comes to usernames and passwords so that they don't appear in cleartext on github 😄.

You can configure environment variables like so:

```bash
export MYVAR='myPassword'
```

If you want it to be permanent, add it to the `.bashrc` file or the corresponding file for your shell.

The RPi (router) should now be able to connect to Eduroam. You can test it by running the following command:

```bash
sudo /sbin/wpa_supplicant -i wlan0 -c /etc/wpa_supplicant/wpa_supplicant.conf
```

You can make sure the router is connected with this command `wpa_cli status` (or `iwconfig`).

#### DHCP configuration

The router will act as a DHCP server that will distribute IPs to the RPis so that we can easily connect to them via SSH.

The steps are simple:

1. Download the DHCP server `sudo apt install isc-dhcp-server`.
2. Edit `/etc/dhcp/dhcpd.conf` and add the following:

```bash
default-lease-time 3600;
max-lease-time 7200;

subnet 192.168.1.0 netmask 255.255.255.0 {
    option-routers 192.168.1.254;
    option domain-name-servers 8.8.8.8;
    range 192.168.1.10 192.168.1.30;
}
```

3. Set the interface you want your DHCP server to be configured on in `/etc/default/isc-dhcp-server` by appending `eth0` in `INTERFACESv4=""`.

4. Restart the service:

```bash
sudo systemctl restart isc-dhcp-server.service
```

You can check the leases that are distributed to your clients via the following command:

```bash
dhcp-lease-list
```

#### Routing configuration

##### IP forwarding

Activate IP forwarding by editing the `/etc/sysctl.conf` file and uncommenting `net.ipv4.ip_forward=1`.

##### NAT

Activate NAT Masquerade with `iptables`:

- `sudo apt install iptables`
- `sudo iptables -t nat -A POSTROUTING -o wlan0 -j MASQUERADE`
- `sudo apt install iptables-persistent` to render the configuration persistent on reboot.

You can check your configuration with `sudo iptables -t nat -L`.

##### Default route (MIGHT NOT BE NEEDED ANYMORE)

Delete the current default route:

```bash
sudo ip r d default
```

Add a new one to the Eduroam gateway:

```bash
sudo ip r a default via 10.56.0.1 dev wlan0
```

Your packets should now be routed correctly 📦.

#### Run everything on startup

We don't want to manually connect the router to the Wifi and configure the default routes everytime we turn the router on. To solve this problem, we are going to create a simple systemd service that will run on startup. This service will call the `wpa_setup.sh` script which does all the heavy lifting. 

`wpa_supplicant.service`:

```bash
[Unit]
Description=Auto connect to eduroam
Wants=network.target
After=network.target
[Service]
Type=simple
ExecStart=/home/router/wpa_setup.sh
RemainAfterExit=yes
[Install]
WantedBy=multi-user.target
```

`wpa_setup.sh`:

```bash
#!/bin/bash

# Log file path
LOG_FILE="/home/router/events.log"

# Bring the interface up
sudo /sbin/ip link set wlan0 up
echo "$(date): Brought wlan0 up" >> $LOG_FILE

# Start wpa_supplicant in the background (-B option)
sudo /sbin/wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant/wpa_supplicant.conf
echo "$(date): Started wpa_supplicant" >> $LOG_FILE

# Wait for a bit to ensure connection
sleep 10

# Log iwconfig output
/sbin/iwconfig >> $LOG_FILE
echo "---------------------------------------" >> $LOG_FILE

# Request an IP address
sudo /sbin/dhclient wlan0
echo "$(date): dhclient ok" >> $LOG_FILE

# Wait to ensure IP is assigned
sleep 10

# Log current IP addresses
/sbin/ip a >> $LOG_FILE
echo "---------------------------------------" >> $LOG_FILE
```

You can check that everything is running smoothly by looking at the log file defined in the `LOG_FILE` variable.

Lastly, enable the service so that it runs on reboot:

```bash
sudo systemctl enable wpa_supplicant.service
```

Congratulations, the lab is up and ready to go 🎉🍾🍾🍾🍾.

#### Connect the RPi via SSH

Now that the network lab is up, you can connect to the RPi through SSH.

Connect the RPi to the network. It should receive an IP address in the same subnet as your PC via DHCP. If you don't have access to the network router to figure out the IP, you can use [Nmap](https://nmap.org/download.html) or [Advanced IP Scanner](https://www.advanced-ip-scanner.com/) to scan the network and discover the IP of your RPi.
Once you have the IP, connect to it via SSH using the following command:

```bash
ssh starflit@x.x.x.x
```

> You should also be able to use the hostname that you set to connect to it.

Congratulations, you now have full access to your RPi 🎉🎉.

Don't forget to update your packages:

```bash
sudo apt-get update && sudo apt-get upgrade
```

## Python development environment

Now that we have a full access to our RPi through SSH, it's time to install
the development environment.

In terms of IDE, I recommend Pycharm but VSCode does the trick.

Working with python is easy, but it's required to be mindful!

> [![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black)
>
> We use `black` for the linting.
> If you're using Pycharm, you should configure it to run on save.
> Otherwise, you're highly encouraged to run `python -m black .` before committing changes!

### Install Pycharm Professional Edition

As said previously, VSCode is ok for python development, but Pycharm is
awesome.

To install Pycharm, go to their website at
[https://www.jetbrains.com/pycharm/download/](https://www.jetbrains.com/pycharm/download/).
You can download their toolbox, but I'd rather install Pycharm standalone
since I won't use the other IDEs.

Follow the steps to install it for your OS at
[https://www.jetbrains.com/help/pycharm/installation-guide.html#standalone](https://www.jetbrains.com/help/pycharm/installation-guide.html#standalone).

Now, note that as you're probably a student, you have a __free Pycharm Pro 
license__ ! To activate it, create an account with your student email address 
and voilà !

When asked to activate the license, just log to your account and you now have 
access to Pycharm Professional Edition.

### Remote developing on the Raspberry PI 3

While VSCode and Pycharm allow you to open a remote IDE through SSH,
you'll soon find out that RPi 3 are pretty slow, hence remote developing
becomes complicated. And thus, the title was clickbait, and we won't be doing 
remote developing ^^

The workaround to that issue lies in
[rsync](https://linux.die.net/man/1/rsync). _Please make sure that it's 
installed for you._

For those unfamiliar with rsync, rsync is a cool tool that allows file 
synchronization through SSH.

Since only the python code will run on the RPi, one only need to copy its 
content on the remote RPi.

Here's the "marche à suivre":

- Log through SSH to your remote RPi.

- Create a folder named `starflit`:
```sh
cd ~  # go to your home directory
mkdir starflit
```

- Now, log out from SSH and copy the content of the python folder from your computer:
```
cd raspberry/python/  # enter the python folder (if not already the working dir)
rsync -Pru ./ strandbeest1:/home/<user>/starflit/
```

- Log back through SSH and go the `starflit` folder created and check that the files are present:
```
cd starflit/
ls -lacv  # should display the content of your python folder
```

### Create a python virtual environment

__What is it and why do I need that ?__

A virtual environment is a environment allowing you to run python code for a
specific project, without using the default environment on your device.

Imagine working on multiple project having different requirements. How can
these coexist nicely ? How to know what dependencies belong to what project ?
One should __always__ create a virtual environment while using Python.

> Here, we use the default solution for this, but note that their exist some
> nicer ones, like [Poetry](https://python-poetry.org/) that you're
> encouraged to use.

For that purpose, make sure that you have python installed on your computer.

> On Linux, make sure that you have the python-venv package corresponding to 
> your python version (such as `python3.10-venv`).

Then, just follow this tutorial:
[https://docs.python.org/3/library/venv.html](https://docs.python.org/3/library/venv.html)

> Since you're installing your virtual environment on a raspberry PI and not
> on your personal computer, you have to pass an extra argument when creating
> it :
>
> `python -m venv venv --system-site-packages`
>
> This will add the dependencies from the Raspberry PI into your environment.

You're ready to install dependencies now!

### About dependencies ...

We're working with [pip](https://pypi.org/project/pip/), hence we keep a file 
called `requirements.txt` in the python folder.

#### Install dependencies

In your virtual env, call `pip install -r requirements.txt`.

You should be ready to work now, but let's see how to add a dependency ...

#### Add a dependency

To add a dependency:

```sh
pip install <your-dep>            # install the dependency in your env
pip freeze -l > requirements.txt  # add it to the list of dependencies.
                                  # -l indicate to keep only local packages.
                                  # This is required to avoid avoid the packages
                                  # retrieved by --system-site-packages
```

If you're on Windows, copy-paste the output of `pip freeze` into `requirements.txt`.

### Actually executing the code

#### Environment variables

Are you here because you didn't create the `.env` file ?

The `.env` file contains your environment variables for the project. It's 
ignored by git since everyone can have a different environment and that some private data might lie in there.

In order to be able to execute the code, one has to create this file in the 
same folder as the python code, where it's executed.

Use `cp default.env .env` to create it with the default values.
Update the variables inside it to match your environment.

Hopefully, this doc is up-to-date and here's the list of the variables you'll 
have to declare in this file:
- `SERIAL_PORT` : The full path to the port to which the Arduino is connected
- `SERIAL_BAUD` : The speed of the serial transmission

#### Run the code

If you came down to here, you should be ready to execute the code. Be sure to 
have read all the prerequisites above beforehand!

In your python venv, run the command : `python main.py`.

You'll have to pass some arguments to the command above. To
see which ones, run `python main.py -h`. Note that you'll
have to give it the model, which is the `.tflite` file, 
and a list of labels, which is `labels.txt`.

> If you've done some fine-tuning, you could change the
> model with a different `.tflite` file.

Voilà!

#### Fastening the process

When testing your features on the RPi, keep two terminals open:
- One on your computer in the `python` folder,
- the other on the remote in the `starflit` folder.

Once you made your changes, just run this command on your first terminal:

```sh
rsync -Pru ./ strandbeest1:/home/$USER/starflit/
```

And the code will be updated on the other side, so you just have to run it on 
the second terminal, making sure you're in your virtual environment.

## Deployment on the Raspberry

To deploy the python script on the RPi, you'll find an install script named
`install.sh` to run on a freshly flashed RPi. This script will update the RPi,
install all the system requirements, clone the `raspberry/` folder and enable
the `starflit.service` to run at boot.

> The file `starflit.service` defines a Debian service, managed by
> [systemd](https://wiki.debian.org/systemd).

To get the file on the RPi, make sure it has access to the Internet, and pull
it from GitHub with curl.

```sh
curl -v https://raw.githubusercontent.com/lbo462/starflit/master/raspberry/install.sh -o install.sh

chmod +x install.sh  # Make it executable
./install.sh         # Actually run the script 
```

To check that the service is working correctly, use
`systemctl status starflit.service`.

> You need to have the Arduino card plugged on `/dev/USBtty0` (you can
> edit this port in `/opt/starflit/raspberry/python/.env`).

You can restart the service with
`systemctl restart starflit.service`.

And you can check the logs with
`journalctl -u starflit.service -f`.

__Uninstall 😭__

You can undo what the install script did with the uninstall script:

```sh
curl -v https://raw.githubusercontent.com/lbo462/starflit/master/raspberry/uninstall.sh -o uninstall.sh

chmod +x uninstall.sh
./uninstall.sh
```

---

If you came down to here, well done. You're now officialy promoted as a
fully operational 🚀 _Starflit agent_ 🚀. All the Starflit crew wish you good 
luck for your project and hope that your flight goes smooth.

Note that you'll find way more documentation here, so it's not over. Have a
quick pause, and come back to explore all the documentation we wrote for you.