/*! \file SpatialReference.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


// Includes standard
#include <cstring>
#include <string>
#include <vector>

// Includes Suri
#include "SpatialReference.h"
#include "suri/messages.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/CoordinatesTransform.h"
#include "suri/Wkt.h"

#include <cpl_error.h>
#include <ogr_spatialref.h>

// Defines
/** defines para formar WKT de georeferencia */
/** nodo referencia espacial geografica */
#define WKT_GEOGCS_NODE "GEOGCS"
/** nodo referencia espacial proyectada */
#define WKT_PROJCS_NODE "PROJCS"
/** simbolo que separa nodos en wkt */
#define WKT_NODE_SEPARATOR "|"
/** nodo datum */
#define WKT_DATUM_NODE "DATUM"
/** nodo unidad */
#define WKT_UNIT_NODE "UNIT"
/** nodo elipsoide */
#define WKT_SPHEROID_NODE "SPHEROID"
/** nodo proyeccion. */
#define WKT_PROJECTION_NODE "PROJECTION"
/** Caracter que se usa para separar nombre y codigo de autoridad */
#define AUTHORITYDATA_SEPARATOR ':'
/** nodo de conversion a datum wgs84 **/
#define BURSA_WOLF_PARAMETERS_NODE "TOWGS84"

/** namespace suri */
namespace suri {

SpatialReference::SpatialReference(const std::string& Wkt): wkt_(Wkt) {
}

/**
 * Devuelve el parametro especificado de la referencia espacial contenida en un
 * WKT.
 * @param[in] Wkt wkt con la referencia espacial.
 * @param[in] Parameter nombre del nodo del cual se quiere obtener un valor.
 * @param[in] Position posicion dentro del nodo (zero based)
 * @return string con el valor del parametro deseado en caso de exito.
 * @return el valor de la constante message_NO_DATA(S/D) en caso de error.
 * \see http://www.gdal.org/ogr/classOGRSpatialReference.html#cc815e7bd21c1c052304368c0eefe8be
 */
std::string SpatialReference::GetSpatialReferenceParameter(const std::string &Wkt,
                                         const std::string &Parameter,
                                         const int Position) {
   std::string retval = message_NO_DATA;

   char* pszProjection = new char[Wkt.length() + 1];
   std::memset(pszProjection, 0, Wkt.length() + 1);
   std::strcpy(pszProjection, Wkt.c_str());

   OGRSpatialReferenceH hSRS = OSRNewSpatialReference(NULL);
   if (OSRImportFromWkt(hSRS, &pszProjection) == CE_None) {
      const char* pparametervalue = OSRGetAttrValue(hSRS, Parameter.c_str(), Position);
      if (pparametervalue != NULL)
         retval.assign(pparametervalue);
      OSRDestroySpatialReference(hSRS);
   }

   return retval;
}

/* Devuelve el nombre del Sistema Geografico. Invoca a
 * GetSpatialReferenceParameter.
 * @param[in] Wkt wkt con la referencia espacial.
 * @return nombre del Sistema Geografico en caso de exito.
 * @return el valor de la constante message_NO_DATA(S/D) en caso de error.
 */
std::string SpatialReference::GetGeoCoordinateSystemName(const std::string &Wkt) {
   return GetSpatialReferenceParameter(Wkt, WKT_GEOGCS_NODE);
}

/* Devuelve las unidades del Sistema Geografico. Invoca a
 * GetSpatialReferenceParameter.
 * @param[in] Wkt wkt con la referencia espacial.
 * @return unidades del Sistema Geografico en caso de exito.
 * @return el valor de la constante message_NO_DATA(S/D) en caso de error.
 */
std::string SpatialReference::GetGeoCoordinateSystemUnits(const std::string &Wkt) {
   std::string nodepath = WKT_GEOGCS_NODE;
   nodepath += WKT_NODE_SEPARATOR;
   nodepath += WKT_UNIT_NODE;
   return GetSpatialReferenceParameter(Wkt, nodepath);
}

/* Devuelve el Datum del Sistema Geografico. Invoca a
 * GetSpatialReferenceParameter.
 * @param[in] Wkt wkt con la referencia espacial.
 * @return Datum del Sistema Geografico en caso de exito.
 * @return el valor de la constante message_NO_DATA(S/D) en caso de error.
 */
std::string SpatialReference::GetGeoCoordinateSystemDatumName(const std::string &Wkt) {
   std::string nodepath = WKT_GEOGCS_NODE;
   nodepath += WKT_NODE_SEPARATOR;
   nodepath += WKT_DATUM_NODE;
   return GetSpatialReferenceParameter(Wkt, nodepath);
}

/* Devuelve el Elipsoide del Sistema Geografico. Invoca a
 * GetSpatialReferenceParameter.
 * @param[in] Wkt wkt con la referencia espacial.
 * @return Elipsoide del Sistema Geografico en caso de exito.
 * @return el valor de la constante message_NO_DATA(S/D) en caso de error.
 */
std::string SpatialReference::GetGeoCoordinateSystemSpheroidName(const std::string &Wkt) {
   std::string nodepath = WKT_GEOGCS_NODE;
   nodepath += WKT_NODE_SEPARATOR;
   nodepath += WKT_SPHEROID_NODE;
   return GetSpatialReferenceParameter(Wkt, nodepath);
}

/**
 * Devuelve string (authority_name:authority_code) del Sistema Geografico
 * @param[in] Wkt wkt con la referencia.
 * @return string (authority_name:authority_code) de wkt en caso de exito.
 * @return el valor de la constante message_NO_DATA(S/D) en caso de error.
 */
std::string SpatialReference::GetGeoCoordinateSystemAuthorityData(const std::string &Wkt) {
   std::string wkt = Wkt;
   if (IsProjected(Wkt))
      wkt = GetNodeWkt(Wkt, WKT_GEOGCS_NODE);
   return GetAuthorityId(wkt);
}


/* Devuelve el nombre del Sistema Proyectado. Invoca a
 * GetSpatialReferenceParameter.
 * @param[in] Wkt wkt con la referencia espacial.
 * @return nombre del Sistema Proyectado en caso de exito.
 * @return el valor de la constante message_NO_DATA(S/D) en caso de error.
 */
std::string SpatialReference::GetProjCoordinateSystemName(const std::string &Wkt) {
   return GetSpatialReferenceParameter(Wkt, WKT_PROJCS_NODE);
}

/** Devuelve el nombre de la funcion de proyeccion */
std::string SpatialReference::GetProjCoordinateSystemProjectionName(const std::string &Wkt) {
   return GetSpatialReferenceParameter(Wkt, WKT_PROJECTION_NODE);
}

/* Devuelve las unidades del Sistema Proyectado. Invoca a
 * GetSpatialReferenceParameter.
 * @param[in] Wkt wkt con la referencia espacial.
 * @return unidades del Sistema Proyectado en caso de exito.
 * @return el valor de la constante message_NO_DATA(S/D) en caso de error.
 */
std::string SpatialReference::GetProjCoordinateSystemUnits(const std::string &Wkt) {
   std::string nodepath = WKT_PROJCS_NODE;
   nodepath += WKT_NODE_SEPARATOR;
   nodepath += WKT_UNIT_NODE;
   return GetSpatialReferenceParameter(Wkt, nodepath);
}

/** Devuelve string (authority_name:authority_code) del Sistema Proyectado */
std::string SpatialReference::GetProjCoordinateSystemAuthorityData(const std::string &Wkt) {
   if (!IsProjected(Wkt))
      return "";
   return SpatialReference::GetAuthorityId(Wkt);
}

/* Indica si el sistema coordenado es proyectado.
 * @param[in] Wkt wkt con la referencia espacial.
 * @return true en caso de que el sistema coordenado sea proyectado
 * @return false en caso de que el sistema coordenado no sea proyectado
 */
bool SpatialReference::IsProjected(const std::string &Wkt) {
   bool retval = false;

   char* pszProjection = new char[Wkt.length() + 1];
   std::memset(pszProjection, 0, Wkt.length() + 1);
   std::strcpy(pszProjection, Wkt.c_str());

   OGRSpatialReferenceH hSRS = OSRNewSpatialReference(NULL);
   if (OSRImportFromWkt(hSRS, &pszProjection) == CE_None) {
      retval = (OSRIsProjected(hSRS) == TRUE);
      OSRDestroySpatialReference(hSRS);
   } else {
      delete[] pszProjection;
   }
   return retval;
}

/* Indica si el sistema coordenado es geografico.
 * @param[in] Wkt wkt con la referencia espacial.
 * @return true en caso de que el sistema coordenado sea geografico
 * @return false en caso de que el sistema coordenado no sea geografico
 */
bool SpatialReference::IsGeographic(const std::string &Wkt) {
   bool retval = false;

   char* pszProjection = new char[Wkt.length() + 1];
   std::memset(pszProjection, 0, Wkt.length() + 1);
   std::strcpy(pszProjection, Wkt.c_str());

   OGRSpatialReferenceH hSRS = OSRNewSpatialReference(NULL);
   if (OSRImportFromWkt(hSRS, &pszProjection) == CE_None) {
      retval = (OSRIsGeographic(hSRS) == TRUE);
      OSRDestroySpatialReference(hSRS);
   }

   return retval;
}

/* 
 * Devuelve el wkt del Datum asociado al Sistema Geografico
 * @param[in] Wkt wkt con la referencia.
 * @return Datum del Sistema Geografico en caso de exito.
 * @return el valor de la constante message_NO_DATA(S/D) en caso de error.
 */
std::string SpatialReference::GetGeoCoordinateSystemDatum(const std::string &Wkt) {
   std::string nodepath = WKT_GEOGCS_NODE;
   nodepath += WKT_NODE_SEPARATOR;
   nodepath += WKT_DATUM_NODE;
   return GetNodeWkt(Wkt, nodepath);
}

/**
 * Devuelve el wkt del elipsoide asociado al Sistema Geografico
 * @param[in] Wkt wkt con la referencia espacial.
 * @return Wkt del elipsoide del Sistema Geografico en caso de exito.
 * @return el valor de la constante message_NO_DATA(S/D) en caso de error.
 */
std::string SpatialReference::GetGeoCoordinateSystemSpheroid(const std::string &Wkt) {
   std::string nodepath = WKT_GEOGCS_NODE;
   nodepath += WKT_NODE_SEPARATOR;
   nodepath += WKT_SPHEROID_NODE;
   return GetNodeWkt(Wkt, nodepath);
}


/** Determina si es un sistema de referencia "raster" **/
bool SpatialReference::IsPixelLineSpatialRef(const std::string& SRWkt ) {
   return (SRWkt.compare(RASTER_SPATIAL_REFERENCE) == 0);
}

/**
 * Crea el objeto a partir del WKT
 * Para verificar si es un sistema de referencia proyectado
 * @param[in] SRWkt WKT del sistema de coordenadas
 * @return true si es proyectado.
 * @return false si no es proyectado o ERROR
 */
bool SpatialReference::IsProjectedSpatialRef(const std::string &SRWkt) {
   /** Se comenta el codifo con #ifdef dado que ya existe una verificacion sobre
    *  si el Wkt es proyectado mediante OGR (en la Clase SpatialReference) **/
#ifdef __USE_OLD_SR_DETECTION__
   // Verifica el WKT de entrada
   if (!Wkt::IsValid(SRWkt)) {
      return false;
   }

   // Crea el WKT
   Wkt* psrinwkt = Wkt::Create(SRWkt);
   if (psrinwkt->GetRootValue() == WKT_PROJCS_NODE) {
      Wkt::Destroy(psrinwkt);
      return true;
   } else {
      Wkt::Destroy(psrinwkt);
      return false;
   }
#else
   return IsProjected(SRWkt);
#endif
}

/**
 * Crea el objeto a partir del WKT
 * Para verificar si es un sistema de referencia geografico
 * @param[in] SRWkt WKT del sistema de coordenadas
 * @return true si es geografico.
 * @return false si no es geografico o ERROR.
 */
bool SpatialReference::IsGeographicSpatialRef(const std::string &SRWkt) {
   /** Se comenta el codifo con #ifdef dado que ya existe una verificacion sobre
    *  si el Wkt es georreferenciado mediante OGR (en la Clase SpatialReference) **/
#ifdef __USE_OLD_SR_DETECTION__
   // Verifica el WKT de entrada
   if (!Wkt::IsValid(SRWkt)) {
      return false;
   }

   Wkt* psrinwkt = Wkt::Create(SRWkt);
   if (psrinwkt->GetRootValue() == WKT_GEOGCS_NODE) {
      Wkt::Destroy(psrinwkt);
      return true;
   } else {
      Wkt::Destroy(psrinwkt);
      return false;
   }
#else
   return IsGeographic(SRWkt);
#endif
}

/**
 * Devuelve string (authority_name:authority_code)
 * @param[in] Wkt wkt con la referencia.
 * @return string (authority_name:authority_code) de wkt en caso de exito.
 * @return el valor de la constante message_NO_DATA(S/D) en caso de error.
 */
std::string SpatialReference::GetAuthorityId(const std::string &Wkt) {
   suri::Wkt* psrswkt = Wkt::Create(Wkt);
   if (!psrswkt)
      return "";
   std::string authority = psrswkt->GetAuthorityName() + ":" +
                                             psrswkt->GetAuthorityCode();
   Wkt::Destroy(psrswkt);
   return authority;
}

/**
 * Trata de obtener wkt con autoridad
 */
std::string SpatialReference::GetWktWithAuthorityId(const std::string& AuthorityId) {
   std::string authority, code;
   size_t found = AuthorityId.find(AUTHORITYDATA_SEPARATOR);
   if (!found)
      return "";
   authority = AuthorityId.substr(0, found);
   code = AuthorityId.substr(found + 1);

   return GetWktWithAuthority(authority, code);
}


/** Trata de obtener wkt con autoridad */
std::string SpatialReference::GetWktWithAuthority(const std::string &AuthorityName,
                                          const std::string &AuthorityCode) {
   Wkt *pwkt = Wkt::CreateWithAuthorityCode(AuthorityName, AuthorityCode);
   if (!pwkt)
      return "";

   std::string wkt = pwkt->GetWktString();
   Wkt::Destroy(pwkt);
   return wkt;
}

/**
 * Devuelve un string con el wkt asociado
 * @return string con el wkt
 */
std::string SpatialReference::GetWkt() const {
   return wkt_;
}

/**
 * Retorna wkt del nodo especificado
 * @param[in] Wkt wkt con la referencia espacial.
 * @param[in] Parameter nombre del nodo del cual se quiere obtener un valor.
 * @return string con wkt del nodo especificado
 * @return el valor de la constante message_NO_DATA(S/D) en caso de error.
 */
std::string SpatialReference::GetNodeWkt(const std::string &Wkt,
                                         const std::string &Parameter) {
   OGRSpatialReference srs(Wkt.c_str());
   OGR_SRSNode* pnode = srs.GetAttrNode(Parameter.c_str());
   std::string returnvalue;
   if (pnode) {
      char* data;
      pnode->exportToWkt(&data);
      returnvalue = data;
      OGRFree(data);
   }
   return returnvalue;
}

/** Devuelve el eje semi mayor del elipsoide asociado al wkt*/
double SpatialReference::GetSpheroidSemiMajorAxis(const std::string Wkt) {
   if (Wkt.empty())
      return 0;
   OGRSpatialReference srs(Wkt.c_str());
   return srs.GetSemiMajor();
}

/** Devuelve el factor de achatamiento del elipsoide asociado al wkt*/
double SpatialReference::GetSpheroidInverseFlatteningFactor(
      const std::string Wkt) {
   if (Wkt.empty())
      return 0;
   OGRSpatialReference srs(Wkt.c_str());
   return srs.GetInvFlattening ();
}

/** Obtiene el wkt del elipsoide que contiene el datun **/
std::string SpatialReference::GetDatumSpheroidWkt(const std::string& Wkt) {
   return GetNodeWkt(Wkt, WKT_SPHEROID_NODE);
}

/** Devuelve el sistema geografico asociado al proyectado **/
std::string SpatialReference::GetProjGeoreferenceSpatialReference(
      const std::string &ProjectedWkt) {
   return GetNodeWkt(ProjectedWkt, WKT_GEOGCS_NODE);
}

/** Obtiene el wkt que representa los parametros bursa wolf. El mismo
 *  viene definido en un nodo del estilo:
 *  TOWGS84[-637,-549,-203,0,0,0,0],AUTHORITY["EPSG","6236"]]
 *  @return string con el wkt que define los params
 *  @return string vacio en caso de no encontrar el nodo wkt
 */
std::string SpatialReference::GetBursaWolfDatumParameters(const std::string& DatumWkt) {
   return GetNodeWkt(DatumWkt, BURSA_WOLF_PARAMETERS_NODE);
}

/** Verifica la validez del sistema de referencia que se pasa
 *  por parametro (en formato wkt)
 *  @param[in] Wkt sistema de referencia en Wkt
 *  @return true en caso de que el sistema de referencia sea valido
 *  @return false en caso contrario
 **/
bool SpatialReference::IsValidSpatialReference(const std::string& Wkt) {
	OGRSpatialReferenceH hSRS = OSRNewSpatialReference(NULL);
	char* pprojection = new char[Wkt.length() + 1];
	std::memset(pprojection, 0, Wkt.length() + 1);
	std::strcpy(pprojection, Wkt.c_str());
	bool ok = OSRImportFromWkt(hSRS, &pprojection) == CE_None;
	OSRDestroySpatialReference(hSRS);
	if (!ok){
		delete[] pprojection;
	}
	return ok;
}
}  // namespace suri
