LabVVIS
Version 1.0
December 2005
Bing-Chang Lai
==============

Getting Started
---------------
1. To install, open the "LabVVIS.pkg" installation program.

2. IMPORTANT! You must copy the labvvis directory in /Applications/LabVVIS to the user.lib directory of your LabVIEW folder. For example, if you installed LabVIEW 7.1 in the default location, you need to copy the labvvis folder to "/Applications/National Instruments/LabVIEW 7.1/user.lib".

3. Try the examples files in /Applications/LabVVIS/Examples to verify your installation is working.

4. Register to unlock LabVVIS if necessary. Without registration, you are limited to 5 images per run of LabVIEW.


Registration
------------
Without registration, LabVVIS will allow you to create up to 5 images. This means that at best, you will be able to run your program 5 times, before you need to restart LabVIEW.

To register for a key, use the Register LabVVIS program in the /Applications/LabVVIS directory.


Uninstallation
--------------
An uninstall.sh is provided to remove the files installed. Please note that
you will need to have administrative rights in order to run this script.

IMPORTANT! Please read uninstall.sh before using, to make sure you are
happy with what is being deleted. As it is a UNIX script, files will
disappear immediately without going to the Recycle Bin.


Notes
-----

1. To compile with GCC 4.0, cintools/fundtypes.h, line 34 was changed from
		typedef short double	float64;
   to
		typedef /*short*/ double	float64;

