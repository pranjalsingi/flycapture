# wireless-microscope

Step 1: Download Flycapture SDK from https://www.ptgrey.com/support/downloads
	(Camera : Chameleon 3, Model : CM3-U3-50S5C-CS, OS : Linux Ubuntu 16.04)
	
	Download the FlyCapture 2.11.3.121 SDK - Linux Ubuntu 16.04 (64-bit) from Software

Step 2: Goto https://www.ptgrey.com/tan/10685 and follow the steps
	- Install and configure linux
		* sudo apt-get install libgtkmm-2.4-dev libglademm-2.4-dev libusb-1.0
		* sudo apt-get install libraw1394-11 libgtkmm-2.4-1v5 libglademm-2.4-1v5 libgtkglextmm-x11-1.2-dev libgtkglextmm-x11-1.2 libusb-1.0-0

	- Check Linux version -> If its 3.5.0 or newer then its fine or follow steps at the URL metioned above

	- Configure USBFS

	-Install flycapture SDK
 
Step x: Uncompress it:
	tar zxvf flycapture2-2.11.3.121-amd64-pkg.tgz
