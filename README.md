# VCGLibForParaview
VCGLibForParaview is an university project for the Scientific and Large Data
Visualization course of the University of Pisa.

The main goal is bringing the VGC functionalities inside Paraview but also
provide a set of utilities to make really confortable moving between the VCG 
world and the VTK one and viceversa.

## Installation
Once downlaod the source code, go into the directory

```
cd VCGLibForParaview
```

and get all the dependencies by running

```
git submodules update --init
```

this starts the download of the VCGLib from the `develop` branch.

In order to build the Paraview plugin, make a directory (outside the  source 
directoy preferably) where the `MakeFile` will be generated

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

Finally start the build running

```
make
``` 

and inside the directory a `libVCG` shared library should be created.

Now open Paraview and load the library throught the Plugin Manager
(`Tools > Manage Plugin`) by clicking in `Load New...` and selecting the
shared library. If no errors are occured the entry `VCG` is shown with status 
`Loaded` and in the `Filters` section the `VCG` group has been added with
all the filters of this plugin as well.

## Contributing
Paraview allow you to extend it by `Filters` but also `Reader` and `Writer`, 
as explain here [Paraview/Plugin HowTo](https://www.paraview.org/Wiki/ParaView/Plugin_HowTo).
Since that the actual version of the plugin exports just Paraview's filters
only their will be covered by this section.

### Filters
It's highly recommended reading the Paraview documentation [Adding a Filter](https://www.paraview.org/Wiki/ParaView/Plugin_HowTo#Adding_a_Filter) on the *Plugin HowTo* Paraview Wiki page,
since the following instruction steps are derived from those ones
described in that page.

First of all, create a directory over the path `src/filter` with the name of
the new filter (e.g. `src/filters/NewVCGAlgorithm`) and put there all the
source code files. As name for the filter class use the form `vcgXXXFilter`
(e.g. `vcgNewVCGAlgorithmFilter`) in order to distinguish it from the VTK
classes.

Then there are two files to update to include the new filter inside the plugin's
library:

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
section of the Paraview `Filter` menu.
