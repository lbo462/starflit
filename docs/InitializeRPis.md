# Initialize a RPI and connect to it via SSH

## Format the micro SD card

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

## Connect via SSH

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