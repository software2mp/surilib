/*! \file ConvolutionFiltersEditorPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CONVOLUTIONFILTERSEDITORPART_H_
#define CONVOLUTIONFILTERSEDITORPART_H_

#include "DefaultTable.h"
#include "EquationParser.h"
#include "LibraryItemEditorPart.h"
#include "TablePart.h"
#include "suri/DataViewManager.h"

#include <wx/notebook.h>

namespace suri {

DECLARE_EVENT_OBJECT(ConvolutionFiltersEditorPartEventHandler);

class ConvolutionFilterItemAttribute;
class PageFilterWidget;

class ConvolutionFiltersEditorPart : public suri::LibraryItemEditorPart {
public:
   FRIEND_EVENT_OBJECT(ConvolutionFiltersEditorPartEventHandler);

   /**
    * Ctor.
    */
   ConvolutionFiltersEditorPart(
         const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
         UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature);

   /**
    * Dtor.
    */
   virtual ~ConvolutionFiltersEditorPart();

   /**
    * Indica si el Part tiene cambios para salvar.
    */
   virtual bool HasChanged();

   /**
    * Obtiene un vector con los atributos marcados para ser eliminados.
    */
   virtual std::vector<LibraryItemAttribute*>* GetAttributesToRemove();

   /**
    * Metodo para hacer limpieza. Este codigo esta puesto para cubrir una
    * emergencia. Es para salir del paso. Cuando los parts dejen de existir
    * esto va a desaparecer y no hay problema.
    */
   virtual bool CleanUp();

private:
   EVENT_OBJECT_PROPERTY(ConvolutionFiltersEditorPartEventHandler);

   /**
    * Metodo auxiliar privado que configura el part en si para cada editor en particular.
    * Este metodo es el que realmente tienen que implementar las clases hijas para
    * lograr la especializacion a la hora de iniciar el part (metodo que se llama
    * en CreateToolWindow, a modo de Template Method).
    */
   virtual bool ConfigureFeatures();

   /** Carga un item a la tabla. */
   void LoadItemToTable(const LibraryItem* pItem);

   /** Chequea la ecuacion ingresada */
   void OnEquationCheckClick(wxCommandEvent& Event);

   /** Reajusta la dimension de la matriz y resetea sus valores a 0. */
   void OnRefreshClick(wxCommandEvent& Event);

   /** Agrega un nuevo filtro */
   void OnAddFilterClick(wxCommandEvent& Event);

   /** Elimina el filtro seleccionado */
   void OnDeleteFilterClick(wxCommandEvent& Event);

   /** Metodo que actualiza los datos cambiados en el part */
   virtual void UpdateFields(const LibraryItem* pItem, bool Modifiable = true);

   /** Metodo que transforma los datos en atributos para ser guardados */
   virtual std::vector<LibraryItemAttribute*>* CreateAttributesFromFields();

   /** Agrega un nuevo filtro. */
   void AddNewFilter(ConvolutionFilterItemAttribute* pAttr = NULL, int FilterId = -1);

   /** Actualiza el estado del notebook widget. */
   void UpdateNotebook();

   /** Obtiene un vector con los IDs de los filtros disponibles ordenados de menor a mayor. */
   std::vector<int> GetFids(LibraryItem::MapAttribute& Attributes);

   /** id del panel de convolucion */
   static const std::string CONVOLUTIONFILTERS_PANEL;

   /** id del panel donde se ubica el cgu **/
   static const std::string CONVOLUTIONFILTERS_CGU_PANEL;
   std::vector<PageFilterWidget*> filters_;
   std::vector<std::string> filtersToRemove_;

   /** Item en edicion **/
   const LibraryItem* pCurrentItem_;
   int multiplicity_; // Evaluar si va a ser necesario este atributo.
   wxNotebook* pNotebook_; //
   int matrixdimension_; // Dimension de la matriz.
   int filtersnumber_;   // Cantidad de filtros por item.
};

}  // namespace suri

#endif  // CONVOLUTIONFILTERSEDITORPART_H_
