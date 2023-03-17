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

#ifndef SPECTRALSELECTIONPART_H_
#define SPECTRALSELECTIONPART_H_

#include <string>
#include <vector>
#include <map>

// Includes Suri
#include "suri/RasterElement.h"
#include "BandSelectionPart.h"
#include "SingleLayerBandSelectionPart.h"
#include "CombinationInfoGenerator.h"
#include "suri/ProcessAtributeProvider.h"

// Includes Wx
#include "wx/arrstr.h"
#include "wx/listctrl.h"

// Defines

/** namespace suri */
namespace suri {
/** Declara la clase SpectralSelectionPartEvent */
DECLARE_EVENT_OBJECT(SpectralSelectionPartEvent);

/** Parte para seleccionar bandas */
/**
 * Esta clase crea un part que permite hacer una seleccion de bandas de un
 * elemento rasster
 */
class SpectralSelectionPart : public Part,
                              public ProcessAtributeProvider,
                              public InputBandListener,
                              public CombinationInfoGenerator {
public:
   /** Declara friend la clase SpectralSelectionPartEvent */
   FRIEND_EVENT_OBJECT(SpectralSelectionPartEvent);

   /** Ctor. */
   explicit SpectralSelectionPart(SelectionPart* pSelectionPartP);

   /** Dtor. */
   virtual ~SpectralSelectionPart();

   /** Crea la ventana de la parte */
   virtual bool CreateToolWindow();
   /** Agrega las columnas a las listas, iconos y nombres de bandas */
   void ConfigureOutputBandList();
   /** Devuelve el estado del Part. */
   virtual bool IsEnabled();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Por defecto la lista de bandas esta vacia */
   virtual void SetInitialValues();
   /** Devuelve el icono de la parte */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Metodo que se ejecuta con el boton para agregar bandas */
   void OnAddButton(wxCommandEvent &Event);
   /** Metodo que se ejecuta con el boton para quitar bandas */
   void OnRemoveButton(wxCommandEvent &Event);
   /** Metodo que se ejecuta con el boton para subir bandas */
   void OnUpButton(wxCommandEvent &Event);
   /** Metodo que se ejecuta con el boton para bajar bandas */
   void OnDownButton(wxCommandEvent &Event);
   /** Metodo que se ejecuta ante un evento en la lista de salida */
   void OnOutputSelection(wxListEvent &Event);
   /** Verifica que haya por lo menos una banda seleccionada */
   virtual bool HasValidData();
   /** Devuelve la lista de bandas seleccionadas */
   std::vector<BandInfo> GetSelectedBands();
#ifdef __UNUSED_CODE__
// Pasa a la clase base como AddOutputBands
   /** Agrega los items seleccionados a la lista de salida */
   void AddSelectedItem();
#endif
   /** Elimina los items seleccionados de la lista de salida */
   void RemoveSelectedItem();
   /** Agrega el elemento definido en la posicion indicada en la lista de salida */
   bool AddOutputBand(BandInfo pBandInfoP, int Position);
   /** Elimina el item seleccionado de la lista de salida */
   bool RemoveItem(const int &Item, bool UpdateInternalBufferP = false);
   /** Actualizael indice de la banda en el control wx */
   void UpdateBandIndex();
   /** Obtiene la lista de bandas de la lista wx */
   virtual std::vector<BandInfo> GetOutputBands();
   /** Metodo que devuelve la posicion para insertar el proximo elemento */
   virtual int GetNextInsertPosition();
   /** Actualiza el estado de la parte */
   virtual void Update() { }
   /** Lanzado cuando cambia el tamanio de la ventana */
   void OnResize(wxSizeEvent &Event);

   /**
    * Metodo que recibe la notificacion de que se han selecciondo una o varias bandas.
    */
   virtual void OnBandSelected(std::vector<BandInfo> BandIxsP);

   /**
    * Agrega a los vectores la informacion de la combinacion correspondiente
    * a cada uno.
    */
   static void AppendCombinationInfo(std::vector<wxXmlNode*>& UrlsP,
                                     std::vector<wxXmlNode*>& CombinationsP, int GenIdP,
                                     std::string& UrlP, std::string& BandsP);

   /**
    * Devuelve la informacion de combinacion de bandas.
    */
   virtual CombinationInfo* GetCombinationInfo();

private:
   /** Declara la propiedad pEventHandler_ del tipo SpectralSelectionPartEvent* */
   EVENT_OBJECT_PROPERTY(SpectralSelectionPartEvent);

   // Part que actua como origen de datos.
   SelectionPart* pSelectionPart_;

   // buffer para simular el buffer interno que deberia tener el control lista (wxListCtrl)
   std::map<int, BandInfo> bandsBuffer_;

   // vector con las bandas seleccionadas (se utiliza para la funcion de rollback)
   std::vector<BandInfo> selectedBands_;

   // Notificaciones de salida vacia
   OutputEmptyListener* pOutputEmptyListener_;

   // Solicitud de elementos de entrada
   InputRequestedListener* pInputRequestedListener_;

   /**
    * Verifica si el item es valido para agregar comprobando que no sea
    * un item que ya existe en el buffer interno.
    */
   bool IsValidItem(BandInfo pItemP);

   /**
    * Configura el objeto que va a recibir las notificacion de que
    * el control de destino esta vacio.
    */
   void SetOutputEmptyListener(OutputEmptyListener* pOutputEmptyListener);

   /**
    * Devuelve el objeto que va a recibir las notificacion de que
    * el control de destino esta vacio.
    */
   OutputEmptyListener* GetOutputEmptyListener() const;

   /**
    * Configura el objeto que va a recibir las notificacion de que
    * el control esta solicitando elementos de entrada.
    */
   void SetInputRequestedListener(InputRequestedListener* pOutputEmptyListener);

   /**
    * Devuelve el objeto que va a recibir las notificacion de que
    * el control esta solicitando elementos de entrada.
    */
   InputRequestedListener* GetInputRequestedListener() const;
};

}  // namespace suri

#endif  // SPECTRALSELECTIONPART_H_
