cmake_minimum_required(VERSION 2.6)


if(COMMAND cmake_policy)
	cmake_policy(SET CMP0011 OLD)
endif(COMMAND cmake_policy)

IF (NOT SURILIB_INCLUDE_DIR)
	SET(SURILIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
	SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/tools)
ENDIF(NOT SURILIB_INCLUDE_DIR)
IF (NOT SURILIB_FOUND)
	INCLUDE("tools/UseSuriLib.cmake")
ENDIF(NOT SURILIB_FOUND)

OPTION(SURILIB_SHARED_BUILD "Compilar SuriLib como biblioteca dinamica." ON)

IF(SURILIB_SHARED_BUILD)
  SET(SURI_LIBRARY_TYPE SHARED)
  ADD_DEFINITIONS(-DDLL_EXPORTS)
ELSE(SURILIB_SHARED_BUILD)
  SET(SURI_LIBRARY_TYPE STATIC)
ENDIF(SURILIB_SHARED_BUILD)


INCLUDE("test/CMakeFiles.txt")
SET(EXTRA_SOURCES ${EXTRA_SOURCES} ${SURITEST_SOURCES})
INCLUDE("core/CMakeFiles.txt")
SET(EXTRA_SOURCES ${EXTRA_SOURCES} ${SURICORE_SOURCES})
INCLUDE("widgets/CMakeFiles.txt")
SET(EXTRA_SOURCES ${EXTRA_SOURCES} ${SURIWIDGETS_SOURCES})
INCLUDE("da/CMakeFiles.txt")
SET(EXTRA_SOURCES ${EXTRA_SOURCES} ${SURIDATAACCESS_SOURCES})
#INCLUDE("resources/CMakeFiles.txt")

include_directories("third_party/Eigen/include")

WXWIDGETS_ADD_RESOURCES(SuriLib resources/libResources.xrc
	resources/AddColumnPart.xrc
	resources/AdjustTool.xrc
	resources/AngleMapperParameters.xrc
	resources/AnimationTaskTool.xrc
	resources/AnimationTool.xrc
	resources/AnotationProperties.xrc
   resources/BandCombinationSelection.xrc
	resources/BandMath.xrc
   resources/BandSelection.xrc
	resources/Buffer.xrc
   resources/C3DToolbar.xrc
	resources/CategorizedVectorStytlePart.xrc
	resources/ChoiceSelection.xrc
	resources/ClassificationOptions.xrc
	resources/CoordinateNavigator.xrc
	resources/ConditionalVectorStyle.xrc
	resources/ContextMenu.xrc
	resources/Convolution.xrc
	resources/CoregisterTool.xrc
	resources/CorregisterGcpGeneration.xrc
	resources/CsvFormatSelectionPart.xrc
	resources/DatumEdition.xrc 
	resources/DualViewerSelection.xrc
	resources/DynamicViewerWidget.xrc
	resources/EditCoordinates.xrc
	resources/ElementMenu.xrc
	resources/EllipsoidEdition.xrc
	resources/EquationInSituEditor.xrc
   resources/FieldLayerSelection.xrc
	resources/FilterLibrary.xrc
   resources/FilterLibrarySelectionPartEx.xrc
	resources/FilterPagePanel.xrc
	resources/FilterSelection.xrc
	resources/GeneralPixelInformation.xrc
	resources/GeorreferenceProperties.xrc
	resources/GenericHtmlViewer.xrc
	resources/GenericProperties.xrc
	resources/HotlinkEditor.xrc
	resources/IndexSelection.xrc
	resources/KMeansParameters.xrc
	resources/LatitudeLongitudeInput.xrc
	resources/LayerList.xrc
	resources/LayerTable.xrc
	resources/LibraryItemSelection.xrc
	resources/LibraryNameEdition.xrc
	resources/MainMenu.xrc
	resources/MapTool.xrc
	resources/MapToolSetup.xrc
	resources/MaxLikelihoodParameters.xrc
	resources/MeasurementTool.xrc
	resources/MetadataPropertiesPart.xrc
	resources/PixelInformation.xrc 
	resources/ProcessHistoryPart.xrc
	resources/EquationVariableMappingWidget.xrc
	resources/SingleClasificationParameters.xrc resources/ClassMappingTable.xrc
	resources/TransformationModelParametersPart.xrc resources/UniversalCoordinatesInput.xrc
	resources/DecimalCoordinateInputPart.xrc resources/ClassFusion.xrc
	resources/SubsetCoordinateInput.xrc resources/HtmlTree.xrc
	resources/MaskSelection.xrc resources/SpatialSelection.xrc
	resources/VectorToolbar.xrc
	resources/ClassificationToolbar.xrc resources/ClassificationClassSelection.xrc
	resources/GeoReference.xrc resources/VectorTableEditor.xrc 
	resources/CoregisterTableToolbar.xrc resources/VectorTableToolbar.xrc 
	resources/GeoReferenceTableToolbar.xrc resources/WarpParameters.xrc
	resources/BandEnhancement.xrc resources/MeassureWidget.xrc resources/About.xrc
	resources/Navigation.xrc resources/UniversalLibraryEditor.xrc
	resources/FrameWidget.xrc resources/DensitySliceEdition.xrc
	resources/VectorTableStructureEdition.xrc resources/HtmlReport.xrc
	resources/LibraryItemEditorDebug.xrc resources/TableEditorPart.xrc
	resources/Viewer2DSelector.xrc resources/SingleLayerBandSelection.xrc
	resources/MultiLayerBandSelection.xrc resources/RasterPixelInformationPart.xrc
	resources/CoregisterOrderSelect.xrc resources/GeoreferenceRefSys.xrc
	resources/ColorTableSelection.xrc
	resources/GeoreferenceTool.xrc
	resources/VectorFormatSelectionPart.xrc
	resources/MapEditionToolbar.xrc
	resources/SearchWidget.xrc
	resources/QueryBuilderPart.xrc
	resources/GenerateVectorConditionClass.xrc
	resources/ImageProperties.xrc
	resources/ManualEnhancement.xrc
	resources/PlotterWidget.xrc
	resources/OffsetConfig.xrc
	resources/OutputPageConfig.xrc
	resources/OutputSelection.xrc
	resources/QueryBuilder.xrc
	resources/PancromaticFussion.xrc
   resources/PartContainer.xrc
	resources/PixelLineInput.xrc
	resources/PrincipalComponentesAnalysis.xrc
	resources/ProjectedSystemEdition.xrc
	resources/RasterFeaturesWidget.xrc
	resources/RasterGenericImport.xrc
	resources/RasterOffsetConfig.xrc
	resources/RasterOutputSize.xrc
	resources/RasterPixelInformation.xrc
	resources/RasterProperties.xrc
	resources/RegistrationCodeInputPart.xrc
	resources/Setup3D.xrc
	resources/SetupGeneral.xrc
	resources/SetupMeasurementStyle.xrc
	resources/SetupVectorialEditionStyle.xrc
	resources/SetupVisualizationStyle.xrc
	resources/SharpeningInputPan.xrc
   resources/SharpeningInputRGB.xrc
   resources/SharpeningElements.xrc
   resources/SimpleVectorStylePart.xrc
	resources/SingleViewerSelection.xrc
	resources/SnapProperties.xrc
	resources/SpatialFilterInSitu.xrc
	resources/SpatialRsEdition.xrc
	resources/SpectralSignEditor.xrc
	resources/SpatialSubsetSelection.xrc
	resources/SpectralSelection.xrc
	resources/StadisticsSource.xrc
	resources/StatisticParameterSelection.xrc
	resources/SynchronizeViewers.xrc
	resources/TasseledCapTransformationPart.xrc
	resources/TerrainProperties.xrc
   resources/ThresholdClassSelectionPart.xrc
   resources/ThresholdClassDefinitionWidget.xrc
	resources/ThresholdParameters.xrc
   resources/VectorProperties.xrc
	resources/VectorTable.xrc
	resources/VectorOperation.xrc
	resources/VectorQueryInformation.xrc
	resources/Viewer2DProperties.xrc
	resources/Viewer3DProperties.xrc
	resources/ViewerProperties.xrc
	resources/WmsInSituEditor.xrc
	resources/WxsCreateLayer.xrc
	resources/WxsDataSource.xrc
	resources/WxsProperties.xrc
	resources/WxsVisualization.xrc
	resources/WxsLayerImporter.xrc
	resources/XyTableImport.xrc
	${EXTRA_RESOURCES}
)

IF(SURILIB_MONOLITIC_BUILD)			
	ADD_LIBRARY(${SURILIB_LIBRARY} ${SURI_LIBRARY_TYPE}
			${SURICORE_SOURCES}
			${SURITEST_SOURCES}
			${SURIDATAACCESS_SOURCES}
			${RESOURCES_SOURCES}
			${SURIWIDGETS_SOURCES}
	)
ELSE(SURILIB_MONOLITIC_BUILD)	
	ADD_LIBRARY(${SURILIB_LIBRARY}-core ${SURI_LIBRARY_TYPE}
			${SURICORE_SOURCES}
			${SURITEST_SOURCES}
			${SURIDATAACCESS_SOURCES}
	)
	ADD_LIBRARY(${SURILIB_LIBRARY}-widgets ${SURI_LIBRARY_TYPE}
			${RESOURCES_SOURCES} 
			${SURIWIDGETS_SOURCES}
	)
ENDIF(SURILIB_MONOLITIC_BUILD)

IF(SURILIB_SHARED_BUILD)
  TARGET_LINK_LIBRARIES(${SURILIB_LIBRARIES})
ENDIF(SURILIB_SHARED_BUILD)
