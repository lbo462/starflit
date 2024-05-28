# Network Lab

## Why?

This lab was configured to be able to configure the RPis via SSH, without needing to connect them to a monitor.

To build this lab, you will need:

- a network switch
- a RPi (which you will configure as a DHCP server and router).

## Instructions

In order to configure a RPi as a router, you will first need to flash RaspbianOS on a microSD card. Instructions to do so can be found here [InitializeRPis](InitializeRPis.md#format-the-micro-sd-card).

In the `cmdline.txt` file in the boot partition, add the following line: `ip=192.168.1.254`.

Connect both your computer and the RPi to the switch. You should now be able to connect to it via SSH with the aforementioned IP.

> Once connected, don't forget to `sudo apt-get update && apt-get upgrade`.

For the connection to the internet, we will be connecting to the Eduroam network via Wifi. It is a WPA2 network configured with PEAP / MSCHAPv2 authentication (this is import because this creates many complications...).

### WPA configuration

1. Edit the `/etc/wpa_supplicant/wpa_supplicant.conf` file.

2. Insert the following configuration:

```bash
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev                                               update_config=1                                                   ap_scan=1
network={
ssid="eduroam"
scan_ssid=1
key_mgmt=WPA-EAP
eap=PEAP
identity="${EDUROAM_EMAIL}"
phase2="auth=MSCHAPV2"                                            password="${EDUROAM_PW}"
anonymous_identity="anonymous@insa-lyon.fr"
}  
```

> For the identity and password, you can use your own university email address and password.

It is a good security measure to use environment variables in configuration files when it comes to usernames and passwords so that they don't appear in cleartext on github 😄.

You can configure environment variables like so:

```bash
export MYVAR='myPassword'
```

If you want it to be permanent:

```bash
#TODO
```

The RPi (router) should now be able to connect to Eduroam. You can test it by running the following command:

```bash
sudo /sbin/wpa_supplicant -i wlan0 -c /etc/wpa_supplicant/wpa_supplicant.conf
```

You can make sure the router is connected with this command `wpa_cli status`.

### DHCP configuration

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

### Routing configuration

#### IP forwarding

Activate IP forwarding by editing the `/etc/sysctl.conf` file and uncommenting `net.ipv4.ip_forward=1`.

#### NAT

Activate NAT Masquerade with `iptables`:

- `sudo apt install iptables`
- `sudo iptables -t nat -A POSTROUTING -o wlan0 -j MASQUERADE`
- `sudo apt install iptables-persistent` to render the configuration persistent on reboot.

You can check your configuration with `sudo iptables -t nat -L`.

#### Default route

Delete the current default route:

```bash
sudo ip r d default
```

Add a new one to the Eduroam gateway:

```bash
sudo ip r a default via 10.56.0.1 dev wlan0
```

Your packets should now be routed correctly 📦.

### Run everything on startup

We don't want to manually connect the router to the Wifi and configure the default routes everytime we turn the router on. To solve this problem, we are going to create a service that will run on startup, as well as a script to configure the default route.

```bash
[Unit]
Description=WPA supplicant
Wants=network.target
After=network.target
[Service]
Type=simple
ExecStartPre=/sbin/ip link set wlan0 up
ExecStart=/home/router/setup_wpa.sh
RemainAfterExit=yes
[Install]
WantedBy=multi-user.target
```

<!-- TODO: add scripts -->