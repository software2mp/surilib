/*! \file DensitySliceEditorPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <vector>
#include <string>

// Includes Suri
#include "LibraryTableEditionPart.h"

// Includes Wx
#include "wx/grid.h"

// Defines
#ifndef DENSITYSLICEEDITORPART_H_
#define DENSITYSLICEEDITORPART_H_

namespace suri {

// Forwards
class Table;
class RangeItemAttribute;
DECLARE_EVENT_OBJECT(DensitySliceEventHandler);
class wxGenericTableBase;
class TablePart;

/**
 * Clase que representa un editor de un item de biblioteca de tabla de intervalos.
 */
class DensitySliceEditionPart : public suri::LibraryTableEditionPart {
   /** constructor copia **/
   DensitySliceEditionPart(const DensitySliceEditionPart&);

public:
   /** constructor **/
   DensitySliceEditionPart(
         const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
         UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature);

   /** destructor **/
   virtual ~DensitySliceEditionPart();

private:
   FRIEND_EVENT_OBJECT(DensitySliceEventHandler);
   EVENT_OBJECT_PROPERTY(DensitySliceEventHandler);
   static const int RANGE_NAME_COL = 0;
   static const int LOW_COL = 1;
   static const int HIGH_COL = 2;
   static const int HEXCOLOR_COL = 3;
   /** bool que indica si se debe completar los rangos para no dejar huecos**/
   bool continuosRange_;
   /**
    *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
    *  Este metodo es el que realmente tienen que implementar las clases hijas para
    * lograr la especializacion a la hora de iniciar el part (metodo que se llama
    * en CreateToolWindow, a modo de Template Method).
    **/
   virtual bool ConfigureFeatures();

   /**
    * Metodo auxiliar privado que es necesario implementar en las clases hijas.
    * Este metodo es el encargado de actualizar realmente la informacion en el part
    * Es llamado en el metodo SetActiveItem a modo de Template Method
    **/
   virtual void UpdateFields(const LibraryItem* pItem, bool Modifiable = true);
   /**
    * Metodo auxiliar privado que agrega comportamiento adicional al rollback de la
    * clase base. Este comportamiento adicional es necesario cuando
    * el rollback involucra objetos adicionales a los items de biblioteca
    * del editor (ej. parts compuesto en donde se necesita llamar a los rollbacks de
    * los mismos)
    * @return true en caso de realizar correctamente el rollback "interno"
    * @return false en caso de que ocurra algun error durante el rollback
    */
   virtual bool OnRollback();
   /** Metodo auxiliar que carga el contenido del item seleccionado en la tabla del Part
    * @param[in] pItem item que sera representado por la tabla
    * @param[in] AddRangeToItem bool que indica si es necesario agregar rangos al item
    * que se quiere mostrar en la tabla
    */
   virtual void LoadItemToTable(const LibraryItem* pItem, bool AddRangeToItem = false);
   /**
    * Agrega un rango perteneciente a un item a la tabla de intervalos
    * @param pRange
    */
   void AddRangeToTable(RangeItemAttribute* pRange);
   /**
    * Obtiene de la tabla los rangos cargados
    * y crea un item asociado
    * @param pItem item sobre  el cual cargar los rangos
    */
   virtual void LoadAttributesFromTable(std::vector<LibraryItemAttribute*>* &pAttributes);
   /** Metodo que se llama cuando se cambia el valor de una celda **/
   void OnCellChangeEvent(wxGridEvent& Event);
   /** Metodo que se llama cuando se presiona el boton de agregar columna*/
   void OnAddRowClick(wxCommandEvent& Event);
   /**
    * A partir del valor actualizado en la columna y fila que se indica, se actualizan
    * los limites de los rangos qeu contiene la tabla
    * @param Column columna en donde se cambio el valor
    * @param Row fila en donde se cambio el valor
    * @param Value valor actualizado
    */
   void UpdateRangeLimits(int Column, int Row, const std::string& Value);
   /**
    * Actualiza hacia los limites de los rangos de la tabla hacia adelante
    * @param Column columna en donde se cambio el valor
    * @param Row fila en donde se cambio el valor
    * @param Value valor actualizado
    */
   void ForwardRangeLimitsUpdate(int Column, int Row, const double Value);
   /**
    * Actualiza hacia los limites de los rangos de la tabla hacia atras
    * @param Column columna en donde se cambio el valor
    * @param Row fila en donde se cambio el valor
    * @param Value valor actualizado
    */
   void BackwardRangeLimitsUpdate(int Column, int Row, const double Value);
   /** Metodo que dado un item busca que tenga la menos un atributo correspondiente a un
    *  intervalo de tabla de colores
    *  @param[in] pItem item sobre el cual se quiere consultar
    *  @return true en caso de que tenga al menos un atributo, false en caso contrario
    */
   bool IsRangeItem(const LibraryItem* pItem);
   /** Agrega las columnas de la tabla */
   virtual void AppendColumns();
   /** xrc de panel de editor de density slice **/
   static const std::string DS_EDITION_XRC_NAME;
   /** id del panel donde se ubica el cgu **/
   static const std::string DS_CGU_PANEL;
};
}

#endif /* DENSITYSLICEEDITORPART_H_ */
