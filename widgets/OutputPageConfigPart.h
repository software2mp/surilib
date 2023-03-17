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

#ifndef OUTPUTPAGECONFIGPART_H_
#define OUTPUTPAGECONFIGPART_H_

// Includes Suri
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "ReprojectionProcess.h"

// Includes Wx
#include "wx/spinctrl.h"
#include "wx/textctrl.h"

namespace suri {

class Dimension;

namespace ui {

/** Declara clase OutputPageConfigPartEvent */
DECLARE_EVENT_OBJECT(OutputPageConfigPartEvent);

/**
 * Control grafico para configurar la pagina de salida
 */
class OutputPageConfigPart : public ProcessAtributeProvider, public Part {
   FRIEND_EVENT_OBJECT(OutputPageConfigPartEvent);
public:
   /** Constructor */
   OutputPageConfigPart(ReprojectionProcess::ReprojectionParameters *pReprojectionParameters,
                        World* pInputWorld, World* pOutputWorld);
   /** Destructor */
   virtual ~OutputPageConfigPart();
   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();

private:
   /** Objeto que conecta los eventos */
   EVENT_OBJECT_PROPERTY(OutputPageConfigPartEvent);
   /** Actualiza el estado de la parte */
   virtual void Update() {};
   /** Handler para manejar el evento de cambio de estado del checkbox de titulo */
   void OnCheckboxAddTitleSubtititleClicked(wxCommandEvent &Event);
   /** Handler para manejar el evento de cambio de estado del checkbox de margenes */
   void OnCheckboxAddMarginsClicked(wxCommandEvent &Event);
   /** Retorna las dimensiones en centimetros basandose en las dimensiones de salida en pixeles */
   void CalculateWidthHeightDimensions(double &Width, double &Height);
   /** Metodo auxiliar que calcula la dimension en pixel linea */
   void CalculatePixelLineDimensions(double &Pixels, double &Lines, double Width, double Height);
   /** Handler para manejar el evento de cambio de algun parametro en los TextCtrl del Part */
   void OnParametersChangedEventHandler(wxCommandEvent &Event);
   /** Metodo auxiliar para conectar eventos en los componentes del part */
   void ConnectEvents();
   /** Actualiza los valores de los TextCtrl de dimension */
   void SetDimensions(double Width, double Height);
   /** Metodo auxiliar que setea el estado de los TextCtrl de los margenes */
   void SetMarginControlsStatus();
   /** Recalcula la dimension del output */
   void OnSpinDpiChange(wxCommandEvent &Event);
   /** Metodo axuliar que calcula el tamanio en pixeles de los margenes */
   void SetMarginPixelSize();
   /** Metodo auxiliar que calcula el tamanio de pixel de entrada */
   Dimension CalculateCenterDimension();
   wxSpinCtrl* pDpiSpin_;
   wxTextCtrl *pTitleText_, *pSubtitleText_, *pWidthText_, *pHeightText_, *pLeftText_,
              *pBottomText_, *pRightText_, *pTopText_;
   wxCheckBox* pTitleCheck_, *pMarginsCheck_;
   double pixels_, lines_;
   int dpi_;
   ReprojectionProcess::ReprojectionParameters *pReprojectionParameters_;
   /** Mundo de entrada */
   World* pInputWorld_;
   /** Mundo de salida */
   World* pOutputWorld_;
};

} /** namespace ui */
} /** namespace suri */

#endif /* OUTPUTPAGECONFIGPART_H_ */
