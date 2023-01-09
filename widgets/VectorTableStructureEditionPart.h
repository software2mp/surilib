/*! \file VectorTableStructureEditionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORTABLESTRUCTUREEDITIONPART_H_
#define VECTORTABLESTRUCTUREEDITIONPART_H_

// Includes standard

// Includes Suri
#include "suri/Part.h"
#include "VectorDatasource.h"

// Includes Wx
#include "wx/xrc/xmlres.h"
#include "wx/grid.h"

// Includes App

// Defines

namespace suri {

// Forwards
class Table;
DECLARE_EVENT_OBJECT(ComboSelectionEvent);
class VectorLayer;

/**
 * Part para seleccion de interpretacion de campo como hipervinculo.
 */
class VectorTableStructureEditionPart : public suri::Part {
public:
   /** Ctor */
   VectorTableStructureEditionPart(VectorDatasource* pVectorDatasource,
                                   VectorLayer* pLayer);
   /** Dtor */
   virtual ~VectorTableStructureEditionPart();
   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado del Part */
   virtual void Update();
   /**
    * Devuelve el icono de la parte
    * @param[out] ToolBitmap icono de la ventana
    * @param[in] X ancho de icono
    * @param[in] Y alto de icono
    */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;

private:
   FRIEND_EVENT_OBJECT(ComboSelectionEvent);
   EVENT_OBJECT_PROPERTY(ComboSelectionEvent);
   /** Metodo que captura el evento de cambio del choice **/
   void OnComboSelectionEventHandler(wxCommandEvent &Event);
   /*! Nombre de xrc de Part */
   static const std::string VECTOR_TABLE_STRUCTURE_EDITION_XRC_NAME;
   /** fuente de datos vectorial en edicion **/
   VectorDatasource* pVectorDatasource_;
   Table* pTable_;
   /** Capa activa que se encuentra en edicion **/
   VectorLayer* pLayer_;
};
}  /** namespace suri **/

#endif /* VECTORTABLESTRUCTUREEDITIONPART_H_ */
