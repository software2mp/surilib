/*! \file xmlnames.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

/** \cond SURI_XML_NAMES */
#ifndef XMLNAMES_H
#define XMLNAMES_H

// Defines
#ifdef __DEFINES_DE_NODOS_SIN_UTILIZAR__

#  define INPUT_NODE "entrada"
#  define LIST_NODE "lista"
#  define MASK_NODE "mascara"
#define DIMENSION_NODE "dimension"
#  define SAMPLES_NODE "columnas"
#  define ROWS_NODE "filas"
#define BAND_FILE_NODE "banda"
// #  define INDEX_NODE "indice" pasado a propiedades
#  define DATATYPE_NODE "dato"
#  define LAMBDA_NODE "lambda"
#  define FWMH_NODE "fwmh"
#  define NODATA_NODE "nodato"

#define MISC_NODE "miscelaneos"
#  define SENSOR_NODE "sensor"
#  define MISSION_NODE "mision"
#  define PRODUCT_NODE "producto"
#  define STATION_NODE "estacion"
#  define ADQ_NODE "adquisicion"
#  define PROCESSING_NODE "procesamiento"
#     define SOFTWARE_NODE "software"

#define COLORMAP_VALUE "paleta"

#define BRIGHTNESS_VALUE "brillo" // pasado a nodo y propiedad
#endif

#define XML_FILE_ENCODING "ISO-8859-1"
#define NODE_SEPARATION_TOKEN "|"

// ------------------------------------------------------------------------------
// ------------------------------------NODOS-------------------------------------
// ------------------------------------------------------------------------------
// GENERICOS (utilizados en todos los contextos)
#define PAT_NODE "pat"
#define PROJECT_NODE "proyecto"
#define CONTEXT_NODE "contexto"
#define TYPE_PROPERTY_VALUE_CONTEXT_2D "Context2D"
#define TYPE_PROPERTY_VALUE_CONTEXT_3D "Context3D"
#define TYPE_PROPERTY_VALUE_DYNAMIC_CONTEXT "DynamicContext"
#define CONTEXT_ELEMENT_NODE "elemento"
#define URL_NODE "url"
#define NAME_NODE "nombre"
#define TYPE_NODE "tipo"
#define COPYRIGHT_NODE "copyright"
#define DEPENDENCY_NODE "dependencias"
#define DESCRIPTION_NODE "descripcion"
#define DETAILS_NODE "detalles"
#define ROW_NODE "fila"
#define ICON_NODE "icono"
#define HEIGHT_NODE "altura"
#define WIDTH_NODE "ancho"
#define COLLECTION_NODE "parametros"
#define COLLECTION_PARAM_NODE "parametro"
#define PARAM_NAME_ATTRIBUTE "nombre"
#define PARAM_SERIALIZER_ATTRIBUTE "serializador"
#define PROCESS_ADAPT_LAYER_NODE "proceso"
// NODOS DE ARCHIVO
#define ELEMENTS_NODE "elementos"
#define ELEMENT_NODE "elemento"
#define FILE_NODE "archivo"
#  define SIZE_NODE "tamano"
#  define MUX_NODE "multiplexado"
#  define FORMAT_NODE "formato"
#     define PROPERTIES_NODE "propiedades"
#        define COMPRESSION_NODE "compresion"
#        define TILE_NODE "baldosa"
#        define DATA_TYPE_NODE "tipo_de_dato"
#        define NOT_VALID_DATA_VALUE "valor_no_valido"
#        define SUBSET_NODE "subset"
#           define SUBSET_LR_NODE "lr"
#           define SUBSET_UL_NODE "ul"
#           define SUBSET_X_NODE "x"
#           define SUBSET_Y_NODE "y"
#           define SUBSET_Z_NODE "z"
#        define BANDS_NODE "bandas"
#           define BAND_NODE "banda"
#              define BAND_NAME "nombre"
#              define BAND_POSITION "posicion"
#  define RAW_METADATA "rawmetadata"
#  define CAPABILITIES_NODE "capacidades"
// NODOS DE TABLA X-Y
#define CSV_FILE_NODE "csv"
#  define X_COLUMN_NODE "columnax"
#  define Y_COLUMN_NODE "columnay"
#  define START_FROM_ROW_NODE "comenzarenfila"
#  define HEADER_NODE "cabecera"
#  define TOKEN_NODE "separador"
#  define CUSTOM_TOKEN_NODE "personalizado"
// PATH PARA RECORRIDO DE XML
#define PATH_PROPERTIES_NODE_PIPES FILE_NODE NODE_SEPARATION_TOKEN   \
                                   FORMAT_NODE NODE_SEPARATION_TOKEN \
                                   PROPERTIES_NODE

#define PATH_NOT_VALID_DATA_VALUE_NODE_PIPES PATH_PROPERTIES_NODE_PIPES \
                                             NODE_SEPARATION_TOKEN      \
                                             NOT_VALID_DATA_VALUE

#define PATH_DATA_TYPE_NODE_PIPES PATH_PROPERTIES_NODE_PIPES \
                                  NODE_SEPARATION_TOKEN      \
                                  DATA_TYPE_NODE
// NODOS PARA SELECCION VECTORIAL
#define VECTOR_SELECTION_NODE "seleccion"
#  define FID_SELECTION "features"

// NODOS DE RECINTO
#define ENCLOSURE_NODE "recinto"
#  define ENCLOSURE_TYPE_AREA_NODE "tipo_area"
#  define ENCLOSURE_DESC_NODE "descripcion"
#  define ENCLOSURE_TYPE_PROPERTY "tipo"
#  define ENCLOSURE_QUERY_NODE "query"
#  define ENCLOSURE_TYPE_NODE "area"
#     define TRAIN_ENCLOSURE_TYPE "entrenamiento"
#     define TEST_ENCLOSURE_TYPE "test"

// NODOS CLASES DE CLASIFICACION.
#define CLASSIFICATION_CLASSES_NODE "clasificacion"
#  define CLASSIFICATION_CLASS_NODE "clase"
#  define CLASSIFICATION_NO_CLASS_NODE "noclase"
#  define CLASSIFICATION_BAND_NODE "banda"
#     define CLASSIFICATION_BAND_MIN_NODE "min_umbral"
#     define CLASSIFICATION_BAND_MAX_NODE "max_umbral"
#  define CLASSIFICATION_CLASS_PROPERTY "indice"
#     define CLASSIFICATION_CLASS_NAME_NODE "nombre"
#     define CLASSIFICATION_CLASS_COLOR_NODE "color"
#     define CLASSIFICATION_CLASS_ENCLOSURE_NODE "recinto"
#  define NON_PARAMETRIC_CLASS_NAME_VALUE_d "Clase valor %d"
#     define CLASSIFICATION_REFLECTANCE_NODE "reflectancia"
#  define SAM_NODE "sam"

// NODOS DE DIMENSION
#define DIMENSION_NODE "dimension"
#define EXTENT_NODE "extent"
#define RASTER_NODE "raster"
#define UL_NODE "ul"
#define LR_NODE "lr"
#define X_NODE "x"
#define Y_NODE "y"

// PARTICULARES
// NODOS DE CAPAS(Contextos: Imagen, Mapa, Terreno)
#define ACTIVELAYER_NODE "capa_activa"
#define LAYERS_NODE "capas"
#  define LAYER_NODE "capa"
#    define STYLE_NODE "estilo"
#        define LIBRARY_STYLE_NODE_PROPERTY "estilo-biblioteca"
#    define FILTER_NODE "filtro"
#    define ATTRIBUTE_FILTER_NODE "filtro-atributos"

#define LAYERS_EXPO_NODE   "capas-expo"

// NODOS DE GEOREFERENCIA(Contextos: Imagen, Mapa, Terreno)
#define WORLD_NODE "mundo"
#define GEORREFERENCE_NODE "georreferencia"
#  define SPATIAL_REFERENCE_NODE "sistema"
#  define MATRIX_NODE "matriz"
#  define RASTER_SPATIAL_MODEL_NODE MATRIX_NODE

// NODOS PARA HISTORIAL DE PROCESAMIENTO
#define PROCESS_HISTORY_NODE "historial-procesamiento"
#define PROCESS_NODE "procesamiento"

//  NODOS DE RENDERIZACION E INFERIORES(Contextos: Imagen, Mapa, Terreno)
#define RENDERIZATION_NODE "renderizacion"

/** Definicion de nodos para renderizacion vectorial de estilos clasificados **/
#define CATEGORIZED_LAYER_NODE "estilo-clasificado"
#  define   CATEGORY_CLASS_NODE "clase"
#     define   CATEGORY_CONDITION_NODE "condicion"
#     define   CATEGORY_STYLE_NODE "estilo"
#     define   CATEGORY_LABEL_NODE "etiqueta"

/**! Nodo para combinacion de bandas de mas de una fuente de datos*/
#define BAND_STACKING_NODE "apilamiento"

#define BAND_STACKING_ID "id"
#  define BAND_COMBINATION_NODE "combinacion"
#  define BAND_MULTIPLEXOR_NODE "multiplexor"
#  define REPROJECTION_NODE "reproyeccion"
#     define REPROJECTION_TYPE_NODE "tipo"
#     define REPROJECTION_TYPE_ALGORITHM_NODE "algoritmo"
#     define REPROJECTION_TYPE_ORDER_NODE "orden"
#     define REPROJECTION_TYPE_COEFFICIENT_NODE "coeficientes"
#     define REPROJECTION_TYPE_GCPLIST_NODE "GCPList"
#     define REPROJECTION_TYPE_DELTA_NODE "delta"
#        define   REPROJECTION_TYPE_GCPLIST_URL_NODE "url"
#        define   REPROJECTION_TYPE_GCPLIST_GCP_NODE "GCP"
#  define CANVAS_CACHE_NODE "cache"
#  define BANDS_NODE "bandas"
#  define COLOR_TABLE "tablacolores"
#  define BRIGHTNESS_NODE "lut"
#  define LUT_NODE "LUT"
#     define LUT_ENABLED_PROPERTY ACTIVE_PROPERTY
#     define LUT_NAME_PROPERTY "nombre"
#     define TABLE_NODE "tabla"
#        define INPUT_BAND_PROPERTY "inputband"
#        define OUTPUT_BAND_PROPERTY "outputband"
#        define KEYS "claves"
#        define VALUES "valores"
#  define ALGEBRA_NODE "algebra"
#     define EQUATION_NODE "ecuacion"
#  define LUT_BRIGHTNESS_NODE "brillo"
#  define LUT_CONTRAST_NODE "contraste"
#  define INTERPOLATION_NODE "interpolacion"
#  define ALGEBRAIC_NODE "algebra"
#     define EQUATION_NODE "ecuacion"
#  define CLASSIFICATION_NODE "clasificacion"
#     define CLASSES_NODE "clases"
#        define CLASS_NODE "clase"
#           define NAME_NODE "nombre"
#           define COLOR_NODE "color"
#           define STATISTICS_NODE "estadisticas"
#              define MEAN_NODE "media"
#              define VARIANCE_NODE "varianza"
#              define COVARIANCE_MATRIX_NODE "covarianza"
#              define CORRELATION_MATRIX_NODE "correlacion"
#              define MIN_NODE "minimo"
#              define MAX_NODE "maximo"
#              define SAMPLES_NODE "muestras"
#              define HISTOGRAM_NODE "histograma"
#              define EIG_NODE "eig"
#              define ACCUM_COVAR_NODE "acumulador_covar"

#define TASSELEDCAP_NODE         "tasseledcap"
#define TASSELEDCAP_SENSOR_NODE  "sensor"
#define TASSELEDCAP_OUTPUT_NODE  "salida"

#define PCANALYSIS_NODE             "acp"
#define PCANALYSIS_COMPONENTS_NODE  "componentes"
#define PCANALYSIS_COMPONENT_NODE   "componente"
#define PCANALYSIS_ORDER_ATTR_NODE  "orden"
#define PCANALYSIS_AUTOVECTOR_NODE  "autovector"


// #     define ZOOM_IN_INTERPOLATION_NODE "mas"
// #     define ZOOM_OUT_INTERPOLATION_NODE "menos"
// #     define NEAREST_NEIGHBOUR_INTERPOLATION_PROPERTY "NN"
// #     define CUBIC_COMBOLUTION_INTERPOLATION_PROPERTY "CC"
// NODOS DE RENDERIZACION SOLO 3D
#  define EXAGERATION_NODE "exageracion"
#  define FACTOR_NODE "factor"
#  define BACKGROUND_COLOR_NODE "color_de_fondo"
#  define CAMERA_STEP_NODE "paso_de_camara"
#  define TEXTURE_NODE "textura"
#  define DATA_CAST_NODE "casteo"

// NODOS DE MAPA - SEYMOUR PRI
#define LEGEND_NODE "leyenda"
#define GRID_NODE "grilla"
#define NORTH_NODE "norte"
#define SCALE_NODE "escala"

/** NODOS FILTRO CONVOLUCION */
#define CONVOLUTION_NODE "filtroconvolucion"
#define ALGORITHM_VALUE_KERNELFILTER "KernelFilter"
#define ALGORITHM_VALUE_MAYORITYFILTER "MayorityFilter"
#define ALGORITHM_VALUE_MINORITYFILTER "MinorityFilter"
#define FILTER_WITDH "ColumnasFiltro"
#define FILTER_HEIGHT "FilasFiltro"

// NODOS CARACTERISTICAS(Contextos: Fotografia, Imagen(HL), Mapa(HL), Terreno(HL))
#define FEATURES_NODE "caracteristicas"
#  define TITLE_NODE "titulo"
#  define TEXT_NODE "texto"
#  define HTML_NODE "html"
#  define PREVIEW_IMAGE_NODE "vista_previa"

/** NODOS WAVELETS */
#define DAUBECHIES_NODE "transformadaDaubechies"
#define WAVELET_MODULUS_NODE "modulowavelet"
#define DECIMATION_NODE "decimado"

/** NODOS ENTROPIA */
#define ENTROPY_NODE "entropia"

#define THRESHOLD_NODE "umbral"
#define THRESHOLD_HIGH_PROPERTY "valormaximo"
#define THRESHOLD_LOW_PROPERTY "valorminimo"
#define THRESHOLD_PERCENT_PROPERTY "porcentaje"

// NODOS DE HOTLINK Y ANOTACIONES
#define LABEL_NODE "etiqueta"
#define HOTLINK_NODE "hotlink"

// NODO GRUPO
#define GROUPS_NODE "grupos"
#define GROUP_NODE  "grupo"
#define CHILDREN_NODE "hijos"

// NODO INFORMACION
#define INFORMATION_NODE "informacion"
#  define AUTHOR_NODE "autor"
#  define DATE_NODE "fecha"
#     define DAY_NODE "dia"
#     define MONTH_NODE "mes"
#     define YEAR_NODE "ano"
#  define PREVIEW_NODE "preview"

// VALORES DEFAULT - NODO INFORMACION
#define DEFAULT_PROJECT_AUTHOR ""
#define DEFAULT_PROJECT_NAME ""
#define DEFAULT_PROJECT_COPYRIGHT ""

/** Propiedades nodo raiz de Proyecto */
#define APPLICATION_PROPERTY "aplicacion"
#define VERSION_PROPERTY "version"
#define OS_PROPERTY "os"

/** Valores default de nodo raiz de Proyecto */
#define VERSION_PROPERTY_VALUE "1.0"
#define TYPE_PROPERTY_VALUE_ABSOLUTE "absoluto"
#define TYPE_PROPERTY_VALUE_RELATIVE "relativo"

// NODO CLASIFICACION
/** debe quedar en ingles ya que es el nombre de la clase */
#define MAX_LIKELIHOOD_NODE "MaxLikelihoodAlgorithm"
#define CLASSIFICATION_ALGORITHM_NODE "classificationalgorithm"
#define LU_MATRIX_NODE "NodoLU"
#define TRHEASHOLD_NODE "umbral"
#define NDV_PIXEL_VALUE_NODE "valor_pixel_valor_no_valido"
#define NO_CLASS_PIXEL_VALUE_NODE "valor_pixel_no_clasificado"
#define FUNCTION_CONSTANT_NODE "constante"
#define KMEANS_NODE "KMeansAlgorithm"
#define TRAINING_AREA_NODE "area_entrenamiento"
#define CLUSTER_CONTAINER_NODE "contendor_cluster"
#define CLUSTER_NODE "cluster"
#define CLUSTER_INDEX_NODE "indice"
#define CLUSTER_FUSSION_INDEX "indice_fusion"
#define NOCLUSTER_NODE "nocluster"
#define CLASSID_NODE "idclase"
#define ALGORITHM_NAME_NODE "nombre_algoritmo"
#define ALGORITHM_VALUE_MAX_LIKELIHOOD "MaxLikelihoodAlgorithm"
#define ALGORITHM_VALUE_MIN_DISTANCE "MinimumDistanceAlgorithm"
#define ALGORITHM_VALUE_MAHALANOBIS "MahalanobisAlgorithm"
#define ALGORITHM_VALUE_PARALLELEPIPED "ParallelepipedAlgorithm"
#define ALGORITHM_VALUE_CLASSFUSION "ClassFussionAlgorithm"
#define ALGORITHM_VALUE_THRESHOLD "ThresholdClassificationAlgorithm"
#define ALGORITHM_VALUE_SAM "SpectralAngleMapperAlgorithm"
#define ALGORITHM_TYPE_NODE "tipo"

// NODO MARGEN
#define MARGIN_NODE "margen"
#  define MARGIN_ORIENTATION_NODE "orientacion"
#  define MARGIN_DIMENSION_NODE "dimension"
#  define MARGIN_TEXT_NODE "texto"


// ---------------------------------------------------------------------------//
// -----------------------------NODOS PROYECTO 2.0 ---------------------------//
// ---------------------------------------------------------------------------//

// Por ahora se coloca un 2 al final de cada definicion indicando
// que corresponden a la version 2.0
#define PROJECT_NODE2 "Project"
#  define PROJECT_GENERAL_NODE2 "General"
#     define PROJECT_NAME_NODE2 "Name"
#     define PROJECT_AUTHOR_NODE2 "Author"
#     define PROJECT_COPYRIGHT_NODE2 "CopyRight"
#     define PROJECT_DATE_NODE2 "Date"
#        define PROJECT_DATE_YEAR_NODE2 "Year"
#        define PROJECT_DATE_MONTH_NODE2 "Month"
#        define PROJECT_DATE_DAY_NODE2 "Day"
#     define LIBRARYLIST_NODE2 "LibraryList"
#        define LIBRARY_NODE2 "Library"
#           define LIBRARY_ITEM_NODE2 "LibraryItem"
#              define LIBRARY_ITEM_NAME_NODE2 "Name"
#              define LIBRARY_ITEM_DESCRIPTION_NODE2 "Description"
#  define DATASOURCEGROUP_NODE "DatasourceGroup"
#     define DATASOURCEID_NODE "DatasourceId"
#  define DATASOURCELIST_NODE2 "DatasourceList"
#     define DATASOURCE_NODE2 "Datasource"
#        define DATASOURCE_ID_PROPERTY "id"
#  define VIEWCONTEXT_COLLECTION_NODE2 "ViewContextCollection"
#  define VIEWCONTEXT_NODE2 "ViewContext"
#     define VIEWCONTEXT_GENERAL_NODE2 "General"
#        define VIEWCONTEXT_NAME_NODE2 "Name"
#        define VIEWCONTEXT_TITLE_NODE2 "Title"
#     define LAYER_LIST_NODE2 "LayerList"
#        define LAYER_NODE2 "Layer"
#           define LAYER_NAME_NODE2 "Name"
#           define LAYER_TITLE_NODE2 "Title"
#           define LAYER_SERVER_NODE2 "Server"
#           define LAYER_SERVER_SERVICE_PROPERTY "service"
#           define LAYER_SERVER_SURI_SERVICE_VALUE "SURI:LDS"
#              define LAYER_SERVER_ONLINE_RESOURCE_NODE2 "OnlineResource"
#                 define ONLINE_RESOURCE_TYPE_PROPERTY "xlink:type"
#                 define ONLINE_RESOURCE_TYPE_PROPERTY_VALUE "simple"
#                 define ONLINE_RESOURCE_HREF_PROPERTY "xlink:href"
#           define LAYER_EXTENSION_NODE2 "Extension"

#define ENHANCEMENT_NODE "realce"
#define ENHANCEMENT_NODE_PROP_BAND_COUNT "bandas"
#define ENHANCEMENT_NODE_PROP_NAME "nombre"
#define ENHANCEMENT_NODE_PROP_ACTIVE "activo"
#define ENHANCEMENT_NODE_BAND "banda"
#define ENHANCEMENT_NODE_BAND_PROP_BINS "bins"
#define ENHANCEMENT_NODE_BAND_PROP_MIN "min"
#define ENHANCEMENT_NODE_BAND_PROP_MAX "max"
#define ENHANCEMENT_NODE_BAND_VALUES "valores"

// ------------------------------------------------------------------------------
// ---------------------------------PROPIEDADES----------------------------------
// ------------------------------------------------------------------------------
#define APP_VERSION_PROPERTY "version_app"
#define PAT_VERSION_PROPERTY "version_pat"
#define OS_VERSION_PROPERTY OS_PROPERTY
#define ENCODING_PROPERTY "encoding"
#define TYPE_PROPERTY TYPE_NODE
#define NAME_PROPERTY NAME_NODE
#define INTERPOLATION_PROPERTY_ZOOM_IN "mas"
#define INTERPOLATION_PROPERTY_ZOOM_OUT "menos"
#define LAYER_INDEX_PROPERTY "indice"
#define QUANTITY_PROPERTY "cantidad"
#define OUTPUT_PROPERTY "salida"
#define BAND_INDEX_PROPERTY "indice"
#define BAND_VALIDITY_PROPERTY "validez"
#define ID_PROPERTY "id"
#define ROWS_PROPERTY "filas"
#define COLUMNS_PROPERTY "columnas"
#define ACTIVE_PROPERTY "activo"
#define ACTIVE_PROPERTY_VALUE "si"
#define INACTIVE_PROPERTY_VALUE "no"
#define BIN_PROPERTY "bin"
#define MINIMUM_PROPERTY "min"
#define MAXIMUM_PROPERTY "max"
#define CLASS_ID_PROPERTY ID_PROPERTY


// ------------------------------------------------------------------------------
// ----------------------------VALORES DE PROPIEDADES----------------------------
// ------------------------------------------------------------------------------
#define PAT_VERSION_PROPERTY_VALUE "1.0"
#define ENCODING_PROPERTY_VALUE XML_FILE_ENCODING
#define TYPE_PROPERTY_VALUE_PAT_RELATIVE TYPE_PROPERTY_VALUE_RELATIVE
#define TYPE_PROPERTY_VALUE_PAT_ABSOLUTE TYPE_PROPERTY_VALUE_ABSOLUTE
#define TYPE_PROPERTY_VALUE_GENERIC "generico"
#define TYPE_PROPERTY_VALUE_RASTER "raster"
#define TYPE_PROPERTY_VALUE_VECTOR "vector"
#define TYPE_PROPERTY_VALUE_TERRAIN "terreno"
#define TYPE_PROPERTY_VALUE_MAP "Mapa"
#define TYPE_PROPERTY_VALUE_HTML HTML_NODE
#define NAME_PROPERTY_VALUE_RASTER "Imagenes"
#define NAME_PROPERTY_VALUE_MAP "Mapas"
#define NAME_PROPERTY_VALUE_TERRAIN "Terrenos 3D"
#define NAME_PROPERTY_VALUE_DOCUMENTS "Documentos"
#define NAME_PROPERTY_VALUE_PHOTOGRAPHS "Fotografias"
#define NAME_PROPERTY_VALUE_MULTIMEDIA "Multimedia"
#define NAME_PROPERTY_VALUE_ACTIVITIES "Actividades"
#define NAME_PROPERTY_VALUE_LUT_BRIGHTNESS "brillo/contraste"
#define NAME_PROPERTY_VALUE_LUT_GRAYSCALE "escala de grises"
#define NAME_PROPERTY_VALUE_LUT_COLORMAP "mapa de colores"
#define NAME_PROPERTY_VALUE_LUT_GENERIC "generica"
#define NAME_PROPERTY_VALUE_UNKNOWN "desconocido"
#define INTERPOLATION_PROPERTY_VALUE_NEAREST_NEIGHBOUR "NN"
#define INTERPOLATION_PROPERTY_VALUE_CUBIC_COMBOLUTION "CC"

#define COLOR_TABLE_LABELS    "etiquetas"
#define COLOR_TABLE_BOUNDARY  "limite"
#define COLOR_TABLE_LABEL     "etiqueta"

/**
 * Native Datatypes
 */
#define INTEGER_NODE "int"
#define STRING_NODE "texto"
#define VECTOR_NODE "vector"

#endif   // XMLNAMES_H
/** \endcond */
