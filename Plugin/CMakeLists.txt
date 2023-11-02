paraview_add_plugin(PVSliceFilter
  REQUIRED_ON_SERVER
  VERSION "1.0"
  MODULES PVSliceFilter::vtkPVSliceFilter
  MODULE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/vtkPVSliceFilter/vtk.module")
