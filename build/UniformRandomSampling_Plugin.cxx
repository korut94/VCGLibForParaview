/*=========================================================================

   Program: ParaView
   Module:    pqParaViewPlugin.cxx.in

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2. 

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

========================================================================*/
#include "UniformRandomSampling_Plugin.h"

#include "vtkObjectFactory.h"
#include "vtkPVPluginLoader.h"

#include "/home/andreamnt94/Project/UniformRandomSampling/build/vtkSMXML_UniformRandomSampling.h"

#include "/home/andreamnt94/Project/UniformRandomSampling/build/UniformRandomSampling_doc.h"

namespace
{
  // This ensures that when the shared library for this plugin is
  // unloaded during finalization sequence, it notifies the vtkPVPluginLoader
  // so it does not double-dlclose() an already unloaded plugin.
  // This does not affect static builds and hence we don't need to worry about
  // making sure this instance gets created in static builds.
  static class UniformRandomSampling_PluginCleaner
    {
  public:
    UniformRandomSampling_PluginCleaner()
      {
      }
    ~UniformRandomSampling_PluginCleaner()
      {
      // The plugin library is being unloaded.
      // Let the plugin loader know so it doesn't try to unload it again.
      vtkPVPluginLoader::PluginLibraryUnloaded("UniformRandomSampling");
      }
    } UniformRandomSampling_PluginCleaner_Instance;
}

//-----------------------------------------------------------------------------
// Used to push a string returns from a function to a vector.
template <class T, class F>
void PushBack(T& vector, F& fun)
{
  char* text = fun();
  vector.push_back(text);
  delete []text;
}

//-----------------------------------------------------------------------------
#ifdef plugin_type_servermanager
# if defined(INITIALIZE_WRAPPING) || defined(INITIALIZE_EXTRA_CS_MODULES)
#include "vtkClientServerInterpreter.h"
#include "vtkClientServerInterpreterInitializer.h"

#   if defined(INITIALIZE_WRAPPING)
extern "C" void UniformRandomSampling_Initialize(vtkClientServerInterpreter *);
#   endif



extern "C" void VTK_EXPORT UniformRandomSampling_CombinedInitialize(
  vtkClientServerInterpreter *interp)
{
#   if defined(INITIALIZE_WRAPPING)
  UniformRandomSampling_Initialize(interp);
#   endif

  // Now initialize any extra kits as requested.
  
}
# endif
#endif

//-----------------------------------------------------------------------------
void UniformRandomSampling_Plugin::GetBinaryResources(
  std::vector<std::string>& resources)
{
  PushBack(resources, UniformRandomSampling_doc);

  (void)resources;
}

//-----------------------------------------------------------------------------
#ifdef plugin_type_gui


#endif

//-----------------------------------------------------------------------------
#ifdef plugin_type_servermanager
vtkClientServerInterpreterInitializer::InterpreterInitializationCallback
UniformRandomSampling_Plugin::GetInitializeInterpreterCallback()
{
#if defined(INITIALIZE_WRAPPING) || defined(INITIALIZE_EXTRA_CS_MODULES)
  return UniformRandomSampling_CombinedInitialize;
#else
  return NULL;
#endif
}

//-----------------------------------------------------------------------------
void UniformRandomSampling_Plugin::GetXMLs(std::vector<std::string> &xmls)
{
  PushBack(xmls, UniformRandomSamplingUniformRandomSamplingInterfaces);

  (void)xmls;
}
#endif

//-----------------------------------------------------------------------------
#ifdef plugin_type_gui
QObjectList UniformRandomSampling_Plugin::interfaces()
{
  QObjectList ifaces;
#ifdef PUSH_BACK_PV_INTERFACES
  PUSH_BACK_PV_INTERFACES(ifaces);
#endif
  return ifaces;
}
#endif

//-----------------------------------------------------------------------------
#ifdef plugin_type_python


void UniformRandomSampling_Plugin::GetPythonSourceList(std::vector<std::string>& modules,
  std::vector<std::string>& sources,
  std::vector<int> &package_flags)
{
  const char *moduleNames[] = {
    
  };
  char *moduleSources[] = {
    
  };
  const int packageFlags[] = {
    
  };

  int num_modules = sizeof(moduleNames)/sizeof(const char *);
  for (int cc=0; cc < num_modules; cc++)
    {
    modules.push_back(moduleNames[cc]);
    sources.push_back(moduleSources[cc]);
    package_flags.push_back(packageFlags[cc]);

    // free allocated memory.
    delete moduleSources[cc];
    moduleSources[cc] = NULL;
    }
}
#endif

//-----------------------------------------------------------------------------
UniformRandomSampling_Plugin::UniformRandomSampling_Plugin()
{
#if !defined(BUILD_SHARED_LIBS) && defined(plugin_type_gui)
  // For static builds, initialize the Qt resources as well as the Qt plugin.
  
#ifdef plugin_type_gui_qt4
  // Qt 4
  Q_IMPORT_PLUGIN(UniformRandomSampling)
#else
  // Qt 5
  Q_IMPORT_PLUGIN(UniformRandomSampling_Plugin)
#endif

#endif
}

//-----------------------------------------------------------------------------
// Mark this as a ParaView-ServerManager plugin.
PV_PLUGIN_EXPORT(UniformRandomSampling, UniformRandomSampling_Plugin)

//-----------------------------------------------------------------------------
#if defined(plugin_type_gui) && defined(plugin_type_gui_qt4)
// Required to mark this as a Qt plugin.
Q_EXPORT_PLUGIN2(UniformRandomSampling,  UniformRandomSampling_Plugin)
#endif
