<div align="center">
  
  <p>
    <h1>
      <a href="https://github.com/imsamroy/Cryptographer">
        <img src="Cryptographer.svg" width=128 alt="Cryptographer" />
      </a>
      <br />
      Cryptographer
    </h1>
    <h4>Cipher any message and be able to decipher it</h4>
  </p>

  <p>
    <a href="https://github.com/imsamroy/Cryptographer/releases/latest">
      <img src="https://img.shields.io/github/release/imsamroy/Cryptographer.svg" alt="Latest Release" />
    </a>
    <a href="https://github.com/imsamroy/Cryptographer/releases">
      <img src="https://img.shields.io/github/downloads/imsamroy/Cryptographer/total.svg" alt="Total Downloads" />
    </a>
    <a href="https://github.com/imsamroy/Cryptographer/blob/master/LICENSE">
      <img src="https://img.shields.io/github/license/imsamroy/Cryptographer.svg" alt="License" />
    </a>
    <a href="https://github.com/imsamroy/Cryptographer/actions/workflows/linux.yml">
      <img src="https://img.shields.io/github/actions/workflow/status/imsamroy/Cryptographer/linux.yml?label=linux" alt="Linux Build Status" />
    </a>
    <a href="https://github.com/imsamroy/Cryptographer/actions/workflows/windows.yml">
      <img src="https://img.shields.io/github/actions/workflow/status/imsamroy/Cryptographer/windows.yml?label=windows" alt="Windows Build Status" />
    </a>
    <a href="https://github.com/imsamroy/Cryptographer">
      <img src="https://img.shields.io/github/stars/imsamroy/Cryptographer" alt="GitHub Stars" />
    </a>
  </p>

</div>

## Preview

[cryptographer-test.webm](https://github.com/imsamroy/Cryptographer/assets/92716271/a46f98fc-f522-4239-9020-def658f25beb)

## Supported platforms/systems

Cryptographer is currently supported on the following platforms/systems:

- GNU/Linux (x86_64)
- Windows (x86_64)

## Installation

Currently, Cryptographer can be installed easily on most GNU/Linux distributions and Windows systems

### Installing on GNU/Linux systems:

For GNU/Linux users, Cryptographer is available as an AppImage which can run on all common Linux distributions with just these 3 simple steps:

- Download the latest Linux release for Cryptographer from GitHub releases: https://github.com/imsamroy/Cryptographer/releases/latest/download/Cryptographer-Linux-x86_64.AppImage

- In your file manager right click the AppImage file, go to "Properties" then "Permissions" and mark it as executable or allow it to run as executable. Alternatively, you can open your terminal in the directory where you downloaded it and run:
  ```shell
  chmod a+x Cryptographer*.AppImage
  ```

- Now that it is allowed to run as an executable, the application is ready to launch! Simply double-click on the AppImage file, or alternatively in your terminal run:
  ```shell
  ./Cryptographer*.AppImage
  ```

### Installing on Windows systems:

For Windows users, Cryptographer can be installed on your Windows system using the official installer.

- Download the latest Windows release for Cryptographer from GitHub releases: https://github.com/imsamroy/Cryptographer/releases/latest/download/Cryptographer-Windows-Installer-x86_64.exe

- Run the exe file which you downloaded and it will open the Cryptographer Windows Installer which will guide you through the very simple Windows installation process.

- Once installation is finished, just open your Start Menu and you will find "Cryptographer" there installed as an app.

**PLEASE NOTE:** When it will ask you for the installation directory, please choose an empty folder which you created or else, leave it as the default one which will be given. If you choose a folder which is not empty, during uninstallation it will delete that entire folder and it will delete your other files too which will be there in that folder. When you try to select a non-empty folder you will get a warning from the installer so please follow it.

#### Uninstall from Windows system:

For Windows users, Cryptographer can be uninstalled using the Maintenance Tool which is added in your system during the installation process.

- Open the folder which you selected as the installation directory during the installation process

- You will find a file named like "maintenancetool.exe" there, run it. After it opens, select Remove/Uninstall components and it will guide you through the uninstallation.

**PLEASE NOTE:** Only proceed with uninstallation if when selecting the installation directory during installation, you had not got any warning from the installer that you are selecting a non-empty folder. Uninstalling will delete the entire folder which you had chosen as the installation directory so please ensure that the folder you had chosen during installation was an empty folder you created or, you left it as the default value.

## Building from source

- Follow any online guide to install the following on your system to be able to build from source:
  - Qt (either Qt5 or Qt6)
  - GCC (for Linux), or MinGW (for Windows)

- On Linux if you have git installed then run:
  ```shell
  git clone https://github.com/imsamroy/Cryptographer.git
  ```
  On Windows download and extract this zip: https://github.com/imsamroy/Cryptographer/archive/refs/heads/main.zip

- Now you have the project source code in the directory named "Cryptographer" or "Cryptographer-main", cd into that directory in your Terminal/Command Prompt and then run:
  ```shell
  qmake -r Cryptographer.pro -o Makefile
  ```
 Now on Linux run ```make``` or, on Windows run ```mingw32-make```

- Now you should see the executable for your system in that folder or in a folder named something like "release" in that folder
