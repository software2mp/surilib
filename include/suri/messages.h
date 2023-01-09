/*! \file suri/messages.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

/** \cond SURI_MESSAGES */
#ifndef SURI_MESSAGES_H_
#define SURI_MESSAGES_H_

/**
 * Prefijos de los define para las distintas funciones de cada constante:
 *
 *  tooltip    : Mensajes emergentes de cualquier elemento de pantalla
 *  content    : contenido default de un campo (Ej: texto default de un
 *               wxTextBox, Opciones de un wxComboBox, etc)
 *  label      : texto invariante que debe mostrado en pantalla (Ej: texto de
 *               un wxStaticBox)
 *  caption    : Titulo de ventana/dialogo
 *  filter     : Filtro de extensiones para los dialogos (No se traduce, solo
 *               para reusarlo)
 *  message    : Cualquier mensaje al usuario que sera mostrado mediante un SHOW_XXX()
 *  question   : Pregunta al usuario
 *  menu       : Entrada de menu
 *  format     : String de formateo de datos
 *  filename   : Nombre de archivo
 *  extension  : Extension (con punto)
 *  name       : Nombre de algo (por ejemplo Punto)
 *  progress   : Mensajes de progreso de una tarea
 *  html       : Texto que se muestra dentro de docuemnto en formato html.
 */

// Includes Estandar
#include <string>

// TOOLTIPS----------------------------------------------------------------------
#define tooltip_SAVE_VIEW "Guardar vista"
#define tooltip_ZOOM_FULL_EXTENT "Ajustar vista a ventana"
#define tooltip_ZOOM_LAYER_EXTENT "Ajustar vista a capa"
#define tooltip_ZOOM_ONE_TO_ONE "Ajustar vista a relacion 1:1 con imagen"
#define tooltip_PANORAMA_WINDOW "Mostrar ventana de Panorama"
#define tooltip_QUERY "Consultar hipervinculo"
#define tooltip_ZOOM "Magnificar"
#define tooltip_DRAG "Arrastrar"
#define tooltip_EYE_CAMERA "Camara de vision sobre terreno"
#define tooltip_SPHERICAL_CAMERA "Camara esferica"
#define tooltip_BUTTON "Boton"
#define tooltip_NAVIGATION_WIDGET "Navegar"
#define tooltip_PIXEL_LINE_NAVIGATION_WIDGET "Navegar por Pixel y Linea"
#define tooltip_GoButton "Ir a coordenadas"
#define tooltip_VIEWPORT_PROPERTIES "Propiedades de visualizador"
#define tooltip_VIEWPORT_LINK "Enlazar visualizadores"
#define tooltip_QUICK_MEASUREMENT "Medicion rapida"
#define tooltip_PIXEL_QUERY "Consulta de pixel"

// Tooltips----------------------------------------------------------------------
#define tooltip_ANOTATION "Crear anotacion"
#define tooltip_MARK "Agregar punto"
#define tooltip_DELETE "Eliminar punto"
#define tooltip_CLOSE "Cerrar vector"
#define tooltip_CLOSE_GEOMETRY "Cerrar geometria"
#define tooltip_DISCARD "Descartar vector"
#define tooltip_SAVE "Guardar vector"
#define tooltip_POINT "Dibujar punto"
#define tooltip_LINE "Dibujar linea"
#define tooltip_POLYGON "Dibujar poligono"
#define tooltip_GPS_POINT "Punto Gps"
#define tooltip_GPS_LINE "Linea Gps"
#define tooltip_GPS_POLYGON "Poligono Gps"
#define tooltip_SELECT_GEOMETRIES "Seleccionar geometria"
#define tooltip_EDIT_VECTOR "Modificar punto"
#define tooltip_CREATE_FEATURE "Agregar elemento"
#define tooltip_CREATE_POLYGON_VECTOR "Crear capa vectorial de poligonos"
#define tooltip_CREATE_LINE_VECTOR "Crear capa vectorial de lineas"
#define tooltip_CREATE_POINT_VECTOR "Crear capa vectorial de puntos"
#define tooltip_CREATE_ANNOTATION_VECTOR "Crear vector de anotaciones"
#define tooltip_TASK_MENU "Iniciar / Finalizar edicion vectorial"
#define tooltip_COREGISTER_MENU "Iniciar / Finalizar edicion de puntos de control"
#define tooltip_GEOREFERENCE_MENU "Iniciar/Finalizar tarea de georreferenciacion"
#define tooltip_DELETE_POINT "Eliminar nodo"
#define tooltip_REMOVE_GEOMETRY "Eliminar capa"
#define tooltip_EDIT_TABLE "Editar tabla de vector"
#define tooltip_SNAP_PROPERTIES "Autoensamblado"
#define tooltip_ADD_GCP "Agregar punto"
#define tooltip_SELECT_GCP "Seleccionar punto de control"
#define tooltip_SHOW_GCPTABLE "Mostrar tabla con puntos de control"
#define tooltip_START_COREGISTER_PROCESS "Corregistrar/georreferenciar"
#define tooltip_START_GEOREFERENCE_PROCESS "Georreferenciar"
#define tooltip_START_COREGISTER_TASK "Iniciar tarea de corregistro"
#define tooltip_STOP_COREGISTER_TASK "Finalizar tarea de corregistro"
#define tooltip_CREATE_GCP_FILE "Crear archivo de puntos de control imagen a imagen"
#define tooltip_AUTOGENERATE_GCP_FILE "Generar archivo de puntos de control automaticamente"
#define tooltip_CREATE_PCT_FILE "Crear archivo de puntos de control en terreno"
#define tooltip_OPEN_GCP_FILE "Cargar puntos de control imagen a imagen"
#define tooltip_OPEN_PCT_FILE "Cargar puntos de control en terreno"
#define tooltip_SWITCH_BASE_WARP "Intercambiar base y warp"
#define tooltip_PREDICTION "Predecir"
#define tooltip_CLEAR_SELECTION "Limpia la seleccion"
#define tooltip_SELECT_ALL "Selecciona todos los registros"
#define tooltip_INVERT_SELECTION "Invierte la seleccion actual"
#define tooltip_QUERY_BUILDER "Armado de consultas"
#define tooltip_ADD_COLUMN "Agregar columna"
#define tooltip_CONVERTO_CLASS "Convierte la geometria a clase"
#define tooltip_MAP_TASK_MENU "Iniciar/Finalizar tarea de creacion de mapas"

// Mensaje ventanas de configuracion
#define caption_APPLICATION_CONFIGURATION \
   "- Vetana de configuracion"
// Mensajes para capas vectoriales
#define message_CANT_CONVERT_TO_CLASS "No es posible convertir la capa a clase"

// Herramientas operaciones raster-----------------------------------------------
#define message_CONFUSION_MATRIX \
   "La matriz tiene datos invalidos. Cada clase debe tener asignada un area diferente"
#define message_CANT_DELETE_ITEM "No es posible eliminar el item %s"
#define message_CANT_ADD_ITEM "No es posible agregar item %s"
#define message_ITEM_SELECTED_NOT_EXIST_IN_LIBRARY_s \
   "El item %s no existe en la biblioteca del Sistema"
#define message_ONLY_ONE_RASTER_SELECTED "Solo debe haber una imagen seleccionada."
#define message_ONLY_CLASIFICATED_RASTER "La imagen debe ser clasificada"
#define message_CLASIFICATED_RASTER_AND_POLYGON \
   "La imagen debe ser clasificada y se debe tener por lo menos un poligono en la lista"

#define message_INVALID_THRESHOLD_DEF \
   "No es posible aplicar los cambios. La definicion de la clase no es correcta"

#define message_FORMAT_NOT_SUPPORTED "Formato no soportado ('%s')"
#define message_INVALID_BAND_NAME_SELECTED \
   "No es posible agregar la banda %s ya que la misma se encuentra marca como invalida"
#define message_NO_RASTER_BANDS "El archivo de entrada %s no tiene bandas raster."
#define message_NO_GCPS_AVAILABLE "No hay GCPs disponibles para georreferenciar."
#define message_GCP_PARSE_FAILED "Ha fallado la lectura de GCPs en la linea %d:\n%s\n\nError: %s"
#define message_INVALID_GCP "GCP Invalido"
#define message_INVALID_COMMENT "Comentario con atributos invalidos"
#define message_CANNOT_OPEN_SOURCE_FILE_GDAL \
   "No se puedo abrir el archivo de origen (Error en GDALOpen - %d - %s)"

#define tooltip_RASTER_EXPORTER_BUTTON "Exportar elemento Raster"
#define tooltip_RASTER_FILTER_BUTTON "Filtrar elemento Raster"
#define tooltip_RASTER_LIBRARY_FILTER_BUTTON "Aplicar filtro de convolucion"
#define tooltip_RASTER_INDEX_BUTTON "Calcular indices normalizados"
#define tooltip_RASTER_BANDMATH_BUTTON "Algebra de bandas"
#define tooltip_RASTER_CLASSIFICATION_BUTTON "Clasificar en forma supervisada"
#define tooltip_RASTER_UNSUPERVISED_CLASSIFICATION_BUTTON \
   "Clasificar en forma no supervisada"
#define tooltip_STATISTICS_EXPORTER_BUTTON "Calcular estadisticas globales"
#define tooltip_CLASS_STATISTICS_EXPORTER_BUTTON "Calcular estadisticas de clase"
#define tooltip_CLASS_FUSSION "Fusion de clases"
#define tooltip_CONFUSION_MATRIX "Matriz de Confusion"
#define tooltip_RASTER_CLASSIFICATION_FILTER "Filtro postclasificacion"
#define caption_PROCESS_HISTORY "Historial de procesamiento"
#define caption_RASTER_GENERIC_IMPORTER "Importar raster generico"
#define caption_CLASS_FUSSION_PROCESS_NAME "Fusion de clases"
#define caption_CONFUSION_MATRIX_PROCESS_NAME "Matriz de Confusion"
#define caption_RASTER_EXPORTER_PROCESS_NAME "Proceso de Exportacion Raster"
#define caption_RASTER_BANDMATH_PROCESS_NAME "Algebra de bandas"
#define caption_MASK_SELECTION_PART "Mascara"
#define caption_RASTER_FILTER_PROCESS_NAME "Filtros"
#define caption_RASTER_CLASSIFICATION_PROCESS_NAME "Clasificacion Supervisada"
#define tooltip_THRESHOLD_CLASSIFICATION "Clasificacion por umbrales"
#define caption_RASTER_UNSUPERVISED_CLASSIFICATION_PROCESS_NAME \
   "Clasificacion No Supervisada"
#define caption_GLOBAL_STATISTICS "Calcular estadisticas globales"
#define caption_GLOBAL_CLASS_STATISTICS "Calcular estadisticas en areas de clasificacion"
#define caption_GLOBAL_CLASS_STATISTICS_EXPORTER "Exportar estadisticas en areas de clasificacion"
#define caption_MULTIMEDIA_PROPERTIES "Propiedades multimedia"
#define caption_EXIT_WIDGET "Esta seguro que desea salir?"

#define content_CLASS_COLUMN_NAME "Clase"
#define content_TRAIN_ENCLOSURE_COLUMN_NAME "Entrenamiento"
#define content_TEST_ENCLOSURE_COLUMN_NAME "Prueba"

#define content_LANGUAGE_SPANISH "Espaniol"

// Herramientas animacion--------------------------------------------------------
#define tooltip_STOP_ANIMATION "Detener"
#define tooltip_START_ANIMATION "Animar/Pausar"
#define tooltip_PREVIOUS_FRAME_ANIMATION "Retroceder"
#define tooltip_NEXT_FRAME_ANIMATION "Avanzar"
#define tooltip_FLICKER_FRAME_ANIMATION "Comparar"
// Herramientas proyectos--------------------------------------------------------
#define tooltip_PROJECT_NEW_BUTTON "Nuevo proyecto"
#define tooltip_PROJECT_OPEN_BUTTON "Abrir proyecto"
#define tooltip_PROJECT_CLOSE_BUTTON "Cerrar proyecto"
#define tooltip_PROJECT_SAVE_BUTTON "Guardar proyecto"
#define tooltip_PROJECT_SAVE_AS_BUTTON "Guardar Proyecto como..."
#define tooltip_PROJECT_IMPORT_BUTTON "Importar"
#define tooltip_PROJECT_EXPORT_BUTTON "Exportar"

// Herramientas capas------------------------------------------------------------
#define tooltip_LAYER_ADD_BUTTON "Agregar capa"
#define tooltip_LAYER_ADD_REMOTE_BUTTON "Agregar capa remota"
#define tooltip_LAYER_ADD_CSV_BUTTON "Importador de tabla X/Y"
#define tooltip_LAYER_ADD_TERRAIN_BUTTON "Abrir terreno"
#define tooltip_LAYER_REMOVE_BUTTON "Eliminar capa"
#define tooltip_LAYER_DISPLAY_BUTTON "Desplegar"
#define tooltip_LAYER_HIDE_BUTTON "Ocultar"
#define tooltip_LAYER_SHOW_PROPERTIES_BUTTON "Propiedades"
#define tooltip_LAYER_DISPLAY_ALL_BUTTON "Desplegar en todos"
#define tooltip_LAYER_HIDE_ALL_BUTTON "Ocultar en todos"
#define tooltip_LAYER_CREATE_GROUP_BUTTON "Crear grupo"
#define tooltip_LAYER_EXPORT_LAYER_BUTTON "Exportar Capa"
#define caption_LAYER_EXPORT "Exportar capa raster"
#define caption_VECTOR_LAYER_EXPORT "Exportar capa vector"
#define caption_VECTOR_LAYER_SELECTION_EXPORT "Exportar seleccion de capa vector"
#define caption_STACK_BANDS "Apilar Bandas"
#define caption_VISUALIZATION_EXPORT "Exportar visualizacion"
#define caption_MOSAIC "Mosaico"
#define caption_VECTOR_RASTERIZATION "Rasterizacion de capa vectorial"
#ifdef __DEBUG__
#define tooltip_LAYER_ENCLOSURE_ADD_LAYER_BUTTON "Abrir recinto"
#define tooltip_LAYER_ENCLOSURE_CLEAN_LAYER_BUTTON "Limpiar recinto"
#define tooltip_LAYER_ENCLOSURE_VALIDATE_LAYER_BUTTON "Validar recinto"
#define tooltip_LAYER_CLASSIFIEDRASTER_ADD_LAYER_BUTTON "Abrir raster clasificado"
#define tooltip_LAYER_CLASSIFIEDRASTER_CLEAN_LAYER_BUTTON "Limpiar raster clasificado"
#define tooltip_LAYER_CLASSIFIEDRASTER_VALIDATE_LAYER_BUTTON "Validar raster clasificado"
#endif

// Descripciones para edicion de tablas------------------------------------------
#define tooltip_TABLE_ADD_COL "Agregar columna"
#define tooltip_TABLE_DEL_COL "Eliminar columna"
#define tooltip_TABLE_ADD_ROW "Agregar fila"
#define tooltip_TABLE_DEL_ROW "Eliminar fila"

// Menu--------------------------------------------------------------------------
#define menu_DISPLAY "&Desplegar"
#define menu_SHOWTABLE "Tabla de atributos"
#define menu_CLEAN_VECTOR_SELECTION "Limpiar seleccion"
#define menu_SHOWHISTOGRAM "Histograma"
#define menu_HIDE "&Ocultar"
#define menu_HIDE_ALL "Ocultar en todos"
#define menu_DISPLAY_ALL "Desplegar en todos"
#define menu_CLOSE "&Cerrar"
#define menu_PROPERTIES "&Propiedades"
#define menu_ASPECT "&Aspecto"
#define menu_SHOW_HIDDEN_ELEMENTS "Mostrar elementos ocultos"
#define menu_CONVERTO_TRAIN_CLASS "Convertir poligonos a area de entrenamiento"
#define menu_CONVERTO_TEST_CLASS "Convertir poligonos a area de prueba"

#define menu_COPYTO "Copiar en..."
#define menu_COPYTO_IMAGE "&Imagen"
#define menu_COPYTO_MAP "&Mapa"
#define menu_COPYTO_3D "&Terreno"
#define menu_COPYTO_DOCUMENTS "&Documentos"
#define menu_COPYTO_EXCERCISES "&Actividades"

// CONTENT-----------------------------------------------------------------------
#define content_TEXT "Texto"

// LABEL-------------------------------------------------------------------------
// Pen y Brush
#define label_TRANSPARENT "Transparente"
#define label_SOLID "Solido"
#define label_OF_POINTS "De puntos"
#define label_LONG_POINTED  "Punteado largo"
#define label_SHORT_POINTED "Punteado corto"
#define label_POINT_AND_LINE "Punto y linea"
#define label_RIGHT_DIAG  "Diagonal derecha"
#define label_CROSS_DIAG "Diagonal cruzado"
#define label_LEFT_DIAG "Diagonal izquierda"
#define label_CROSSED "Cruzado"
#define label_HORIZONTAL  "Horizontal"
#define label_VERTICAL "Vertical"
// Symbol
#define label_NO_SYMBOL "Sin simbolo"
#define label_CIRCLE "Circulo"
#define label_QUAD "Cuadrado"
#define label_TRIANGLE "Triangulo"
#define label_CROSS "Cruz"
#define label_PENTAG "Pentagono"
#define label_CIRCLE_NO_FILL "Circulo sin relleno"
#define label_QUAD_NO_FILL  "Cuadrado sin relleno"
#define label_TRIANGLE_NO_FILL "Triangulo sin relleno"
#define label_CROSS_NO_FILL "Cruz sin relleno"
#define label_PENTAG_NO_FILL "Pentagono sin relleno"
// Fonts
#define label_NO_LABEL "Sin etiqueta"
#define label_NORMAL "Normal"
#define label_DECORATIVE "Decorativa"
#define label_ROMAN "Romana"
#define label_CURSIVE "Cursiva"
#define label_SWISS  "Suiza"
#define label_MODERN "Moderna"
#define label_TELETYPE  "Teletipo"
// Navigation
#define label_NORTH "N"
#define label_SOUTH "S"
#define label_WEST "O"
#define label_EAST "E"
// Tables
#define label_NO_HOTLINK "Sin hipervinculo"
#define label_NEW_FIELD "Nuevo campo"
#define label_INT_DATA_TYPE "Entero"
#define label_UCHAR_DATA_TYPE "Texto"
#define label_DOUBLE_DATA_TYPE "Numero decimal"
#define label_DATA_TABLE "Tabla datos"
#define label_HOTLINK_EDITOR "Editor de hipervinculos"
#define label_READ_ONLY "Solo Lectura"
#define label_LAYER_TABLE "Capa #"
#define label_CALCULATED_DATA_TYPE "Calculada"

#define label_DASH_SEPARATOR " - "
#define label_EXIT "Salir"

// CAPTION-----------------------------------------------------------------------
#define caption_PANORAMA "Panorama"
#define caption_SAVE_REPORT "Guardar reporte"
#define caption_STATISTICS_REPORT "Reporte de estadisticas"
#define caption_CONFUSION_MATRIX_REPORT "Reporte con matriz de confusion"
#define caption_SAVE_VISUALIZATION "Guardar vista"
#define caption_OPENGL_CANVAS "Canvas OpenGL"
#define caption_PART "Parte"
#define caption_PART_CONTAINER "Contenedor de partes"
#define caption_TOOL "Herramienta"
#define caption_NOTEBOOK "Cuaderno de notas"
#define caption_LIBRARY "Biblioteca"
#define caption_SPECTRAL_SIGN_LIBRARY "Firmas espectrales"
#define caption_CONVOLUTION_FILTERS "Filtros de convolucion"
#define caption_EQUATION_LIBRARY "Ecuaciones"
#define caption_INDEX_LIBRARY "Indices"
#define caption_SPATIAL_REFERENCE_LIBRARY "Sistemas de referencia"
#define caption_DATUM_LIBRARY "Datums"
#define caption_ELLIPSOID_LIBRARY "Elipsoides"
#define caption_DENSITY_SLICE_LIBRARY "Tablas de intervalos"
#define caption_ELEMENT_PROPERTIES "Propiedades de elemento"
#define caption_GENERAL_PROPERTIES "Generales"
#define caption_MAP_TOOL_PROPERTIES "Herramientas de mapa"
#define caption_XML_PROPERTIES "Propiedades XML"
#define caption_CLASS_PROPERTIES "Clases"
#define caption_TERRAIN_PROPERTIES "Propiedades de terreno"
#define caption_VECTOR_PROPERTIES "Estilo"
#define caption_VECTOR_STYLE_LIBRARY "Estilos vectoriales"
#define caption_VECTOR_PROPERTIES_SIMUR "Propiedades de vector"
#define caption_ANOTATION_INPUT "Ingresar anotacion"
#define caption_ANOTATION_PROPERTIES "Etiqueta"
#define caption_GENERIC_HTML_VIEWER "Visualizador HTML generico"
#define caption_VIEWER "Visualizador"
#define caption_VIEWPORT_PROPERTIES "Propiedades de visualizador"
#define caption_VIEWER_d "Visualizador #%d"
#define caption_VIEWER2D "Visualizador 2D"
#define caption_VIEWER3D "Visualizador 3D"
#define caption_NAVIGATION_TOOL "Navegador"
#define caption_FILE_EXPORTER_TOOL "Exportar Imagen"
#define caption_REPROJECTION_TOOL "Reproyectar capa raster"
#define caption_SCALE_TOOL "Escalar imagen"
#define caption_TRANSFORMATION_TOOL "Transformacion"
#define caption_STATISTICS_EXPORTER_TOOL "Calcular Estadisticas"
#define caption_SPECTRAL_SELECTION "Seleccion espectral"
#define caption_SPATIAL_SELECTION "Seleccion espacial"
#define caption_BUFFER_CALCULATION "Calculo de buffer para capas vectoriales"
#define caption_MERGE_LAYERS "Fusion de capas vectoriales"
#define caption_VECTOR_OPERATION "Operaciones vectoriales geometricas"
#define caption_MERGE_LAYERS "Fusion de capas vectoriales"
#define caption_RASTER_IMPORTER "Importador generico raster"
#define caption_BAND_COMBINATION_SELECTION "Combinacion de bandas"
#define caption_OUTPUT_SELECTION "Archivo"
#define caption_MOSAIC_CONFIGURATION "Configuracion"
#define caption_FILTER_SELECTION "Filtro"
#define caption_COLUMN_NAME_INPUT "Nuevo campo"
#define caption_VECTOR_TABLE_EDITION "Estructura de datos"
#define caption_LAYERS "Listado de capas"
#define caption_CLASSIFICATION_OPTIONS "Opciones de clasificacion"
#define caption_CLASSIFICATION_EXPORTER_TOOL "Clasificacion"
#define caption_SUPERVISED_CLASSIFICATION_TOOL "Clasificacion supervisada"
#define caption_INDEX_SELECTION "Seleccion de indice"
#define caption_INDEX_EXPORTER_TOOL "Indices estandarizados"
#define caption_FILTER_EXPORTER_TOOL "Filtros espaciales"
#define caption_SELECT_HOTLINK "Seleccionar archivo"
#define caption_RASTER_PROPERTIES "Imagen"
#define caption_RASTER_MODEL_PROPERTIES "Modelo raster"
#define caption_GEORREFERENCE_PROPERTIES "Georreferencia"
#define caption_GEO_MODEL_PROPERTIES "Modelo geografico"
#define caption_ENHANCEMENT_SELECTION "Seleccion de realce"
#define caption_BAND_COMBINATION_AND_ENHANCEMENT "Combinacion de bandas y realce"
#define caption_SAVE_VECTOR "Guardar vector"
#define caption_KMEANS_OPTIONS "Opciones de clasificacion"
#define caption_OPERATION_IN_PROGRESS "Operacion en progreso"
#define caption_ENHANCEMENT_BOX_NAME "Utilizar realce"
#define caption_ENHANCEMENT_CHOICE_SELECTION "Seleccion de realce"
#define caption_COLORTABLE_BOX_NAME "Utilizar tabla de colores"
#define caption_COLORTABLE_CHOICE_SELECTION "Seleccion de tabla de colores"
#define caption_REGISTRATION_PART "Registracion"
#define caption_SINGLE_VIEWER_PART "Seleccione visualizador"
#define caption_GCP_AUTO_GENERATION_PART "Generacion automatica de GCPs"
#define caption_DUAL_VIEWER_PART "Seleccione visualizadores"
#define caption_BAND_MATH_PART "Algebra de bandas"
#define caption_VIEWER_SYNCHRONIZATION "Sincronizar visualizadores"
#define caption_RASTER_OUTPUT_SIZE_PART "Dimension de salida"
#define caption_COREGISTER_TOOL "Corregistro/georreferencia"
#define caption_GEOREFERENCE_TOOL "Georreferencia con PCT"
#define caption_PIXEL_INFORMATION "Consulta de pixel"
#define caption_BAND_ENHANCEMENT_PART "Realce de banda"
#define caption_ABOUT_PART "Acerca de"
#define caption_TABLE_STRUCTURE_EDITOR_PART "Estructura de tabla"
#define caption_SELECT_IMAGE "Seleccionar la imagen a utilizar"
#define caption_SELECT_TEXTURE "Seleccionar imagen de textura para el terreno"
#define caption_SELECT_ELEMENT "Seleccionar el elemento para agregar"
#define caption_TABLE_PART "Editor de Tabla"
#define caption_VECTOR_TABLE_PART "Editor de Tabla Vectorial"
#define caption_VIEWER_2D_SELECTOR "Seleccion visualizadores"
#define caption_GCP_FILE "Archivo con puntos de control"
#define caption_ADD_COLUMN "Agregar columna"
#define caption_SHARPENING_TOOL "Fusion multiespectral con pancromatica"
#define caption_SHARPENING_INPUT_RGB "Seleccion bandas RVA"
#define caption_SHARPENING_INPUT_PAN "Seleccion banda de alta resolucion"
#define caption_SHARPENING_INPUT_ELEMENTS "Seleccion de entradas"
#define caption_METADATA_PROPERTIES "Otros metadatos"
#define caption_METADATA_FILE "Archivo de metadatos"
#define caption_SPECTRAL_ANGLE_MAPPER "Mapeo de angulo espectral"
#define caption_TCTRANSFORMATION_TOOL "Transformacion Tasseled-Cap"
#define caption_CLASSIFICATION_PARAMETERS_IMPORTER "Importacion de parametros de clasificacion"
#define caption_PCANALYSIS_TOOL "Analisis de componentes principales"
#define caption_CONFIGURATION_PART "Configuracion"
#define caption_OUTPUT_PAGE_CONFIG "Configuracion de pagina"
#define caption_HISTOGRAM_MATCHING "Histograma de origen"
// HERRAMIENTA DE MAPA-----------------------------------------------------------
#define label_GRID "Grilla de mapa"
#define label_NO_GRID "Ninguna"        // No agregar M: viene de XRC
#define label_1_DEGREE "1 grado"       // No agregar M: viene de XRC
#define label_2_DEGREES "2 grados"     // No agregar M: viene de XRC
#define label_5_DEGREES "5 grados"     // No agregar M: viene de XRC
#define label_10_DEGREES "10 grados"   // No agregar M: viene de XRC
#define label_20_DEGREES "20 grados"   // No agregar M: viene de XRC
#define label_30_DEGREES "30 grados"   // No agregar M: viene de XRC
#define label_MAP_LAYERS "Capas de mapa"

#define label_NORTH_SHORT "N"
#define label_SOUTH_SHORT "S"
#define label_EAST_SHORT "E"
#define label_WEST_SHORT "O"

#define caption_NORTH_BUTTON "Muestra norte"
#define caption_SCALE_BUTTON "Muestra escala"
#define caption_LEGEND_BUTTON "Muestra referencias"

#define label_LEGEND "Leyenda de mapa"
#define message_GRID_VECTOR_CREATION_ERROR "Error al crear vector de grilla"
// HERRAMIENTA INFO DE ZOOM------------------------------------------------------
#define format_DEGREE_MINUTES_SECONDS "%2d^g %2d' %2.2f\" %s"
#define format_ZOOM_IN "x%0.2f"
#define format_ZOOM_OUT "x1 / %0.2f"

// Draw Tool---------------------------------------------------------------------
#define label_POINT "Punto"
#define label_LINE "Linea"
#define label_POLYGON "Poligono"
#define label_UNKNOWN_VECTOR "Vector desconocido"
#define label_ANOTATION "Anotacion"

// Meassure Tool-----------------------------------------------------------------
#define label_DISTANCE "Distancia"
#define label_AREA "Area"
#define caption_DISTANCE "Medir distancia"
#define caption_AREA "Medir area"
#define format_MEASSURE_DISPLAY_fss "%.2f %s%s"
#define invalid_MEASSURE "Medicion invalida"
#define label_SQUARE "^2"
#define name_METER_UNIT "m"
#define name_KILOMETER_UNIT "km"
#define name_HECTARE_UNIT "ha"

// HERRAMIENTA DE INFO DE PIXEL--------------------------------------------------
#define message_CURSOR_OUTSIDE_VALID_AREA "Fuera del area valida"
#define format_PIXEL_LINE_ff "XY:(%.2f;%.2f)"
#define format_PIXEL_LINE_fff "XYZ:(%.2f;%.2f;%.2f)"
#define message_INVALID_DN "no valido"

// HERRAMIENTA DE ANIMACION------------------------------------------------------
#define message_INVALID_ELEMENT_SELECTION_FOR_ANIMATION_ERROR \
   "Seleccion de elementos incompatible para animacion."
#define message_ACTIVE_ELEMENT_OUTSIDE_SELECTION_FOR_ANIMATION_ERROR \
   "Hay al menos un elemento activo afuera del grupo de animacion."

// Meassure Tool-----------------------------------------------------------------
#define message_MEASSURE_DISTANCE "Distancia: %s"
#define message_MEASSURE_AREA "Area: %s"

// Content-----------------------------------------------------------------------
#define content_DEFAULT_COLUMN_NAME "nombre"
#define content_DEFAULT_ANOTATION "Anotacion"

// FILTER------------------------------------------------------------------------
#define FILTER_SEPARATOR "|"
#define EXTENSION_SEPARATOR ";"
#define EXTENSION_DELIMITER_OPEN "("
#define EXTENSION_DELIMITER_CLOSE ")"

/** macros auxiliares */
#define JOIN_1(Parameter) Parameter
/** Concatena 2 parametros con un separador. */
#define JOIN_2(Parameter1, Parameter2, Separator) Parameter1 +                    \
   std::string(Separator) + Parameter2
/** Concatena 3 parametros con un separador. Utiliza JOIN_2 */
#define JOIN_3(Parameter1, Parameter2, Parameter3, Separator)                     \
   JOIN_2(JOIN_2(Parameter1, Parameter2, Separator), Parameter3,                  \
          Separator)
/** Concatena 4 parametros con un separador. Utiliza JOIN_3 */
#define JOIN_4(Parameter1, Parameter2, Parameter3, Parameter4, Separator)         \
   JOIN_2(JOIN_3(Parameter1, Parameter2, Parameter3, Separator),                  \
          Parameter4, Separator)
/** Concatena 5 parametros con un separador. Utiliza JOIN_4 */
#define JOIN_5(Parameter1, Parameter2, Parameter3, Parameter4, Parameter5,        \
               Separator)                                                         \
   JOIN_2(JOIN_4(Parameter1, Parameter2, Parameter3, Parameter4,                  \
                 Separator), Parameter5, Separator)
/** Concatena 6 parametros con un separador. Utiliza JOIN_5 */
#define JOIN_6(Parameter1, Parameter2, Parameter3, Parameter4, Parameter5,        \
               Parameter6, Separator)                                             \
   JOIN_2(JOIN_5(Parameter1, Parameter2, Parameter3, Parameter4,                  \
                 Parameter5, Separator), Parameter6, Separator)
/** Concatena 7 parametros con un separador. Utiliza JOIN_6 */
#define JOIN_7(Parameter1, Parameter2, Parameter3, Parameter4, Parameter5,        \
               Parameter6, Parameter7, Separator)                                 \
   JOIN_2(JOIN_6(Parameter1, Parameter2, Parameter3, Parameter4,                  \
                 Parameter5, Parameter6, Separator), Parameter7, Separator)
/** Concatena 8 parametros con un separador. Utiliza JOIN_7 */
#define JOIN_8(Parameter1, Parameter2, Parameter3, Parameter4, Parameter5,        \
               Parameter6, Parameter7, Parameter8, Separator)                     \
   JOIN_2(JOIN_7(Parameter1, Parameter2, Parameter3, Parameter4,                  \
                 Parameter5, Parameter6, Parameter7, Separator),                  \
                 Parameter8, Separator)
/** Concatena 9 parametros con un separador. Utiliza JOIN_8 */
#define JOIN_9(Parameter1, Parameter2, Parameter3, Parameter4, Parameter5,        \
               Parameter6, Parameter7, Parameter8, Parameter9, Separator)         \
   JOIN_2(JOIN_8(Parameter1, Parameter2, Parameter3, Parameter4,                  \
                 Parameter5, Parameter6, Parameter7, Parameter8,                  \
                 Separator), Parameter9, Separator)
/** Concatena 10 parametros con un separador. Utiliza JOIN_9 */
#define JOIN_10(Parameter1, Parameter2, Parameter3, Parameter4, Parameter5,       \
               Parameter6, Parameter7, Parameter8, Parameter9, Parameter10,       \
               Separator)                                                         \
   JOIN_2(JOIN_9(Parameter1, Parameter2, Parameter3, Parameter4,                  \
                 Parameter5, Parameter6, Parameter7, Parameter8,                  \
                 Parameter9, Separator), Parameter10, Separator)
/** Concatena 11 parametros con un separador. Utiliza JOIN_10 */
#define JOIN_11(Parameter1, Parameter2, Parameter3, Parameter4, Parameter5,       \
               Parameter6, Parameter7, Parameter8, Parameter9, Parameter10,       \
               Parameter11, Separator)                                            \
   JOIN_2(JOIN_10(Parameter1, Parameter2, Parameter3, Parameter4,                 \
                 Parameter5, Parameter6, Parameter7, Parameter8,                  \
                 Parameter9, Parameter10, Separator), Parameter11,                \
                 Separator)
/** Concatena 12 parametros con un separador. Utiliza JOIN_11 */
#define JOIN_12(Parameter1, Parameter2, Parameter3, Parameter4, Parameter5,       \
               Parameter6, Parameter7, Parameter8, Parameter9, Parameter10,       \
               Parameter11, Parameter12, Separator)                               \
   JOIN_2(JOIN_11(Parameter1, Parameter2, Parameter3, Parameter4,                 \
                 Parameter5, Parameter6, Parameter7, Parameter8,                  \
                 Parameter9, Parameter10, Parameter11, Separator),                \
                 Parameter12, Separator)

/** Concatena 13 parametros con un separador. Utiliza JOIN_12 */
#define JOIN_13(Parameter1, Parameter2, Parameter3, Parameter4, Parameter5,       \
               Parameter6, Parameter7, Parameter8, Parameter9, Parameter10,       \
               Parameter11, Parameter12, Parameter13, Separator)                  \
   JOIN_2(JOIN_12(Parameter1, Parameter2, Parameter3, Parameter4,                 \
                 Parameter5, Parameter6, Parameter7, Parameter8,                  \
                 Parameter9, Parameter10, Parameter11, Parameter12, Separator),   \
                 Parameter13, Separator)

/** Concatena 14 parametros con un separador. Utiliza JOIN_13 */
#define JOIN_14(Parameter1, Parameter2, Parameter3, Parameter4, Parameter5,       \
               Parameter6, Parameter7, Parameter8, Parameter9, Parameter10,       \
               Parameter11, Parameter12, Parameter13, Parameter14, Separator)     \
   JOIN_2(JOIN_13(Parameter1, Parameter2, Parameter3, Parameter4,                 \
                 Parameter5, Parameter6, Parameter7, Parameter8,                  \
                 Parameter9, Parameter10, Parameter11, Parameter12, Parameter13,  \
                 Separator),                                                      \
                 Parameter14, Separator)

/** Concatena 15 parametros con un separador. Utiliza JOIN_14 */

#define JOIN_15(Parameter1, Parameter2, Parameter3, Parameter4, Parameter5,      \
                Parameter6, Parameter7, Parameter8, Parameter9, Parameter10,     \
                Parameter11, Parameter12, Parameter13, Parameter14, Parameter15, \
                Separator)   \
        JOIN_2(JOIN_14(Parameter1, Parameter2, Parameter3, Parameter4,           \
                Parameter5, Parameter6, Parameter7, Parameter8,                  \
                Parameter9, Parameter10, Parameter11, Parameter12,               \
                Parameter13, Parameter14, Separator), Parameter15, Separator)

/** Construye un filtro con extension */
/* Esta macro devuelve un string de la forma:
 *    <Nombre> (<Extension>) | <Filtro>
 * @param[in] Name   nombre del filtro. Es el visualizado en el dialogo de archivos.
 * @param[in] Extension   extension del filtro. Es la cadena que se muestra al
 * lado del nombre. Por ejemplo: JPG (*.jpg)
 * @param[in] Filtro   filtro propiamente dicho. Este parametro no se visualiza en
 * el dialogo de archivos pero es el que se utiliza para filtrar los archivos.
 * Por ejemplo: *.jpg; *.jpeg; *.jpe
 * \note es importante no utilizar esta si no se pasa el parametro Extension
 */
#define BUILD_FILTER_WITH_EXT(Name, Extension, Filter) _(Name) + std::string( \
      " " EXTENSION_DELIMITER_OPEN Extension EXTENSION_DELIMITER_CLOSE  \
      " " FILTER_SEPARATOR Filter)

/** Construye un filtro */
/* Esta macro devuelve un string de la forma:
 *    <Nombre> | <Filtro>
 * @param[in] Name   nombre del filtro. Es el visualizado en el dialogo de archivos.
 * @param[in] Filtro   filtro propiamente dicho. Este parametro no se visualiza en
 * el dialogo de archivos pero es el que se utiliza para filtrar los archivos.
 * Por ejemplo: *.jpg; *.jpeg; *.jpe
 */
#define BUILD_FILTER(Name, Filter) _(Name) + std::string(FILTER_SEPARATOR Filter)

/** Nombres de filtros */
#define name_ALL_SUPPORTED_FILES "Todos los archivos soportados"
#define name_ALL_FILES "Todos los archivos"

#define name_HTM "HTM"
#define name_XLS "XLS"
#define name_BMP "BMP"
#define name_JPG "JPG"
#define name_PNG "PNG"
#define name_TIF "TIF"
#define name_GIF "GIF"
#define name_XML "Xml"
#define name_CEOS_FILES "Archivos CEOS"
#define name_HDF5_FILES "Archivos HDF5"
#define name_L1B_FILES "Archivos NOAA L1B"
#define name_GPX_FILES "Archivos GPX"
#define name_J2K_JP2_FILES "Archivos JP2/J2K"
#define name_KML_FILES "Archivos KML"
#define name_COMMON_IMAGE_FILES "Archivos de Imagen"
#define name_TIF_FILES "Archivos GeoTiff"
#define name_FAST_FORMAT_FILES "Archivos Fast Format"
#define name_VECTOR_FILES "Archivos Vectoriales"
#define name_GCP_FILES "Archivos gcp"
#define name_TXT_FILES "Archivos de texto"
#define name_PTS_FILES "Archivos de puntos"
#define name_SHAPEFILE_FILES "Archivos ESRI-Shapefile"
#define name_OGC_WMS_TMS "Archivos WMS/TMS/OGC"
#define name_MTL "Archivos MTL (Landsat-8)"
#define name_TERRAIN_FILES "Archivos de Terreno"
#define name_TEXTURE_FILES "Archivos de Textura"

/** Extensiones de los Filtros */
#ifdef __WXMSW__
#  define filter_extension_ALL "*.*"
#  define filter_extension_BMP "*.bmp"
#  define filter_extension_JPG JOIN_3("*.jpg", "*.jpeg", "*.jpe",                 \
                                      EXTENSION_SEPARATOR)
#  define filter_extension_PNG "*.png"
#  define filter_extension_GIF "*.gif"
#  define filter_extension_TIF JOIN_2("*.tif", "*.tiff", EXTENSION_SEPARATOR)
#  define filter_extension_FST JOIN_4("*_hrf.fst", "*_htm.fst", "*_hpn.fst",      \
                                      "header.dat", EXTENSION_SEPARATOR)
#  define filter_extension_SHAPEFILE "*.shp"
#  define filter_extension_CEOS "VOL-*"
#  define filter_extension_HDF5 JOIN_2("*.hdf", "*.h5", EXTENSION_SEPARATOR)
#  define filter_extension_L1B "*.l1b"
#  define filter_extension_GPX "*.gpx"
#  define filter_extension_KML "*.kml"
#  define filter_extension_J2K_JP2 JOIN_2("*.j2k", "*.jp2", EXTENSION_SEPARATOR)
#  define filter_extension_HTM JOIN_2("*.htm", "*.html", EXTENSION_SEPARATOR)
#  define filter_extension_XLS "*.xls"
#  define filter_extension_GCP "*.gcp"
#  define filter_extension_TXT "*.txt"
#  define filter_extension_PTS "*.pts"
#  define filter_extension_OGC_WMS_TMS "*.xml"
#  define filter_extension_MTL "*.txt"
#else //Si no esta definido
#  define filter_extension_ALL "*"
#  define filter_extension_BMP "*.[Bb][Mm][Pp]"
#  define filter_extension_JPG JOIN_3("*.[Jj][Pp][Gg]", "*.[Jj][Pp][Ee][Gg]",     \
                                      "*.[Jj][Pp][Ee]", EXTENSION_SEPARATOR)
#  define filter_extension_PNG "*.[Pp][Nn][Gg]"
#  define filter_extension_GIF "*.[Gg][Ii][Ff]"
#  define filter_extension_TIF JOIN_2("*.[Tt][Ii][Ff]", "*.[Tt][Ii][Ff][Ff]",     \
                                      EXTENSION_SEPARATOR)
#  define filter_extension_FST JOIN_4("*_[Hh][Rr][Ff].[Ff][Ss][Tt]",              \
                                      "*_[Hh][Tt][Mm].[Ff][Ss][Tt]",              \
                                      "*_[Hh][Pp][Nn].[Ff][Ss][Tt]",              \
                                      "*.[Dd][Aa][Tt]", EXTENSION_SEPARATOR)
#  define filter_extension_SHAPEFILE "*.[Ss][Hh][Pp]"
#  define filter_extension_CEOS "[Vv][Oo][Ll]-*"
#  define filter_extension_HDF5 JOIN_2("*.[Hh][Dd][Ff]", "*.[Hh]5",               \
                                      EXTENSION_SEPARATOR)
#  define filter_extension_L1B "*.[Ll]1[Bb]"
#  define filter_extension_GPX "*.[Gg][Pp][Xx]"
#  define filter_extension_KML "*.[Kk][Mm][Ll]"
#  define filter_extension_J2K_JP2 JOIN_2("*.[Jj]2[Kk]", "*.[Jj][Pp]2",    \
                                      EXTENSION_SEPARATOR)
#  define filter_extension_HTM JOIN_2("*.[Hh][Tt][Mm]", "*.[Hh][Tt][Mm][Ll]", \
                                      EXTENSION_SEPARATOR)
#  define filter_extension_XLS "*.[Xx][Ll][Ss]"
#  define filter_extension_GCP "*.[Gg][Cc][Pp]"
#  define filter_extension_TXT "*.[Tt][Xx][Tt]"
#  define filter_extension_PTS "*.[Pp][Tt][Ss]"
#  define filter_extension_OGC_WMS_TMS "*.[Xx][Mm][Ll]"
#  define filter_extension_MTL "*.[Tt][Xx][Tt]"
#endif

#define filter_extension_GEORREFERENCED_IMAGE JOIN_2(filter_extension_TIF,        \
                                                     filter_extension_FST,        \
                                                     EXTENSION_SEPARATOR)
#define filter_extension_COMMON_IMAGE JOIN_5(filter_extension_BMP,                \
                                             filter_extension_JPG,                \
                                             filter_extension_PNG,                \
                                             filter_extension_GIF,                \
                                             filter_extension_TIF,                \
                                             EXTENSION_SEPARATOR)
/** \attention wxFileDialog agrega extension para CEOS, entonces se coloca antes para evitarlo */
#define filter_extension_VIEWER_2D JOIN_9(filter_extension_CEOS,                  \
                                          filter_extension_FST,                   \
                                          filter_extension_COMMON_IMAGE,          \
                                          filter_extension_HDF5,                  \
                                          filter_extension_L1B,                   \
                                          filter_extension_VECTOR,                \
                                          filter_extension_OGC_WMS_TMS,           \
                                          filter_extension_J2K_JP2,               \
                                          filter_extension_MTL,                   \
                                          EXTENSION_SEPARATOR)

#define filter_extension_VIEWER_3D JOIN_2(filter_extension_TERRAIN,               \
                                          filter_extension_VECTOR,                \
                                          EXTENSION_SEPARATOR)
#define filter_extension_TERRAIN filter_extension_TIF
#define filter_extension_TEXTURE filter_extension_GEORREFERENCED_IMAGE
#define filter_extension_VECTOR JOIN_3(filter_extension_SHAPEFILE,                \
                                       filter_extension_GPX,                      \
                                       filter_extension_KML,                      \
                                       EXTENSION_SEPARATOR)
#define filter_extension_GROUNDCONTROLPOINT JOIN_3(filter_extension_GCP,    \
                                       filter_extension_TXT,                      \
                                       filter_extension_PTS,                      \
                                       EXTENSION_SEPARATOR)

// Extension---------------------------------------------------------------------
#define name_PROJECT_FILES "Archivos de Proyecto"
#define name_PAT_FILES "Archivos PAT"
#ifdef __WXMSW__
#  define filter_extension_PROJECT "*.suri"
#  define filter_extension_PAT "*.pat"
#else
#  define filter_extension_PROJECT "*.[Ss][Uu][Rr][Ii]"
#  define filter_extension_PAT "*.[Pp][Aa][Tt]"
#endif

/** Extensiones (opcional)------------------------------------------------------ */
#define extension_PROJECT_FILE "*." extension_PROJECT
#define extension_PAT_FILE "*." extension_PAT

// SE DEJAN POR COMPATIBILIDAD, ver ContextManagerToolbar.
#define extension_PAT "pat"
#define extension_PROJECT "suri"

/** Filtros--------------------------------------------------------------------- */
#define filter_extension_PROJECT_PAT JOIN_2(filter_extension_PROJECT,             \
                                            filter_extension_PAT, EXTENSION_SEPARATOR)
#define filter_SAVE_PROJECT JOIN_2(                                               \
      BUILD_FILTER_WITH_EXT(name_PROJECT_FILES, extension_PROJECT_FILE,           \
                            filter_extension_PROJECT),                            \
      BUILD_FILTER_WITH_EXT(name_ALL_FILES, extension_ALL, filter_extension_ALL), \
      FILTER_SEPARATOR)

#define filter_OPEN_PROJECT JOIN_2(                                               \
      BUILD_FILTER(name_ALL_SUPPORTED_FILES, filter_extension_PROJECT),           \
      filter_SAVE_PROJECT, FILTER_SEPARATOR)

#define filter_OPEN_PROJECT_PAT JOIN_4(                                           \
      BUILD_FILTER(name_ALL_SUPPORTED_FILES, filter_extension_PROJECT_PAT),       \
      BUILD_FILTER_WITH_EXT(name_PROJECT_FILES, extension_PROJECT_FILE,           \
                            filter_extension_PROJECT),                            \
      BUILD_FILTER_WITH_EXT(name_PAT_FILES, extension_PAT_FILE, filter_extension_PAT), \
      BUILD_FILTER_WITH_EXT(name_ALL_FILES, extension_ALL, filter_extension_ALL), \
      FILTER_SEPARATOR)

/** Extensiones (opcional)------------------------------------------------------ */
#define extension_ALL "*.*"
#define extension_HTM "*.htm"
#define extension_XLS "*.xls"
#define extension_BMP "*.bmp"
#define extension_JPG "*.jpg"
#define extension_JP2 "*.jp2"
#define extension_J2K "*.j2k"
#define extension_PNG "*.png"
#define extension_GIF "*.gif"
#define extension_TIF "*.tif"
#define extension_FST "*.fst"
#define extension_SHAPEFILE "*.shp"
#define extension_CEOS "VOL-*"
#define extension_HDF5 "*.h5"
#define extension_L1B "*.l1b"
#define extension_GPX "*.gpx"
#define extension_KML "*.kml"
#define extension_GCP "*.gcp"
#define extension_TXT "*.txt"
#define extension_PTS "*.pts"
#define extension_CSV "*.csv"
#define extension_VECTOR JOIN_1(extension_SHAPEFILE)
#define extension_GROUNDCONTROLPOINT JOIN_3(extension_GCP, extension_TXT,                     \
                                extension_PTS, EXTENSION_SEPARATOR)
#define extension_TERRAIN extension_TIF
#define extension_TEXTURE JOIN_2(extension_TIF, extension_FST, EXTENSION_SEPARATOR)

// Tools-------------------------------------------------------------------------
#define caption_UNKNOWN_TOOL "Herramienta Desconocida"
#define caption_ADJUSTMENTS_TOOL "Ajustes de imagen"
#define caption_ANIMATION_TOOL "Animacion de imagenes"
#define caption_MAP_TOOL "Creacion de mapas"

#define caption_MEASSURE_TOOL "Mediciones"
#define caption_DRAW_TOOL "Edicion vectorial"
#define caption_DRAW_CREATION_TOOL "Graficos y anotaciones"
#define caption_CLASSIFICATION_TOOL "Clasificacion tematica"
#define caption_GPS_TOOL "Edicion de vectores con GPS"

/** Filtros--------------------------------------------------------------------- */
#define filter_ALL_FILES BUILD_FILTER_WITH_EXT(name_ALL_FILES, extension_ALL,     \
                                               filter_extension_ALL)

#define filter_VIEWER_2D_ELEMENTS JOIN_15(                                          \
        BUILD_FILTER(name_ALL_SUPPORTED_FILES, filter_extension_VIEWER_2D),         \
        BUILD_FILTER(name_COMMON_IMAGE_FILES, filter_extension_COMMON_IMAGE),       \
        BUILD_FILTER(name_TIF_FILES, filter_extension_TIF),                         \
        BUILD_FILTER(name_FAST_FORMAT_FILES, filter_extension_FST),                 \
        BUILD_FILTER(name_CEOS_FILES, filter_extension_CEOS),                       \
        BUILD_FILTER(name_HDF5_FILES, filter_extension_HDF5),                       \
        BUILD_FILTER(name_L1B_FILES, filter_extension_L1B),                         \
        BUILD_FILTER(name_VECTOR_FILES, filter_extension_VECTOR),                   \
        BUILD_FILTER(name_GPX_FILES, filter_extension_GPX),                         \
        BUILD_FILTER(name_KML_FILES, filter_extension_KML),                         \
        BUILD_FILTER(name_J2K_JP2_FILES, filter_extension_J2K_JP2),                 \
        BUILD_FILTER(name_SHAPEFILE_FILES, filter_extension_SHAPEFILE),             \
        BUILD_FILTER(name_OGC_WMS_TMS, filter_extension_OGC_WMS_TMS),               \
        BUILD_FILTER(name_MTL, filter_extension_MTL),                               \
        BUILD_FILTER_WITH_EXT(name_ALL_FILES, extension_ALL, filter_extension_ALL), \
        FILTER_SEPARATOR)

#define filter_VIEWER_3D_ELEMENTS JOIN_8(                                         \
      BUILD_FILTER(name_ALL_SUPPORTED_FILES, filter_extension_VIEWER_3D),         \
      BUILD_FILTER_WITH_EXT(name_TERRAIN_FILES, extension_TERRAIN,                \
                            filter_extension_TERRAIN),                            \
      BUILD_FILTER_WITH_EXT(name_TEXTURE_FILES, extension_TEXTURE,                \
                            filter_extension_TEXTURE),                            \
      BUILD_FILTER_WITH_EXT(name_VECTOR_FILES, extension_VECTOR,                  \
                            filter_extension_VECTOR),                             \
      BUILD_FILTER(name_GPX_FILES, filter_extension_GPX),                         \
      BUILD_FILTER(name_KML_FILES, filter_extension_KML),                         \
      BUILD_FILTER(name_SHAPEFILE_FILES, filter_extension_SHAPEFILE),             \
      BUILD_FILTER_WITH_EXT(name_ALL_FILES, extension_ALL, filter_extension_ALL), \
      FILTER_SEPARATOR)

#define filter_OPEN_TERRAIN_ELEMENTS JOIN_3(                                      \
      BUILD_FILTER(name_ALL_SUPPORTED_FILES, filter_extension_TERRAIN),           \
      BUILD_FILTER_WITH_EXT(name_TERRAIN_FILES, extension_TERRAIN,                \
                            filter_extension_TERRAIN),                            \
      BUILD_FILTER_WITH_EXT(name_ALL_FILES, extension_ALL, filter_extension_ALL), \
      FILTER_SEPARATOR)

#define filter_OPEN_TEXTURE_ELEMENTS JOIN_3(                                      \
      BUILD_FILTER(name_ALL_SUPPORTED_FILES, filter_extension_TEXTURE),           \
      BUILD_FILTER_WITH_EXT(name_TEXTURE_FILES, extension_TEXTURE,                \
                            filter_extension_TEXTURE),                            \
      BUILD_FILTER_WITH_EXT(name_ALL_FILES, extension_ALL, filter_extension_ALL), \
      FILTER_SEPARATOR)

#define filter_SAVE_VISUALIZATION JOIN_6(                                         \
      BUILD_FILTER(name_ALL_SUPPORTED_FILES, filter_extension_COMMON_IMAGE),      \
      BUILD_FILTER_WITH_EXT(name_BMP, extension_BMP, filter_extension_BMP),       \
      BUILD_FILTER_WITH_EXT(name_TIF, extension_TIF, filter_extension_TIF),       \
      BUILD_FILTER_WITH_EXT(name_JPG, extension_JPG, filter_extension_JPG),       \
      BUILD_FILTER_WITH_EXT(name_PNG, extension_PNG, filter_extension_PNG),       \
      BUILD_FILTER_WITH_EXT(name_ALL_FILES, extension_ALL, filter_extension_ALL), \
      FILTER_SEPARATOR)

#define filter_SAVE_STATISTICS JOIN_2(                                     \
      BUILD_FILTER_WITH_EXT(name_HTM, extension_HTM, filter_extension_HTM),      \
      BUILD_FILTER_WITH_EXT(name_XLS, extension_XLS, filter_extension_XLS),      \
      FILTER_SEPARATOR)

#define filter_SAVE_VECTOR JOIN_3(                                                \
      BUILD_FILTER(name_ALL_SUPPORTED_FILES, filter_extension_VECTOR),            \
      BUILD_FILTER_WITH_EXT(name_VECTOR_FILES, extension_VECTOR,                  \
                            filter_extension_VECTOR),                             \
      BUILD_FILTER_WITH_EXT(name_ALL_FILES, extension_ALL, filter_extension_ALL), \
      FILTER_SEPARATOR)

#define filter_SAVE_GCP JOIN_5(                                                \
      BUILD_FILTER(name_ALL_SUPPORTED_FILES, filter_extension_GROUNDCONTROLPOINT), \
      BUILD_FILTER_WITH_EXT(name_GCP_FILES, extension_GCP, filter_extension_GCP),  \
      BUILD_FILTER_WITH_EXT(name_TXT_FILES, extension_TXT, filter_extension_TXT),  \
      BUILD_FILTER_WITH_EXT(name_PTS_FILES, extension_PTS, filter_extension_PTS),  \
      BUILD_FILTER_WITH_EXT(name_ALL_FILES, extension_ALL, filter_extension_ALL),  \
      FILTER_SEPARATOR)

#define filter_SAVE_RASTER_OPERATION JOIN_3(                                      \
      BUILD_FILTER(name_ALL_SUPPORTED_FILES, filter_extension_TIF),               \
      BUILD_FILTER_WITH_EXT(name_TIF_FILES, extension_TIF, filter_extension_TIF), \
      BUILD_FILTER_WITH_EXT(name_ALL_FILES, extension_ALL, filter_extension_ALL), \
      FILTER_SEPARATOR)

// MESSAGE-----------------------------------------------------------------------

// Mensaje de error en inicio de proceso de reproyeccion
#define message_INVALID_REPROJECTION_RASTER_ERROR \
   "Para iniciar el proceso de reproyeccion se debe tener una imagen seleccionada" \
   "y esta debe poseer un sistema de referencia valido"
// Agregar Elemento/Textura
#define message_ADD_ELEMENT_ERROR "Error al intentar agregar elementos."
#define message_ADD_TEXTURE_ERROR "Error al agregar la textura."
#define message_LAYER_BLOCK "No es posible eliminar la capa si la misma se encuentra bloqueada"
#define message_LAYER_BLOCK_s "No es posible eliminar la capa %s si se encuentra bloqueada"
#define message_ELEMENT_REMOVE_CONFIRMATION "Esta seguro que desea remover la capa?"
#define message_ELEMENT_REMOVE_ELEMENT_CONFIRMATION "Esta seguro que desea quitar el elemento?"

// Bitmaps
#define message_CANT_CREATE_BITMAP_s "No se puede crear el bitmap %s."
#define message_ADD_ELEMENT_ERROR_s "No se pudo agregar el elemento: %s."

// Vectores----------------------------------------------------------------------
#define message_POINT_INSERT_ERROR "Error al ingresar punto."
#define message_POINT_OUTSIDE_ALLOWED_AREA_ERROR "Punto afuera del area valida."
#define message_CANT_DELETE_VECTOR "No se puede borrar el archivo."
#define message_VECTOR_DELETION_ERROR "Ocurrio un error al eliminar el archivo."

// Edicion-----------------------------------------------------------------------
#define message_ELEMENT_IN_EDITION "Existe un elemento en edicion."

#define message_CANT_EDIT_ELEMENT_WITHOUT_SR "No se puede editar/crear vectores en una"\
                                             " capa sin sistema de referencia"

// Mark button-------------------------------------------------------------------
#define message_POINT_OUTSIDE_IMAGE "Punto fuera de la imagen activa."

// Capas-------------------------------------------------------------------------
#define message_VECTOR_LAYER_OF_s "Capa vectorial de %s"

// General-----------------------------------------------------------------------
#define message_CLASS_NOT_FOUND "No se encuentra la clase."
#define message_EXCEPTION_s "Excepcion: %s."
#define message_EXCEPTION_TRY_s "Intento de excepcion: %s."
#define message_NOT_APPLICABLE "N/A"
#define message_NOT_AVAILABLE "No disponible."
#define message_NA "N/D"
#define message_PERCENT_VALUE_d "%d %%"
#define message_NO_DATA "S/D"

// PARTs-------------------------------------------------------------------------
#define message_PART_SAVE_ERROR "Error al salvar los cambios."

// Raster------------------------------------------------------------------------
#define message_READING_BANDS "Leyendo Bandas"
#define BAND_NAME_PREFIX_d "Banda #%d"
#define message_INVALID_NO_DATA_VALUE "Valor de dato no valido incorrecto"
#define message_ALL_BANDS_INVALID_ERROR "Debe existir alguna banda valida en la imagen"

// Renderer----------------------------------------------------------------------
#define message_RENDERER_CREATION_ERROR_ss \
   "Error al crear el renderizador de %s para el elemento %s."
#define message_RENDERING_PROGRESS "Renderizando"
#define message_RENDERER_ELEMENT_ERROR_s "No se pudo desplegar el elemento %s."

// Vectores----------------------------------------------------------------------
#define message_UNDEFINED "Sin Definir"   // migrar a xmlnames.h
#define message_POINT "Punto"             // migrar a xmlnames.h
#define message_LINE "Linea"              // migrar a xmlnames.h
#define message_POLYGON "Poligono"        // migrar a xmlnames.h
#define message_GEOM_COLLECTION "Multigeometria" // migrar a xmlnames.h
#define message_VECTOR_CATEGORIZED_STYLE_INVALID \
   "Configuracion de clases incorrecta, revise las definiciones"
#define message_VECTOR_STYLE_ELEMENT_INVALID_TYPE_ERROR \
   "Error al obtener el tipo de vector a patir del estilo."
#define message_VECTOR_INVALID_EXT_ERROR "Extension invalida."
#define message_OPEN_VECTOR_ERROR "Error al abrir el vector."
#define message_LAYER_MODIFIED_CLOSE_ERROR \
   "Error al salvar registro. La tabla de datos fue modificada con la geomertia abierta."
#define message_MEASSURE_ERROR \
   "No es posible guardar la medicion, verifique que ha cerrado la misma"
#define message_REQUIRED_VECTOR_ELEMENT \
   "Para acceder al modo de edicion vectorial es necesario seleccionar una capa vectorial"

// Hotlink-----------------------------------------------------------------------
#define message_NO_APP_ERROR_s "No se encontro una aplicacion para desplegar %s."
#define message_UNKNOW_EXTENTION_s "%s no es un tipo de archivo conocido."
#define message_NO_FILE_s "Error no se pudo encontrar %s."

// Archivos----------------------------------------------------------------------
#define message_READ_FILE_ERROR "Error al leer el archivo."

// QUESTION----------------------------------------------------------------------
#define question_SAVE_PART_CHANGES "Las opciones contienen cambios. Desea guardarlos?"
#define question_APPLY_PART_CHANGES "Desea aplicar los cambios realizados?"
#define question_INVALID_DATA "Datos Invalidos. Desea restaurar los datos anteriores?"
#define question_TOO_MANY_DIRECTORY_RECURSIONS \
   "Demasiada recursion al eliminar directorios. Desea continuar?"
#define question_APPLY_CHANGES "Desea aplicar los cambios realizados?"
#define question_DELETE_ITEM "Esta seguro que desea eliminar el item?"
#define question_DIR_NOT_EMPTY \
   "El directorio seleccionado contiene archivos, si continua puede perder datos. Desea continuar?"
#define question_CONFIRM_EXIT_PROCESS "Esta seguro de que desea abandonar el proceso?"

// NAVIGATION WIDGET-------------------------------------------------------------
#define message_DEGREE_ERROR_s "Error en grados de %s."
#define message_MINUTE_ERROR_s "Error en minutos de %s."
#define message_SECOND_ERROR_s "Error en segundos de %s."
#define message_LONGITUDE "longitud"
#define message_LATITUDE "latitud"
#define message_COORDINATE_OUT_OF_RANGE \
   "El rango coordenadas debe ser latitud +/-90.0 y longitud +/-180.0."
#define message_POINT_NOT_IN_WORLD message_POINT_OUTSIDE_ALLOWED_AREA_ERROR

// PIXEL LINE NAVIGATION WIDGET--------------------------------------------------
#define message_LINE_ERROR "Fila fuera de la imagen."
#define message_PIXEL_ERROR "Columna fuera de la imagen."
#define message_TO_LONG_ERROR "Error en las coordenadas."

// APLICATION--------------------------------------------------------------------
#define message_CONFIGURATION_ERROR \
   "Error al inicializar la configuracion. Por favor reinstalar el software."
#define message_APPLICATION_INITIALIZATION_ERROR "Error al inicializar la aplicacion."
#define message_UNSUPPORTED_LANGUAGE \
   "El idioma no es soportado por el sistema: se usaran traducciones disponibles."
#define message_MAIN_TOOL_NOT_CREATED "Error al crear la ventana principal."
#define caption_APPLICATION_NAME "Aplicacion basada en SuriLib."
#define message_APPLICATION_INSTANCE_RUNNING "Ya existe una instancia de la aplicacion corriendo."
#define message_INSTALL_FILES_NOT_FOUND \
   "Error al validar la instalacion. Por favor reinstalar el software."
#define caption_PRINT "Imprimir"
#define question_OVERWRITE_FILE "Desea sobreescribir el archivo?"
#define message_INVALID_DIRECTORY "Directorio inexistente."
#define message_INVALID_FILENAME "Nombre de archivo invalido."
#define message_FILENAME_WITHOUT_PERMISSION "No tiene permiso de escritura sobre el archivo."
#define message_DIRECTORY_WITHOUT_PERMISSION "No tiene permiso de escritura sobre el directorio."

// TERRENOS---------------------------------------------------------------------
#define message_TEXTURE_RASTER_ERROR "Textura invalida."
#define message_TERRAIN_TEXTURE_SPATIAL_REFERENCE_ERROR \
   "La referencia espacial del terreno y la textura no son compatibles."
#define message_TERRAIN_TEXTURE_SUBSET_ERROR "El terreno y la textura no tienen superposicion."

// EXPORTER WIDGET--------------------------------------------------------------
#define message_NOT_ENOUGHT_DISK_SPACE "Espacio de disco suficiente."
#define message_INVALID_OUTPUT "El archivo de origen no puede ser igual al de destino."
#define message_SAVE_ERROR "Error al guardar el archivo."
#define message_RASTER_OPERATION_ERROR "Error en la operacion raster para el elemento %s."
#define message_GET_WORLD_ERROR "No se pudo obtener el sistema de salida."

// SPATIALSELECTIONPART----------------------------------------------------------
#define message_LATLONG_POSITION_ERROR "Los puntos estan fuera de la imagen."
#define message_SUBSET_ORDER_ERROR "Los puntos estan invertidos."
#define label_IMAGE_BUTTON "Imagen"
#define label_WINDOW_BUTTON "Ventana"
#define tooltip_IMAGE_BUTTON "Coordenadas de imagen completa"
#define tooltip_WINDOW_BUTTON "Coordenadas de la ventana"

// BAND SELECTION PART-----------------------------------------------------------
#define message_INVALID_BAND_SELECTED "Seleccion de bandas invalida."

// SPECTRALSELECTIONPART---------------------------------------------------------
#define label_INPUT_BANDLIST "Bandas disponibles"
#define label_OUTPUT_BANDLIST "Bandas seleccionadas"
#define tooltip_ADD_BUTTON "Agregar al listado de seleccion"
#define tooltip_REMOVE_BUTTON "Eliminar del listado de seleccion"
#define tooltip_UP_BUTTON "Subir en el listado de seleccion"
#define tooltip_DOWN_BUTTON "Subir en el listado de seleccion"
#define label_INDEX_BAND_COLUMN "#"

// INDEX SELECTION PART----------------------------------------------------------
#define message_INCOMPLETE_BAND_SELECTION "Falta seleccionar banda."
#define message_INVALID_BAND_SELECTION "Las bandas deben ser distintas."

// BANDMATH PART---------------------------------------------------------------
#define label_VARIABLES "Variables"
#define label_BANDS "Bandas"
#define content_INTEGEROF_d "Entero de %d bits"
#define content_UNSIGNEDINTEGEROF_d "Entero no signado de %d bits"
#define content_FLOATOF_d "Punto flotante de %d bits"
#define message_UPDATE_VARIABLES_TO_CONTINUE "Actualizar las variables y despues aplicar"
#define message_ERROR_IN_POSITION_sd "Error '%s' en posicion %d"
#define message_ERROR_MULTIPLY_cc "Error: Variable inesperada, intente con %c*%c"
#define message_ERROR_UNEXPECTED_TOKEN_c "Error:Token desconocido %c"
#define message_UNEXPECTED_OPERATOR_cc "Error:Operador invalido %c%c "
#define message_UNEXPECTED_BINARY_OPERATOR "Operador binario no esperado"
#define message_UNKNOWN_TOKEN "Token desconocido"
#define message_UNEXPECTED_EOF "Fin de formula no esperado"
#define message_UNEXPECTED_COMMA "Coma no esperada"
#define message_UNEXPECTED_ARGUMENT "Argumento no esperado"
#define message_UNEXPECTED_VALUE "Valor no esperado"
#define message_UNEXPECTED_VARIABLE "Variable no esperado"
#define message_UNEXPECTED_PARENTHESIS "Parentesis no esperado"
#define message_UNEXPECTED_STRING_POSITION "String en posicion invalida"
#define message_UNEXPECTED_STRING_FUNCTION_PARAMETER \
   "Llamada a funcion de strings con parametro invalido"
#define message_UNEXPECTED_VALUE_FUNCTION_PARAMETER \
   "Llamada a funcion con parametro no numerico"
#define message_MISSING_PARENTHESIS "Falta un parentesis"
#define message_UNEXPECTED_FUNCITON "Funcion no esperada. Ej: sin(8)cos(9)"
#define message_UNKNOWN_CONSTANT "Constante desconocida"
#define message_TOO_MANY_PARAMS "Llamada a funcion con demasiados parametros"
#define message_TOO_FEW_PARAMS "Llamada a funcion con pocos parametros"
#define message_TYPE_CONFLICT "Operador binario entre operandos de distinto tipo"
#define message_STR_RESULT "El resultado es un string"
#define message_GENERIC_ERROR "Error generico"
#define message_CONFLICTLOCAL "Conflicto con locale"
#define message_UNEXPECTED_CONDITIONAL "Condicional no esperado"
#define message_MISSING_ELSE "Falta else"
#define message_MISPLACED_COLON "Coma mal ubicada"
#define message_INTERNAL_ERROR "Error interno"
#define message_UNDEFINED_ERROR "Error indefinido"

// CLASSFUSION PART
#define message_ADD_FUSSION_CLASS_ERROR \
   "Error al agregar la clase. El nombre de la clase a crear ya existe o no posee nombre"
// LUT SELECTION PART -----------------------------------------------------------
#define label_EMPTY_LUT "Desconocido"
#define label_ELEMENT_LUT_COMMENT " - de elemento"

// ENHANCEMENT SELECTION PART----------------------------------------------------
#define message_ENHANCEMENT_ERROR "Error al aplicar el realce."
#define message_ENHANCEMENT_CHOICE_ERROR "Error al aplicar realce seleccionado."

// COLORTABLE SELECTION PART
#define message_COLORTABLE_CHOICE_ERROR "Error al aplicar la tabla de colores seleccionada."

// Tarea
#define message_TASK_ACTIVE "No es posible iniciar la tarea mientras haya otra iniciada"
#define message_CANT_EXECUTE_ACTION "No es posible ejecutar esa accion sin que la tarea" \
   " se encuentre iniciada"
#define message_CANT_FINISH_TASK "La tarea no pudo finalizar correctamente"
// Corregistro
#define message_GCP_NEWFILENAME_ERROR \
   "Nombre de archivo invalido. Posible causa: no se puede sobresctibir un archivo existente"
#define message_GCP_OPENFILENAME_ERROR \
   "Nombre de archivo invalido. El archivo debe existir en disco."
#define message_INCOMPLETE_DATA_ENTRY \
   "No se pudo iniciar la tarea correctamente, faltan datos de entrada."
#define message_CANT_START_COREGISTER_TASK \
   "No se pudo iniciar la tarea correctamente, se necesitan al menos dos visualizadores activos" \
   " con imagenes desplegadas."
#define message_GCPAUTOGEN_NO_RASTER_MODEL \
   "La imagen base no posee modelo raster para corregistrar"
// TablePart----------------------------------------- ----------------------------
#define message_DATA_ERROR_FLOAT_dd "Error en fila %d columna %d. Deberia ser real."
#define message_DATA_ERROR_INT_dd "Error en fila %d columna %d. Deberia ser entero."
#define message_FIELD_NAME_DUPLICATED "Ya existe un campo con ese nombre."
#define message_VECTOR_SAVE_TO_DISK_ERROR "No se pudo guardar el vector en disco"
#define message_NON_ASCII_FIELD_NAME_ERROR "El nombre del campo debe ser ASCII"

// AddColumnPart------------------------------------------------------------------
#define message_SINTAX_VALID "La sintaxis de la ecuacion es correcta."
#define message_SINTAX_ERROR "Error en la sintaxis de la ecuacion."

// Viewer2DProperties
#define message_GIS_ERROR "No se puede cambiar al Modo Imagen con elementos desplegados en SIG"

// Raster------------------------------------------------------------------------
#define message_RASTER_IMAGE_MUST_BE_DISPLAYED "imagen"

// RASTER PROPERTIES PART--------------------------------------------------------
#define label_COLOR_TABLE "Tabla de Colores"
#define label_RGB "RVA"
#define label_NDVI "NDVI"
#define label_NDWI "NDWI"
#define label_SMI "SMI"
#define label_RVI "RVI"

// PROCESOS ---------
#define message_PROCESS_EXECUTION_ERROR "Error al ejecutar el proceso"
#define caption_DEFAULT_PROCESS_TITLE "Proceso Raster"
#define message_PROCESS_START_ERROR \
   "Es necesario  tener al menos una capa seleccionada en modo imagen para iniciar el proceso"
#define message_SELECTION_EXPORT_PROCESS_START_ERROR \
   "Es necesario tener una seleccion activa para iniciar el proceso"
#define message_PROCESS_EXPORT_VECTOR_LAYER_START_ERROR \
   "Es necesario tener al menos una capa vectorial seleccionada para iniciar el proceso"
#define message_PROCESS_EXPORT_VECTOR_OPERATION_LAYER_START_ERROR "Es necesario tener al menos" \
   " una capa vectorial seleccionada y dos capas vectoriales cargadas para iniciar el proceso"
#define message_PROCESS_MERGE_LAYERS_START_ERROR \
   "Es necesario tener al menos dos capas vectoriales seleccionadas"
#define message_STATISTICS_EXPORTER "Las estadisticas se exportaron de forma exitosa"
#define message_MOSAIC_START_ERROR "Es necesario tener al menos dos capas Raster seleccionadas"
#define message_HISTOGRAM_MATCHING_ERROR \
   "Es necesario tener al menos dos capas raster cargadas"
#define message_MOSAIC_SPATIAL_ERROR "Las bandas deben tener el mismo sistema de referencia"
#define message_MOSAIC_BAND_ERROR "Deben tener la misma cantidad de bandas"
// CLASIFICADOR------------------------------------------------------------------
#define message_RASTER_CONFIGURATION_ERROR "Error al preparar el raster para el procesamiento."
#define caption_MAXIMUN_LIKELIHOOD "Maxima verosimilitud"
#define caption_MINIMUM_DISTANCE "Distancia Minima"
#define caption_THRESHOLD_CLASS "Clasificacion por umbrales"
#define caption_THRESHOLD_DEFINITION "Definicion de umbrales por clase"
#define caption_PARALLELEPIPED "Paralelepipedo"
#define caption_MAHALANOBIS "Mahalanobis"
#define caption_KMEANS "KMeans"
#define message_INVALID_VECTOR_EXTEND_s \
   "El area de entrenamiento se encuentra fuera de la imagen: %s."
#define message_INVALID_CLASS_s \
   "No se pudo usar el area de entrenamiento %s para la clasificacion."
#define message_MEANS_CALCULATION_ERROR \
   "Error al calcular los parametros estadisticos de las areas de entrenamiento."
#define message_PERCENTAGE_d "%d %%"
#define caption_CLASS_MAPPING "Asignar clases"
#define caption_CLASS_FUSION "Fusionar clases"
#define caption_CLASS_SELECTION_PART "Areas de clasificacion"

// WMS
#define caption_ADD_REMOTE_LAYER "Agregar capa remota"
#define caption_WMS_SOURCE "Fuente wms"
#define caption_WMS_PROPERTIES "Propiedades wms"
#define caption_WMS_VISUALIZATION "Visualizacion wms"
#define caption_WMS_LIBRARY "Servidores WMS"
#define message_INVALID_TOOL_ON_WMS "Esta operacion no puede efectuarse sobre una capa remota"

// PROPIEDADES VISUALIZADOR
#define caption_VIEWER3D_PROPERTIES "Visualizador 3D"

// WIDGET MEDICION
#define tooltip_MEASURE_DISTANCE "Medir distancia"
#define tooltip_MEASURE_AREA "Medir area"
#define tooltip_MEASSURE_WIDGET "Herramienta medicion"

// LAYERTREEWIDGET
#define caption_NEW_GROUP_INPUT "Ingresar el nombre del grupo"
#define content_DEFAULT_GROUP_NAME "Nuevo grupo"

// SEARCH WIDGET
#define message_ERROR_EDITING_READ_ONLY_TABLE \
   "El Proyecto esta en edicion, limpie la consulta vuelva a intentar."

// PROGRESO ---------------------------------------------------------------------
#define progress_ADDING_FILES "Agregando archivos"
#define progress_EXTRACTING_FILES "Extrayendo archivos"

// ICONOS
#define message_IMAGE_TOO_BIG "Imagen muy grande"

// Texto que se muestra en formato html.
#define html_IMAGEINFO "Datos de la imagen"
#   define html_PIXELAREA "Area de pixel"
#   define html_UNIT "Unidades"
#   define html_PIXELCOUNT "Cantidad de pixels por banda"
#   define html_IMAGEAREA "Area de la imagen"
#   define html_URL "Url"
#   define html_EXTENT "Extension"

#define html_TRAINNING_STATISTICS "Areas de entrenamiento"
#define html_TEST_STATISTICS "Areas de test"

#define html_STATISTICS "Estadisticas"
#   define html_BASICPARAMETERS "Parametros basicos"
#      define html_PARAMETERS "Parametro"
#      define html_MEAN "Media"
#      define html_VARIACE "Varianza"
#      define html_MAXIMUN "Maximo"
#      define html_MINIMUN "Minimo"
#      define html_MODE "Moda"
#   define html_CORRELATIONMATRIX "Matriz de correlacion"
#   define html_COVARIANCEMATRIX "Matriz de covarianza"
#   define html_EIGENVALUE "Autovalor"

#   define html_TRAINNINGCONFUSIONMATRIX "Matriz de confusion sobre areas de entrenamiento"
#   define html_TESTCONFUSIONMATRIX "Matriz de confusion sobre areas de test"
#   define html_TOTAL "Total"
#   define html_CLASS "Clase"
#   define html_NO_CLASS "sin clase"


#define html_HISTOGRAM "Histogramas"
#   define html_DN "DN"
#   define html_COUNT "Cuentas"
#   define html_AREA "Areas"
#   define html_CLASS_NAME "Clase"

#define label_NDVI_0_SLICE_LABEL    "Sin Vegetacion"
#define label_NDVI_02_SLICE_LABEL   "Vegetacion dispersa"
#define label_NDVI_03_SLICE_LABEL   "Vegetacion escasa"
#define label_NDVI_04_SLICE_LABEL   "Vegetacion moderada"
#define label_NDVI_05_SLICE_LABEL   "Vegetacion abundante"
#define label_NDVI_06_SLICE_LABEL   "Vegetacion muy abundante"
#define label_NDVI_07_SLICE_LABEL   "Vegetacion tupida"
#define label_NDVI_1_SLICE_LABEL    "Selva"

#define label_NDWI_M03_SLICE_LABEL  "Sin Agua"
#define label_NDWI_M02_SLICE_LABEL  "Agua dispersa"
#define label_NDWI_M01_SLICE_LABEL  "Agua Escasa"
#define label_NDWI_0_SLICE_LABEL    "Agua moderada"
#define label_NDWI_01_SLICE_LABEL   "Agua abundante"
#define label_NDWI_02_SLICE_LABEL   "Agua muy abundante"
#define label_NDWI_03_SLICE_LABEL   "Agua en exceso"
#define label_NDWI_04_SLICE_LABEL   "Oceano"

#define message_SEND_ELEMENT_TO_ERROR \
"No es posible copiar el elemento %s en el contexto. El elemento no es compatible con el contexto."

// SEARCH WIDGET
#define message_INVALID_CONDITION "La condicion de la busqueda es invalida."

// TABLE IMPORT X-Y PART
#define message_EMPTY_FILE_ERROR "Debe seleccionar un archivo."
#define message_REPEAT_COLUMN_ERROR "No puede haber columnas repetidas."
#define message_OPEN_FILE_ERROR "No se pudo abrir el archivo."
#define message_COLUMNX_EQUALS_COLUMNY_ERROR \
   "Las columnas X e Y tienen que estar en posiciones diferentes"
#define message_TOKEN_ERROR "Error en el separador"
#define message_COLUMN_VALUE_ERROR "El formato de punto flotante de la columna %d es incorrecto"

#endif /* SURI_MESSAGES_H_ */
/** \endcond */
#ifndef MESSAGES_H_
#define MESSAGES_H_

/**
 * Prefijos de los define para las distintas funciones de cada constante:
 *
 *  tooltip		: Mensajes emergentes de cualquier elemento de pantalla
 *  content		: contenido default de un campo (Ej: texto default de un wxTextBox,
 *  Opciones de un wxComboBox, etc)
 *  label		: texto invariante que debe mostrado en pantalla (Ej: texto de un wxStaticBox)
 *  caption		: Titulo de ventana/dialogo
 *  filter		: Filtro de extensiones para los dialogos (No se traduce, solo para reusarlo)
 *  message		: Todo mensaje al usuario que sera mostrado mediante un SHOW_XXX()
 *  question	: Pregunta al usuario
 *  menu			: Entrada de menu
 *  format		: String de formateo de datos
 *  filename	: Nombre de archivo
 *  extension	: Extension (con punto)
 *  name			: Nombre de algo (por ejemplo Punto)
 *
 */

// Tooltips ----------------------------------------------------------------------
// Caption  --------------------------------------------------------------------
#define caption_SELECT_GENERIC_IMAGE "Seleccionar imagen"
#define caption_SELECT_PAT_DIR "Seleccionar el directorio para guardar el archivo PAT comprimido"
#define caption_SELECT_PAT_PREVIEW "Seleccionar vista previa del PAT"
#define caption_SELECT_TEXTURE "Seleccionar imagen de textura para el terreno"
#define caption_SELECT_ELEMENT "Seleccionar el elemento para agregar"

#define caption_INSTALL_PAT "Instalar PAT"
#define caption_PAT_MANAGER "Administracion de PATs"
#define caption_BLANK_PAT "Nuevo PAT"
#define caption_SAVE_PAT "Guardar PAT"
#define caption_PAT_PROPERTIES "Descripcion de PAT"
#define caption_PAT_TOOLBAR "Herramientas de PAT"
#define caption_PAT_INFORMATION "Informacion de PAT"
#define caption_HTML_VIEWER "Visualizador de HTML"
#define caption_PRINT "Imprimir"
#define caption_HELP_s "Ayuda: %s"
#define caption_PAT_INSTALL_DIR "Directorio donde se instalara el PAT"
#define caption_SAVE_AS "Guardar como..."

// Contextos
#define caption_RASTER_CONTEXT "Imagenes"  // migrar al xmlnames
#define caption_MAP_CONTEXT "Mapas"   // migrar al xmlnames
#define caption_TERRAIN_CONTEXT "Terrenos 3D"  // migrar al xmlnames
#define caption_DOCUMENTS_CONTEXT "Documentos"  // migrar al xmlnames
#define caption_PHOTOGRAPHS_CONTEXT "Fotografias"  // migrar al xmlnames
#define caption_MULTIMEDIA_CONTEXT "Multimedia"
#define caption_ACTIVITIES_CONTEXT "Actividades"  // migrar al xmlnames
// Propiedaes
#define caption_PHOTO_PROPERTIES "Propiedades de fotografia"
#define caption_HOTLINK_PROPERTIES "Propiedades de hipervinculo"

// #define content_SELECT_PAT "Seleccione un PAT para obtener informacion"
#define content_NO_DESCRIPTION "Sin descripcion"
#define content_INVALID_DATE "Fecha invalida"
#define content_FALLBACK_DATE "Fecha invalida"
#define content_FALLBACK_AUTHOR "Anonimo"

// Extension -------------------------------------------------------------------
#define extension_DOT_PAT ".pat"
#define extension_PAT "pat"
#define extension_XML "xml"

// Filename----------------------------------------------------------------------
// #define filename_DESCRIPTION_XML "descripcion.xml"
// #define filename_CONFIGURATION_XML "configuracion.xml"

/** macros auxiliares */
#define JOIN_1(Parameter) Parameter
/** Concatena 2 parametros con un separador. */
#define JOIN_2(Parameter1, Parameter2, Separator) Parameter1 +                    \
   std::string(Separator) + Parameter2
/** Concatena 3 parametros con un separador. Utiliza JOIN_2 */
#define JOIN_3(Parameter1, Parameter2, Parameter3, Separator)                     \
   JOIN_2(JOIN_2(Parameter1, Parameter2, Separator), Parameter3,                  \
          Separator)
/** Concatena 4 parametros con un separador. Utiliza JOIN_3 */
#define JOIN_4(Parameter1, Parameter2, Parameter3, Parameter4, Separator)         \
   JOIN_2(JOIN_3(Parameter1, Parameter2, Parameter3, Separator),                  \
          Parameter4, Separator)
/** Concatena 5 parametros con un separador. Utiliza JOIN_4 */
#define JOIN_5(Parameter1, Parameter2, Parameter3, Parameter4, Parameter5,        \
               Separator)                                                         \
   JOIN_2(JOIN_4(Parameter1, Parameter2, Parameter3, Parameter4,                  \
                 Separator), Parameter5, Separator)
#define JOIN_6(Parameter1, Parameter2, Parameter3, Parameter4, Parameter5,        \
               Parameter6, Separator)                                             \
   JOIN_2(JOIN_5(Parameter1, Parameter2, Parameter3, Parameter4,                  \
                 Parameter5, Separator), Parameter6, Separator)

// TODO(Sebastian - TCK# 848): Mover este codigo a la libreria
#define FILTER_SEPARATOR "|"
#define EXTENSION_SEPARATOR ";"
#define BUILD_FILTER(Name, Filter) _(Name) + std::string(FILTER_SEPARATOR Filter)

// TODO(Sebastian - TCK# 848): Mover este codigo a la libreria
#ifdef __WXMSW__
#  define filter_extension_ALL "*.*"
#  define filter_extension_BMP "*.bmp"
#  define filter_extension_JPG JOIN_3("*.jpg", "*.jpeg", "*.jpe",                 \
                                      EXTENSION_SEPARATOR)
#  define filter_extension_PNG "*.png"
#  define filter_extension_GIF "*.gif"
#  define filter_extension_TIF JOIN_2("*.tif", "*.tiff", EXTENSION_SEPARATOR)
#  define filter_extension_FST JOIN_4("*_hrf.fst", "*_htm.fst", "*_hpn.fst",      \
                                      "header.dat", EXTENSION_SEPARATOR)
#  define filter_extension_SHAPEFILE "*.shp"
#  define filter_extension_CEOS "VOL-*"
#  define filter_extension_HDF5 JOIN_2("*.hdf", "*.h5", EXTENSION_SEPARATOR)
#  define filter_extension_L1B "*.l1b"
#  define filter_extension_GPX "*.gpx"
#  define filter_extension_KML "*.kml"
#  define filter_extension_HTM JOIN_2("*.htm", "*.html", EXTENSION_SEPARATOR)
#  define filter_extension_XLS "*.xls"
#else
#  define filter_extension_ALL "*"
#  define filter_extension_BMP "*.[Bb][Mm][Pp]"
#  define filter_extension_JPG JOIN_3("*.[Jj][Pp][Gg]", "*.[Jj][Pp][Ee][Gg]",     \
                                      "*.[Jj][Pp][Ee]", EXTENSION_SEPARATOR)
#  define filter_extension_PNG "*.[Pp][Nn][Gg]"
#  define filter_extension_GIF "*.[Gg][Ii][Ff]"
#  define filter_extension_TIF JOIN_2("*.[Tt][Ii][Ff]", "*.[Tt][Ii][Ff][Ff]",     \
                                      EXTENSION_SEPARATOR)
#  define filter_extension_FST JOIN_4("*_[Hh][Rr][Ff].[Ff][Ss][Tt]",              \
                                      "*_[Hh][Tt][Mm].[Ff][Ss][Tt]",              \
                                      "*_[Hh][Pp][Nn].[Ff][Ss][Tt]",              \
                                      "*.[Dd][Aa][Tt]", EXTENSION_SEPARATOR)
#  define filter_extension_SHAPEFILE "*.[Ss][Hh][Pp]"
#  define filter_extension_CEOS "[Vv][Oo][Ll]-*"
#  define filter_extension_HDF5 JOIN_2("*.[Hh][Dd][Ff]", "*.[Hh]5",               \
                                      EXTENSION_SEPARATOR)
#  define filter_extension_L1B "*.[Ll]1[Bb]"
#  define filter_extension_GPX "*.[Gg][Pp][Xx]"
#  define filter_extension_KML "*.[Kk][Mm][Ll]"
#  define filter_extension_HTM JOIN_2("*.[Hh][Tt][Mm]", "*.[Hh][Tt][Mm][Ll]",     \
                                      EXTENSION_SEPARATOR)
#  define filter_extension_XLS "*.[Xx][Ll][Ss]"
#endif

// TODO(Sebastian - TCK# 848): Mover este codigo a la libreria
#define filter_extension_GEORREFERENCED_IMAGE JOIN_2(filter_extension_TIF,        \
                                                     filter_extension_FST,        \
                                                     EXTENSION_SEPARATOR)
#define filter_extension_COMMON_IMAGE JOIN_5(filter_extension_BMP,                \
                                             filter_extension_JPG,                \
                                             filter_extension_PNG,                \
                                             filter_extension_GIF,                \
                                             filter_extension_TIF,                \
                                             EXTENSION_SEPARATOR)
#define filter_extension_VIEWER_2D JOIN_9(filter_extension_CEOS,                  \
                                          filter_extension_FST,                   \
                                          filter_extension_COMMON_IMAGE,          \
                                          filter_extension_HDF5,                  \
                                          filter_extension_L1B,                   \
                                          filter_extension_VECTOR,                \
                                          filter_extension_OGC_WMS_TMS,           \
                                          filter_extension_J2K_JP2,               \
                                          filter_extension_MTL,                   \
                                          EXTENSION_SEPARATOR)
#define filter_extension_VIEWER_3D JOIN_2(filter_extension_TERRAIN,               \
                                          filter_extension_VECTOR,                \
                                          EXTENSION_SEPARATOR)
#define filter_extension_TERRAIN filter_extension_TIF
#define filter_extension_TEXTURE filter_extension_GEORREFERENCED_IMAGE
#define filter_extension_VECTOR JOIN_3(filter_extension_SHAPEFILE,                \
                                       filter_extension_GPX,                      \
                                       filter_extension_KML,                      \
                                       EXTENSION_SEPARATOR)

// TODO(Sebastian - TCK# 848): Mover este codigo a la libreria
/** Filtros--------------------------------------------------------------------- */
#define filter_ALL_FILES BUILD_FILTER_WITH_EXT(name_ALL_FILES, extension_ALL,     \
                                               filter_extension_ALL)

// TODO(Sebastian - TCK# 848): Mover este codigo a la libreria
// Filter------------------------------------------------------------------------
#define filter_extension_COMMON_IMAGE_FORMATS_LOAD JOIN_5(filter_extension_BMP,      \
                                                filter_extension_PNG,                \
                                                filter_extension_JPG,                \
                                                filter_extension_TIF,                \
                                                filter_extension_GIF,                \
                                                EXTENSION_SEPARATOR)

#define filter_COMMON_IMAGE_FORMATS_LOAD BUILD_FILTER(name_COMMON_IMAGE_FILES,       \
                                   filter_extension_COMMON_IMAGE_FORMATS_LOAD)

// TODO(Sebastian - TCK# 848): Mover este codigo a la libreria
// Context Filter----------------------------------------------------------------
#define filter_extension_CONTEXT_IMAGE_FILES_LOAD JOIN_6(filter_extension_BMP,      \
                                               filter_extension_PNG,                \
                                               filter_extension_JPG,                \
                                               filter_extension_TIF,                \
                                               filter_extension_GIF,                \
                                               filter_extension_SHAPEFILE,          \
                                               EXTENSION_SEPARATOR)

// TODO(Sebastian - TCK# 848): Mover este codigo a la libreria
#define filter_CONTEXT_IMAGE_FILES_LOAD BUILD_FILTER("Archivos Soportados",         \
                                   filter_extension_CONTEXT_IMAGE_FILES_LOAD)

// TODO(Sebastian - TCK# 848): Mover este codigo a la libreria
#define filter_extension_CONTEXT_MAP_FILES_LOAD JOIN_6(filter_extension_BMP,        \
                                               filter_extension_PNG,                \
                                               filter_extension_JPG,                \
                                               filter_extension_TIF,                \
                                               filter_extension_GIF,                \
                                               filter_extension_SHAPEFILE,          \
                                               EXTENSION_SEPARATOR)

// TODO(Sebastian - TCK# 848): Mover este codigo a la libreria
#define filter_CONTEXT_MAP_FILES_LOAD BUILD_FILTER("Archivos Soportados",           \
                                   filter_extension_CONTEXT_MAP_FILES_LOAD)

// TODO(Sebastian - TCK# 848): Mover este codigo a la libreria
#define filter_CONTEXT_TERRAIN_FILES_LOAD "Archivos Soportados|*.tif;*.tiff;*.shp"

// TODO(Sebastian - TCK# 848): Mover este codigo a la libreria
#define filter_CONTEXT_DOCUMENT_FILES_LOAD "Archivos de Documentos|*.html;*.htm;*.pdf"

// TODO(Sebastian - TCK# 848): Mover este codigo a la libreria
#define filter_CONTEXT_PHOTO_FILES_LOAD \
   "Archivos de Fotografias|*.bmp;*.png;*.jpg;*.jpeg;*.jpe;*.gif"

#define filter_CONTEXT_MULTIMEDIA_FILES_LOAD \
   "Archivos multimedia|*.bmp;*.png;*.jpg;*.jpeg;*.jpe;*.gif;*.wav;*.mp3;*.avi;*.mpg;*.mpeg;*.mp4;*.wmv"

// TODO(Sebastian - TCK# 848): Mover este codigo a la libreria
#define filter_CONTEXT_ACTIVITY_FILES_LOAD "Archivos de Actividades|*.html;*.htm;*.pdf"

// TODO(Sebastian - TCK# 848): Mover este codigo a la libreria
#define filter_ALL_FILES_LOAD "Todos los Archivos (*.*)|*.*"

// TODO(Sebastian - TCK# 848): Mover este codigo a la libreria
#define filter_FORMAT_SEPARATOR "|"

// Format-----------------------------------------------------------------------
#define filter_PAT_FILES "Archivos PAT (*.pat)|*.pat;*.PAT" //No agregar M:
// Label------------------------------------------------------------------------

//Meassure Tool
#define caption_DISTANCE "Medir distancia"
#define caption_AREA "Medir area"

// Menu-------------------------------------------------------------------------

// Message----------------------------------------------------------------------

// Context
// #define message_EMPTY_CONTEXT "Error al agregar contextos: lista de contextos vacia."
// #define message_CONTEXT_ADD_ERROR
// "Error al crear archivo descripcion.xml: no se pudo agregar contextos."
// #define message_UNNAMED_CONTEXT "Contexto sin nombre."

// Dir
#define message_OPEN_DIR_ERROR_s "Error al intentar abrir directorio: %s."
// #define message_LIST_DIR_ERROR "N/D"
#define message_NOT_A_VALID_DIR "%s no es un directorio valido."
#define message_DELETE_DIR_ERROR_s "Error al eliminar directorio: %s."
// #define message_CREATE_DIR_ERROR_2_s "Path invalido. No se puede crear directorio: %s."
// #define message_DIR_NOT_FOUND "No existe o no se pudo crear directorio con templates de datos."
// #define message_TEMP_DIR_CREATION_ERROR "No se pudo generar directorio temporal."
#define message_HOTLINK_DIR_CREATION_ERROR \
   "No se pudo generar directorio temporal para hipervinculos."
#define message_VECTOR_DIR_CREATION_ERROR "No se pudo generar directorio temporal para vectores."

// Element
#define message_COPY_ELEMENT_ERROR "Error al copiar el elemento."
#define message_INVALID_OR_DUPLICATED_CONTEXT_ELEMENT_ss \
   "El elemento %s no es de un tipo valido o ya existe con ese nombre de archivo en el contexto %s."
#define message_CLOSE_ELEMENTS_ERROR "Error al intentar cerrar elementos."

// File
// #define message_OPEN_FILE_ERROR "N/D"
// #define message_READ_FILE_ERROR "N/D"
#define message_DELETE_FILE_ERROR_s \
   "Error al intentar eliminar archivo: %s. Verifique que exista y que no este siendo usado."
#define message_MISSING_FILE "Archivo inexistente."
#define message_FILE_NOT_FOUND_s "No existe el archivo: %s."
// #define message_FILE_IS_OPENED "Archivo ya abierto"
#define message_TEMP_FILE_CREATION_ERROR "Falla creacion del archivo temporal."
#define message_HTML_FILES_NOT_FOUND "No se pudo obtener archivos para desplegar informacion."

// General ---------------------------------------------------------------------
#define window_title_MEASUREMENT "Configuracion Medicion"
#define window_title_VECTORIAL_VISUALIZATION "Visualizacion Vectorial"
#define window_title_VECTORIAL_EDITION "Edicion vectorial"
#define window_title_3D_CONFIGURATION "Configuracion 3D"
#define message_NO_DEFINED "No definido"
#define message_MISSING_TYPE_PROPERTY "Sin propiedad <tipo>."
// #define message_NULL_LIST "Lista de elementos invalida."
// #define message_INSERT_ERROR_s "No se pudo insertar %s."
#define message_MISSING_TEMPLATE "No se encontro el template."
// #define message_SHOWING_INFO "Mostrando Info"
// #define message_PATPROPERTIES_REMOVE_ERROR "Error al intentar eliminar Part."
// #define message_NO_LEGENDS "No hay leyendas para dibujar."
// #define message_ERROR_CREATING_MAIN_TOOL "No se creo la herramienta principal."
// #define message_MISSING_LAYER_d "No se encuentra la capa: %d."
// #define message_MISSING_FEATURE "No se encuentra nodo caracteristicas."
// #define message_MISSING_FEATURES "No se pudieron obtener valores del nodo caracteristicas."
// #define message_INFORMATION_FILES_MISSING
// "No se pudo obtener archivos para desplegar informacion."
// #define message_SHOW_PAT_PROPERTIES_ERROR "Error al intentar agregar PatPropertiesHtmlViewerPart."
// #define message_SHOW_PAT_INFO_ERROR "Error al intentar agregar PatInfoPart."
#define message_INVALID_IMAGE "La imagen seleccionada no es una imagen valida."

// Hotlinks
// #define message_HOTLINK_COPY_ERROR "Error al intentar copiar hipervinculos."
// #define message_MISSING_HOTLINK "No se encuentra el nodo hipervinculo."
#define message_UPDATE_HOTLINKS_ERROR "Error al modificar hipervinculos."

// PATs
#define message_PAT_XML_GENERATION_ERROR "No se pudo generar archivo de configuracion del PAT."
// #define message_INSTALLED_PAT_SEARCH_ERROR "Error buscando PATs instalados."
#define message_PAT_XML_SAVE_ERROR "No se pudo salvar archivo de configuracion del PAT."
#define message_PAT_NAME_MISSING "Debe ingresar un nombre para el PAT."
#define message_SELECT_PAT "Seleccione un PAT de la lista."

#define message_PAT_OPEN_ERROR "No se pudo abrir el PAT."
#define message_INSTALL_PAT_ERROR "Error al instalar el PAT."
#define message_OVERWRITEN_FILE_s "Se sobreescribio el archivo: %s"
#define message_OVERWRITEN_FILES "Se sobrescribieron archivos."

// #define message_PAT_INFO_NODE_MISSING "No se pudo obtener nodo informacion."
// #define message_PAT_NODE_ERROesR "Error al agregar contextos: nodo PAT vacio."
// #define message_PAT_ROOT_CREATE_ERROR
// "Error al crear archivo descripcion.xml: no se pudo crear raiz."
#define message_PAT_SUCCESFULL_GENERATION "Archivo generado con exito."
// #define message_PAT_DESCRIPTION_ERROR "Error al intentar obtener descripcion."
#define message_PAT_LOAD_ERROR "Error al cargar el PAT."
#define message_PAT_LOAD_ELEMENTS_ERROR "Los siguientes elementos no pudieron ser cargados:"
#define message_PAT_UNINSTALL_ERROR_s "Error al intentar desinstalar PAT: %s."
#define message_PAT_INSTALL_ERROR "Error al instalar PAT."
#define message_PAT_INSTALL_ERROR_DESCRIPTION_s "Error al instalar PAT. Motivo: %s."
#define message_PORPIETARY_PAT_EXISTS \
   "Error al intentar crear PAT: ya existe un PAT propietario con el mismo nombre."
#define message_PAT_MISSING_FILE_s "Archivo %s del xml no se encuentra en el PAT."
// #define message_NO_COMPRESSED_PAT_s "El PAT %s no es un archivo PAT valido."
#define message_MISSING_PAT_DEPENDENCY "Dependencia de archivos insatisfecha en el PAT."
#define message_PAT_FILE_ADD_ERROR "Error agregando archivos al PAT."
#define message_PAT_CREATE_ERROR "Error al intentar crear PAT."
#define message_PAT_INVALID_TYPE "Archivo PAT invalido."
// #define message_INVALID_PAT_TYPE2 "Tipo de PAT invalido."
#define message_PAT_DIR_CREATION_ERROR "Error al intentar crear directorio de instalacion de PATs."
#define message_PAT_CONSISTENCY_ERROR \
   "Inconsistencia entre archivos dentro del PAT y descripcion.xml."
// #define message_PAT_DIRECTORY_NAME_CONSISTENCY_ERROR
// "Nombre de directorio diferente a nodo informacion|nombre."
#define message_PAT_DESCRIPTION_EXTRACTION_ERROR "Error al descomprimir archivo descripcion.xml."
// #define message_PAT_DESCRIPTION_EXTRACTION_ERROR2 "Fallo extraccion del archivo descripcion.xml."
#define message_PAT_PREVIEW_EXTRACTION_ERROR "Fallo extraccion del archivo de vista previa."
// #define message_PAT_GENERATION_ERROR2 "PAT generado con errores."
#define message_PAT_GENERATION_ERROR_s "PAT generado con errores. Descripcion: %s."
#define message_PAT_INSTALLATION_SUCCESSFULL "Instalacion de PAT exitosa."
#define message_CANNOT_EDIT_INSTALLED_PAT "Solo se pueden editar PATs de usuario."
#define message_PAT_FORMAT_ERROR_s "El PAT: %s no es un archivo PAT comprimido."
#define message_MISSING_PREVIEW "No existe el archivo seleccionado para vista previa."
// #define message_PATINFO_REMOVE_ERROR "Error al intentar eliminar la informacion de PAT."
#define message_NO_FILES_IN_PAT "No se encontraron archivos en el PAT."
// #define message_CREATE_PAT_INSTALLATION_DIR_ERROR
// "No se pudo crear el directorio de instalacion de PATs."
// #define message_CREATE_PAT_INSTALLATION_DIR_ERROR2
// "No se pudo crear el directorio de instalacion para el PAT: "
// #define message_CREATE_PAT_INSTALLATION_DIR_ERROR3
//"No se pudo crear el directorio de instalacion para el PAT: %s."
#define message_COMPRESSED_PAT_GENERATION_ERROR "No se pudo generar el PAT comprimido. Motivo: %s."
#define message_PAT_ELEMENTS_NOT_FOUND "No se encontraron referencias a los elementos del PAT."
#define message_PAT_NOT_ENOUGH_SPACE "Espacio en disco insuficiente"
#define message_EMPTY_PAT_ERROR "No se puede generar un PAT sin elementos."
#define message_INCOMPATIBLE_PAT_VERSION "Version de PAT incompatible, descargar nueva version."

// Terrain
#define message_TERRAIN_MUST_BE_DISPLAYED "un terreno"
#define message_MUST_SELECT_TEXTURE  "Debe seleccionar una textura para el terreno."
#define message_WRONG_TYPE_VECTOR_ss \
   "Error al intentar agregar vector: %s. El vector no es de tipo: %s."

// Generico
// se complementa con message_RASTER_IMAGE_MUST_BE_DISPLAYED y message_TERRAIN_MUST_BE_DISPLAYED
#define message_ELEMENT_MUST_BE_DISPLAYED_s "Debe haber al menos una %s desplegada."
#define message_FIRST_APP_PARAMETER "Archivo PAT a instalar"
#define message_SAVE_CHANGES_ERROR "Error al intentar salvar cambios."

// URL
// #define message_ABSOLUTE_URL_CONVERTION_ERROR "Error al convertir URL a absoluto."
#define message_MISSING_URL "No se encuentra nodo URL."

// Vector

// XML -------------------------------------------------------------------------
#define message_XML_LOAD_ERROR "Error al cargar archivo xml."

// Question --------------------------------------------------------------------
#define question_USER_PAT_EXISTS \
   "Ya existe un PAT de usuario con el mismo nombre. Desea reemplazarlo?"
#define question_CONFIRM_PAT_DELETION "Esta seguro que desea eliminar el PAT seleccionado?"
#define question_SAVE_CHANGES "Hay cambios sin guardar. Desea guardarlos?"
#define question_OVERWRITE_DATA \
   "Ya existe un PAT instalado con el mismo nombre. Desea reemplazarlo?"
#define question_FILE_EXISTS "Archivo ya existente. Desea reemplazarlo?"
#define question_EDITED_ELEMENTS \
   "Hay elementos editandose, si continua estos se perderan. Desea continuar?"

// Aplicacion--------------------------------------------------------------------
#define APPLICATION_BUILD "1.90.0" "-" SVN_REVISION

// VectorOperations
#define message_CANT_START_PROCESS_WITHOUT_SR \
   "No se puede iniciar el proceso sin sistema de referencia"
#define message_SAME_LAYER_ERROR "No se puede realizar una operacion vectorial sobre la misma capa"
#define message_DIFFERENT_SR_WARNING "Las capas tienen sistema de referencia diferente y el " \
                                     "resultado puede no ser el esperado"
#define message_FLOAT_INPUT_ERROR "Solo pueden ingresarse valores numericos y punto"
#define message_INVALID_COLUMN_FIELD "El item seleccionado no corresponde a un campo"
#define message_DUPLICATED_ITEM "El item seleccionado ya existe en la capa destino"
#define message_DIFFERENT_LAYER_TYPES "No pueden fusionarse capas de diferentes tipos"

#define message_WRONG_BAND_COUNT "La cantidad de bandas del elemento seleccionado no concuerda " \
   "con la cantidad de registros en las firmas seleccionadas."

// Spectral Angle Mapper
#define message_MISSING_CLASSES_ERROR "No se puede iniciar el proceso sin clases"

// Output Page Configuration
#define message_MISSING_MARGINS_ERROR \
   "No se puede agregar titulo y subtitulo sin margenes superior e inferior"
#endif /* MESSAGES_H_ */
