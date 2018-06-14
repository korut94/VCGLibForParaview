# VCGLibForParaview
VCGLibForParaview is an university project for the Scientific and Large Data
Visualization course of the University of Pisa.

The main goal is bringing the VGC functionalities inside Paraview but also
provide a set of utilities to make really confortable passing from the VCG 
world to the VTK one and viceversa.

## Install
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

## Collaborate
Paraview allow you to extend it by `Filters` but also `Reader` and `Writer`, 
as explain here [Paraview/Plugin HowTo](https://www.paraview.org/Wiki/ParaView/Plugin_HowTo).
Since that the actual version of the plugin exports only Paraview's filters
only their will be covered by this section.

### Filters
