# DiskCacheControl
This is tiny console (CLI) utility for Windows® platform for controlling onboard HDD write caching. No reboot required.

Usage examples:  
**dcc.exe C: on**  
turns on onboard disk caching of HDD on which resides C disk  
**dcc.exe C:**  
shows current cache status  
**dcc.exe C: off**  
turn it off back  
"on" and "off" are case-sensitive  
exit code 0 means success or already in desired state  

# System requirements
Tested on SATA HDD in AHCI mode on Win10 x64 but should work on other versions.
x64 build provided just in case, x86-32 bit binary works fine on x64 Windows®.

# Development tools
Visual Studio Express 2015 (14.0) for Desktop
