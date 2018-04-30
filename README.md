Lidar Viewer as Virtual Top-down View (2.5D)
==========================================

This project is to visualize LiDAR data as a 2D image (virtual top-down view) with the ability to zoom in/out and pan. 

The application reads XYZ or XYZC file (in which X,Y,Z is 3D coordinates and C is color) and visualize the results in the form of 2.5D image. It offers zoom and pan functionality.

The program was developed using MS Visual Studio .NET 2003 using the CImage class for presenting image. It was implemented using MFC with the document/view architecture.

The application screen is presented below.

![Application screen](https://github.com/stanathong/lidar_viewer/blob/master/Figure_application_screen.jpg)

### Test data

Test data can be found the folder __Testdata__. There are 3 datasets:

* CSite2_orig.txt     19MB
* CSite2_orig.txt     14MB
* CSite2_orig.txt     20MB

### Note
This LiDAR Viewer project is part of my assignment for LiDAR class (2009 Winter).
