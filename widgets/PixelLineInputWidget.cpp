/*! \file PixelLineInputWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/PixelLineInputWidget.h"

// Includes Suri
#include "suri/ViewerWidget.h"
#include "logmacros.h"
#include "resources.h"
#include "suri/RasterSpatialModel.h"
#include "suri/RasterElement.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/DataViewManager.h"
// Includes Wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
// Includes standar
#include <list>
#include <sstream>
// forward
void InitPixelLineInputXmlResource();

/** namespace suri */
namespace suri {
/** Filtro valores aceptados por la ventana de ingreso de coordenadas */
wxString PLNumberFilter[] = { wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"),
                              wxT("6"), wxT("7"), wxT("8"), wxT("9"), wxT("0") }; /*! valores */

#ifdef __UNUSED_CODE__
/*! permitidos para el ingreso */
/*! de coordenadas PL */
/**
 * Constructor
 * @param[in] pWorld puntero a la ventana del mundo
 */
PixelLineInputWidget::PixelLineInputWidget(suri::World *pWorld) :
      NavigationInputWidget() {
   windowTitle_ = GetWindowTitle();
   pRasterElement_ = NULL;
   pDataViewManager_ = NULL;
}
#endif

/** ctor **/
PixelLineInputWidget::PixelLineInputWidget() :
      NavigationInputWidget() {
   windowTitle_ = GetWindowTitle();
   pDataViewManager_ = NULL;
}

/**
 * Destructor
 */
PixelLineInputWidget::~PixelLineInputWidget() {
}

/**
 * Carga herramieta que permite al usuario ingresar coordenadas en latitud
 * y longitud.
 * Hace connect de cerrar, click sobre iconos 'n/s' y 'e/o' y sobre eventos
 * lostfocus de los textCtrl para actualizar signo.
 * @return bool que indica si se pudo crear la ventana (true siempre)
 */
bool PixelLineInputWidget::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_PIXEL_LINE_INPUT"));

   // wxTextValidator degree
   // wxTextValidator degree
   wxTextValidator textvalidator = wxTextValidator(wxFILTER_INCLUDE_CHAR_LIST);
   wxArrayString degreearray = wxArrayString(10, PLNumberFilter);
   textvalidator.SetIncludes(degreearray);
   GET_CONTROL(*pToolWindow_, wxT("ID_LINE"), wxTextCtrl)->SetValidator(textvalidator);
   GET_CONTROL(*pToolWindow_, wxT("ID_LINE"), wxTextCtrl)->InitDialog();
   GET_CONTROL(*pToolWindow_, wxT("ID_PIXEL"), wxTextCtrl)->SetValidator(textvalidator);
   GET_CONTROL(*pToolWindow_, wxT("ID_PIXEL"), wxTextCtrl)->InitDialog();
   pParentWindow_->Fit();
   return true;
}

/**
 * Obtiene el titulo del navegador
 * @return string con el titulo de la ventana
 */
const wxString PixelLineInputWidget::GetWindowTitle() const {
   return _(caption_NAVIGATION_TOOL);
}

/**
 * retorna la coordenada que ingresaron e informa si es valida. Si la
 * coordenada es valida actualiza el signo del control llamado a
 * UpdateControlSign.
 * @param[out] PixelLineCoord coordenada que se ingreso en control
 * @return bool que indica si la coordenada en control es valida
 */
bool PixelLineInputWidget::GetCoordinate(Coordinates &PixelLineCoord) {
   if (pToolWindow_ == NULL)
      return false;
   bool returnvalue = true;
   // Para norte
   wxString line = USE_CONTROL(*pToolWindow_, wxT("ID_LINE"), wxTextCtrl,
         GetValue(), wxT(message_NOT_APPLICABLE) );
   wxString pixel = USE_CONTROL(*pToolWindow_, wxT("ID_PIXEL"), wxTextCtrl,
         GetValue(), wxT(message_NOT_APPLICABLE) );
   long lineaslong = 0;
   long pixelaslong = 0;
   if (!pixel.ToLong(&pixelaslong) || !line.ToLong(&lineaslong)) {
      SHOW_STATUS(wxT(message_TO_LONG_ERROR));
      return false;
   }
   if (returnvalue) {
      PixelLineCoord.x_ = pixelaslong;
      PixelLineCoord.y_ = lineaslong;
      TransformCoordinates(PixelLineCoord);
   }
   return returnvalue;
}

/**
 * Cambia la coordenada que se muestra por el valor indicado
 * @param[in] PixelLineCoord nueva coordenada en sistema de referencia del
 *             mundo
 * @return true si existe el elemento con la imagen activa y las coordenadas estan
 *       dentro de la imagen; false en otro caso.
 */
bool PixelLineInputWidget::SetCoordinate(Coordinates PixelLineCoord) {
   TransformCoordinates(PixelLineCoord, true);

   int line = SURI_ROUND(int, PixelLineCoord.y_);
   int pixel = SURI_ROUND(int, PixelLineCoord.x_);

   // Genero los strings
   std::stringstream linestream;
   std::stringstream pixelstream;
   linestream << line;
   pixelstream << pixel;

   // Los muestro por pantalla
   GET_CONTROL(*pToolWindow_, wxT("ID_LINE"), wxTextCtrl)
      ->SetValue(linestream.str().c_str());
   GET_CONTROL(*pToolWindow_, wxT("ID_PIXEL"), wxTextCtrl)
      ->SetValue(pixelstream.str().c_str());

   return true;
}

#ifdef __UNUSED_CODE__
/**
 * Asigna el elemento que representa a la imagen activa.
 * @param[in] pRasterElement elemento de la imagen activa
 */
void PixelLineInputWidget::SetCurrentRasterElement(RasterElement* pRasterElement) {
   pRasterElement_ = pRasterElement;
}
#endif
/**
 * Objetivo: Devuelve el icono de propiedades
 * Por ahora no tiene en cuenta el X e Y dado que devuelve el de 32x32 pixels
 * @param[out] ToolBitmap incono de la ventana
 * @param[in] X ancho del icono
 * @param[out] Y alto del icono
 */
void PixelLineInputWidget::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_NAVIGATION_WIDGET, ToolBitmap);
}

/**
 * Transforma las coordenadas recibidas usando el modelo raster del elemento.
 * 
 * @param[out] Coords Coordenadas a transformar.
 * @param[in] Inverse Indica si tiene que transformar de P-L a X-Y sino es X-Y a P-L.
 */
void PixelLineInputWidget::TransformCoordinates(Coordinates& Coords, bool Inverse) {
   RasterElement* praster = NULL;
   if (pDataViewManager_ != NULL) {
      ViewcontextInterface* pviewctx =
            pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();
      if (pviewctx) {
         TreeSelectionManagerInterface* pmanselection = pDataViewManager_->GetSelectionManager();
         if (pmanselection) {
            std::list<NodePath> nodepathlist = pmanselection->GetSelectedNodes();
            std::list<NodePath>::const_iterator it = nodepathlist.begin();
            while (!praster && it != nodepathlist.end()) {
               suri::NodePath path = *it;
               SuriObject::UuidType id = path.GetLastPathNode()->GetContent();
               LayerInterface* player = pviewctx->GetAssociatedLayer(id);
               if (player) {
                     Element* pelement = player->GetElement();
                     praster = dynamic_cast<RasterElement*>(pelement);
               }
               ++it;
            }
         }
      }
   }
#ifdef  __UNUSED_CODE__
     ViewcontextManagerInterface* pviewcttmgr = pDataViewManager_->GetViewcontextManager();
      if (pviewcttmgr != NULL) {
         ViewcontextInterface* pviewctt = pviewcttmgr->GetSelectedViewcontext();
         if (pviewctt != NULL) {
            LayerList* plist = pviewctt->GetLayerList();
            if (plist != NULL) {
               Element* pelement = plist->GetActiveElement(0, 6);
               praster = dynamic_cast<RasterElement*>(pelement);
            }            
         }
      }
#endif
   if (praster != NULL) {
      RasterSpatialModel* prastermodel = 
         RasterSpatialModel::Create(praster->GetRasterModel());
      if (prastermodel != NULL) {
         if (Inverse) {
            prastermodel->InverseTransform(Coords);
         }
         else
            prastermodel->Transform(Coords);
      }
      RasterSpatialModel::Destroy(prastermodel);
   }
}

}  // namespace suri
