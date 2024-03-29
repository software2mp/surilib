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

#ifndef KMEANSPART_H_
#define KMEANSPART_H_

// Includes standard
// Includes Suri
#include "ClassificationPart.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {

class World;
class LayerList;

// forwards
namespace raster {
namespace data {
   class StatisticsBase;
} 
}
/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(KMeansPartWidgetEvent);

/** Permite clasificar usando kmeans */
/**
 * Utiliza KMeansCanvas y KMeansAlgorithm para clasificar la imagen.
 * Le da al usuario la opcion de elegir numero de clases, numero maximo
 * de iteraciones y el threshold.
 */
class KMeansPart : public ClassificationPart {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(KMeansPartWidgetEvent);
   /** Ctor */
   KMeansPart(Element* pElement);
   /** Dtor */
   virtual ~KMeansPart();

   /** Crea la ventana de la parte */
   virtual bool CreateToolWindow();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
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
// void GetWindowIcon(wxBitmap& ToolBitmap,int X,int Y) const;
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
   /** Calcula iteracion de algoritmo KMeans */
   bool GetNextMean(RasterElement* pRasterElement,
                    const std::vector<std::vector<double> > &InitialMeans,
                    std::vector<std::vector<double> > &NextMean);
   /** Calcula que porcentaje se modificaron las medias */
   double CalculateChange(const std::vector<std::vector<double> > &InitialMeans,
                          const std::vector<std::vector<double> > &NextMean);
   /** Calcula las medias iniciales */
   void GetInitialMeans(int ClassCount, int BandCount,
                        std::vector<std::vector<double> > &InitialMeans);
   /**
    *  En funcion del minimo y maximo de la imagen (por banda) se generan clases iniciales de forma
    *  aleatoria.
   **/
   void GetRandomInitialMeans(int ClassCount, int BandCount,
                              suri::raster::data::StatisticsBase* pStatistics, 
                              std::vector<std::vector<double> > &InitialMeans);
   /**  En funcion del minimo y maximo de la imagen (por banda)
    *  se fracciona el intervalo (min,max) en la cantidad de clases
    *  y se selecciona la media por cada banda de forma random dentro de esos
    *  randoms    
   **/
   void GetPartialRandomInitialMeans(int ClassCount, int BandCount,
                              suri::raster::data::StatisticsBase* pStatistics, 
                              std::vector<std::vector<double> > &InitialMeans);
   /**
    * En funcion del minimo y maximo de la imagen (por banda) se generan clases iniciales 
    * utilizando maxima dispercion por la diagonal
   **/
   void GetMaximunDispersionInitialMeans(int ClassCount, int BandCount,
                              suri::raster::data::StatisticsBase* pStatistics, 
                              std::vector<std::vector<double> > &InitialMeans);

   /** Cambia el valor del slider */
   void SetSliderCtrlValue(double Threshold);
   /** Retorna el valor del slider */
   double GetSliderCtrlValue();
   /** Cambia la cantidad de clases en control */
   void SetClassCountCtrlValue(int ClassCount);
   /** Retorna la cantidad de clases seleccionada por usuario */
   int GetClassCountCtrlValue();
   /** Cambia el numero maximo de iteraciones en control */
   void SetMaxIterationsCtrlValue(int ClassCount);
   /** Retorna el numero maximo de iteraciones seleccionada por usuario */
   int GetMaxIterationsCtrlValue();
   /** modifica el texto que informa la posicion */
   virtual void OnScroll(wxScrollEvent& Event);
   /*! threshold inicial cargado en el constructor */
   double defaultThreshold_;
   /*! variable con threshold interno(sel por usuario en el ultimo commit). */
   double threshold_;
   /*! cantidad default de clases que se van a usar para clasificar */
   int defaultClassCount_;
   /*! cantidad de clases que se van a usar para clasificar
    * (sel por usuario en el ultimo commit) */
   int classCount_;
   /*! maxima cantidad default de iteraciones */
   int defaultMaxIterations_;
   /* que se van a realizar */
   /*! maxima cantidad de iteraciones que se van a
    * realizar(sel por usuario en el ultimo commit) */
   int maxIterations_;
   /*! Objeto que conecta los  eventos */
   EVENT_OBJECT_PROPERTY(KMeansPartWidgetEvent);
   Element* pElement_;
   int method_;
};
}  // namespace suri

#endif /* KMEANSPART_H_ */
