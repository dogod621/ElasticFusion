# ElasticFusion #
Real-time dense visual SLAM system capable of capturing comprehensive dense globally consistent surfel-based maps of room scale environments explored using an RGB-D camera.
( Just make it build easyer for Windows via using CMake, and use librealsense2 instead of librealsense. Original Author: https://github.com/mp3guy/ElasticFusion  )

# Related Publications #
Please cite this work if you make use of our system in any of your own endeavors:

* **[ElasticFusion: Real-Time Dense SLAM and Light Source Estimation](http://www.thomaswhelan.ie/Whelan16ijrr.pdf)**, *T. Whelan, R. F. Salas-Moreno, B. Glocker, A. J. Davison and S. Leutenegger*, IJRR '16
* **[ElasticFusion: Dense SLAM Without A Pose Graph](http://thomaswhelan.ie/Whelan15rss.pdf)**, *T. Whelan, S. Leutenegger, R. F. Salas-Moreno, B. Glocker and A. J. Davison*, RSS '15

# 1. What do I need to build it? #

## 1.1. Windows - Visual Studio ##
* Windows 7/10 with Visual Studio 2017 (Though other configurations may work, but 2019 will crash in run time)
* [CMake] (https://cmake.org/)
* OpenGL
* [CUDA >= 7.0](https://developer.nvidia.com/cuda-downloads)
* [OpenNI2](https://github.com/occipital/OpenNI2)
* [SuiteSparse] (https://github.com/jlblancoc/suitesparse-metis-for-windows)
* [Eigen] (http://eigen.tuxfamily.org)
* [Pangolin](https://github.com/stevenlovegrove/Pangolin)
  * zlib (Pangolin can automatically download and build this)
  * libjpeg (Pangolin can automatically download and build this)
* [librealsense2] (https://github.com/IntelRealSense/librealsense) - Optional (for Intel RealSense cameras)

1. Create folders:<br/>
Your project folder structure should be this manner:<br/>
  Root/<br/>
  |<br/>
  |-ElasticFusion/ # Source code and CMakeLists of ElasticFusion<br/>
  | CMakeLists.txt # CMakeLists, you should follow this file to place headers, libs and dlls of Eigen, SuiteSparse and Pangolin<br/>
  |<br/>
  |-Dependency/    # The folder that is used to place dependency libraries: Eigen, SuiteSparse and Pangolin<br/>
  |<br/>
  |-build          # The folder that is used to build ElasticFusion<br/>
  |<br/>
  |-install        # Final folder that contains Libs and Exes<br/>

2. Install cmake and cuda, OpenNI2, RealSense SDK(Optional). 
3. Download and build from source Eigen, SuiteSparse.
4. Download and build from source Pangolin but pay attention to the following cmake settings. 
There will be a lot of dependencies where path was not found. 
That is OK except OPENNI2 and EIGEN3 (those should be set to valid paths, and make sure define "HAVE_EIGEN"). 
You also need to set MSVC_USE_STATIC_CRT to false in order to correctly link to ElasticFusion projects. 
5. Refer to my CmakeList.txt, copy the headers, libs and dlls of Eigen, SuiteSparse and Pangolin to right place ( I didnot wrie cmake find package for Eigen, SuiteSparse and Pangolin ) 
6. Run Cmkae and it will build all automatically for you.

# 2. How do I use it? #
There are three subprojects in the repo:

* The *Core* is the main engine which builds into a shared library that you can link into other projects and treat like an API. 
* The *GUI* is the graphical interface used to run the system on either live sensor data or a logged data file. 
* The *GPUTest* is a small benchmarking program you can use to tune the CUDA kernel launch parameters used in the main engine. 

The GUI (*ElasticFusion*) can take a bunch of parameters when launching it from the command line. They are as follows:

* *-cal <calibration>* : Loads a camera calibration file specified as *fx fy cx cy*.
* *-l <logfile>* : Processes the specified .klg log file.
* *-p <poses>* : Loads ground truth poses to use instead of estimated pose.
* *-c <confidence>* : Surfel confidence threshold (default *10*).
* *-d <depth>* : Cutoff distance for depth processing (default *3*m).
* *-i <icp>* : Relative ICP/RGB tracking weight (default *10*).
* *-ie <error>* : Local loop closure residual threshold (default *5e-05*).
* *-ic <count>* : Local loop closure inlier threshold (default *35000*).
* *-cv <covariance>* : Local loop closure covariance threshold (default *1e-05*).
* *-pt <photometric>* : Global loop closure photometric threshold (default *115*).
* *-ft <threshold>* : Fern encoding threshold (default *0.3095*).
* *-t <time>* : Time window length (default *200*).
* *-s <skip>* : Frames to skip at start of log.
* *-e <end>* : Cut off frame of log.
* *-f* : Flip RGB/BGR.
* *-icl* : Enable this if using the [ICL-NUIM](http://www.doc.ic.ac.uk/~ahanda/VaFRIC/iclnuim.html) dataset (flips normals to account for negative focal length on that data).
* *-o* : Open loop mode.
* *-rl* : Enable relocalisation.
* *-fs* : Frame skip if processing a log to simulate real-time.
* *-q* : Quit when finished a log.
* *-fo* : Fast odometry (single level pyramid).
* *-nso* : Disables SO(3) pre-alignment in tracking.
* *-r* : Rewind and loop log forever. 
* *-ftf* : Do frame-to-frame RGB tracking. 
* *-sc* : Showcase mode (minimal GUI).

Essentially by default *./ElasticFusion* will try run off an attached ASUS sensor live. You can provide a .klg log file instead with the -l parameter. You can capture .klg format logs using either [Logger1](https://github.com/mp3guy/Logger1) or [Logger2](https://github.com/mp3guy/Logger2). 

# 5. Datasets #

We have provided a sample dataset which you can run easily with ElasticFusion for download [here](http://www.doc.ic.ac.uk/~sleutene/datasets/elasticfusion/dyson_lab.klg). Launch it as follows:

```bash
./ElasticFusion -l dyson_lab.klg
```

# 6. License #
ElasticFusion is freely available for non-commercial use only.  Full terms and conditions which govern its use are detailed [here](http://www.imperial.ac.uk/dyson-robotics-lab/downloads/elastic-fusion/elastic-fusion-license/) and in the LICENSE.txt file.
