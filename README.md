# DiskCacheControl
This is tiny console windows utility with only one function: controls disk onboard write cache.

Usage examples:  
**dcc.exe C: on**  
turns on onboard disk cache of HDD on which resides C disk  
**dcc.exe C:**  
shows current cache status  
**dcc.exe C: off**  
turn it off  

# System requirements
Tested on SATA HDD in AHCI mode on Win10 x64 but should work on other versions.
x64 build provided just in case, 32-bit binary works fine on x64 windows.

# Development tools
Visual Studio Express 2015 (14.0) for Desktop
