/*! \file VectorTablesPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "VectorTablesPart.h"

// Includes standard
// Includes Suri
#include "suri/messages.h"
#include "resources.h"
#include "logmacros.h"
#include "VectorDatasource.h"
#include "suri/VectorEditor.h"
#include "LayerTablePart.h"
#include "HotlinkFieldEditorPart.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
#include "wx/choicebk.h"

// Defines
/**
 * No se usa vector en memoria porque para remplazar el que esta en disco no debe
 * estar abierto. El renderizador siempre tiene un Vector abierto.
 */
#undef __USE_MEMORY_VECTOR__
/** utilizado para decidir si se crea un choicebook o no. */
#define __NO_CHOICEBOOK__

// forward
/** Inicializa recursos de la ventana. */
void InitVectorTableXmlResource();

/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(VectorTablesPartEvent, VectorTablesPart)
      IMPLEMENT_EVENT_CALLBACK(OnUiUpdate, OnUiUpdate(event), wxUpdateUIEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/**
 * Constructor
 * @param[in] pVectorDatasource elemento vectorial que se usa para editar el vector
 * @param[in] HotlinkPartType flag que indica si la tabla desplegada debe
 * ser de hotlink
 * @return instancia de la clase VectorTablesPart
 */
VectorTablesPart::VectorTablesPart(VectorDatasource* pVectorDatasource,
                                   bool HotlinkPartType) :
      pVectorDatasource_(pVectorDatasource), pVectorElement_(NULL), pVectorEditor_(NULL),
#ifdef __USE__MEMORY__VECTOR__
      pMemoryVector_(NULL),
#endif
      pChoicebook_(NULL),
      hotlinkPartType_(HotlinkPartType),
      NEW_EVENT_OBJECT(VectorTablesPartEvent)
{
   // Creo ventana
   if (hotlinkPartType_) {
      windowTitle_ = wxT(label_HOTLINK_EDITOR);
   } else {
      windowTitle_ = wxT(label_DATA_TABLE);
   }
}


/**
 * @param[in] pVectorElement elemento vectorial que se usa para editar el vector
 * @param[in] HotlinkPartType flag que indica si la tabla desplegada debe
 * ser de hotlink
 * @return instancia de la clase VectorTablesPart
 */
VectorTablesPart::VectorTablesPart(VectorElement* pVectorElement, bool HotlinkPartType) :
      pVectorDatasource_(NULL), pVectorElement_(pVectorElement), pVectorEditor_(NULL),
#ifdef __USE__MEMORY__VECTOR__
      pMemoryVector_(NULL),
#endif
      pChoicebook_(NULL),
      hotlinkPartType_(HotlinkPartType),
      NEW_EVENT_OBJECT(VectorTablesPartEvent) {
   // Creo ventana
   if (hotlinkPartType_) {
      windowTitle_ = wxT(label_HOTLINK_EDITOR);
   } else {
      windowTitle_ = wxT(label_DATA_TABLE);
   }
}

/** Dtor */
VectorTablesPart::~VectorTablesPart() {
#ifdef __USE__MEMORY__VECTOR__
   suri::Vector::Close(pMemoryVector_);
#endif
   delete pVectorEditor_;
   // Se elimina a mano porque el constructor no se llama correctamente
   // TODO: No eliminar a mano
   std::vector<Part*>::iterator it = layerTableParts_.begin();
   for (; it != layerTableParts_.end(); it++)
      delete *it;
   layerTableParts_.clear();
}

/** Crea la ventana de la parte */
/**
 * Crea VectorTablePart o HotLinkFieldEditorPart deacuerdo a flag recibido
 * en constructor y conecta los eventos
 * @return informa si se pudo crear la ventana
 */
bool VectorTablesPart::CreateToolWindow() {
   if (!pVectorDatasource_) {
      return false;
   }
#ifndef __NO_CHOICEBOOK__
   pToolWindow_ = wxXmlResource::Get()->
   LoadPanel(pParentWindow_, wxT("ID_VECTOR_TABLE_PART") );
   wxChoicebook *pchoicebook=
   XRCCTRL(*pToolWindow_, wxT("ID_CHOICEBOOK"), wxChoicebook);

   if ( !pchoicebook ) {
      return false;
   }
#endif

   // Creo vector editor y abro el vector
   pVectorEditor_ = new VectorEditor();

#ifdef __USE__MEMORY__VECTOR__
   suri::Vector *pin = suri::Vector::Open(pVectorElement_->GetUrl().c_str() );
   std::string vectormemoryurl=std::string(wxT("shpmemory:") )+
   pVectorElement_->GetUrl().c_str();
   pMemoryVector_ = suri::Vector::Open(vectormemoryurl.c_str(),
         suri::Vector::ReadWrite);
   pin->Copy(pMemoryVector_);

   pVectorEditor_->OpenVector(vectormemoryurl.c_str() );

   suri::Vector::Close(pin);
#else
   pVectorEditor_->OpenVector(pVectorDatasource_->GetUrl().c_str());
   std::string vectormemoryurl = pVectorDatasource_->GetUrl().c_str();
#endif

   // verifico que al menos exista el ReadOnlyVector
   int layercount = 0;
   if (pVectorEditor_->GetReadOnlyVector()) {
      layercount = pVectorEditor_->GetReadOnlyVector()->GetLayerCount();
   } else {
      REPORT_DEBUG("D: No se pudo abrir el vector");
      delete pVectorEditor_;
      pVectorEditor_ = NULL;
   }

#ifdef __NO_CHOICEBOOK__
   layercount = layercount > 1 ? 1 : layercount;
#endif

   for (int i = 0; i < layercount; i++) {
      // reindexacion de i para que tome la capa activa en caso de pasar por cero y la cero para
      // cuando pase por la activa (enroque de capas), compatibilidad con iteracion por capas.
      // TODO(Gabriel - TCK #854): Ver que ahora muestra datos en la tabla aunque no tenga.
      int layer = i;
      if (layer == 0)
         layer = pVectorDatasource_->GetActiveLayer();
      else if (layer == pVectorDatasource_->GetActiveLayer())
         layer = 0;
      Part* ppart = NULL;
      std::string layername = "";
      // TODO: Eliminar casteo a tipo de part contenido
      if (hotlinkPartType_) {
         HotlinkFieldEditorPart* photlinkpart = new HotlinkFieldEditorPart(
               layer, pVectorDatasource_);
         layername = photlinkpart->GetName();
         ppart = photlinkpart;
      } else {
         LayerTablePart* playerpart = new LayerTablePart(layer, vectormemoryurl, EDIT_ALL);
         layername = playerpart->GetName();
         ppart = playerpart;
      }
#ifndef __NO_CHOICEBOOK__
      ppart->CreatePartTool(pchoicebook);
      pchoicebook->AddPage(ppart->GetWindow(), layername.c_str() );
#else
      ppart->CreatePartTool(pParentWindow_);
      pToolWindow_ = ppart->GetWindow();
#endif
      layerTableParts_.push_back(ppart);
   }

   if (pToolWindow_ != NULL) {
      pToolWindow_->Connect(wxEVT_UPDATE_UI,
                         wxUpdateUIEventHandler(VectorTablesPartEvent::OnUiUpdate),
                         NULL, pEventHandler_);
   }
   // cierro el vector
   if (pVectorEditor_) {
      pVectorEditor_->CloseVector();
   }

   return true;
}

/**
 * Indica si el Part tiene cambios para salvar.
 * @return true si alguna de las tablas se modifico.
 * @return false si no se modifico alguna de las tablas.
 */
bool VectorTablesPart::HasChanged() {
   modified_ = false;
   for (int i = 0; i < (int) layerTableParts_.size(); i++) {
      if (layerTableParts_[i]->HasChanged()) {
         modified_ = true;
      }
   }
   return modified_;
}

/**
 * Aplica cambios echos en las tablas al vector en disco
 * @return informa si tuvo exito
 */
bool VectorTablesPart::CommitChanges() {
   if (!HasValidData()) {
      return false;
   }

   bool returnvalue = true;
   for (int i = 0; i < (int) layerTableParts_.size() && returnvalue; i++) {
      if (!layerTableParts_[i]->CommitChanges()) {
         returnvalue = false;
      }
   }

   // Renderizo por si se eliminaron features de las capas.
   Element* pelement =
         pVectorDatasource_ ? pVectorDatasource_->GetElement() : pVectorElement_;
   pelement->SetChanged();
   pelement->SendViewerUpdate();

#ifdef __USE__MEMORY__VECTOR__
   returnvalue &= pMemoryVector_->Save(pVectorElement_->GetUrl().c_str(), "ESRI Shapefile");

   // std::string vectormemoryurl=std::string("c:/testA/vect2.shp");
   // suri::Vector *pout =
   // suri::Vector::Open(pVectorElement_->GetUrl().c_str(),suri::Vector::ReadWrite);
   /*suri::Vector *pout = suri::Vector::Open(pVectorElement_->GetUrl().c_str(),suri::Vector::ReadWrite);
    pMemoryVector_->Copy(pout);
    suri::Vector::Close(pout);*/
#endif

   return returnvalue;
}

/**
 * Elimina los cambios echos a las tablas
 * @return informa si todas las tablas tuvieron exito
 * \todo eliminar casteo a tipo de part contenido
 */
bool VectorTablesPart::RollbackChanges() {
#ifndef __NO_CHOICEBOOK__
   wxChoicebook *pchoicebook=
   XRCCTRL(*pToolWindow_, wxT("ID_CHOICEBOOK"), wxChoicebook);
#endif
   // size_t selected = pchoicebook->GetSelection();
   bool returnvalue = true;
   for (int i = 0; i < (int) layerTableParts_.size() && returnvalue; i++) {
      if (!layerTableParts_[i]->RollbackChanges()) {
         returnvalue = false;
      }

#ifndef __NO_CHOICEBOOK__
      // TODO: Eliminar casteo a tipo de part contenido
      if ( hotlinkPartType_ )
      {
         HotlinkFieldEditorPart* photlinkpart =
         dynamic_cast<HotlinkFieldEditorPart*>(layerTableParts_[i]);

         if ( photlinkpart ) {
            pchoicebook->SetPageText(i, photlinkpart->GetName() );
         }
      } else
      {
         LayerTablePart* playerpart =
         dynamic_cast<LayerTablePart*>(layerTableParts_[i]);

         if ( playerpart ) {
            pchoicebook->SetPageText(i, playerpart->GetName() );
         }
      }
   }
   if ( layerTableParts_.size()==1 ) {
      pchoicebook->AddPage(layerTableParts_[0]->GetWindow(), "temp");
   }
   pchoicebook->ChangeSelection(1);
   pchoicebook->ChangeSelection(0);
   if ( layerTableParts_.size()==1 ) {
      pchoicebook->RemovePage(1);
   }
#else
   }
#endif

   return returnvalue;
}

/**
 * Restaura los datos originales en todas las tablas
 * \todo eliminar casteo a tipo de part contenido
 */
void VectorTablesPart::SetInitialValues() {
#ifndef __NO_CHOICEBOOK__
   wxChoicebook *pchoicebook=
   XRCCTRL(*pToolWindow_, wxT("ID_CHOICEBOOK"), wxChoicebook);
#endif
   // size_t selected = pchoicebook->GetSelection();
   for (int i = 0; i < (int) layerTableParts_.size(); i++) {
      layerTableParts_[i]->SetInitialValues();
#ifndef __NO_CHOICEBOOK__
      // TODO: Eliminar casteo a tipo de part contenido
      if ( hotlinkPartType_ )
      {
         HotlinkFieldEditorPart* photlinkpart =
         dynamic_cast<HotlinkFieldEditorPart*>(layerTableParts_[i]);
         if ( photlinkpart ) {
            pchoicebook->SetPageText(i, photlinkpart->GetName() );
         }
      } else
      {
         LayerTablePart* playerpart =
         dynamic_cast<LayerTablePart*>(layerTableParts_[i]);
         if ( playerpart ) {
            pchoicebook->SetPageText(i, playerpart->GetName() );
         }
      }
#endif
   }
}

/**
 * Devuelve el icono de la parte
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho de icono
 * @param[in] Y alto de icono
 */
void VectorTablesPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   if (hotlinkPartType_) {
      GET_BITMAP_RESOURCE(icon_HOTLINK_TABLE_EDITOR, ToolBitmap);
   } else {
      GET_BITMAP_RESOURCE(icon_VECTOR_TABLE_EDITOR, ToolBitmap);
   }
}

/**
 * Informa todas las tablas tienen datos validos
 * @return Si alguna de las tablas tiene un dato invalido falla
 */
bool VectorTablesPart::HasValidData() {
   bool returnvalue = true;
   for (int i = 0; i < (int) layerTableParts_.size(); i++) {
      if (!layerTableParts_[i]->HasValidData()) {
         returnvalue = false;
      }
   }
   return returnvalue;
}

// ------------------------------------------------------------------------------
/**
 * Este metodo responde a los eventos UIUpdate(que se disparan cuando no hay
 * eventos de usuario en la cola de eventos de wx).
 * Setea flag de Part modificado verificando el estado de las textctrl.
 * \todo No deberia usarse un UiUpdate para setear datos, el UiUpdate solo debe
 *  usarse para actualizar la interfaz de usuario!
 * @param[in]	Event: evento
 */
void VectorTablesPart::OnUiUpdate(wxUpdateUIEvent &Event) {
   Event.Skip();
}
}

