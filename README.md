WebLights
=========

This is a project that can be used to control lights from the web using Arduino and the Nanoshields system from Circuitar.
It includes a web server written in [Python](https://www.python.org/), using the [Django](https://www.djangoproject.com/) framework, that stores the current status of one or more lights.
The status of the lights on the web server is accessed by the Arduino, via an Ethernet shield, that uses it to control the lights.
The web server provides a web interface that can be accessed with a browser to update the status of the lights and control them in real time.
An administrative web interface is provided to add or remove lights.

This project was built and tested using Nanoshields from http://www.circuitar.com.br, but it can be easily adapted for use with other compatible hardware, or with hardware built from scratch using an official Arduino Ethernet shield.

Here is a description of the files/subdirectories:

- `arduino`: Arduino sketches
    - `WebDimmer`: an Arduino sketch to dim a light using a triac and a zero crossing detector.
    - `WebLight`: an Arduino sketch to implement a simple on/off switch using a relay.
- `static`: images, CSS and Javascript code.
- `templates`: contains the HTML template for the light control panel.
- `WebLights`: main Django project and settings.
- `WebLightsPanel`: Django app for the light control panel.

To run this project you will need the following Arduino libraries, in addition to the standard ones:

- [Nanoshield_EEPROM](https://github.com/circuitar/Nanoshield_EEPROM): used to access the MAC address embedded in the Ethernet Nanoshield. You don't need this if you are using an official Ethernet shield, but you will need to hard-code you MAC addresses in the Arduino sketch.
- [TimerOne](https://code.google.com/p/arduino-timerone/downloads/list): used only in the `WebDimmer` Arduino sketch.

## Installation Instructions

These are installation instructions for a simple setup of a web server in your local machine. To create a robust installation, please check the "WARNING" section below.

1. Install Python 2.7
    1. On Windows, download and install [this](http://python.org/ftp/python/2.7.6/python-2.7.6.msi)
    1. On Mac OSX, follow [these instructions](http://docs.python-guide.org/en/latest/starting/install/osx/)
    1. On Linux, follow [these instructions](http://docs.python-guide.org/en/latest/starting/install/linux/)
1. Install `pip`
    1. On Windows:
        1. Download and install `setuptools` for Python 2.7 from [here](http://www.lfd.uci.edu/~gohlke/pythonlibs/#setuptools)
        1. Download and install `pip` from [here](http://www.lfd.uci.edu/~gohlke/pythonlibs/#pip)
        1. Add `C:\Python27` and `C:\Python27\Scripts` to your PATH environment variable:
            1. Click Start, start typing "Edit the system environment variables" and open it
            1. Click "Environment Variables..."
            1. In "System Variables", select "PATH" and click "Edit..."
            1. At the end of "Variable value:", add `;C:\Python27\Scripts` and click OK three times
    1. On Linux or on Mac OSX, you can find the instructions Python installation links above
1. Install Django 1.6
    1. On Windows:
        1. Open an administrator command prompt by right-clicking in "Command Prompt" and selecting "Run as Administrator"
        1. Run `pip install Django`
    1. On Linux of Mac OSX:
        1. Open the terminal and run `sudo pip install Django`
1. Download the ZIP with all the source code or clone this GIT repository
1. Open a command prompt and go to the directory where this README file is, and where manage.py resides
1. Run `python manage.py syncdb`
    1. When asked to create a superuser, answer "yes"
    1. Select a username, e-mail and password of your choice
1. Find the IP address of your computer in the local network running `ipconfig` in Windows or `ifconfig` in Mac or Linux
    1. For the instructions below, we will assume your IP is `192.168.0.10`, so please replace it as needed
1. Run `python manage.py runserver 192.168.0.10:8000`
    1. On Windows, if a dialog shows up asking for permissions, select "Allow access"
    1. If needed, allow access to port 8000 in your firewall by adding a rule (or disable the firewall for a quick test, but it's not recommended)
1. Open you browser at http://192.168.0.10:8000/admin, and log in using the username and password you created above
1. Upload the Arduino code in the `arduino` folder
1. Open you browser at http://192.168.0.10:8000/admin and enjoy!

### WARNING

This is a sample application and is not suitable for a production environment. It offers no authentication or other security mechanisms. If you wish to run it in a secure production environment, here are some recommendations:

- Run it on a physically isolated network if you just need a local application
- Use a proper production deployment for Django instead of the development web server outlined here (there are lots of tutorial for that on the web)
- If you are running it remotely over the web, do so through a secure VPN
- Take care of database size, and create some job to delete old sensor data
- Create proper firewall rules in your server
- This code is provided free of charge, so run it at your own risk!

---

Copyright (c) 2014 Circuitar

This software is released under an MIT license. See the attached LICENSE file for details.
