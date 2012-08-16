1. Build C libraries
====================

* Build `labvvis.framework`.

  In the topmost directory

      $ make


2. Connect to LabVIEW
=====================

* Copy `src/labvvis/labview/labvvis` to `/Applications/National\ Instruments/LabVIEW\ 2011/user.lib/` (This is the LabVIEW user library).


3. Deploy
=========

* You must copy `labvvis.framework` to your `/Library/Frameworks` folder, otherwise LabVIEW will not find the required libraries.


4. Test LabVVIS
===============

* After you perform all the steps in 1-3, you should be able to use LabVVIS. 
* Note that, LabVIEW will not reload changes made to the framework until you restart it.
