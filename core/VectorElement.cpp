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

// Includes Estandar
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>

// Includes suri
#include "suri/VectorElement.h"
#include "suri/Configuration.h"
#include "suri/messages.h"
#include "suri/Vector.h"
#include "AnotationElement.h"
#include "logmacros.h"
#include "suri/xmlnames.h"
#include "suri/VectorStyleManager.h"
#include "resources.h"
#include "VectorRenderer.h"
#include "VectorPropertiesPart.h"
#include "AnotationPropertiesPart.h"
#include "VectorTableStructureEditionPart.h"
#include "suri/Subset.h"
#include "suri/Dimension.h"
#include "VectorTablesPart.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/md5.h"
#include "VectorLayer.h"
#include "HotLinkPropertiesPart.h"
#include "suri/LibraryManagerFactory.h"
#include "suri/XmlFunctions.h"
#include "EnclosureValidator.h"
#include "EnclosureManipulator.h"
#include "EnclosureInformation.h"
#include "wxmacros.h"

// includes wx
#include "wx/xrc/xmlres.h"
#include "wx/filename.h"
#include "wx/dir.h"
#include "wx/clrpicker.h"  // color picker
#include "wx/spinctrl.h"   // textbox con up y down arrows
#include "wx/sizer.h"      // sizer
// includes ORG
#include "ogrsf_frmts.h"

// Defines
/** Cantidad de extensiones que representan a un vector (todas las de un shapefile) */
#ifdef __UNIX__
#  define ShapefileExtensionsSize 10
#else
#  define ShapefileExtensionsSize 5
#endif

// Este flag determina que wxFileName no agregue nada al path. Se define aca
// porque no esta definido en documentacion de wxWidgets(esta definido en trunk)
#define __wxPATH_NO_VOLUME_NO_SEPARATOR__ 0 /*! Ver comentario arriba */
/** el array ShapefileExtensions se utiliza para poder crear de forma mas dinamica los nodos */
/** dependencias de un Vector. */
std::string ShapefileExtensions[] = { "dbf", "shx", "sbn", "sbx", "prj"
#ifdef __UNIX__
		, "DBF", "SHX", "SBN", "SBX", "PRJ"};
#else
}		;
#endif

/** namespace suri */
namespace suri {
/** Macro para registrar el tipo de elemento */
AUTO_REGISTER_CLASS(Element, VectorElement, 0)

/**
 * Constructor
 * @return instancia de la clase VectorElement
 */
VectorElement::VectorElement() :
		Element(wxT(TYPE_PROPERTY_VALUE_VECTOR)), activeLayer_(0), activeLayerSetted_(
				false) {
}

// ------------------------------------------------------------------------------
/** Dtor */
VectorElement::~VectorElement() {
}

// ------------------------------------------------------------------------------
/**
 * Obtiene el icono.
 * @return string para el icono
 */
wxString VectorElement::DoGetIcon() const {
	// default
   EnclosureValidator validator;
   if (validator.IsValid(const_cast<VectorElement*>(this))) {
      EnclosureManipulator manipulator;
      EnclosureInformation einfo =
                     manipulator.GetEnclosureInformation(const_cast<VectorElement*>(this));
      std::string icon = einfo.GetEnclosureType() == EnclosureInformation::Train ?
                        icon_TRAIN_CLASS_16 : icon_TEST_CLASS_16;
      return icon;
   }
	wxString icon = wxT(icon_VECTOR_ELEMENT);
	wxString type = const_cast<VectorElement*>(this)->GetTypeAsString();
	// punto
	if (type == TYPE_DECORATOR(message_POINT)) {
		icon = icon_POINT_VECTOR_ELEMENT;
		// linea
	} else if (type == TYPE_DECORATOR(message_LINE)) {
		icon = icon_LINE_VECTOR_ELEMENT;
		// poligono
	} else if (type == TYPE_DECORATOR(message_POLYGON)) {
		icon = icon_POLIGON_VECTOR_ELEMENT;
      // Collecion
	}
	return icon;
}

/**
 * Crea una previsualizacion del elemento
 * @param[out] Preview lugar donde renderizo el preview
 * @param[in] Width ancho del icono
 * @param[in] Height altura del icono
 */
void VectorElement::GetPreview(wxBitmap &Preview, int Width, int Height) const {
   VectorRenderer::Parameters params = VectorRenderer::GetParameters(GetNode());
   if (!params.layerstyle_.empty()) {
      VectorStyle *pvecstyle = VectorStyle::Create(params.layerstyle_.begin()->second);
	   VectorRenderer::PreviewStyle(pvecstyle, Preview, Width, Height);
	   VectorStyle::Destroy(pvecstyle);
   } else {
      GET_BITMAP_RESOURCE(icon_PREVIEW_VECTOR_ELEMENT, Preview);
      ESCALE_BITMAP(Preview, Width, Height)
   }
}

// ------------------------------------------------------------------------------
/** Funcion estatica para crear un VectorElement a partir de un FileName.
 * Setea extencion a .shp, crea VectorElement con esta extencion y
 * verifica que el vector sea valido.
 * @param[in] FileName url donde quiero crear el vector
 * @param[in] Metadata mapa de metadatos
 * @return VectorElement con URL del FileName
 * \attention el VectorElement retornado(y su eliminacion) es
 *            responsabilidad del invocante,  La eliminacion
 *            se debe realizar mediante Destroy().
 */
VectorElement* VectorElement::Create(const std::string &FileName,
                                     const Option& Metadata) {
   // Creo el elemento con la factory, asi lo registra.
   // El elemento creado debe ser eliminado mediante Destroy()
   VectorElement *pelement = dynamic_cast<VectorElement *>(Element::Create("VectorElement"));

   // Verifico el casteo
   if (pelement) {
      if (pelement->Initialize(FileName) != pelement) {
         delete pelement;
         pelement = NULL;
      }
   }

   return pelement;
}

/**
 * Devuelve el identificador unico del elemento. En caso de que el contenido del
 * nodo url del Elemento sea un directorio, el UID solo estara formado por las
 * dependencias del mismo. En caso de que el contenido del nodo url del Elemento
 * sea un archivo (con extension), el UID estara formado por el url concatenado
 * con las dependencias del mismo.
 * @return UID del elemento.
 */
suri::Element::UidType VectorElement::GetUid() const {
   REPORT_DEBUG("Obteniendo uid %s", GetXmlAsString().c_str());
	wxFileName fn(GetUrl());
	// gdal considera que si no tiene extension .shp => es un directorio
	// por lo que el URL no terminaria en / que es lo que wx espera para
	// considerarlo un directorio, por eso se consulta por la falta de extension
	if (!fn.HasExt()) {
		// obtengo dependencias.
		std::vector<wxString> depvec = GetDependency();
		// si no tiene dependencias, utilizo el Element::GetUid()
		if (depvec.empty()) {
			return Element::GetUid();
		}
		return suri::MD5String(
				join(depvec.begin(), depvec.end(), wxT(":")).c_str());
	} else {
		std::string uid = Element::GetUid();
      uid.append(GetAttributeFilter());
      return suri::MD5String(uid.c_str());
	}
}

// ------------------------------------------------------------------------------
/*
 * obtiene la referencia espacial del elemento para la capa activa
 * @return string con la referencia espacial del elemento.
 */
wxString VectorElement::GetSpatialReference() const {
	return GetSpatialReference(GetActiveLayer());
}

// ------------------------------------------------------------------------------
/**
 * Obtiene la referencia espacial del elemento
 * @param[in] Layer indice de la capa con la referencia espacial que me interesa
 * @return string con la referencia espacial del elemento.
 * @return string vacio si el elemento no tiene ref. espacial.
 */
wxString VectorElement::GetSpatialReference(int Layer) const {
	wxString wantednode = GEORREFERENCE_NODE;
	wantednode += NODE_SEPARATION_TOKEN;
	wantednode += SPATIAL_REFERENCE_NODE;
	wantednode += NODE_SEPARATION_TOKEN;
	wantednode += LAYERS_NODE;
	wxXmlNode *pnode = GetNode(wantednode);
	pnode = GetLayerNode(Layer, pnode);
	if (pnode && pnode->GetChildren()) {
		return pnode->GetChildren()->GetContent();
	}
	return wxT("");
}

/**
 * configura la referencia espacial de la capa activa del elemento
 * @param[in] Wkt texto con la referencia espacial
 * @return informa si pudo configurar la referencia espacial
 */
bool VectorElement::SetSpatialReference(const std::string &Wkt) {
	return SetSpatialReference(GetActiveLayer(), Wkt);
}

/**
 * Configura la referencia espacial de la capa indicada del elemento
 * @param[in] Layer indice de la capa que quiero configurar
 * @param[in] Wkt texto con la referencia espacial
 * @return informa si pudo configurar la referencia espacial
 */
bool VectorElement::SetSpatialReference(int Layer, const std::string &Wkt) {
	wxString wantednode = GEORREFERENCE_NODE;
	wantednode += NODE_SEPARATION_TOKEN;
	wantednode += SPATIAL_REFERENCE_NODE;
	wantednode += NODE_SEPARATION_TOKEN;
	wantednode += LAYERS_NODE;
	wxXmlNode *pnode = GetNode(wantednode);
	// no existe
	if (!pnode) {
		pnode = AddNode(GetNode(wxT("")), wantednode);
	}
	return SetLayerNode(Layer, pnode, Wkt) != NULL;
}

// ------------------------------------------------------------------------------
/**
 * Devuelve ventanas de propiedades de los elementos Vector.
 * @return vector con punteros a los Part generados.\n
 * NOTA: en caso de tener que agregar Part, RECORDAR que GetParts()
 * devuelve 2 Part(1ro Comun a todos los elementos, 2do Para editar xml(solo en DEBUG))
 * ==> los Part a agregar deben agregarse entre medio de dichos Part's.
 */
std::vector<Part *> VectorElement::DoGetParts(DataViewManager *pDataViewManager,
		LayerInterface *pLayer, DatasourceInterface *pDatasource) {
	std::vector<Part *> vectorparts;
	if (!pDataViewManager || !pDatasource || !pLayer) {
		vectorparts.push_back(new AnotationPropertiesPart(this, true));
		vectorparts.push_back(new VectorTablesPart(this, false));
		if (GetTypeAsString().IsSameAs(TYPE_DECORATOR("PUNTO"), false))
			vectorparts.push_back(new VectorTablesPart(this, true));
	} else {
		VectorLayer* pvectorlayer = dynamic_cast<VectorLayer*>(pLayer);
		VectorDatasource* pvectordatasource =
				dynamic_cast<VectorDatasource*>(pDatasource);
		if (pvectordatasource) {
#ifdef __DEBUG__
			vectorparts.push_back(new VectorTablesPart(pvectordatasource, false));
#endif
			if (Configuration::GetParameter("app_short_name", "") != wxT("SoPI")) {
				// Capa de tipo punto y Simur o !SoPI
				if (GetTypeAsString().IsSameAs(TYPE_DECORATOR("PUNTO"), false)) {
					vectorparts.push_back(
							new HotLinkPropertiesPart(pvectordatasource,
									pvectorlayer));
				}
			} else {
				vectorparts.push_back(
						new VectorTableStructureEditionPart(pvectordatasource,
								pvectorlayer));
         }
		}
	}
	return vectorparts;
}

/** Devuelve vector con Parts especificos de la visualizacion elemento. */
std::vector<suri::Part*> VectorElement::DoGetVisualizationParts(
		DataViewManager* pDataViewManager, LayerInterface *pLayer,
		DatasourceInterface *pDatasource) {
	std::vector<Part *> vectorparts;
	VectorLayer* pvectorlayer = dynamic_cast<VectorLayer*>(pLayer);
   if (pvectorlayer) {
      vectorparts.push_back(
            new VectorPropertiesPart(pDataViewManager,
                                     pvectorlayer));
		vectorparts.push_back(new AnotationPropertiesPart(pvectorlayer, true));
	}
	return vectorparts;

}

// ------------------------------------------------------------------------------
/**
 * Devuelve un string con el tipo de elemento. El string devuelto queda formado
 * por message_VECTOR_LAYER_OF y el tipo de Vector que lo obtiene del nodo capas.
 * @return  cadena con el tipo de elemento en caso de exito.
 * @return  "N/A" en caso de que no se encuentre el nodo capas.
 */
wxString VectorElement::GetTypeAsString() {
	std::string type = GetStringType();
	if (type.compare(_(message_NOT_APPLICABLE)) == 0)
		return _(message_NOT_APPLICABLE);
	return TYPE_DECORATOR(type.c_str());
}

/**
 * Obtiene el WKT del estilo de la capa indicada del vector
 * @param[in] Layer indice de la capa que quiero configurar
 * @return WKT del estilo
 * @return String vacio en caso de no poder obtener el estilo
 */
std::string VectorElement::GetStyle(int Layer) const {
	std::string path = std::string(RENDERIZATION_NODE) + NODE_SEPARATION_TOKEN
			+ LAYERS_NODE;
	wxXmlNode *plsnode = GetLayerNode(Layer, GetNode(path));
	wxXmlNode *psnode = plsnode ? GetNode(STYLE_NODE, plsnode) : NULL;
	if (!psnode) {
		// REPORT_AND_FAIL_VALUE("D:No se pudo obtener el nodo de estilo (%d)", wxT(""), Layer);
		// TODO(): En caso de no existir el nodo "estilo" existe la posibilidad
		// de que venga configurado el estilo dentro del nodo capa. Por eso
		// en caso de !psnode se obtiene el contenido del nodo
		return plsnode ? plsnode->GetNodeContent().c_str() : "";
	}
	return psnode->GetNodeContent().c_str();
}

/**
 * Permite setear el WKT del estilo del vector
 * @param[in] Layer indice de la capa que quiero configurar
 * @param[in] WktStyle texto con el estilo
 */
void VectorElement::SetStyle(const std::string &WktStyle, int Layer) {
	wxXmlNode *prnode = GetNode(RENDERIZATION_NODE);
	// no existe, lo creo
	if (!prnode) {
		prnode = AddNode(GetNode(wxT("")), RENDERIZATION_NODE);
	}

	// Agrega los nodos dentro de CAPAS
	wxXmlNode *plnode = GetNode(LAYERS_NODE, prnode);
	// no existe, lo creo
	if (!plnode) {
		plnode = AddNode(prnode, LAYERS_NODE);
	}
	// nodo del esitlo de la capa
	wxXmlNode *plsnode = GetLayerNode(Layer, plnode);
	if (!plsnode) {
		std::stringstream ss;
		ss << Layer;
		plsnode = AddNode(plnode, LAYER_NODE, "", LAYER_INDEX_PROPERTY,
				ss.str(), false);
	}

	wxXmlNode *psnode = GetNode(STYLE_NODE, plsnode);
	if (psnode)
		psnode->GetChildren()->SetContent(WktStyle);
	else
		AddNode(plsnode, STYLE_NODE, WktStyle);
}

/**
 * Permite setear el WKT del estilo de la capa activa del vector
 * @param[in] WktStyle texto con el estilo
 */
void VectorElement::SetStyle(const std::string &WktStyle) {
	SetStyle(WktStyle, GetActiveLayer());
}

/**
 * Obtiene el subset con el extent del vector
 * @param[out] ElementExtent extent del vector
 */
void VectorElement::GetElementExtent(Subset &ElementExtent) {
	Subset auxsubset(0, 0, 0, 0);
	int layercount = GetLayerCount();
	if (layercount == 0) {
		return;
	}
	// Agrega un nodo de estilo por cada layer
	// TODO(Gabriel - TCK #854): Ver si esta afecta. Charlar con javier
	GetLayerExtent(ElementExtent, 0);
	for (int nlayer = 1; nlayer < layercount; nlayer++) {
		GetLayerExtent(auxsubset, nlayer);
		ElementExtent = Join(ElementExtent, auxsubset);
	}
}

/**
 * Retorna la cantidad de capas en el nodo
 * @return cantidad de capas en el nodo
 */
int VectorElement::GetLayerCount() {
	wxXmlNode *pnode = GetNode(wxString::Format("%s", LAYERS_NODE));
	wxString layerindex;
	layerindex = pnode->GetPropVal(QUANTITY_PROPERTY, "-1");
	long idxcount = -1;
	layerindex.ToLong(&idxcount);
	return static_cast<int>(idxcount);
}

/**
 * Obtiene el subset con el extent del layer indicado del nodo. Si no
 * existe llama a SaveVectorExtent para crear/completar el nodo dimension.
 * @param[in] Layer capa de la que se quiere el extent
 * @param[out] LayerExtent extent de la capa pedida
 */
void VectorElement::GetLayerExtent(Subset &LayerExtent, int Layer) {
	// Obtengo extent de capa
	wxXmlNode* pnode = GetNode(
			wxString::Format("%s%s%s", DIMENSION_NODE, NODE_SEPARATION_TOKEN,
					LAYERS_NODE));
	if (!pnode) {
		SaveVectorExtent();
	}

	wxXmlNode *plsnode = GetLayerNode(Layer, pnode);
	// Obtengo subset
	pnode = GetNode(wxString::Format("%s", EXTENT_NODE), plsnode);
	GetSubset(LayerExtent, pnode);
}

/** Actualiza extent y dependencias */
void VectorElement::UpdateData() {
	SaveVectorExtent();
	SetDependencies();
}

/**
 * Guarda el extent de la capa en el nodo
 * @param[in] Layer nro capa al que pertenece el extent
 * @param[in] LayerExtent extent a guardar en nodo
 */
void VectorElement::SetLayerExtent(const Subset &LayerExtent, int Layer) {
	// Obtengo dimension
	wxXmlNode *pnode = GetNode(wxString::Format("%s", DIMENSION_NODE));
	if (!pnode) {
		REPORT_AND_FAIL("Error al encontrar el subset del raster");
	}
	// Agrega los nodos dentro de CAPAS
	wxXmlNode *plnode = GetNode(LAYERS_NODE, pnode);
	if (!plnode) {
		plnode = AddNode(pnode, LAYERS_NODE);
	}
	// Creo nodo en capa
	wxXmlNode *plsnode = GetLayerNode(Layer, plnode);
	if (!plsnode) {
		std::stringstream ss;
		ss << Layer;
		plsnode = AddNode(plnode, LAYER_NODE, "", LAYER_INDEX_PROPERTY,
				ss.str(), false);
	}
	wxXmlNode *pextentnode = AddNode(plsnode, EXTENT_NODE);
	SetSubset(LayerExtent, pextentnode);
}

/**
 * Recorre las capas y guarda para cada una un nodo con el extent
 * usando funcion SetLayerExtent
 */
void VectorElement::SaveVectorExtent() {
	Vector *pVector = Vector::Open(GetUrl().c_str());
	if (!pVector) {
		return;
	}
	// Agrego un nodo a dimension por cada capa
	// Agrega nodo Dimenion. Si existe lo remplaza con uno en blanco
	AddNode(GetNode(wxT("")), DIMENSION_NODE);
	// Agrega nodo CAPA por cada layer
	for (int nlayer = 0; nlayer < pVector->GetLayerCount(); nlayer++) {
		// Calcula extent de capa
		OGRLayer *player = pVector->GetLayer(nlayer);
		if (!player) {
			Vector::Close(pVector);
			REPORT_DEBUG("D:Vector invalido.");
			return;
		}

		OGREnvelope envelope;
		if (player->GetExtent(&envelope, TRUE) == OGRERR_FAILURE) {
			REPORT_DEBUG("D:Vector con extension desconocida.");
			SetLayerExtent(Subset(0, 0, 0, 0), nlayer);
			break;
		}

		// Calcula el extent
		// \TODO: Reconocer el Subset en funcion del SR. Quedo hardcodeado para WGS84
		Subset wextent(envelope.MinX, envelope.MaxY, envelope.MaxX,
				envelope.MinY);
		SetLayerExtent(wextent, nlayer);
	}

	Vector::Close(pVector);
}

bool VectorElement::InitVector(Vector *pVector,
                               const std::string &Filename) {
   Vector::VectorType type = Vector::Undefined;
   return InitVector(pVector, Filename, type);
}


// ------------------------------------------------------------------------------
/** Inicializa los nodos del elemento
 * @param[in] pVector vector con el que voy a configurar el elemento
 * @param[in] Filename url del vector creado
 * @param[in] type tipo de vector (fuerza un tipo de vector, util para leer KMLs)
 * \todo si es un directorio, agrega el archivo correspondiente a la primer capa hacer que lo haga para todas las capas
 * @return true si pudo inicializar el elemento.
 * @return false si no pudo inicializar o si no existe estilo.
 */
bool VectorElement::InitVector(Vector *pVector,
                               const std::string &Filename,
                               Vector::VectorType type) {
	if (!pVector) {
		REPORT_AND_FAIL_VALUE("D:No se puede inicializar el elemento", false);
	}
	// Agrega el nodo URL
	SetUrl(Filename);
	// Agrega nodo GEORREFERENCIA
	// Agrega un nodo CAPA por cada layer
	for (int nlayer = 0; nlayer < pVector->GetLayerCount(); nlayer++)
		SetSpatialReference(nlayer, pVector->GetLayerSR(nlayer));

	// Agrega los nodos dentro de ARCHIVO
	SetFileNode(pVector->GetOption(Vector::Format));

	// Agrelga los nodos dentro de RENDERIZACION
	AddNode(GetNode(wxT("")), RENDERIZATION_NODE);
	// Agrega nodos dentro de CAPAS con atributo la cantidad de layers
	std::stringstream ss;
	ss << pVector->GetLayerCount();
	wxXmlNode *playersnode = AddNode(GetNode(wxT("")), LAYERS_NODE, wxT(""),
			wxT(QUANTITY_PROPERTY), ss.str(), false);
	ss.str("");

	// Agrega nodo TIPO
	AddNode(playersnode, TYPE_NODE);
   wxString typeVector;
   // Agrega nodo CAPA por cada layer
   for (int nlayer = 0; nlayer < pVector->GetLayerCount(); nlayer++) {
      wxXmlNode *pchild = SetLayerNode(nlayer, playersnode, "");
      Vector::VectorType currentType = type;
      if (currentType == Vector::Undefined){
         REPORT_DEBUG("%s", GetXmlAsString().c_str());
         currentType = pVector->GetLayerType(nlayer);
      }
      AddNode(pchild, TYPE_NODE,
            pVector->GetVectorTypeAsString(currentType));
      typeVector += pVector->GetVectorTypeAsString(currentType);
      if ((nlayer - 1) != pVector->GetLayerCount()) {
         typeVector += wxT(", ");
      }
      ss << nlayer;
      AddNode(pchild, DESCRIPTION_NODE, wxT("Capa ") + ss.str());
      ss.str("");
   }
   wxString typeaux = typeVector;
   typeVector.EndsWith(", ", &typeaux);

	wxXmlNode *ptypenode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, TYPE_NODE);
	ptypenode->AddChild(
			new wxXmlNode(ptypenode, wxXML_TEXT_NODE, wxT(""), typeaux));
	AddNode(playersnode, ptypenode, true);

	wxFileName filename(Filename, wxPATH_UNIX);

   // Si es multigeometria, devuelve que es valido para poder crear un elemento por cada geometria
   Vector::VectorType currentType = type;
   if (currentType == Vector::Undefined){
      currentType = pVector->GetLayerType(0);
   }
   if (currentType == Vector::GeometryCollection){
      return true;
   }

	bool validStyle = ConfigureStyle(pVector, currentType);
   if (!validStyle){
      return false;
   }

	// Si es un directorio, agrega el archivo de la primer capa, pero no para el resto

	UpdateData();

	return true;
}

/**
 * Devuelve una lista de nodos correspondientes al los hipervinculos
 * de la capa
 * @param[in] hotLinkNodesList : referencia a lista contenedora de nodos de hipervinculo
 * @return void
 */

void VectorElement::GetHotlinkNodesList(
		std::list<std::string>& hotLinkNodesList) const {
	hotLinkNodesList.clear();
	// obtengo el nodo capas
	wxXmlNode *playernode = GetNode(wxT(LAYERS_NODE));
	// obtengo el nodo de la capa buscada
	playernode = GetLayerNode(activeLayer_, playernode);
	if (!playernode)
		return;
	wxXmlNode *phlnode = playernode->GetChildren();
	while (phlnode != NULL) {
		if (phlnode->GetName().Cmp(wxT(HOTLINK_NODE)) == 0)
			hotLinkNodesList.push_back(
					std::string(phlnode->GetNodeContent().c_str()));
		phlnode = phlnode->GetNext();
	}
}

/**
 * Devuelve el nombre del campo que contiene el url del hotlink
 * @param[in]  Layer: indice de la capa del vector.
 * @return string con nombre del campo en la base de datos del vector.
 */
wxString VectorElement::GetHotLinkFieldName(const int Layer) const {
	// obtengo el nodo capas
	wxXmlNode *pnode = GetNode(wxT(LAYERS_NODE));
	// obtengo el nodo de la capa buscada
	pnode = GetLayerNode(Layer, pnode);
	if (!pnode) {
		REPORT_AND_FAIL_VALUE("D:No se encuentra la capa: %d", wxT(""), Layer);
	}
	/*
	 pnode = pnode->GetChildren();
	 if (!pnode) {
	 REPORT_AND_FAIL_VALUE("D:No se encuentra el hijo del nodo", wxT(""));
	 }
	 */
	// busco nodo hotlink
	wxXmlNode* phlnode = GetNode(wxT(HOTLINK_NODE), pnode);

	if (!phlnode) {
		REPORT_AND_FAIL_VALUE("D:No se encuentra el nodo hotlink", wxT(""));
	}

	return phlnode->GetNodeContent().c_str();
}

/**
 * Setea el nombre del campo que contiene los url de los hotlinks
 * @param[in]  Layer: indice de la capa del vector.
 * @param[in]  FieldName: nombre del campo a setear en la base de datos del vector.
 */
void VectorElement::SetHotLinkFieldName(const int Layer,
		const wxString &FieldName) {
	// obtengo el nodo capas
	wxXmlNode *playersnode = GetNode(wxT(LAYERS_NODE));
	// obtengo el nodo de la capa buscada
	wxXmlNode *playernode = GetLayerNode(Layer, playersnode);
	// Agrego el nodo HotLink.
	AddNode(playernode, wxT(HOTLINK_NODE), FieldName);
}

/**
 * Setea el nombre del campo que contiene los url de los hotlinks
 * @param[in]  FieldName: nombre del campo a setear en la base de datos del vector.
 */
void VectorElement::SetHotLinkFieldName(const wxString &FieldName) {
	SetHotLinkFieldName(GetActiveLayer(), FieldName);
}

/**
 * Informa si el estado del elemento es valido.
 * Crea Datasource con url del vector e indica si pudo crearlo
 * @return indica si pudo crear datasource con url del elemento
 */
bool VectorElement::IsValid() {
	OGRDataSource *pdatasource = OGRSFDriverRegistrar::Open(GetUrl().c_str());
	if (!pdatasource) {
		return false;
	}
	OGRDataSource::DestroyDataSource(pdatasource);
	return true;
}

bool VectorElement::ConfigureStyle(Vector* pVector, Vector::VectorType type) {
   for (int nlayer = 0; nlayer < pVector->GetLayerCount(); nlayer++) {
      Vector::VectorType currentType = type;
      if (type == Vector::Undefined){
         currentType = pVector->GetLayerType(nlayer);
      }
      // Carga el estilo
      suri::VectorStyle * pvecstyle =
            VectorStyleManager::Instance().GetDefaultStyle(currentType);
      if (!pvecstyle) {
         REPORT_AND_FAIL_VALUE("D: no existe estilo para el vector.", false);
      }
      if (pvecstyle->GetPen()) {
         pvecstyle->GetPen()->color_.red_ = rand() % 255;
         pvecstyle->GetPen()->color_.green_ = rand() % 255;
         pvecstyle->GetPen()->color_.blue_ = rand() % 255;
      } else if (pvecstyle->GetSymbol()) {
         pvecstyle->GetSymbol()->color_.red_ = rand() % 255;
         pvecstyle->GetSymbol()->color_.green_ = rand() % 255;
         pvecstyle->GetSymbol()->color_.blue_ = rand() % 255;
      }
      SetStyle(pvecstyle->GetWkt(), nlayer);
   }
   return true;
}


/** Determina y agrega las dependencias.
 * Si el directorio del URL existe busca archivos con extenciones .shp y crea
 * dependencias para este archivo y otros archivos existentes con mismo nombre
 * pero extencion diferente(en lista ext de shapefile)
 * Si el directorio no existe crea las dependencias directamente
 */
void VectorElement::SetDependencies() {
	// si se guardo el url como directorio
	if (wxDir::Exists(GetUrl())) {
		// inicializo fn como directorio (para evitar problemas con la ultima /)
		wxFileName fn(GetUrl(), wxEmptyString);
		wxDir dir(fn.GetFullPath());
		wxString currentfile;
		std::string pspecs[] = { "*.shp", "*.SHP" };
		for (int i = 0; i < 2; i++) {
			bool flag = dir.GetFirst(&currentfile, pspecs[i]);
			while (flag) {
				// agrego como dependencia el shape
				fn = wxFileName(
						dir.GetName() + fn.GetPathSeparator() + currentfile);
				if (fn.FileExists()) {
					AddDependency(
							(fn.GetVolume() + fn.GetVolumeSeparator()
									+ fn.GetFullPath(wxPATH_UNIX)).c_str());
				}
				// Agrega nodo DEPENDENCIAS
				for (size_t i = 0; i < ShapefileExtensionsSize; i++) {
					fn.SetExt(ShapefileExtensions[i]);
					if (fn.FileExists()) {
						AddDependency(
								(fn.GetVolume() + fn.GetVolumeSeparator()
										+ fn.GetFullPath(wxPATH_UNIX)).c_str());
					}
				}
				flag = dir.GetNext(&currentfile);
			}
		}
	} else {
		// inicializo fn como nombre de archivo
		wxFileName fn(GetUrl());
		// pregunto si existe como archivo para evitar hacer N preguntas
		// en el ciclo, es un poco mas optimo
		// verifica que no este el string de vector en memoria (cabeza)
		if (GetUrl().find("shpmemory") == std::string::npos
				&& fn.FileExists()) {
			// Agrega nodo DEPENDENCIAS
			for (size_t i = 0; i < ShapefileExtensionsSize; i++) {
				fn.SetExt(ShapefileExtensions[i]);
				if (fn.FileExists()) {
					AddDependency(
							(fn.GetVolume() + fn.GetVolumeSeparator()
									+ fn.GetFullPath(wxPATH_UNIX)).c_str());
				}
			}
		}
	}
}

/** Obtiene el nodo XML correspondiente a una capa.
 * @param[in]  Layer indice de la capa que busco.
 * @param[in]  pLayersNode nodo donde debo buscar por la capa
 * \attention el nodo retornadao es el nodo dentro de pLayersNode(no una copia)
 * @return puntero al nodo xml de la capa indicada.
 * @return NULL en caso de error.
 */
wxXmlNode *VectorElement::GetLayerNode(int Layer,
		wxXmlNode *pLayersNode) const {
	wxXmlNode *pnode = NULL;
	if (pLayersNode) {
		pnode = pLayersNode->GetChildren();
	}
	// busco el nodo capa de indice Layer
	while (pnode) {
		wxString layerindex;
		layerindex = pnode->GetPropVal(LAYER_INDEX_PROPERTY, "-1");
		long idx = -1;
		layerindex.ToLong(&idx);
		if (idx == Layer) {
			break;
		}
		pnode = pnode->GetNext();
	}
	return pnode;
}

/** Carga un nodo XML correspondiente a una capa.
 * @param[in]  Layer indice de la capa que busco.
 * @param[in]  pLayersNode nodo donde debo buscar por la capa
 * @param[in]  NodeContent nuevo contenido del nodo de la capa
 * \attention el nodo retornado es responsabilidad de esta clase(no eliminar externamente)
 * \note cuando hacemos el AddNode obtenemos un nodo que no debe eliminarse, por lo tanto
 * se propaga la advertencia de no borrarlo.
 * @return puntero al nodo xml de la capa cargada.
 */
wxXmlNode *VectorElement::SetLayerNode(int Layer, wxXmlNode *pLayersNode,
		const wxString &NodeContent) {
	std::stringstream ss;
	ss << Layer;
	wxXmlNode *playernode = GetLayerNode(Layer, pLayersNode);
	if (playernode && playernode->GetChildren()) {
		playernode->GetChildren()->SetContent(NodeContent);
	}
	playernode = AddNode(pLayersNode, LAYER_NODE, NodeContent,
			LAYER_INDEX_PROPERTY, ss.str(), false);
	return playernode;
}

/**
 * Inicializa el elemento invocando al metodo ::InitVector. Genera el url del
 * elemento teniendo en cuenta de si es un archivo o un directorio. Si es un
 * archivo valida que la extension del mismo sea "shp", caso contrario falla.
 * @param[in] FileName url del vector.
 * @param[in] Metadata mapa de metadatos
 * @return puntero a VectorElement si tuvo exito.
 * @return NULL en caso de error.
 */
VectorElement* VectorElement::Initialize(const std::string &FileName,
                                         const Option& Metadata) {
	wxFileName fn(FileName);
	metadata_ = Metadata;
	std::string filename = metadata_.GetOption("Filename");
	if (!filename.empty()) {
	   wxXmlNode *prnode = AddNode(GetNode(wxT("")), CSV_FILE_NODE);
	   std::string columnx = metadata_.GetOption("ColumnX");
	   std::string columny = metadata_.GetOption("ColumnY");
	   std::string token = metadata_.GetOption("Token");
	   std::string startfromrow = metadata_.GetOption("StartFromRow");
	   std::string hasheader = metadata_.GetOption("Header");
	   std::string hascustomtoken = metadata_.GetOption("CustomToken");
	   AddNode(prnode, X_COLUMN_NODE, wxT(columnx));
	   AddNode(prnode, Y_COLUMN_NODE, wxT(columny));
	   AddNode(prnode, TOKEN_NODE, wxT(token));
	   AddNode(prnode, START_FROM_ROW_NODE, wxT(startfromrow));
	   AddNode(prnode, HEADER_NODE, wxT(hasheader));
	   AddNode(prnode, FILE_NODE, wxT(filename));
	   AddNode(prnode, CUSTOM_TOKEN_NODE, wxT(hascustomtoken));
	}
	std::string pathvector = wxT("");
	if (fn.HasExt()) {
		/* \todo (03/03/2010 - Alejandro): en caso de que se necesiten agregar mas
		 * extensiones: crear vector con extensiones invalidas y reemplazar if por
		 * for que recorra dicho vector.*/
		if (fn.GetExt() == wxT("dbf") || fn.GetExt() == wxT("shx")) {
			// TCK #4557 - Se comenta el REPORT_ERROR
			// no afecta a la funcionalidad de la aplicacion
			// REPORT_ERROR(message_VECTOR_INVALID_EXT_ERROR);
			return NULL;
		}
		pathvector = fn.GetVolume().c_str() + fn.GetVolumeSeparator()
				+ fn.GetFullPath(wxPATH_UNIX).c_str();
	} else {
		fn = wxFileName(FileName, wxT(""));
		if (!fn.IsOk()) {
			REPORT_DEBUG(wxT("D:FileName mal creado"));
			return NULL;
		}
		// Esto se separa en tres pasos porque en windows el comando
		// GetPath(wxPATH_GET_VOLUME, wxPATH_UNIX) no retorna el volumen(=> con
		// vectores en memoria 'shpmemory:..' no funciona)
		pathvector = fn.GetVolume().c_str();
		pathvector += fn.GetVolumeSeparator().c_str();
		pathvector +=
				fn.GetPath(__wxPATH_NO_VOLUME_NO_SEPARATOR__, wxPATH_UNIX).c_str();
	}
	SetUrl(pathvector);

	// Abre el Vector
	Vector* pvec = Vector::Open(GetUrl().c_str(), Vector::ReadWrite);
   if (pvec == NULL)
      pvec = Vector::Open(GetUrl().c_str(), Vector::ReadOnly);

   // * TODO (#8200-danieltesta): este codigo hace que el contenido de los archivos
   // * KML sea eliminado asi que por ese motivo queda comentado. Probe con edicion
   // * vectorial y exportacion y anduvo todo. Habria que ver si tiene sentido que
   // * esto este aca.
   // * (matiaslafroce): Mientras trabajaba sobre #10632 descubri que muy
   // * probablemente la eliminacion del contenido de los KML sea porque se
   // * abren en modo lectura y escritura. Si se abren en solo lectura el contenido
   // * permanece pero no se puede, por ejemplo, agregar capas.
   // TCK: 5722
   // if (pvec != NULL) {
   //    pvec->CreateLayer(GetUrl().c_str(), GetSpatialReference().c_str(), Vector::Undefined);
   // }

   // Si el vector no es valido, no tiene layers o no se puede inicializar el elemento
   if (!pvec || pvec->GetLayerCount() < 1 || !InitVector(pvec, pathvector.c_str())) {
      // Cierra el vector
      Vector::Close(pvec);
      return NULL;
   }
   // Cierra el vector
   Vector::Close(pvec);
   return this;
}

/**
 * Setea la capa activa (solo se puede setear una vez)
 * param[in] ActiveLayer numero de capa activa
 * @return true si el numero de capa esta dentro del rango valido, false en caso contrario
 */
bool VectorElement::SetActiveLayer(int ActiveLayer) {
	if (!activeLayerSetted_) {
		// Verifico que el numero de capa este dentro del rango valido
		if (ActiveLayer < 0 || ActiveLayer > GetLayerCount())
			return false;
		activeLayer_ = ActiveLayer;
		activeLayerSetted_ = true;
		std::string activelayer;
		std::stringstream out;
		out << activeLayer_;
		activelayer = out.str();
		AddNode(GetNode(wxT("")), wxT(ACTIVELAYER_NODE), activelayer.c_str());
		if (GetLayerCount() > 1) {
         Vector* pvector = Vector::Open(GetUrl().c_str());
         if (pvector != NULL) {
            OGRLayer* pogrlayer = pvector->GetLayer(ActiveLayer);
            if (pogrlayer != NULL) {
               wxFileName filename(GetUrl());
               // Si hay mas de una capa al nombre se le concatana el nombre de la capa en OGR
               wxString name = filename.GetName();
               name += "_";
               name += pogrlayer->GetName();
               SetName(name);

               // Para la generacion del ID
               AddNode(GetNode(wxT("")), wxT(DEPENDENCY_NODE), name);
            }
            Vector::Close(pvector);
         }
		}
		return true;
	}
	return false;
}

/**
 * Obtiene la capa activa
 */
const int VectorElement::GetActiveLayer() const {
	return activeLayer_;
}

/** Metodo que devuelve el tipo de vectoren un string sin decorar. **/
std::string VectorElement::GetStringType() const {  
   REPORT_DEBUG("%s", GetXmlAsString().c_str());
	wxXmlNode *pnode = GetNode(
         wxString(LAYERS_NODE) + wxString("|") + wxString(TYPE_NODE));

   // Se posiciona en el nodo capa 0
   wxXmlNode *playernode = pnode->GetNext();
   int activelayer = GetActiveLayer();
   // itera hastas encontrar el nodo de la capa activa
   for (int i = 0 ; i <activelayer && playernode ; ++i) {
         playernode = playernode->GetNext();
   }
	playernode = playernode ? playernode->GetChildren() : NULL;
   // Bajo por el nodo hasta llegar al tipo de capa.
   while (playernode && playernode->GetName().compare(TYPE_NODE) != 0) {
      playernode = playernode->GetNext();
   }

   if (!pnode || !playernode) {
      return _(message_NOT_APPLICABLE);
   }
   return playernode->GetNodeContent().c_str();
}

/**
 * Devuelve el tipo de vector asociado al elemento
 * @return tipo de vector asociado
 */
Vector::VectorType VectorElement::GetType() const {
   std::string type = GetStringType();

   Vector::VectorType vectortype = Vector::Undefined;
   if (type.compare(message_POINT) == 0) {
      vectortype = Vector::Point;
   } else if (type == TYPE_DECORATOR(message_LINE)
         || type.compare(message_LINE) == 0) {
      vectortype = Vector::Line;
   } else if (type == TYPE_DECORATOR(message_POLYGON)
         || type.compare(message_POLYGON) == 0) {
      vectortype = Vector::Polygon;
   } else if (type == TYPE_DECORATOR(message_GEOM_COLLECTION)
         || type.compare(message_GEOM_COLLECTION) == 0) {
      vectortype = Vector::GeometryCollection;
   }
	return vectortype;
}

void VectorElement::SetType(Vector::VectorType Type, bool recursive) {
   wxXmlNode *pTypeNode = GetNode(
         wxString(LAYERS_NODE) + wxString("|") + wxString(TYPE_NODE));
   pTypeNode->GetChildren()->SetContent(Vector::GetVectorTypeAsString(Type));
   if (recursive){
      SetChildrenType(GetNode(wxString(LAYERS_NODE)), Type);
   }
}

void VectorElement::SetChildrenType(wxXmlNode* pLayersNode,
                                    Vector::VectorType type) {
   wxXmlNode* pLayer = pLayersNode->GetChildren();
   while (pLayer){
      if (pLayer->GetName().compare(LAYER_NODE) == 0){
         wxXmlNode* pChild = pLayer->GetChildren();
         while(pChild){
            if (pChild->GetName().compare(TYPE_NODE) == 0){   
               pChild->GetChildren()->SetContent(Vector::GetVectorTypeAsString(type));
            }
            pChild = pChild->GetNext();
         }
      }
      pLayer = pLayer->GetNext();
   }
}


void VectorElement::SetLayerType(Vector::VectorType Type){
   /**
    * TODO matiaslafroce: implementar de forma similar a arriba
    * Seguramente sea de utilidad en el futuro
    */
}

/** Obtiene el nombre de la capa activa
 *  @return string con el nombre de la capa
 *  @return string vacio en caso de no encontrar la capa activa  **/
std::string VectorElement::GetActiveLayerName() const {
	Vector *pvector = Vector::Open(GetUrl().c_str());
	std::string name;
	if (pvector) {
		OGRLayer* player = pvector->GetLayer(activeLayer_);
		if (player) {
			name = player->GetName();
		}
		Vector::Close(pvector);
	}
	return name;
}

std::vector<std::string> VectorElement::GetSelectedFeatures() {
    std::vector<std::string> selectedFeatures;
    wxXmlNode* selectionNode = this->GetNode(VECTOR_SELECTION_NODE);
    wxString features;
    if (selectionNode) {
       selectionNode = selectionNode->GetChildren();
       if (selectionNode->GetName() == FID_SELECTION){
          features = selectionNode->GetNodeContent();
       }
       char featureId[100] = {0};
       memcpy(featureId, features.c_str(), features.size()+1);
       char *token = std::strtok(featureId, ",");
       while (token != NULL) {
            selectedFeatures.push_back(std::string(token)),
            token = std::strtok(NULL, ",");
       }
    }
    return selectedFeatures;
}

bool VectorElement::SetAttributeFilter(const std::string& filter) {
   AddNode(GetNode(""), ATTRIBUTE_FILTER_NODE, filter);
   return true;
}

std::string VectorElement::GetAttributeFilter() const {
   std::string filter;
   std::string path = std::string(ATTRIBUTE_FILTER_NODE);
   wxXmlNode* pFilterNode = GetNode(wxT(path));
   if (pFilterNode){
      filter = GetNode(wxT(path))->GetNodeContent().c_str();
   }
   return filter;
}


} // namespace suri
