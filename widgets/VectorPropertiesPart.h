/*! \file VectorPropertiesPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTORPROPERTIESPART_H_
#define VECTORPROPERTIESPART_H_

// Includes suri
#include "suri/VectorStyleManager.h"
#include "suri/VectorElement.h"
#include "suri/Vector.h"
#include "SimpleVectorStylePart.h"
#include "CategorizedVectorStylePart.h"
#include "suri/Part.h"

/** namespace suri */
namespace suri {

// Forwards
class VectorLayer;
class VectorPropertiesEventHandler;

/** Clase que centraliza la edicion de estilos (que forma parte de las
 * propiedades de una capa vectorial). **/
class VectorPropertiesPart: public suri::Part {
	/** Ctor. de Copia. */
	VectorPropertiesPart(const VectorPropertiesPart &VectorPropertiesPart);

public:
	/** ctor **/
   VectorPropertiesPart(DataViewManager* pDataViewManager, VectorLayer* pLayer);
	/** dtor */
	~VectorPropertiesPart();
	/** Se crea la ventana de herramienta en las clases derivadas */
	virtual bool CreateToolWindow();
	/** Inicializa los controles */
	virtual void SetInitialValues();
	/** Indica si el Part tiene cambios para salvar. */
	virtual bool HasChanged();
	/** Salva los cambios realizados en el Part. */
	virtual bool CommitChanges();
	/** Restaura los valores originales del Part. */
	virtual bool RollbackChanges();
	/** Obtiene el icono del part */
	virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0,
			int Y = 0) const;
	/** Actualiza el estado de la parte */
	virtual void Update() {
	}

private:
	/** Metodo que se llama cuando se lanza el evento de cambio en el choice de seleccion
     * de estilos **/
	void OnStyleChoiceChange(wxCommandEvent& Event);
	VectorLayer *pVectorLayer_; /*! capa vectorial **/
	DataViewManager* pDataViewManager_;
	ui::SimpleVectorStylePart* pSimpleEditor_;
	ui::CategorizedVectorStylePart* pCategorizedEditor_;
	friend class VectorPropertiesEventHandler;
	VectorPropertiesEventHandler* pEventHandler_;
	/** xrc de panel de editor de styles **/
	static const std::string VECTOR_STYLE_EDITION_XRC_NAME;
};
} // namespace suri

#endif /*VECTORPROPERTIESPART_H_*/
