# Virtual Mouse Setup

Mickey is a program creating a virtual mouse in the system, allowing you to controll the mouse cursor with your head.
For the time being, it is available only on Linux, but Mac version should be available in the future.

To allow the creation of of the virtual mouse device, an access to the /dev/uinput file is needed. For that reason the file 99-Mickey.rules
is provided; you need to copy it to the udev rules directory (on Ubuntu it is /lib/udev/rules, but other distros might differ a bit in
this respect). For security reasons, access to the /dev/uinput file is granted to the root and users belonging to the uinput group.
For that reason you have to create uinput group and your user to it - please consult your distribution's documentation to find a correct
way of doing that. The following commands should work on most distros - just change "joe" to your user name:

```
sudo groupadd uinput
sudo usermod -aG uinput joe
```

When that is done, you should restart the machine to let the udev rule do its job.

Now you can start Mickey - please consult its documentation for the rest of the setup.

## Additional tips for Mickey

Mickey can use any of the devices Linuxtrack supports; however only SmatrNav 4 device allows you to also perform clicks - using additional
periferies like foot pedals or sip'n'puff switches.

Although Mickey can work using any model for tracking that Linuxtrack can use, it is best to use the single point model as it offers
most precision (and the additional degrees of freedom aren't used anyway).

Before running Mickey, make sure the tracking is working well - you can verify that in the 3D preview tab of the ltr\_gui's Tracking window.
