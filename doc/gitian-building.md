Gitian building
================

*Setup instructions for a Gitian build of Baricoin Core using a VM or physical system.*

Gitian is the deterministic build process that is used to build the Baricoin
Core executables. It provides a way to be reasonably sure that the
executables are really built from the git source. It also makes sure that
the same, tested dependencies are used and statically built into the executable.

Multiple developers build the source code by following a specific descriptor
("recipe"), cryptographically sign the result, and upload the resulting signature.
These results are compared and only if they match, the build is accepted and provided
for download.

More independent Gitian builders are needed, which is why this guide exists.
It is preferred you follow these steps yourself instead of using someone else's
VM image to avoid 'contaminating' the build.

Table of Contents
------------------

- [Preparing the Gitian builder host](#preparing-the-gitian-builder-host)
- [Getting and building the inputs](#getting-and-building-the-inputs)
- [Building Baricoin Core](#building-baricoin-core)

Preparing the Gitian builder host
---------------------------------

The first step is to prepare the host environment that will be used to perform the Gitian builds.
This guide explains how to set up the environment, and how to start the builds.

Gitian builds are known to be working on recent versions of Ubuntu.
If your machine is already running Ubuntu 18.04, you can perform Gitian builds on the actual hardware.
Alternatively, you can install one of the supported operating systems in a virtual machine.

Please refer to the following documents to set up the operating systems and Gitian.

|                                   | Ubuntu 18.04                                                                             |
|-----------------------------------|------------------------------------------------------------------------------------|
| Setup virtual machine (optional)  | [Create Ubuntu VirtualBox](./gitian-building/gitian-building-create-vm-debian.md) |
| Setup Gitian                      | [Setup Gitian on Ubuntu](./gitian-building/gitian-building-setup-gitian-debian.md) |

Note that a version of `lxc-execute` higher or equal to 2.1.1 is required.
You can check the version with `lxc-execute --version`.

MacOS SDK
---------
In order to sign builds for MacOS, you need to download the free SDK and extract a file. The steps are described [here](./gitian-building/gitian-building-mac-os-sdk.md). Alternatively, you can skip the OSX build by adding `--os=lw` below.

Initial Gitian Setup
--------------------
The `gitian-build.py` script will checkout different release tags, so it's best to copy it:

```
cp baricoin/contrib/gitian-build.py .
```

You only need to do this once:

```
./gitian-build.py --setup
```

By default, base VM of Ubuntu bionic will be created.


Build binaries
--------------
Windows and OSX have code signed binaries, but those won't be available until a few developers have gitian signed the non-codesigned binaries.

To build the most recent tag:

All:

 `./gitian-build.py --detach-sign --no-commit -b satoshi b0.20.0`

Linux:

 `./gitian-build.py --detach-sign --no-commit -b -o l satoshi b0.20.0`

Windows:

 `./gitian-build.py --detach-sign --no-commit -b -o w satoshi b0.20.0`

OSX:

 `./gitian-build.py --detach-sign --no-commit -b -o m satoshi b0.20.0`

How to see options:

`./gitian-build.py --help`

Where `satoshi` is your Github name and `b0.20.0` is the most recent tag. 
To speed up the build, use `-j 5 -m 5000` as the first arguments, where `5` is the number of CPU's you allocated to the VM plus one, and 5000 is a little bit less than then the MB's of RAM you allocated.


Note
----

- Do not run other programs as much as possible. It may be better to stop anti-virus as well.
- It is better to recreate the new gitian-builder folder before starting the next build operation. In that case, backing up the gitian-builder folder after setup can shorten the time.
