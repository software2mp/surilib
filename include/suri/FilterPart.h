/*! \file FilterPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FILTERPART_H_
#define FILTERPART_H_

// Includes Estandar
// Includes Suri
#include "FilterProcess.h"
#include "LibraryItemSelectionInterface.h"
#include "Part.h"
#include "ProcessAtributeProvider.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class LibraryManager;
class BasicLibraryItemSelector;

class FilterPart: public Part, public ProcessAtributeProvider,
   public LibraryItemSelectionInterface{
public:
   explicit FilterPart(LibraryManager* pManager, int Filters = NO_FILTER);
   virtual ~FilterPart();
   // ---------- Permite acceder a los datos configurados ---------
   /** Obtiene el tamanio del kernel del primer elemento de la lista de kernels */
   void GetKernelSize(int &Rows, int &Columns);
   /** Obtiene el tipo de filtro que se va a aplicar */
   virtual FilterProcess::FilterType GetFilterType() = 0;
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Crea la ventana de la parte. */
   virtual bool CreateToolWindow() = 0;
   /** Devuelve el estado del Part. */
   virtual bool IsEnabled();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Configura el editor con el item que se pasa por parametro. Se pasa por parametro
    * un boolean que indica si se trata de la primera seleccion (para que el part no piense
    * que posee cambios cuando en realidad es la seleccion inicial del CGU)*/
   void SetActiveItem(const LibraryItem* pItem);
   /** Actualiza el estado de la parte */
   virtual void Update();
protected:
   /** Devuelve la lista con los filtros */
   virtual std::list<std::vector<std::vector<double> > > GetFilterMatrix(
         FilterProcess::FilterType Type , int Size) = 0;
   /** Devuelve el nodo de renderizacion */
      wxXmlNode* CreateFilterRenderizationNode(std::vector<int> &BandList);
   // ---------- Metodos que interactuan con la gui ----------
   /** Obtiene el tipo de filtro seleccionado en el control wx */
   virtual FilterProcess::FilterType GetSelectedFilterType() = 0;
   /** Configura el tipo de filtro seleccionado en el control wx */
   virtual void SetSelectedFilterType(FilterProcess::FilterType Type) = 0;
   /** Obtiene el tamanio del kernel seleccionado del control wx */
   virtual void GetSelectedKernelSize(FilterProcess::FilterType Type, int &Rows, int &Columns) = 0;
   /** Configura la dimension del kernel en interfaz grafica*/
   virtual void SetSelectedKernelSize(FilterProcess::FilterType Type, int Rows, int Columns) = 0;
   // ---------- Metodos que configuran el nodo -----------
   /** Devuelve el nodo con el algebra de banda segun el filtro seleccionado */
   virtual wxXmlNode* GetBandsEquationNode(int BandCount) = 0;
   /** Devuelve la lista de filtros seleccionados */
   std::list<std::vector<std::vector<double> > > GetSelectedFilterList(int BandCount);
   /** Atributos **/
   LibraryManager* pManager_;
   BasicLibraryItemSelector* pItemSelector_;
   /* filtros */
   std::list<std::vector<std::vector<double> > > filterList_; /*! Lista con los */
   FilterProcess::FilterType selectedFilter_; /*! Tipo de filtro seleccionado */
   const LibraryItem* pSelectedFilter_;
};

} /** namespace suri */

#endif /* FILTERPART_H_ */
