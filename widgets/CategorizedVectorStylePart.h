/*! \file CategorizedVectorStylePart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CATEGORIZEDVECTORSTYLEPART_H_
#define CATEGORIZEDVECTORSTYLEPART_H_


// Includes Estandar
// Includes Suri
#include "suri/Part.h"
#include "TablePart.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class Table;
class DataViewManager;
class VectorLayer;

namespace ui {

// forwards
class CategorizedVectorStylePartHandler;
class VectorStyleClassGenerationWidget;

class CategorizedVectorStylePart : public Part {
public:
   CategorizedVectorStylePart(DataViewManager* pDataViewManager,
                              VectorLayer *pVectorLayer);
   virtual ~CategorizedVectorStylePart();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();
   /** Devuelve si la parte tiene datos validos */
   virtual bool HasValidData() {
      return pTablePart_ ? pTablePart_->HasValidData() : true;
   }

protected:
   friend class CategorizedVectorStylePartHandler;
   /**
    *  Metodo que se llama cuando se genera el evento de click sobre el boton
    *  de remover clase
    */
   void OnRemoveClassClick(wxCommandEvent& Event);
   /**
    *  Metodo que se llama cuando se genera el evento de click sobre el boton
    *  de agregar clase
    */
   void OnAddClassClick(wxCommandEvent& Event);
   /**
    * Metodo que se llama cuando se genera el evento de click sobre el boton
    * de generar clases
    */
   void OnClassGenerationClick(wxCommandEvent& Event);
   /* Metodo auxiliar para la configuracion de la tabla **/
   void ConfigureClassTable();
   /** Metodo auxiliar que genera el nodo xml para la renderizacion vectorial
    *  clasificada a partir de la configuracion realizada por el usuario
    */
   wxXmlNode* GetCategorizedRenderNodeFromTable();
   TablePart* pTablePart_;
   Table* pTable_;
   DataViewManager* pDataViewManager_;
   VectorLayer *pVectorLayer_;
   CategorizedVectorStylePartHandler* pEventHandler_;
   VectorStyleClassGenerationWidget* pClassGenerationWidget_;
};

} /** namespace ui **/
} /** namespace suri */

#endif /* CATEGORIZEDVECTORSTYLEPART_H_ */
