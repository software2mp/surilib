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

// Includes standard
#include <vector>
#include <string>

// Includes Suri
#include "QueryButton.h"
#include "suri/VectorElement.h"
#include "suri/VectorStyle.h"
#include "suri/ViewerWidget.h"
#include "suri/Extent.h"
#include "suri/PartContainerWidget.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/HtmlViewerPart.h"
#include "suri/Configuration.h"

// Includes Wx
// Defines

// Includes ogr
#include "ogr_geometry.h"
#include "ogr_feature.h"
#include "ogrsf_frmts.h"
#include "wx/mimetype.h"

/** Cantidad de puntos que se usan para calcular extent del vector */
#define _VECTORQUERY_EXTENT_POINTS_ 100

/** Se define una extension por defecto con la cual se abren los link que no tengan extension */
#define DEFAULT_HOTLINK_MIMETYPE "text/plain"

/** namespace suri */
namespace suri {
/** Struct con informacion de hotlinks que deben abrirse */
struct HotlinkData {
   std::string url_; /*! url del hotlink */
   VectorElement* vectorElement_; /*! vector para editar */
   int layer_; /*! numero de capa */
   int featureId_; /*! numero de feature */
};

/** Constructor **/
QueryButton::QueryButton(wxWindow *pToolbar, ViewerWidget *pViewer) :
      ToggleButton(pToolbar, wxT(tool_SIMUR_QUERY), wxT(tooltip_QUERY), BUTTON_LEFT_UP),
      pViewer_(pViewer), pQuery_(NULL)  {
}

/**
 * Cambia cursor a flecha con signo de interrogacion
 * @param[in] State estado del boton
 */
void QueryButton::DoSetCursor(bool State) {
   if (State) {
      pViewerWindow_->SetCursor(wxCursor(wxCURSOR_QUESTION_ARROW));
   } else {
      pViewerWindow_->SetCursor(wxCursor(wxCURSOR_DEFAULT));
   }
}

/**
 * Busca hotlinks y los despliega. Usa metodos virtuales para modificar comportamiento
 * segun tipo de contexto.
 * @param[in] Event evento levantar boton izq
 */
void QueryButton::DoOnLeftUp(wxMouseEvent &Event) {
   int x = Event.GetX();
   int y = Event.GetY();
   std::vector<HotlinkData> selectedhotlinks;
   Subset querybox;
   suri::LayerList::ElementListType::iterator it = pList_->GetElementIterator();
   while (it != pList_->GetElementIterator(false)) {
      suri::VectorElement* pvectorelement = dynamic_cast<suri::VectorElement*>(*it);
      // Si el elemento esta activo, es de tipo vector y tiene URL continuo
      if (pvectorelement && (*it)->IsActive()) {
         Vector* pvector = Vector::Open((*it)->GetUrl().c_str());
         if (pvector) {
            // Obtiene el tamanio del vector actual
            VectorElement* pvelem = dynamic_cast<suri::VectorElement*>((*it));
            VectorStyle* pvstyle = VectorStyle::Create(pvelem->GetStyle(pvelem->GetActiveLayer()));
            int vectorsize = pvstyle->GetSymbol()->size_;
            VectorStyle::Destroy(pvstyle);

            // Obtengo el querybox
            GetQueryBox(x, y, vectorsize, querybox);

            int currentlayer = pvectorelement->GetActiveLayer();
            wxString hotlinkfieldname = pvectorelement->GetHotLinkFieldName(currentlayer);
            OGRLayer *player = pvector->GetLayer(currentlayer);
            if (player != NULL && !(hotlinkfieldname.IsEmpty())
                  && (player->GetLayerDefn()->GetFieldIndex(hotlinkfieldname.c_str()) >= 0)) {
               // Transformacion de coordenadas
               std::string wkt1 = pViewer_->GetWorld()->GetSpatialReference();
               std::string wkt2 = pvectorelement->GetSpatialReference(currentlayer).c_str();
               ParameterCollection params;
               params.AddValue<std::string>(TransformationFactory::kParamWktIn, wkt1);
               params.AddValue<std::string>(
                     TransformationFactory::kParamWktOut, wkt2);
               TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
               CoordinatesTransformation *pct = pfactory->Create(
                     TransformationFactory::kExact, params);
               TransformationFactoryBuilder::Release(pfactory);
               if (!pct) {
                  REPORT_DEBUG("D: Error al convertir a sistema de coordenadas de vector");
                  return;
               }

               pct->Transform(querybox, _VECTORQUERY_EXTENT_POINTS_);
               delete pct;

               // Setea el filtro espacial
               Extent query(querybox);
               player->SetSpatialFilterRect(query.min_.x_, query.min_.y_, query.max_.x_,
                                            query.max_.y_);

               // Resetea el reading de la capa para que apunte al primer feature
               player->ResetReading();
               OGRFeature *pfeature = player->GetNextFeature();  // Apunta al primer feature

               while (pfeature != NULL) {
                  /**
                   * Se verifica que cada feature tenga geometria asociada,
                   * para evitar que se abran los hotlinks asociados a registros
                   * en la tabla que no tienen geometria asociada.
                   */
                  OGRGeometry *pgeometry = pfeature->GetGeometryRef();
                  if (pgeometry && pgeometry->IsValid()) {
                     // Agrego el hotlink a lista para desplegar
                     HotlinkData hotlinkdata;
#ifdef __WINDOWS__
                     hotlinkdata.url_ = wxString(
                           wxConvUTF8.cMB2WC(
                                 pfeature->GetFieldAsString(
                                       pfeature->GetFieldIndex(
                                             hotlinkfieldname.c_str()))),
                           wxCSConv(suri::XmlElement::xmlEncoding_.c_str())).c_str();
#else
                     hotlinkdata.url_ = pfeature->GetFieldAsString(
                           pfeature->GetFieldIndex(hotlinkfieldname.c_str()));
#endif  // __WINDOWS__
                     hotlinkdata.vectorElement_ = pvectorelement;
                     hotlinkdata.layer_ = currentlayer;
                     hotlinkdata.featureId_ = pfeature->GetFID();
                     if (!hotlinkdata.url_.empty()) {
                        selectedhotlinks.push_back(hotlinkdata);
                     }
                  }
                  // Analizo el siguiente feature
                  OGRFeature::DestroyFeature(pfeature);
                  pfeature = player->GetNextFeature();
               }
               // Elimina el filtro espacial
               player->SetSpatialFilter(NULL);
            }
            // Cierro el pvector
            Vector::Close(pvector);
         } else {
            // Si no pudo crear el vector informo al usuario
            SHOW_ERROR(message_OPEN_VECTOR_ERROR);
         }
      }
      it++;
   }
   DisplayHotlink(selectedhotlinks);
}

/**
 * Abre el hotlinks usando el sistema operativo.
 * @param[in] Hotlinks vector de HotlinkData de archivos a desplegar
 */
void QueryButton::DisplayHotlink(const std::vector<HotlinkData> &Hotlinks) {
   std::vector<HotlinkData>::const_iterator it = Hotlinks.begin();

   // Recorro todos los vectores en la lista.
   for (; it != Hotlinks.end(); it++) {
      // Verifico que existe el archivo
      wxFileName fnhotlink(it->url_.c_str());
      if (!fnhotlink.FileExists()) {
         SHOW_ERROR(message_NO_FILE_s, it->url_.c_str());
         continue;
      }

      // Verifico que tenga extension
      wxString hotlinkextention = fnhotlink.GetExt();
      /**
       * TODO(Gabriel - TCK #6097): Solucion temporal:
       * En Simur la consulta de hipervinculo
       * despliega un visualizador html con el hipervinculo cargado
       * Este comportamiento es diferente de la consulta de pixel
       * que se realiza en SoPI. Por eso es necesario distinguir
       * este comportamiento.
       * Dado que el codigo de los visualizadores es codigo compartido
       * es lo mismo poner un if aca que en donde se instancia el boton
       * a utilizar para la consulta y poner un if para instanciar
       * una nueva clase que se encargue de desplegar la nueva consulta de
       * hipervinculo (ej. HyperlinkQueryButton)
       * Se debe buscar una mejor solucion a la planteada arriba
       */
      if (Configuration::GetParameter("app_short_name", "") != wxT("SoPI")
            && hotlinkextention.Lower().Find("html") != wxNOT_FOUND) {
         //Crea el htmlviewer con la url obtenida
         HtmlViewerPart *phtmlviewer = new HtmlViewerPart(700, 500);
         PartContainerWidget *ppartcontainer = new PartContainerWidget(
               phtmlviewer, _(caption_HOTLINK_PROPERTIES),
               SUR_BTN_OK,
               SUR_BTN_OK);
         if (phtmlviewer->LoadUrl(it->url_.c_str()))
            ppartcontainer->ShowModal();
         continue;
      }
      /**
       * Creo mime type.
       * La documentacion de wx recomiendan usar la instancia global de
       * wxMimeTypesManager.
       */
      wxFileType *photlinkmimetype;
      if (!hotlinkextention.IsEmpty()) {
         photlinkmimetype = wxTheMimeTypesManager->GetFileTypeFromExtension(
               hotlinkextention);
      } else { // Si el archivo no tiene extension uso el mime por defecto
         photlinkmimetype = wxTheMimeTypesManager->GetFileTypeFromMimeType(
               wxString(DEFAULT_HOTLINK_MIMETYPE));
      }

      if (!photlinkmimetype) {
         SHOW_ERROR(message_UNKNOW_EXTENTION_s, it->url_.c_str());
         continue;
      }

      wxString filepath = fnhotlink.GetFullPath();
#ifdef __WXGTK__
      // Ejecuto app
      /* Nota: hay un posible bug en el metodo GetOpenCommand cuando el path del
       * archivo que recibe tiene espacios ("C:\mi imagen.jpg"). Para corregirlo
       * se agregan comillas para poder escapar los espacios.
       * No se agregan comillas en windows porque genera comandos que no son
       * ejecutables (y no esta el problema de los nombres con espacio en blanco)
       * Ver http://trac.wxwidgets.org/ticket/4607
       * */
      if (filepath.Find(wxT(" ")) != wxNOT_FOUND) {
         filepath = wxT("\"") + filepath + wxT("\"");
      }
#endif
      wxString commandstr = photlinkmimetype->GetOpenCommand(filepath);
#ifdef __WXGTK__
      if (commandstr.StartsWith(wxT("\"\"")) || commandstr.EndsWith(wxT("\"\""))) {
         commandstr = photlinkmimetype->GetOpenCommand(fnhotlink.GetFullPath());
      }
#endif
      if (!commandstr.IsEmpty()) {
         wxExecute(commandstr);
      } else {
         SHOW_ERROR(message_NO_APP_ERROR_s, it->url_.c_str());
      }

      delete photlinkmimetype;
   }
}
}
