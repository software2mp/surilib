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

// Includes Estandar
#include <list>
#include <string>
#include <vector>
// Includes Suri
#include "BandInfo.h"
#include "ConvolutionFilterRenderer.h"
#include "suri/FilterPart.h"
#include "suri/XmlFunctions.h"
// Includes Wx
// Defines
// forwards

namespace suri {
FilterPart::FilterPart(LibraryManager* pManager, int):
   pManager_(pManager), pItemSelector_(NULL),
   selectedFilter_(FilterProcess::LowPassFilter),
   pSelectedFilter_(NULL){
   windowTitle_ = _(caption_FILTER_SELECTION);
}

FilterPart::~FilterPart() {
}

/**
 * Invoca al metodo que devuelve una lista con el filtro segun las opciones
 * seleccionadaas con una banda como parametro, y guarda en filtro en la
 * propiedad interna.
 */
bool FilterPart::CommitChanges(){
   selectedFilter_ = GetSelectedFilterType();
   int rows, columns;
   GetSelectedKernelSize(selectedFilter_, rows, columns);
   filterList_ = GetFilterMatrix(selectedFilter_, rows);
   if (pPal_) {

      std::vector<BandInfo> selectedbands;
      pPal_->GetAttribute<std::vector<BandInfo> >(ProcessAdaptLayer::BandInfoKeyAttr,
                                                   selectedbands);
      std::vector<int> bands;
      for (int ix = 0, lenix = selectedbands.size(); ix < lenix; ++ix) {
         bands.push_back(selectedbands[ix].GetPosition());
      }

      /**
       * TODO matiaslafroce: Quitar este hack
       * El proceso de filtros requiere conocer el numero de bandas de la imagen
       * sin embargo, ese numero lo obtiene del part de seleccionar bandas.
       * Eso hace que el part de filtros sea dependiente del de bandas y
       * haya que poner varios hacks al respecto.
       * El siguiente hack es para el caso de filtros de post-clasificacion,
       * ya que por algun motivo no detecta ninguna banda del modelo de clases.
       */
      if (bands.size() == 0) {
         bands.push_back(0);
      }

      wxXmlNode* pfnode = CreateFilterRenderizationNode(bands);
      pPal_->AddAttribute<wxXmlNode*>(FilterProcess::FilterNodeKey, pfnode);

      // Agrega la ecuacion
      wxXmlNode* pequ = GetBandsEquationNode(selectedbands.size());
      if (pequ != NULL)
         pPal_->AddAttribute<wxXmlNode*>(FilterProcess::EquationNodeKey, pequ);
   }
   return true;
}

/** Configura el editor con el item que se pasa por parametro. Se pasa por parametro
 * un boolean que indica si se trata de la primera seleccion (para que el part no piense
 * que posee cambios cuando en realidad es la seleccion inicial del CGU)*/
void FilterPart::SetActiveItem(const LibraryItem* pItem) {
   pSelectedFilter_ = pItem;
}


/**
 * La parte esta siempre habilitada.
 * @return enabled_
 */
bool FilterPart::IsEnabled() {
   return enabled_;
}

/**
 * Indica si los datos del control wx cambiaron con respecto a las propiedades.
 * @return modified_
 */
bool FilterPart::HasChanged() {
   return modified_;
}

/**
 * Actualiza el estado del part.
 */
void FilterPart::Update() {}

/**
 * Devuelve tamanio de filas y columnas de la matriz almacenada en
 * en el ultimo commit exitoso
 * @param[out] Rows filas de la matriz
 * @param[out] Columns columnas de la matriz
 */
void FilterPart::GetKernelSize(int &Rows, int &Columns) {
   Rows = 3;
   Columns = 3;
   if (!filterList_.empty()) {
      std::vector<std::vector<double> > filter = filterList_.front();
      Rows = filter.size();

      std::vector<double> row = filter.front();
      Columns = row.size();
   }
}

/**
 * Crea el nodo de filtros de convolucion y modifica el vector de bandas
 * seleccionadas para el caso de filtros con mas un de matriz asociada.
 * Crea una variable de tipo parameters con el tamanio del kernel y la lista
 * con los kernel seleccionados. Crea un nodo reenderizacion a partir de los
 * parametros.
 * @param[in]  BandList vector con los indices de las bandas seleccionadas
 * @param[out] BandList vector con los indices de las bandas seleccionadas
 * @return pfilternode nodo renderizador del filtro
 * \attention el nodo devuelto (y su destruccion) es responsabilidad del
 * que lo solicita.
 */
wxXmlNode* FilterPart::CreateFilterRenderizationNode(
                                          std::vector<int> &BandList) {
   ConvolutionFilterRenderer::Parameters parameters;
   int rows = 0, columns = 0;
   GetKernelSize(rows, columns);
   parameters.filterHeight_ = rows;
   parameters.filterWidth_ = columns;
   /*
    Temporalmente se asigna el nombre del algoritmo con if. Ver TCK #760
    con posible refactor
    */
   if (selectedFilter_ == FilterProcess::MaxFilter) {
      parameters.filterName_ = ALGORITHM_VALUE_MAYORITYFILTER;
   } else if (selectedFilter_ == FilterProcess::MinFilter) {
      parameters.filterName_ = ALGORITHM_VALUE_MINORITYFILTER;
   } else {
      parameters.filterName_ = ALGORITHM_VALUE_KERNELFILTER;
      // se mutlplica la cantidad de bandas en funcion de la cantidad de matrices
      std::vector<int> bandlistdoubled;
      int multiplier = pSelectedFilter_ ?
                     StringToNumber<int>(pSelectedFilter_->GetAttribute("dim")->GetValue()) : 1;
      std::vector<int>::iterator iterbands = BandList.begin();
      for (; iterbands != BandList.end(); iterbands++)
         for (int i = 0; i < multiplier; ++i)
            bandlistdoubled.push_back(*iterbands);

      BandList.assign(bandlistdoubled.begin(), bandlistdoubled.end());
   }

   parameters.filters_ = GetSelectedFilterList(BandList.size());

   wxXmlNode * pfilternode = NULL;
   pfilternode = ConvolutionFilterRenderer::GetXmlNode(parameters);
   return pfilternode;
}

/**
 * Este metodo devuelve la lista de filtros. Recibe la cantidad de bandas y genera
 * una lista repitiendo el kernel almacenado en la propiedad interna tantas veces
 * como la cantidad de bandas recibidas. Como ejemplo, para un filtro de deteccion de bordes la
 * lista de la propiedad interna tiene dos matrices, estas dos matrices se repiten
 * en la lista resultante la mitad de veces las bandas recibidas, de forma que en
 * la lista queden tantas matrices como bandas.
 * @param[in] BandCount cantidad de bandas.
 * @return filterlist lista con tantos filtros (matrices) como bandas.
 */
std::list<std::vector<std::vector<double> > > FilterPart::GetSelectedFilterList(
      int BandCount) {
   std::list<std::vector<std::vector<double> > > filterlist;
   if (filterList_.size() == 1) {
      filterlist.assign(BandCount, filterList_.front());
   } else {
      // se multiplican la cantidad de filtros por la cantidad de bandas
      for (int i = 0; i < BandCount ; ++i) {
         std::list<std::vector<std::vector<double> > >::iterator it = filterList_.begin();
         for (; it != filterList_.end(); ++it)
            filterlist.push_back(*it);
      }
   }
   return filterlist;
}

} /** namespace suri */
