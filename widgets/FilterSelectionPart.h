/*! \file FilterSelectionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FILTERSELECTIONPART_H_
#define FILTERSELECTIONPART_H_

// Includes standard
#include <list>
#include <vector>
#include <map>

// Includes suri
#include "suri/FilterPart.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/xml/xml.h"

/** namespace suri */
namespace suri {

// Forwards
/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(FilterSelectionPartEvent);

/** Flag para que se muestre filtro pasa bajos */
#define NO_FILTER                 0x00000000
/** Flag para que se muestre filtro pasa bajos */
#define LOWPASS_FILTER                 0x00000001
/** Flag para que se muestre filtro pasa altos */
#define HIGHPASS_FILTER              0x00000002
/** Flag para que se muestre filtro deteccion de borde */
#define EDGEDETECTION_FILTER            0x00000004
/** Flag para que se muestre filtro para encontrar el menos frecuente */
#define MINORITY_FILTER     0x00000008
/** Flag para que se muestre opcion para encontrar el mas frecuente */
#define MAYORITY_FILTER     0x00000010

/** Clase que crea una parte que sirve para seleccionar tipo y tamanio del filtro */
/**
 * Parte para la configuracion del filtro a aplicar sobre la imagen.
 * En base a los parametros seleccionados crea el filtro correspondiente.
 * Crea una lista de filtros, repitiendo el filtro generado tantas veces como
 * bandas de la imagen se seleccionaron. Con esa lista se configura una variable
 * de tipo parameters, a la que tambien se le configura las dimensiones del
 * kernel; con esta variable se obtiene un nodo xml del filtro de convolucion,
 * que es el nodo que devuelve la herramienta.
 */
class FilterSelectionPart : public FilterPart{
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(FilterSelectionPartEvent);
   /** Ctor. */
   explicit FilterSelectionPart(LibraryManager* pManager, int Filters = NO_FILTER);
   /** Dtor. */
   virtual ~FilterSelectionPart();

   //  ---------- Metodos de part ----------
   /** Crea la ventana de la parte y contecta el evento UIUpdate. */
   virtual bool CreateToolWindow();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Por defecto la lista de bandas esta vacia */
   virtual void SetInitialValues();
   /** Devuelve el icono de la parte */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Evento temporal */
   void OnUiUpdate(wxUpdateUIEvent &Event);

   // ---------- Permite acceder a los datos configurados ---------
   /** Obtiene el tipo de filtro que se va a aplicar */
   FilterProcess::FilterType GetFilterType();

   // ---------- Metodos que configuran el nodo -----------
   /** Devuelve el nodo con el algebra de banda segun el filtro seleccionado */
   wxXmlNode* GetBandsEquationNode(int BandCount);
   /** Devuelve el nodo de renderizacion */
   wxXmlNode* CreateFilterRenderizationNode(std::vector<int> &BandList);
   /** Crea el nodo con el algebra de bandas correspondiente al filtro deteccion de bordes */
   wxXmlNode* CreateEdgeDetectionFilterEquationNode(int BandCount);

protected:
   // ---------- Metodos que interactuan con la gui ----------
   /** Obtiene el tipo de filtro seleccionado en el control wx */
   FilterProcess::FilterType GetSelectedFilterType();
   /** Configura el tipo de filtro seleccionado en el control wx */
   void SetSelectedFilterType(FilterProcess::FilterType Type);
   /** Obtiene el tamanio del kernel seleccionado del control wx */
   void GetSelectedKernelSize(FilterProcess::FilterType Type, int &Rows, int &Columns);
   /** Configura la dimension del kernel en interfaz grafica*/
   virtual void SetSelectedKernelSize(FilterProcess::FilterType Type, int Rows, int Columns);

   // ---------- Metodos que calcular matriz de convolucion ----------
   /** Devuelve la lista con los filtros */
   std::list<std::vector<std::vector<double> > > GetFilterMatrix(
         FilterProcess::FilterType Type, int Size);
   /** Crea una lista de filtros pasabajos */
   std::list<std::vector<std::vector<double> > > CreateLowPassFilter(int Size);
   /** Crea una lista de filtros pasaaltos */
   std::list<std::vector<std::vector<double> > > CreateHighPassFilter(int Size);
   /** Crea una lista de filtros de deteccion de bordes */
   std::list<std::vector<std::vector<double> > > CreateEdgeDetectionFilter();


private:
   /* eventos */
   EVENT_OBJECT_PROPERTY(FilterSelectionPartEvent); /*! Clase para conectar */
   int filters_; /*! Determina los filtros que se mostraran al usuario. Si -1 se muestran todos */
   std::map<FilterProcess::FilterType, int> suportedFilters_; /*! Guarda los filtros */
                                      /* soportados y su posicion en choice */

};
}

#endif /* FILTERSELECTIONPART_H_ */
