/*! \file ParametricClassificationPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PARAMETRICCLASSIFICATIONPART_H_
#define PARAMETRICCLASSIFICATIONPART_H_

// Includes standard
// Includes Suri
#include "ClassificationPart.h"
#include "suri/Element.h"
#include "suri/StatisticsFunctions.h"
#include "ClassStatisticsHtmlListItem.h"
// Includes Wx
#include "wx/choicebk.h"
// Defines

/** namespace suri */
namespace suri {
class ClassSelectionPart;
class LayerList;
class SupervisedParameterSelectionPart;
class VectorElement;
class Clusters;
class ViewcontextManagerInterface;



/** Declara clase ParametricClassificationPartEvent */
DECLARE_EVENT_OBJECT(ParametricClassificationPartEvent);

/** Permite selecionar un o varios poligonos, un umbral y un color */
/**
 * Contiene un ClassSelectionPart para seleccionar clases(poligonos), y una
 * lista de Parts para seleccionar el algoritmo de clasificacion y los parametros
 * asociados (actualmente solo el Maxima verosimilitud, y para la seleccion de
 * parametros un wxColorPickerCtrl para el color de la no clase y un wxSlider
 * para seleccionar el umbral).
 * Calcula las estadisticas correspondientes a cada clase seleccionada (poligono)
 * Crea el nodo clasificador (ClassificationRenderer), que se debe gregar al
 * elemento.
 *
 */
class ParametricClassificationPart : public ClassificationPart {
   /** Declara friend la clase de evento ParametricClassificationPartEvent. */
   FRIEND_EVENT_OBJECT(ParametricClassificationPartEvent);

public:
   typedef enum {
      Supervised = 0, NonSupervised
   } ClassificationType;

public:

   /** Ctor */
   ParametricClassificationPart(LayerList* pElements, Element* pInputElement,
                     DataViewManager* pDataViewManager, int type = Supervised);
   /** Dtor */
   virtual ~ParametricClassificationPart();
   /** Crea la ventana de la parte */
   virtual bool CreateToolWindow();
   /** Devuelve el estado del Part. */
   virtual bool IsEnabled();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa los datos mostrados */
   virtual void SetInitialValues();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Informa si el subset seleccionado es valido */
   virtual bool HasValidData();
   /** Actualiza el estado de la parte */
   virtual void Update() {
   }
   /** Retorna la cantidad de clases */
   virtual int GetClassCount();
   /** Retorna las propiedades de la clase en la posicion indicada.
    * Las Posiciones van de 0 a GetClassCount()-1.**/
   virtual bool GetClassProperties(std::vector<ClassInformation> &Properties);
   /** Retorna el nodo que se debe agregar a elemento para renderizar */
   virtual wxXmlNode* GetClassificationRendererNode(RasterElement* pRasterElement);

protected:
   /** Calcula las estadisticas del poligono */
   virtual bool CalculateStatistics(RasterElement* pRasterElement,
                                    VectorElement* pPolygon, Statistics& Statistics);
   /** Calcula las estadisticas del poligono */
   virtual bool CalculateStatistics(RasterElement* pRasterElement,
                                    VectorElement* pPolygon, suri::raster::data::StatisticsBase* &pStatistics);
   /** Verifica si un vector esta completamente dentro del raster. */
   virtual bool AnalizeIfVectorInsideRaster(Element* pRasterElement,
                                            VectorElement* pVectorElement,
                                            bool &InsideRaster);
#ifdef __WXMSW__
   /** evento OnSize */
   void OnSize(wxSizeEvent& Event);
   /** Evento que se dispara cuando se cambio de pagina del book */
   virtual void OnPageChanged(wxBookCtrlBaseEvent &Event);
#endif
    /*! Part que perite seleccionar entre una o varias clases */
   ClassSelectionPart* pClassSelectionPart_;
   /*! Algoritmo seleccionado por el usuario en el ultimo commit */
   int selectedAlgorithm_;
   /*! lista con elementos que se pasa al ClassSelectionPart */
   LayerList* pElements_;
   /*! Elemento que se desea clasificar(raster) */
   Element* pInputElement_;
   /*! Lista con los parts que permiten ingresar parametros(uno por cada tipo de
    *  clasificacion) */
   std::vector<SupervisedParameterSelectionPart*> pParametersSelectioPartList_;
   int clasificationType_;
   /** Permite obtener la informacion de proyecto abierto */
   DataViewManager* pDataViewManager_;
   EVENT_OBJECT_PROPERTY(ParametricClassificationPartEvent); /*! Objeto que conecta los eventos */   
};
}  // namespace suri

#endif /* PARAMETRICCLASSIFICATIONPART_H_ */
