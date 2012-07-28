A. Build C libraries
--------------------

1. Build "common" target from common/common.xcodeproj
2. Build "vvm" target from vvm/vvm.xcodeproj
3. Build "vvis" target from vvis/vvis.xcodeproj
4. Build "labvvis" target from labvvis/labvvis.xcodeproj
5. Build "labvvis-cin" target from labvvis/labvvis.xcodeproj

B. Connect to LabVIEW
---------------------

1. Copy labvvis/labview/labvvis to /Applications/National\ Instruments/LabVIEW\ 8.2/user.lib/ (This is the LabVIEW user library)

To reload the Camera Control CIN:
2. Reload the CIN (labvvis/Private/Camera Control.vi) using labvvis/labvvis-cin.lsb (built in A.5)


C. Deploy
---------

1. You must copy
	vvm.framework (built in A.2)
	vvis.framework (built in A.3)
	labvvis.framework (build int A.4)
   to your /Library/Frameworks folder, otherwise LabVIEW will not find the required libraries.


D. Test LabVVIS
---------------

* After you perform all the steps in A-C, you should be able to use LabVVIS. 
* To make things more troublesome, LabVIEW will not reload changes made to the framework until you restart it.
