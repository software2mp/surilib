/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

#include "suri/VectorStyleManager.h"

// Includes suri
#include "suri/messages.h"
#include "suri/Wkt.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/DataTypes.h"
#include "logmacros.h"

// Includes wx
#include "wx/wx.h"

// Defines
/** Define para nombre del estilo default. */
#define _DEFAULT_VECTORSTYLETABLE_NAME_ "default"
/** Define para nombre de la mascara default. */
#define _VECTOR_MASK_STYLE_NAME_ "mask"
/** Define para nombre del estilo de mascara exerna por default. */
#define _VECTOR_MASK_EXTERNAL_STYLE_NAME_ "mask_external"
/** LAYER */
#define LAYER_TABLE_NAME "layer"
/** Define para Estilo de desconocido 1 */
#define LAYER_UNDEFINED_1 "VECTORSTYLE[0,\"Desconocido\"]"
/** Define para Estilo de punto 1 */
#define LAYER_POINT_1 \
   "VECTORSTYLE[1,\"Triangulo verde\",SYMBOL[\"Suri-symbol\",4,COLOR[255,0,0,255],SIZE[1]]]"
	//"VECTORSTYLE[1,\"Circulo rojo\",SYMBOL[\"Suri-symbol\",1,COLOR[255,0,0,255],SIZE[1]]]"
/** Define para Estilo de punto 2 */
#define LAYER_POINT_2 \
   "VECTORSTYLE[1,\"Cuadrado cyan\",SYMBOL[\"Suri-symbol\",2,COLOR[128,255,255,255],SIZE[1]]]"
/** Define para Estilo de punto 3 */
#define LAYER_POINT_3 \
   "VECTORSTYLE[1,\"Triangulo verde\",SYMBOL[\"Suri-symbol\",3,COLOR[0,255,0,255],SIZE[1]]]"
/** Define para Estilo de punto 4 */
#define LAYER_POINT_4 \
   "VECTORSTYLE[1,\"Cruz Violeta\",SYMBOL[\"Suri-symbol\",4,COLOR[151,45,232,255],SIZE[1]]]"

/** Define para Estilo de linea 1 */
#define LAYER_LINE_1 \
   "VECTORSTYLE[2,\"Linea verde\",PEN[\"Suri-pen\",1,COLOR[0,255,0,255],WIDTH[1]]]"
/** Define para Estilo de linea 2 */
#define LAYER_LINE_2 \
   "VECTORSTYLE[2,\"Linea naranja\",PEN[\"Suri-pen\",1,COLOR[255,128,0,255],WIDTH[1]]]"
/** Define para Estilo de linea 3 */
#define LAYER_LINE_3 \
   "VECTORSTYLE[2,\"Linea celeste\",PEN[\"Suri-pen\",1,COLOR[40,185,255,255],WIDTH[1]]]"
/** Define para Estilo de linea 4 */
#define LAYER_LINE_4 \
   "VECTORSTYLE[2,\"Linea blanca\",PEN[\"Suri-pen\",1,COLOR[255,255,255,255],WIDTH[1]]]"

/** Define para Estilo de poligono 1 */
#define LAYER_POLYGON_1 \
   "VECTORSTYLE[3,\"Poligono rosa\",PEN[\"Suri-pen\",1,COLOR[255,128,128,255],WIDTH[1]]]"
/** Define para Estilo de poligono 2 */
#define LAYER_POLYGON_2 \
   "VECTORSTYLE[3,\"Poligono amarillo\",PEN[\"Suri-pen\",1,COLOR[255,255,0,255],WIDTH[1]]]"
/** Define para Estilo de poligono 3 */
#define LAYER_POLYGON_3 \
   "VECTORSTYLE[3,\"Poligono azul\",PEN[\"Suri-pen\",1,COLOR[0,0,255,255],WIDTH[1]]]"
/** Define para Estilo de poligono 4 */
#define LAYER_POLYGON_4 \
   "VECTORSTYLE[3,\"Poligono rojo\",PEN[\"Suri-pen\",1,COLOR[255,0,0,255],WIDTH[1]]]"
/** Estilo para que la renderizacion de un vector genere una mascara */
#define MASK_POLYGON_1 \
   "VECTORSTYLE[3,\"Mascara vectorial\",BRUSH[\"Suri-brush\",1,COLOR[255,255,255,255]],PEN[\"Suri-pen\",1,COLOR[255,255,255,255],WIDTH[1]]]"

#define MASK_POLYGON_EXTERNAL \
   "VECTORSTYLE[3,\"Mascara vectorial\",INVERT[\"Invert brush colors\",1],BRUSH[\"Suri-brush\",1,COLOR[255,255,255,255]],PEN[\"Suri-pen\",1,COLOR[255,255,255,255],WIDTH[1]]]"

/** namespace suri */
namespace suri {
// --------------------------- Clase VectorStyleManager -------------------------------------------

/**
 * Constructor
 * @return instancia de la clase VectorStyleManager
 */
VectorStyleManager::VectorStyleManager() :
      defaultCategory_(LAYER_TABLE_NAME) {
   // Carga los estilos del sistema
   LoadDefaultStyles();
}

/** Destructor */
VectorStyleManager::~VectorStyleManager() {
}

/**
 * Constructor de copia
 * @param[in] pVStyleMgr VectorStyleManager del que me estoy copiando
 */
VectorStyleManager::VectorStyleManager(const VectorStyleManager &pVStyleMgr) {
}

/**
 * Con este metodo se emula un singleton, para mantener una sola
 * instancia de la clase.
 * @return referencia a la unica instancia de la clase.
 */
VectorStyleManager& VectorStyleManager::Instance() {
   static VectorStyleManager *pvecstylemanager = new VectorStyleManager();
   return *pvecstylemanager;
}
// ------------------- METODOS DE CONSULTA ------------------------------

/**
 * Devuelve la cantidad total de tablas de estilo sin importar el tipo de vector
 * para el que fueron definidas.
 * @return	cantidad de elementos del mapa
 */
size_t VectorStyleManager::GetTablesCount() {
   return styleTablesMap_.size();
}

/**
 * Devuelve los nombres de las categorias de estilos de vector.
 * @return vector de strings con los nombres de las categorias de estilos de vector.
 */
std::vector<std::string> VectorStyleManager::GetTablesNames() {
   std::vector<std::string> tablesnames;
   std::map<std::string, VectorStyleTable *>::iterator it = styleTablesMap_.begin();
   for (; it != styleTablesMap_.end(); it++)
      tablesnames.push_back(it->second->GetCategoryName());

   return tablesnames;
}

/**
 * Devuelve un puntero a la tabla de estilos cuya posicion se indica con el
 * parametro StyleTableIndex.
 * @param[in]	StyleTableIndex indice de la tabla de estilos requerida.
 * @return	NULL en caso de que StyleTableIndex este fuera de rango.
 * @return	puntero a la tabla de estilos ubicada en la posicion StyleTableIndex.
 * \todo(24/07/2009 - Alejandro): Verificar si es conveniente que se devuelva un
 * puntero a la tabla de estilos o una copia de la misma. Otra opcion seria
 * devolver un puntero a la tabla de estilos y que los estilos que esta posee
 * sean punteros constantes para solo permitir que se modifique la tabla pero no
 * los estilos que ya posee.
 * \attention el VectorStyleTable* retornado es responsabilidad de esta clase
 * no se debe eliminar externamente.
 */
VectorStyleTable *VectorStyleManager::GetTable(const size_t StyleTableIndex) {
   if (StyleTableIndex >= GetTablesCount()) {
      return NULL;
   }

   // Obtengo la tabla de estilos de indice StyleTableIndex.
   size_t index = StyleTableIndex;
   std::map<std::string, VectorStyleTable *>::iterator it = styleTablesMap_.begin();
   for (; it != styleTablesMap_.end() && index > 0; index--)
      it++;

   if (it != styleTablesMap_.end()) {
      return it->second;
   }
   return NULL;
}

/**
 * Devuelve un puntero a la tabla de estilos cuya Categoria y Tipo de Vector
 * coinciden con los indicados por parametros.
 * @param[in]	CategoryName nombre de la categoria (Layer, Draw, Meassure, Grid)
 * @param[in]  Type tipo de vector. \see clase suri::Vector
 * @return	NULL en caso de que no exista una tabla de estilos de la Categoria y
 * Tipo de Vector requerido.
 * @return	puntero a la tabla de estilos de la Categoria y
 * Tipo de Vector requerido.
 * \todo(24/07/2009 - Alejandro): Verificar si es conveniente que se devuelva un
 * puntero a la tabla de estilos o una copia de la misma. Otra opcion seria
 * devolver un puntero a la tabla de estilos y que los estilos que esta posee
 * sean punteros constantes para solo permitir que se modifique la tabla pero no
 * los estilos que ya posee.
 * \attention el VectorStyleTable* retornado es responsabilidad de esta clase
 * no se debe eliminar externamente.
 */
VectorStyleTable *VectorStyleManager::GetTable(const std::string &CategoryName,
                                               Vector::VectorType Type) {
   std::map<std::string, VectorStyleTable *>::iterator it = styleTablesMap_.begin();
   // busco la tabla del tipo Type
   while (it != styleTablesMap_.end()
         && (it->first != CategoryName || it->second->GetVectorType() != Type))
      it++;

   if (it != styleTablesMap_.end()) {
      return it->second;
   }

   return NULL;
}

/**
 * Devuelve un puntero a la tabla de estilos cuya Categoria es la seteada por
 * defecto en el VectorStyleManager y su Tipo de Vector coincide con el indicado
 * por parametro.
 * @param[in]  Type tipo de vector. \see clase suri::Vector
 * @return	NULL en caso de que no exista una tabla de estilos de la Categoria y
 * Tipo de Vector requerido.
 * @return	puntero a la tabla de estilos de la Categoria y
 * Tipo de Vector requerido.
 * \todo(24/07/2009 - Alejandro): Verificar si es conveniente que se devuelva un
 * puntero a la tabla de estilos o una copia de la misma. Otra opcion seria
 * devolver un puntero a la tabla de estilos y que los estilos que esta posee
 * sean punteros constantes para solo permitir que se modifique la tabla pero no
 * los estilos que ya posee.
 * \attention el VectorStyleTable* retornado es responsabilidad de esta clase
 * no se debe eliminar externamente.
 */
VectorStyleTable *VectorStyleManager::GetTable(Vector::VectorType Type) {
   return GetTable(defaultCategory_, Type);
}

/**
 * Agrega una tabla de estilos de la categoria indicada, al final de la
 * coleccion de tablas de estilo que posee el VectorStyleManager.
 * \pre	pVStyleTable no debe ser NULL.
 * \post se agrega una tabla a la coleccion de tablas de estilo.
 * @param[in] CategoryName nombre de la categoria del Vector (layer, draw, meassure, grid)
 * @param[in] pVStyleTable puntero a una tabla de estilo.
 * @return	false en caso de que la tabla de estilos a agregar sea NULL.
 * @return	true en caso de haber podido agregar la tabla de estilos.
 */
bool VectorStyleManager::AddTable(const std::string &CategoryName,
                                  VectorStyleTable *pVStyleTable) {
   if (CategoryName.empty() || !pVStyleTable) {
      REPORT_ERROR(
            "D:Error en el Administrador de Estilos Vectoriales: "
            "Se intento agregar una tabla de estilo invalida.");
      return false;
   }

   styleTablesMap_.insert(std::make_pair(CategoryName, pVStyleTable));
   return true;
}

/**
 * Agrega una tabla de estilos de la categoria default, al final de la coleccion
 * de tablas de estilo que posee el VectorStyleManager.
 * \pre	pVStyleTable no debe ser NULL.
 * \post se agrega una tabla a la coleccion de tablas de estilo.
 * @param[in] pVStyleTable puntero a una tabla de estilo.
 * @return	false en caso de que la tabla de estilos a agregar sea NULL.
 * @return	true en caso de haber podido agregar la tabla de estilos.
 */
bool VectorStyleManager::AddTable(VectorStyleTable *pVStyleTable) {
   return AddTable(defaultCategory_, pVStyleTable);
}

/**
 * Elimina la tabla de estilos cuyo indice se indica por parametro, de la
 * coleccion de tablas de estilo que posee el VectorStyleManager.
 * \pre	StyleTableIndex debe ser menor que la cantidad de tablas.
 * \post	VectorStyleManager con una tabla de estilos menos.
 * @param[in]	StyleTableIndex indice de la tabla a eliminar.
 * @return	false en caso de que StyleTableIndex no sea menor que la cantidad
 * de tablas del VectorStyleManager.
 * @return	true en caso de haber podido eliminar la tabla.
 */
bool VectorStyleManager::DeleteTable(const size_t StyleTableIndex) {
   if (GetTablesCount() == 0) {
      return true;
   }

   if (StyleTableIndex >= GetTablesCount()) {
      return false;
   }

   // Obtengo la tabla de estilos de indice StyleTableIndex.
   size_t index = StyleTableIndex;
   std::map<std::string, VectorStyleTable *>::iterator it = styleTablesMap_.begin();
   for (; it != styleTablesMap_.end() && index > 0; index--)
      it++;

   if (it != styleTablesMap_.end()) {
      styleTablesMap_.erase(it);
   }
   return true;
}

/**
 * Elimina la tabla de estilos de la categoria y tipo indicada por parametro, de
 * la coleccion de tablas de estilo que posee el VectorStyleManager.
 * @param[in]	CategoryName	nombre de la categoria, para ubicar la tabla.
 * @param[in]	Type	tipo de vector dentro de la tabla.
 * @return	false en caso de que no exista la tabla indicada.
 * @return	true en caso de que no existan tablas para eliminar o en caso de
 * haber podido eliminar la tabla indicada.
 */
bool VectorStyleManager::DeleteTable(const std::string &CategoryName,
                                     Vector::VectorType Type) {
   if (GetTablesCount() == 0) {
      return true;
   }

   int index = 0;
   std::map<std::string, VectorStyleTable *>::iterator it = styleTablesMap_.begin();
   // busco la tabla del tipo Type
   while (it != styleTablesMap_.end() && it->first != CategoryName
         && it->second->GetVectorType() != Type) {
      index++;
      it++;
   }

   if (it != styleTablesMap_.end()) {
      return DeleteTable(index);
   }

   return false;
}

/**
 * Elimina la tabla de estilos de la categoria default y tipo indicado por
 * parametro, de la coleccion de tablas de estilo que posee el VectorStyleManager.
 * @param[in]	Type	tipo de vector dentro de la tabla.
 * @return	false en caso de que no exista la tabla indicada.
 * @return	true en caso de que no existan tablas para eliminar o en caso de
 * haber podido eliminar la tabla indicada.
 */
bool VectorStyleManager::DeleteTable(Vector::VectorType Type) {
   return DeleteTable(defaultCategory_, Type);
}

/**
 * Devuelve el estilo default a utilizar para una categoria y tipo de
 * vector indicado por parametro.
 * @param[in]	CategoryName	nombre de la categoria, para ubicar la tabla.
 * @param[in]	Type	tipo de vector dentro de la tabla.
 * @return	NULL en caso de que no exista la tabla indicada o que la tabla no
 * contenga estilos.
 * @return puntero al estilo default a utilizar en caso de exito.
 * \attention el VectorStyle* retornado no debe ser eliminado.
 * \see VectorStyleTable::GetDefaultStyle()
 */
VectorStyle *VectorStyleManager::GetDefaultStyle(const std::string &CategoryName,
                                                 Vector::VectorType Type) {
   VectorStyleTable *pvstyletable = GetTable(CategoryName, Type);
   if (!pvstyletable || pvstyletable->GetStyleCount() == 0) {
      return NULL;
   }

   return pvstyletable->GetDefaultStyle();
}

/**
 * Devuelve el estilo default a utilizar para la categoria default y tipo de
 * vector indicado por parametro.
 * @param[in]	Type	tipo de vector dentro de la tabla.
 * @return	NULL en caso de que no exista la tabla indicada o que la tabla no
 * contenga estilos.
 * @return puntero al estilo default a utilizar en caso de exito.
 * \attention el VectorStyle* retornado no debe ser eliminado.
 * \see VectorStyleTable::GetDefaultStyle()
 */
VectorStyle *VectorStyleManager::GetDefaultStyle(Vector::VectorType Type) {
   return GetDefaultStyle(defaultCategory_, Type);
}

/**
 * Devuelve un estilo de vector particular, especificando Categoria, Tipo e Id
 * del estilo buscado.
 * @param[in]	CategoryName: nombre de la categoria (layer, draw, meassure, grid)
 * @param[in]	VectorType: tipo de vector (point, line, polygon)
 * @param[in]	StyleId: Id del estilo de vector buscado.
 * @return el estilo de vector de la categoria, tipo y Id especificado
 * @return NULL si no existe tabla de la categoria y tipo de vector solicitado.
 * \attention el VectorStyle retornado es responsabilidad del invocante.
 */
VectorStyle *VectorStyleManager::GetStyle(const std::string &CategoryName,
                                          Vector::VectorType VectorType,
                                          size_t StyleId) {
   VectorStyleTable *pvstyletable = GetTable(CategoryName, VectorType);
   if (!pvstyletable || pvstyletable->GetStyleCount() == 0) {
      REPORT_AND_FAIL_VALUE(
            "D:Error en el Administrador de Estilos Vectoriales: No se encontraron estilos "
            "para la categoria: %s tipo de vector: %s.",
            NULL, CategoryName.c_str(), LongToString((long)VectorType).c_str());
   }
   return pvstyletable->GetStyle(StyleId);
}

// ------------------- METODOS PRIVADOS ------------------------------
/**
 * Carga solo los estilos default de la categoria Layer. El resto de los estilos
 * se van cargando a medida que se van necesitando. Ver VectorElementEditor::CreateVectorElement
 * \todo Cargar los estilos del Sistema desde archivo
 */
void VectorStyleManager::LoadDefaultStyles() {
   // Cargo Tabla LAYER Undefined
   const char* pundefinedstylearray[] = { LAYER_UNDEFINED_1 };
   std::vector<std::string> styles(pundefinedstylearray, pundefinedstylearray +
   ARRAY_SIZE(pundefinedstylearray));
   AddTable(new VectorStyleTable(Vector::Undefined, LAYER_TABLE_NAME, styles));
   // Cargo Tabla LAYER Point
   const char* ppointstylearray[] = { LAYER_POINT_1, LAYER_POINT_2, LAYER_POINT_3 };
   styles = std::vector<std::string>(ppointstylearray, ppointstylearray +
   ARRAY_SIZE(ppointstylearray));
   AddTable(new VectorStyleTable(Vector::Point, LAYER_TABLE_NAME, styles));
   // Cargo Tabla LAYER Line
   const char* plinestylearray[] = { LAYER_LINE_1, LAYER_LINE_2, LAYER_LINE_3 };
   styles = std::vector<std::string>(plinestylearray, plinestylearray +
   ARRAY_SIZE(plinestylearray));
   AddTable(new VectorStyleTable(Vector::Line, LAYER_TABLE_NAME, styles));
   // Cargo Tabla LAYER Polygon
   const char* ppolygonstylearray[] = { LAYER_POLYGON_1, LAYER_POLYGON_2,
                                        LAYER_POLYGON_3, LAYER_POLYGON_4 };
   styles = std::vector<std::string>(ppolygonstylearray, ppolygonstylearray +
   ARRAY_SIZE(ppolygonstylearray));
   AddTable(new VectorStyleTable(Vector::Polygon, LAYER_TABLE_NAME, styles));
   // Cargo Tabla
   const char* pmaskstylearray[] = { MASK_POLYGON_1 };
   styles = std::vector<std::string>(pmaskstylearray, pmaskstylearray +
   ARRAY_SIZE(pmaskstylearray));
   AddTable(_VECTOR_MASK_STYLE_NAME_,
            new VectorStyleTable(Vector::Polygon, _VECTOR_MASK_STYLE_NAME_, styles));
   const char* pmaskexternalstylearray[] = { MASK_POLYGON_EXTERNAL };
   styles = std::vector<std::string>(pmaskexternalstylearray, pmaskexternalstylearray +
   ARRAY_SIZE(pmaskexternalstylearray));
   AddTable(_VECTOR_MASK_EXTERNAL_STYLE_NAME_,
               new VectorStyleTable(Vector::Polygon, _VECTOR_MASK_EXTERNAL_STYLE_NAME_, styles));
}
} /*namespace suri*/

