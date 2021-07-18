Table of Contents
------------------

- [Setting up Ubuntu for Gitian building](#setting-up-debian-for-gitian-building)
- [Installing Gitian](#installing-gitian)


Setting up Ubuntu for Gitian building
--------------------------------------

In this section we will be setting up the Ubuntu installation for Gitian building.
We assume that a user `gitianuser` was previously added.

First we need to set up dependencies. Execute the following as root:

```
apt-get install git ruby apt-cacher-ng qemu-utils debootstrap python-cheetah parted kpartx bridge-utils make curl firewalld
brctl addbr br0
ip addr add 10.0.3.1/24 broadcast 10.0.3.255 dev br0
ip link set br0 up
firewall-cmd --zone=trusted --add-interface=br0
ip address show
apt-get install lxc
ip address show
```

Then set up LXC and the rest with the following, which is a complex jumble of settings and workarounds:

Make new file: /etc/sudoers.d/gitian-lxc
```
%sudo ALL=NOPASSWD: /usr/bin/lxc-start
%sudo ALL=NOPASSWD: /usr/bin/lxc-execute
```

Make new file: /etc/rc.local
```
#!/bin/sh -e
brctl addbr br0
ip addr add 10.0.3.1/24 broadcast 10.0.3.255 dev br0
ip link set br0 up
firewall-cmd --zone=trusted --add-interface=br0
exit 0
```
Then `chmod +x /etc/rc.local`

Add below after last line: /home/gitianuser/.profile
```
export USE_LXC=1
export GITIAN_HOST_IP=10.0.3.1
export LXC_GUEST_IP=10.0.3.5
```
Then `reboot`

At the end Ubuntu is rebooted to make sure that the changes take effect. The steps in this
section only need to be performed once.

**Note**: When sudo asks for a password, enter the password for the user `gitianuser` not for `root`.


Check bridge br0
----------------

```
ip address show
```
It is displayed as follows.
```
3: br0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default qlen 1000
    link/ether fe:73:3a:ec:a9:16 brd ff:ff:ff:ff:ff:ff
    inet 10.0.3.1/24 brd 10.0.3.255 scope global br0
       valid_lft forever preferred_lft forever
    inet6 fe80::899:feff:fea2:c7b6/64 scope link 
       valid_lft forever preferred_lft forever
```
If the broadcast address is not set, execute the following as root. Check br0 setting after every VirtualBox startup.
```
ip link set br0 down
brctl delbr br0
brctl addbr br0
ip addr add 10.0.3.1/24 broadcast 10.0.3.255 dev br0
ip link set br0 up
firewall-cmd --zone=trusted --add-interface=br0
ip address show
```


Installing Gitian
------------------

Re-login as the user `gitianuser` that was created during installation.
The rest of the steps in this guide will be performed as that user.

```bash
sudo apt-get install ubuntu-vm-builder
```

**Note**: When sudo asks for a password, enter the password for the user `gitianuser` not for `root`.

Clone the git repositories for baricoin and Gitian.

```bash
git clone https://github.com/devrandom/gitian-builder.git
git clone https://github.com/baricoin/baricoin
git clone https://github.com/bitcoin-core/gitian.sigs.git
git clone https://github.com/bitcoin-core/bitcoin-detached-sigs.git
```

