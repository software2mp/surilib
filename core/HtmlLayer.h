/*! \file HtmlLayer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef HTMLLAYER_H_
#define HTMLLAYER_H_
// Includes Suri
#include "suri/DatasourceInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

class HtmlLayer: public suri::LayerInterface {
public:
	/** Constructor \deprecated */
	HtmlLayer(DatasourceInterface *pDatasource, Element *pElement);
	/** Constructor*/
	explicit HtmlLayer(DatasourceInterface *pDatasource);
	virtual ~HtmlLayer();
	/** Obtiene Datasource asociada al Layer. Trazabilidad entre Datasource-Layer */
	virtual SuriObject::UuidType GetAssociatedDatasourceId() const;
	/** Devuelve el estilo de renderizacion del layer */
	virtual void GetRenderizationStyle(
			const SuriObject::UuidType &LayerId) const;
	/** Devuelve el elemento \deprecated */
	virtual Element* GetElement();
	/** Compara un Datasource contra un Layer */
	virtual bool Equals(DatasourceInterface &Datasource);
	/** Compara dos tipos diferentes de Layers */
	virtual bool Equals(LayerInterface &Layer);
	/** Compara un Layer contra un Element */
	virtual bool Equals(Element &Element);
	/** Oculta el Layer */
	virtual void Hide(bool Hide = true);
	/** Verifica si esta oculto el Layer */
	virtual bool IsHidden() const;
	/** Setea el nombre de la capa **/
	virtual void SetName(const std::string& Name);
	/** Setea el titulo de la capa **/
	virtual void SetTitle(const std::string& Title);
	/** Configura el viewcontext al cual pertenece **/
	virtual void SetViewcontext(ViewcontextInterface* pViewcontext);
	/** Obtiene la instancia de fuente de datos asociada a la capa **/
	virtual DatasourceInterface* FetchAssociatedDatasource();

private:
	/** Constructor de copia */
	HtmlLayer(const HtmlLayer &);
	/** Referencia al elemento que encapsula */
	DatasourceInterface *pDatasource_;
	/** Referencia al elemento \deprecated */
	Element *pElement_;
	/** Variable Ocultacion */
	bool hidden_;
	/** nombre y titulo de la capa **/
	std::string name_, title_;
	/** Viewcontext al que pertenece **/
	ViewcontextInterface* pViewcontext_;
};

} /* namespace suri */
#endif /* HTMLLAYER_H_ */
