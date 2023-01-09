/*! \file OutputPageConfigPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
