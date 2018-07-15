# VCGLibForParaview
VCGLibForParaview is an university project for the Scientific and Large Data
Visualization course of the University of Pisa.

The main goal is bringing the VCG (http://vcg.isti.cnr.it/vcglib/)
functionalities inside Paraview but also providing a set of utilities to make 
really confortable moving between the VCG world and the VTK one or viceversa.
The result is a Paraview plugin with the many advanced VCG algorithms
encapsulated in single place and completely available through the
pipeline-based framework. 

## Table of contents
* [Install](#install)
* [Contents](#contents)
  * [Uniform Random Sampling](#uniform-random-sampling)
  * [Hausdorff Distance](#hausdorff-distance)
* [How to contribute](#how-to-contribute)
  * [Filters](#filters)

## Install
Once downloaded the source code, go into the directory

```
cd VCGLibForParaview
```

and get all the dependencies by running:

```
git submodules update --init
```

this starts the download of the VCGLib from the `develop` branch.

In order to build the Paraview plugin, make a directory (outside the source 
directoy preferably) where the `MakeFile` will be generated:

```
mkdir ../VCGLibForParaview_build && cd ../VCGLibForParaview_build
```

then run `ccmake` from inside the current directoy specifing as argument the 
source directory

```
ccmake ../VCGLibForParaview
```

Generally all the configurations are set correctely, but if you want build the
plugin in `debug` mode you have to set the option `CMAKE_BUILD_TYPE` to
`Debug`. After this type `c` (configure) and `g` (generate the makefile and
exit).

Finally build the plugin running:

```
make
``` 

and inside the directory the `libVCG` shared library should be appeared.

Now open Paraview and load the library through the Plugin Manager
(`Tools > Manage Plugin`) by clicking in `Load New...` and selecting the
shared library. If no errors are occured, the entry `VCG` is shown with status 
`Loaded` and in the `Filters` section the `VCG` group is added with
all the filters of this plugin as well.

## Contents
Since the actual small numbers of the functionalities available through the
puglin, the relative documentation about them is exposed here waiting future
contriubutes to move all into a wiki's page.

### Uniform Random Sampling

### Hausdorff Distance


## How to contribute
Paraview allow you to extend it by `Filter`s but also `Reader`s and `Writer`s, 
as explain here [Paraview/Plugin HowTo](https://www.paraview.org/Wiki/ParaView/Plugin_HowTo).
Since the actual version of the plugin exports just Paraview's filters, only
they will be covered by this section.

### Filters
It's highly recommended reading the Paraview documentation [Adding a Filter](https://www.paraview.org/Wiki/ParaView/Plugin_HowTo#Adding_a_Filter) on the Paraview Wiki page *Plugin HowTo*,
since the following instruction steps are derived from those ones
described in that page.

First of all, create a directory over the path `src/filter` with the name of
the new filter (e.g. `src/filters/NewVCGAlgorithm`) and put there all the
source code files. As name for the filter class use the convention `vcgXXXFilter`
(e.g. `vcgNewVCGAlgorithmFilter`) in order to distinguish it from the VTK
classes.

Then there are two files to update to include the new filter inside the plugin:

#### `CMakeLists.txt`
Add the path of your directory filter inside the `include_directories` statement
and add the path of the filter `.cpp` file in `SERVER_MANAGER_SOURCES`.

```CMake
include_directories(
  vcglib
  ...
  src/filters/NewVCGAlgorithm
)

...

ADD_PARAVIEW_PLUGIN(
  VCG
  "1.0"
  SERVER_MANAGER_XML VCGPlugin.xml
  SERVER_MANAGER_SOURCES  ...
                          src/filters/NewVCGAlgorithm/vcgNewVCGAlgorithmFiler.cpp
)
```

#### `VCGPlugin.xml`
To make interactable your plugin from Paraview you have to add a `SourceProxy`
tag which describe its componenents (e.g. input ports, output ports, parameters,
etc). The body of the `SourceProxy` should be like as follow:

```XML
<SourceProxy name="NewVCGAlgorithmFilter"
             class="vcgNewVCGAlgorithmFilter"
             label="New VCG Algorithm">
  
  <!-- Put your stuff here -->
  
  <Hints> 
    <ShowInMenu category="VCG" />
  </Hints>
</SourceProxy>
```
Remember to declare the `ShowInMenu` tag with the `category` attribute set 
to `VCG`, so the filter will be grouped with the other ones in the `VCG`
section of the Paraview `Filters` menu.
