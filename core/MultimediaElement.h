/** \file MultimediaElement.h
 *
 * @file PatPropertiesHtmlViewerPart.cpp
 *  Software de Procesamiento de Imagenes Satelitales de CONAE.
 *  Copyright 2014-2007 CONAE.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


#ifndef MULTIMEDIAELEMENT_H_
#define MULTIMEDIAELEMENT_H_

//Includes standard

//Includes Suri
#include "suri/Element.h"
#include "suri/Part.h"

//Includes Wx

//Includes App

//Defines

//Forwards
namespace suri {
//! Clase que permite generar elementos con contenido multimedia (Fotografia, PDF, audio, video)
/**
 * Permite:
 * 	- generar elementos Fotografia a partir de un archivo de imagen.
 * 	- obtner un string con el codigo html del html que se despliega en el
 * visualizador del contexto.
 * 	- obtener la ventana de propiedades especificas del elemento multimedia.
 */
class MultimediaElement: public suri::Element {
	//! Ctor. de Copia.
	MultimediaElement(const MultimediaElement &MultimediaElement);

public:
	//! Constructor
	MultimediaElement();
	//!Destructor
	virtual ~MultimediaElement();
	//! retorna un string para el icono
	virtual wxString GetIcon() const;
	   /** retorna un string para el icono */
	virtual wxString DoGetIcon() const;
	//! Devuelve el contenido del nodo titulo
   std::string GetTitle() const;
	//! retorna los detalles del elemento
	wxString GetDetails() const {
		return wxT("");
	}
   std::string GetPreviewImage() const;
	//! Devuelve el contenido del nodo texto
	std::string GetText() const;
	//! Devuelve el Tipo de Elemento
	virtual wxString GetTypeAsString();
	//! Devuelve un string con el html a insertar en el contexto
	bool GetHtmlInfo(wxString &Html) const;
	//! Establece el contenido del nodo titulo
	void SetTitle(const wxString &Title);
	//! Establece el contenido del nodo texto
	void SetText(const wxString &Text);
   void SetPreviewImage(const std::string& Url);
	//! Muestra la herramienta de propiedades
	virtual void ShowPropertyTool() {}
	//! Funcion estatica para crear un MultimediaElement
	static MultimediaElement* Create(const std::string &FileName,
			const std::string &Title = wxT(""),
			const std::string &Text = wxT(""));
   /** Devuelve vector con Parts especificos del elemento. */
   virtual std::vector<suri::Part*> DoGetParts(
         DataViewManager* pDataViewManager = NULL, LayerInterface *pLayer = NULL,
         DatasourceInterface *pDatasource = NULL);

protected:
private:
	virtual MultimediaElement* Initialize(const std::string &FileName);
};
} /** namespace suri **/
#endif /*MULTIMEDIAELEMENT_H_*/
