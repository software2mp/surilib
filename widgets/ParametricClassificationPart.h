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
