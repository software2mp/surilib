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

#ifndef RASTEROUTPUTSIZEPART_H_
#define RASTEROUTPUTSIZEPART_H_

// Includes standard
#include <string>

// Includes Suri
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/CoordinatesTransformInterface.h"
#include "ReprojectionProcess.h"
#include "suri/World.h"
#include "suri/Dimension.h"
#include "suri/ObserverInterface.h"

// Includes Wx
#include "wx/textctrl.h"

// Includes App

// Defines

namespace suri {

// forwards
/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(ParametersChangedEvent);
DECLARE_EVENT_OBJECT(RadioButtonEvent);
DECLARE_EVENT_OBJECT(AspectCheckEvent);

/** Part que modifica la dimension del archivo de salida. */
/**
 * Se puede definir el tamanio en pixels y lineas, la relacion con la imagen de entrada
 * y si la imagen esta proyectada la dimension del raster de salida.
 */
class RasterOutputSizePart : public ProcessAtributeProvider, public Part, public ObserverInterface {
public:
   /** Ctor */
   RasterOutputSizePart(
         ReprojectionProcess::ReprojectionParameters *pReprojectionParameters,
         World* pInputWorld, World* pOutputWorld, bool Enable = true,
         bool Modified = false);
   /** Dtor */
   virtual ~RasterOutputSizePart();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Devuelve si la parte tiene datos validos */
    virtual bool HasValidData();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valor's originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();
   /** Retorna el icono de la herramienta */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;

   /** Actualiza los datos en part cuando se modifica subset a exportar */
   virtual void InputRasterExtentUpdate();

   /** Verifica y corrige si es necesario el tamanio de salida (pixel-linea) */
   void SanitizeOutputSize();

   /** Configura el mundo de salida */
   void SetupOutputWorld();

   /** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
   virtual void Update(NotificationInterface* pNotification);

private:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(ParametersChangedEvent);
   FRIEND_EVENT_OBJECT(RadioButtonEvent);
   FRIEND_EVENT_OBJECT(AspectCheckEvent);
   EVENT_OBJECT_PROPERTY(ParametersChangedEvent);
   /** Eventos para radio buttons*/
   RadioButtonEvent* pRadioButtonEvent_;
   /** Evento de seleccion de check para mantener aspecto **/
   AspectCheckEvent* pAspectCheckEvent_;
   /** Handler para manejar el evento de cambio de algun parametro en el Part */
   void OnParametersChangedEventHandler(wxCommandEvent &Event);
   /** Handler para el evento de activacion/desactivacion de un radio button en el Part*/
   void OnRadioButtonEventHandler(wxCommandEvent &Event);
   /** Handler para el evento del check de conservar aspecto*/
   void OnAspectCheckEventHandler(wxCommandEvent &Event);
   /** Metodo auxiliar para conectar eventos en los componentes del part **/
   void ConnectEvents();
   /** Metodo auxiliar que restaura los valores**/
   void RestoreValues();
   /** Metodo auxiliar que obtiene de los textctrls los valores **/
   void GetTextValuesFromGui(int& Pixels, int& Lines, double& PixelSizeX, double& PixelSizeY,
                             double& TimesX, double& TimesY);
   /** Actualiza los TextCtrol del part en funcion de los valores pasado por parametro**/
   void UpdateGuiControls(int Pixels, int Lines, double PixelSizeX, double PixelSizeY,
                          double TimesX, double TimesY, bool Rollback = false);
   /** Metodo auxiliar que dado VecesX y Veces Y calcula los demas parametros **/
   void CalculateValuesByTimesFactor(double TimesX, double TimesY, int& Pixels, int& Lines,
                                     double& PixelSizeX, double& PixelSizeY);
   /** Metodo auxiliar que dado Pixels y Lines calcula los demas parametros **/
    void CalculateValuesByPixelLines(int Pixels, int Lines, double& PixelSizeX,
                                    double& PixelSizeY, double& TimesX, double& TimesY);
    /** */
    void CalculateValuesByPixelSize(double PixelSizeX, double PixelSizeY, int& Pixels,
                                   int& Lines, double& TimesX, double &TimesY);
   /** Metodo auxiliar que a partir del Sistema de Referencia de
     * llegada configura las unidades de X-Y **/
   void ConfigurePixelSizeUnits();
   Dimension CalculatePixelSizeOut(RasterSpatialModel* pRasterModel);
   /** Metodo auxiliar que calcula el tamanio de pixel de entrada **/
   void CalculateCenterDimension();
   /** Metodo auxiliar que crea un modelo raster a partir del subset, pixel y lineas **/
   RasterSpatialModel* CreateRasterSpatialModel(const Subset& SubsetOut, int Pixels,
                                                int Lines) const;

   /** Metodo para crear un CoordinatesTransform a partir de
    * un ReprojectionProcess::ReprojectionParameters **/
   CoordinatesTransformation*
   getCoordinatesTransfomation(const ReprojectionProcess::ReprojectionParameters
                               *pReproyectionarameters);

   // TODO(Gabriel - TCK #2324): Solucion temporal para comunicar los parametros
   // de la reproyeccion a todo el proceso. Cambiar cuando se mejore
   ReprojectionProcess::ReprojectionParameters *pReprojectionParameters_;
   World* pInputWorld_;
   Subset subsetIn_, subsetOut_;
   /** Sistema de referencia que se uso para hacer calculos actuales */
   std::string currentSrs_;
   /** Mundo de salida que actualizo con subset seleccionado por usuario */
   World* pOutputWorld_;
   bool preserveAspect_, lastPreserveAspect_;
   int pixels_, lines_;
   double pixelSizeOutX_, pixelSizeOutY_, timesX_, timesY_;
   /** Valores anteriores para el recalculo de los nuevos parametros*/
   int pixelsLastValue_, linesLastValue_;
   double pixelSizeOutXLastValue_, pixelSizeOutYLastValue_, timesXLastValue_, timesYLastValue_;
   Dimension centerPixelSizeIn_, centerPixelSizeOut_;
   /** Punteros a los TextCTrl del part */
   wxTextCtrl *pPixelsText_, *pLinesText_, *pPixelSizeXText_, *pPixelSizeYText_,
         *pTimesXText_, *pTimesYText_;
   /** Estado de los Radio Buttons del Part*/
   bool pixelLineActive_, pixelSizeActive_, timesActive_;
   bool lastPixelLineActive_, lastPixelSizeActive_, lastTimesActive_;
   bool validData_;
   /** Mostrar part de resize */
   bool ShowResizePart;
   // Inidica si el usuario cambio manualmente los valores.
   bool pixelSizeChanged_;
};
}

#endif /* RASTEROUTPUTSIZEPART_H_ */
