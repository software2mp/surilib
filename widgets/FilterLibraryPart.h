/*! \file FilterLibraryPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FILTERLIBRARYPART_H_
#define FILTERLIBRARYPART_H_

#include "suri/FilterPart.h"

namespace suri {

class FilterLibraryPartEvent;

class FilterLibraryPart: public FilterPart {
public:
   /**
    * Ctor.
    */
   explicit FilterLibraryPart(LibraryManager* pManager, int Filters = NO_FILTER);

   /**
    * Dtor.
    */
   virtual ~FilterLibraryPart();

   /** Crea la ventana de la parte. */
   virtual bool CreateToolWindow();

   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();

   /** Por defecto la lista de bandas esta vacia */
   virtual void SetInitialValues();

   /** Obtiene el icono de la parte. */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;

   /** Devuelve el nodo con el algebra de banda*/
   wxXmlNode* GetBandsEquationNode(int BandCount);

   /** Evento temporal */
   void OnUiUpdate(wxUpdateUIEvent &Event);

protected:
   /** Obtiene el tipo de filtro que se va a aplicar */
   FilterProcess::FilterType GetFilterType();

   /** Devuelve LibraryFilter */
   virtual FilterProcess::FilterType GetSelectedFilterType();

   /** Implementa interfaz*/
   virtual void SetSelectedFilterType(FilterProcess::FilterType Type);

   /** Devuelve la lista con los filtros */
   std::list<std::vector<std::vector<double> > > GetFilterMatrix(
         FilterProcess::FilterType Type, int Size);

   /**
    * NO SE IMPLEMENTA.
    */
   virtual void GetSelectedKernelSize(FilterProcess::FilterType Type, int &Rows,
                                      int &Columns);

   /**
    * NO SE IMPLEMENTA.
    */
   virtual void SetSelectedKernelSize(FilterProcess::FilterType Type, int Rows,
                                      int Columns);
private:
   EVENT_OBJECT_PROPERTY(FilterLibraryPartEvent);
   bool selected_;
};

}  // namespace suri

#endif /* FILTERLIBRARYPART_H_ */
