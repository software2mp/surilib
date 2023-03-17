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

#ifndef RASTERPROPERTIES_H_
#define RASTERPROPERTIES_H_


#include "suri/DataTypes.h"
#include "suri/ObserverInterface.h"
#include "suri/Part.h"
#include "suri/RasterElement.h"

#include "wx/wx.h"

namespace suri {

DECLARE_EVENT_OBJECT(RasterPropertiesEvent);

class LayerInterface;
class ParameterCollection;
class RasterDatasource;
class RasterFeaturesWidget;
class TablePart;
class Table;
/** Part con propiedades del raster(modelo, dimension, ...) */
/**
 * Muestra informacion de Dimension ,Organization, Identification, MapModel y
 * Bands
 * Solo despliega propiedades, por lo tanto el commit no hace nada.
 */
class RasterProperties : public Part, public suri::ObserverInterface {
public:
   /** Declara friend la clase de evento RasterPropertiesEvent */
   FRIEND_EVENT_OBJECT(RasterPropertiesEvent);

   /** Ctor. legacy */
   explicit RasterProperties(RasterElement* pElement);

   /** Ctor **/
   RasterProperties(RasterDatasource* pDatasource, LayerInterface* pLayer);

   /** Ctor **/
   RasterProperties(RasterDatasource* pDatasource, LayerInterface* pLayer, DataViewManager* pDataViewManager);

   /** Dtor. */
   virtual ~RasterProperties();

   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();

   /** Obtiene el icono del part */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;

   /** Inicializa los controles */
   virtual void SetInitialValues();

   /** Devuelve el estado del Part. */
   virtual bool IsEnabled() { return true; }

   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();

   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();

   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();

   /** Actualiza el estado de la parte */
   virtual void Update() { }

   /** carga datos de dimension */
   void LoadDimensionData();

   /** carga datos de organizacion */
   void LoadOrganizationData();

   /** carga datos de identificacion */
   void LoadIdentificationData();

   /** carga datos de Bandas */
   void LoadBandsData();

   /** Metodo auxiliar que restaura los valores**/
   void RestoreValues();

   /** Metodo de validacion de datos */
   virtual bool HasValidData();

   /** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
   virtual void Update(NotificationInterface* pNotification);

private:
   /** Obtiene el tipo de de dato para del DN  **/
   std::string LoadDataType();

   /** Levanta el valor de datos no validos  **/
   void LoadNoValidValue();

   /** Caraga el valor del chekbox **/
   void LoadNotValidValueCheckBox();

   /** Carga los limites para los valores no validos **/
   void LoadDnValueLimits();

   /** Carga las caracteristicas de la imagen raster. */
   void LoadFeatures();

   /** levanta el control sobre el check box de Valor no Valido **/
   void OnNotDataValueCheck(wxCommandEvent &Event);

   /** Evento para el seteo de un cambio en el not valid DN value **/
   void OnNotValueValidEventHandler(wxCommandEvent &Event);

   /**
    * Evento para el cambio de dimensiones (ancho y alto)
    */
   void OnDimensionChangedHandler(wxCommandEvent& Event);

   /**
    * Evento para el cambio en la cantidad de bandas.
    */
   void OnBandsCountChangedHandler(wxCommandEvent& Event);

   /** controla que los valores setados para not valid DN value se encuentren en sus limites**/
   bool IsbetweenNotValidValueLimts(const wxString &NotValidValue);

   /** Verifica si esta habilitada la carga de valor no valido */
   bool IsNaNEnabled();

   /**
    * Guarda los cambios de dimensiones al elemento.
    */
   void SaveDimToElement(RasterElement* pRaster);

   /**
    * Carga la metadatos para elementos raster crudos.
    */
   void LoadRawMetadata(wxXmlNode* pNode);

   /**
    * Metodo auxiliar para extraer informacion de metadata en el xml.
    */
   void GetRasterMetadata(wxXmlNode* pMetadataNode);

   /**
    * Metodo auxiliar para establecer informacion de metadata en el xml.
    */
   void SetRasterMetadata(wxXmlNode* pMetadataNode);

   /**
    * Guarda los metadatos para elementos raster crudos.
    */
   void SaveRawMetadataToElement(RasterElement* pRaster);

   /** Declara la propiedad pEventHandler_ del tipo RasterPropertiesEvent* */
   EVENT_OBJECT_PROPERTY(RasterPropertiesEvent);

   RasterElement* pElement_; /*! elemento vector. */
   RasterDatasource* pDatasource_; /** fuente de datos **/
   LayerInterface* pLayer_; /** capa del visualizador activo **/
   wxString notValidValue_ , lastNotValidValue_;
   bool dataValueChecked_, lastDataValueChecked_;
   std::string dataType_;
   double minNotValidValueLimit_, maxNotValidValueLimit_;
   bool dimModified_;
   DataViewManager* pDataViewManager_;
   RasterFeaturesWidget* pRasterFeaturesWidget_;
   ParameterCollection* pIOParams_;
   bool rawDataModified_;
   TablePart* pTablePart_;
   bool bandsDataModified_;
   Table* pTable_;
};

}  // namespace suri

#endif  // RASTERPROPERTIES_H_
