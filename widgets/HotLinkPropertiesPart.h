//<COPYRIGHT> a reemplazar a futuro

#ifndef HOTLINKPROPERTIESPART_H_
#define HOTLINKPROPERTIESPART_H_

//Includes standard

//Includes Suri
#include "suri/Part.h"
#include "suri/Element.h"
#include "suri/DatasourceInterface.h"

//Includes Wx

//Includes App
#include "HotLinkVectorElement.h"


//Defines

//Forward
void InitHotLinkPropertiesXmlResource();

DECLARE_EVENT_OBJECT(HotLinkPropertiesPartEvent);

//! Pagina para visualizar/editar las propiedades de un HotLink
/**
 * Permite visualizar y editar las propiedades especificas de un hotlink.
 * Permite ingresar los datos(titulo, descripcion, imagen previa) para generar
 * el html que conforma el HotLink.
 */
class HotLinkPropertiesPart : public suri::Part {
public:

	FRIEND_EVENT_OBJECT(HotLinkPropertiesPartEvent);
   //! Ctor. de Copia.
   HotLinkPropertiesPart(const HotLinkPropertiesPart &HotLinkPropertiesPart);
   //! Ctor. a partir de una fuente de datos
   HotLinkPropertiesPart(suri::DatasourceInterface *pDatasource, suri::LayerInterface* pLayer);
   //! Ctor.
   HotLinkPropertiesPart(HotLinkVectorElement *pElement);
   //! Dtor.
   virtual ~HotLinkPropertiesPart();
   //! Se crea la ventana de herramienta en las clases derivadas
   virtual bool CreateToolWindow();
   //! Inicializa los controles
   virtual void SetInitialValues();
   //! Devuelve el estado del Part.
   virtual bool IsEnabled();
   //! Indica si el Part tiene cambios para salvar.
   virtual bool HasChanged();
   //! Salva los cambios realizados en el Part.
   virtual bool CommitChanges();
   //! Restaura los valores originales del Part.
   virtual bool RollbackChanges();
   //! Evento OnClick - boton para agregar imagen
   void OnAddPreviewCommandButtonClick(wxCommandEvent &Event);
   //! Evento UIUpdate
   virtual void OnUIUpdate(wxUpdateUIEvent &Event);
   //! Obtiene el icono del part
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
   //! Actualiza el estado de la parte
   virtual void Update() {
   }
protected:
   //! Obtiene los valores del nodo Caracteristicas
   bool GetFeaturesValues(wxString &Title, wxString &Description, wxString &ImgUrl,
                          wxString &HtmlImg);
   //! Setea los valores del nodo Caracteristicas
   void SetFeaturesValues(const wxString &Title, const wxString &Description,
                          const wxString &ImgUrl, const wxString &HtmlImg);
private:
   EVENT_OBJECT_PROPERTY(HotLinkPropertiesPartEvent);
   //! Crea html
   bool CreateHtml(const wxString &Title, const wxString &Desc, const wxString &UrlImg,
                   const wxString &HtmlPath = wxT(""));

   HotLinkVectorElement *pElement_;  ///<elemento vector.
   suri::DatasourceInterface *pDatasource_;
   suri::LayerInterface* pLayer_;
#ifdef __SAVE_URLHTML_CODE__
   wxString urlHtml_;  ///< contiene el url del html generado.
#endif
};

#endif /*HOTLINKPROPERTIESPART_H_*/
