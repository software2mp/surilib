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

#include "suri/Wkt.h"

// Includes standard
#include <algorithm>
#include <sstream>

// Includes otros
#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include "cpl_string.h"

//defines
#define EPSG_AUTHORITY_NAME "epsg"

/** namespace suri */
namespace suri {
/** Destructor */
Wkt::~Wkt() {
   delete pRoot_;
}

/** Constructor de copia */
/**
 * @param[in] SWkt objeto a copiar
 */
Wkt::Wkt(const Wkt &SWkt) {
   pRoot_ = SWkt.pRoot_->Clone();
}

/** Creador */
/**
 * @param[in] SWkt Valor de inicializacion del wkt
 * @return Wkt generado. NULL si no pudo crear
 */
Wkt * Wkt::Create(const std::string &SWkt) {
   if (SWkt.empty()) {
      return NULL;
   }

   char * wktstring = static_cast<char *>(CPLMalloc(SWkt.size() + 2));
   char * wktstringcopy = wktstring;
   strncpy(wktstring, SWkt.c_str(), SWkt.size() + 2);

   OGR_SRSNode * pnode = new OGR_SRSNode;

   if (pnode->importFromWkt(&wktstringcopy) != OGRERR_NONE) {
      delete pnode;
      return NULL;
   }

   CPLFree(wktstring);
   Wkt *pwkt = new Wkt;
   pwkt->pRoot_ = pnode;
   return pwkt;
}

/**
 * Crea el WKT a partir de un codigo y autoridad. Actualmente solo
 * funciona con codigos epsg.
 * @param[in] Authority nombre de la autoridad
 * @param[in] Code codigo que identifica la proyeccion
 * @return Wkt* puntero a wkt creado a partir de codigo epsg
 * \attention el puntero retornado es responsabilidad del invocante
 * \deprecated La clase wkt no deberia estar vinculada a un
 * formato/autoridad particular, en este caso srs/EPSG.
 * Usar SpatialReference::GetWktWithAuthority
 */
Wkt* Wkt::CreateWithAuthorityCode(const std::string &Authority,
                                  const std::string &Code) {
   std::string authority = Authority;
   std::transform(authority.begin(), authority.end(), authority.begin(), ::tolower);

   if (authority.compare(EPSG_AUTHORITY_NAME) != 0) return NULL;

   int number = 0;
   std::istringstream ssnumber(Code);
   ssnumber >> number;

   OGRSpatialReference* pogrsr = new OGRSpatialReference;
   if (ssnumber.fail() || pogrsr->importFromEPSG(number) != OGRERR_NONE) return NULL;

   char* wktstring;

   if (pogrsr->exportToWkt(&wktstring) != OGRERR_NONE) return NULL;

   Wkt* returnvalue = Wkt::Create(wktstring);

   CPLFree(wktstring);
   OGRSpatialReference::DestroySpatialReference(pogrsr);

   return returnvalue;
}

/**
 * Destruye el WKT
 * @param[in] pWkt Wkt que se quiere eliminar
 */
void Wkt::Destroy(Wkt * &pWkt) {
   delete pWkt;
   pWkt = NULL;
}

/**
 * Consulta si es valido el wkt
 * @param[in] SWkt
 * @return informa si el wkt es valido
 */
bool Wkt::IsValid(const std::string &SWkt) {
   Wkt *pwkt = Create(SWkt);
   if (pwkt == NULL) {
      return false;
   }
   Destroy(pwkt);
   return true;
}

// ------------------- METODOS DE CONSULTA -------------------------------------
/** Devuelve el WKT en formato string a partir del path referido */
/**
 * @param[in] PathName Path completo de busqueda a partir del que se devuelve el sub-wkt
 * Por defecto se devuelve la todalidad. Ej: "GEOGS|UNITS"
 * @return WKT en formato string, devuelve string vacio si no encontro nada
 */
std::string Wkt::GetWktString(const std::string &PathName) const {
   OGR_SRSNode *pNode = FindNode(PathName);

   if (pNode == NULL) {
      return std::string();
   }

   char * wkt;
   if (pNode->exportToWkt(&wkt) != OGRERR_NONE) {
      return std::string();
   }
   std::string swkt(wkt);

   CPLFree(wkt);
   return swkt;
}

/** Devuelve el WKT en formato string "lindo" (para lectura mas simple) a partir del path referido */
/**
 * @param[in] PathName Path completo de busqueda a partir del que se devuelve el sub-wkt
 * Por defecto se devuelve la totalidad. Ej: "GEOGS|UNITS"
 * @return WKT en formato string para lectura mas simple
 */
std::string Wkt::GetPrettyWktString(const std::string &PathName) const {
   OGR_SRSNode *pNode = FindNode(PathName);

   if (pNode == NULL) {
      return std::string();
   }

   char * wkt;
   if (pNode->exportToPrettyWkt(&wkt) != OGRERR_NONE) {
      return std::string();
   }
   std::string swkt(wkt);

   CPLFree(wkt);
   return swkt;
}

/** Devuelve el valor del nodo */
/**
 * @return Un string con el valor del nodo raiz, o un string vacio si no hay nada
 */
std::string Wkt::GetRootValue() const {
   return std::string(pRoot_->GetValue());
}

/** Devuelve los valores de los hijos que tienen hijos */
/**
 * @param[in] PathName Path completo de busqueda del nodo. Ej: "GEOGS|UNITS"
 * @return Vector con los valores (en string) de los hijos con hijos (nodos)
 */
std::vector<std::string> Wkt::GetNodeChildren(const std::string &PathName) const {
   OGR_SRSNode *pNode = FindNode(PathName);
   std::vector<std::string> values;

   if (pNode == NULL) {
      return values;
   }

   int totalchildren = pNode->GetChildCount();

   for (int i = 0; i < totalchildren; i++)
      if (pNode->GetChild(i)->GetChildCount() > 0) {
         values.push_back(
               GetWktString(PathName + "|" + pNode->GetChild(i)->GetValue()));
      }
   return values;
}

/** Devuelve los valores de los hijos que no tienen hijos */
/**
 * @param[in] PathName Path completo de busqueda del nodo. Ej: "GEOGS|UNITS"
 * @return Vector con los valores (en string) de los hijos sin hijos (valores)
 */
std::vector<std::string> Wkt::GetLeafChildren(const std::string &PathName) const {
   OGR_SRSNode *pNode = FindNode(PathName);
   std::vector<std::string> values;

   if (pNode == NULL) {
      return values;
   }

   int totalchildren = pNode->GetChildCount();

   for (int i = 0; i < totalchildren; i++)
      if (pNode->GetChild(i)->GetChildCount() == 0) {
         values.push_back(std::string(pNode->GetChild(i)->GetValue()));
      }

   return values;
}

/**
 * Retorna el nombre de la autoridad del wkt
 * @return std::string con el nombre de la autoridad
 */
std::string Wkt::GetAuthorityName() const {
   std::string authority = "";

   OGR_SRSNode *pNode = FindNode("");
   char * wkt = NULL;
   OGRSpatialReference* pogrsr = new OGRSpatialReference;
   if (pNode && pNode->exportToWkt(&wkt) == OGRERR_NONE) {
      char *tempwkt = wkt;
      if (pogrsr && pogrsr->importFromWkt(&tempwkt) == OGRERR_NONE) {
         if (pogrsr->GetAuthorityName(NULL)) authority = pogrsr->GetAuthorityName(NULL);
      }
   }
   OGRSpatialReference::DestroySpatialReference(pogrsr);
   CPLFree(wkt);

   return authority;
}

/**
 * Retorna el codigo asignado a la raiz del wkt por la autoridad correspondiente
 * @return std::string con el codigo de la raiz del wkt.
 */
std::string Wkt::GetAuthorityCode() const {
   std::string code = "";

   OGR_SRSNode *pNode = FindNode("");
   char * wkt = NULL;
   OGRSpatialReference* pogrsr = new OGRSpatialReference;
   if (pNode && pNode->exportToWkt(&wkt) == OGRERR_NONE) {
      char *tempwkt = wkt;
      if (pogrsr && pogrsr->importFromWkt(&tempwkt) == OGRERR_NONE) {
         if (pogrsr->GetAuthorityCode(NULL)) code = pogrsr->GetAuthorityCode(NULL);
      }
   }
   OGRSpatialReference::DestroySpatialReference(pogrsr);
   CPLFree(wkt);

   return code;
}

// ------------------- METODOS DE MODIFICACION ---------------------------------
/** Setea el valor del nodo */
/**
 * @param[in] RootValue completo de busqueda del nodo. Ej: "GEOGS|UNITS"
 * @return Verdadero si tuvo exito, falso cuando se pasa un string vacio
 */
bool Wkt::SetRootValue(const std::string &RootValue) {
   if (RootValue.empty()) {
      return false;
   }

   pRoot_->SetValue(RootValue.c_str());
   return true;
}

/** Borra el contenido a partir del path referido */
/**
 * @param[in] PathName Path completo de busqueda del nodo. Ej: "GEOGS|UNITS"
 * @return Verdadero si tuvo exito, falso cuando no encuentra el path
 */
bool Wkt::Delete(const std::string &PathName) {
   OGR_SRSNode *pNode = FindNode(PathName);

   if (pNode == NULL) {
      return false;
   }
   if (pNode == pRoot_) {
      std::string rootvalue = GetRootValue();
      delete pRoot_;
      pRoot_ = NULL;
      pRoot_ = new OGR_SRSNode();
      SetRootValue(rootvalue);
      return true;
   }

   // Busca el padre del nodo a destruir
   char ** pathtokens = CSLTokenizeStringComplex(PathName.c_str(), "|", TRUE, FALSE);
   int totalcount = CSLCount(pathtokens);
   pNode = pRoot_;
   for (int i = 0; i < totalcount - 1; i++)
      pNode = pNode->GetNode(pathtokens[i]);

   // Destruye el nodo
   pNode->DestroyChild(pNode->FindChild(pathtokens[totalcount - 1]));
   CSLDestroy(pathtokens);

   return true;
}

/** Inserta el path indicado */
/**
 * @param[in] PathName Path completo de busqueda del nodo. Ej: "GEOGS|UNITS". Si no existe, lo crea
 * @return Verdadero si tuvo exito, falso en caso de falla
 */
bool Wkt::InsertPath(const std::string &PathName) {
   std::string pathname = PathName;

   if (pathname.empty()) {
      pathname = GetRootValue();
   }

   char ** pathtokens;
   pathtokens = CSLTokenizeStringComplex(pathname.c_str(), "|", TRUE, FALSE);

   if (CSLCount(pathtokens) < 1) {
      return false;
   }

   OGR_SRSNode *pNode = pRoot_;
   int index = 0;

   while (pathtokens[index] != NULL && pNode->GetNode(pathtokens[index]) != NULL) {
      pNode = pNode->GetNode(pathtokens[index]);
      index++;
   }

   while (pathtokens[index] != NULL) {
      OGR_SRSNode * pchild = new OGR_SRSNode(pathtokens[index]);
      pNode->AddChild(pchild);
      pNode = pchild;
      index++;
   }

   CSLDestroy(pathtokens);
   return true;
}

/** Inserta los valores en el path indicado */
/**
 * @param[in] PathName Path completo de busqueda del nodo. Ej: "GEOGS|UNITS". Si no existe, lo crea
 * @param[in] Values Vector con los valores a insertar en el nodo
 * @return Verdadero si tuvo exito, falso en caso de falla
 */
bool Wkt::InsertValue(const std::vector<std::string> &Values,
                      const std::string &PathName) {
   if (!InsertPath(PathName)) {
      return false;
   }

   OGR_SRSNode *pNode = FindNode(PathName);
   if (pNode == NULL) {
      return false;
   }

   int totalvalues = Values.size();
   for (int i = 0; i < totalvalues; i++) {
      OGR_SRSNode * pchild = new OGR_SRSNode(Values.at(i).c_str());
      pNode->AddChild(pchild);
   }

   return true;
}

/** Inserta el valor en el path indicado */
/**
 * @param[in] PathName Path completo de busqueda del nodo. Ej: "GEOGS|UNITS". Si no existe, lo crea
 * @param[in] Value String con valor a insertar
 * @return Verdadero si tuvo exito, falso en caso de falla
 */
bool Wkt::InsertValue(const std::string &Value, const std::string &PathName) {
   std::vector<std::string> values;
   values.push_back(Value);
   return InsertValue(values, PathName);
}

/** Inserta el Wkt en el path indicado */
/**
 * @param[in] PathName Path completo de busqueda del nodo. Ej: "GEOGS|UNITS". Si no existe, lo crea
 * @param[in] SWkt Wkt a insertar, si esta vacio se creara el path unicamente
 * @return Verdadero si tuvo exito, falso en caso de falla
 */
bool Wkt::InsertWkt(const std::string &SWkt, const std::string &PathName) {
   char * wktstring = static_cast<char *>(CPLMalloc(SWkt.size() + 2));
   char * wktstringcopy = wktstring;
   strncpy(wktstring, SWkt.c_str(), SWkt.size() + 2);

   OGR_SRSNode * pnewnode = new OGR_SRSNode;
   if (pnewnode->importFromWkt(&wktstringcopy) != OGRERR_NONE) {
      delete pnewnode;
      CPLFree(wktstring);
      return false;
   }
   CPLFree(wktstring);  // \todo esto podria ir antes del if, y evitarlo 2 veces
   // \todo esto podria meterse en el if de arriba
   if (!InsertPath(PathName)) {
      delete pnewnode;
      return false;
   }

   OGR_SRSNode *pNode = FindNode(PathName);
   if (pNode == NULL) {
      delete pnewnode;
      return false;
   }

   pNode->AddChild(pnewnode);
   return true;
}

/**
 * Devuelve el WKT en formato Proj4.
 */
std::string Wkt::ToProj4() {
   std::string proj4;
   char* pwkt = NULL;

   OGR_SRSNode* pnode = FindNode("");

   OGRSpatialReference* pogrsr = new OGRSpatialReference;
   if (pnode && pnode->exportToWkt(&pwkt) == OGRERR_NONE) {
      char* ptempwkt = pwkt;
      if (pogrsr && pogrsr->importFromWkt(&ptempwkt) == OGRERR_NONE) {
         char* pproj4 = NULL;
         pogrsr->exportToProj4(&pproj4);
         proj4 = pproj4;
         CPLFree(pproj4);
      }
   }
   OGRSpatialReference::DestroySpatialReference(pogrsr);

   CPLFree(pwkt);

   return proj4;
}

/** Recorre el arbol siguiendo el path indicado y devuelve el puntero al nodo */
/**
 * @param[in] PathName Path completo de busqueda a partir del que se devuelve el sub-wkt
 * , por defecto se devuelve el nodo. Ej: "GEOGS|UNITS"
 * @return WKT en formato string, devuelve string vacio si no encontro nada
 */
OGR_SRSNode * Wkt::FindNode(const std::string &PathName) const {
   char ** pathtokens;
   OGR_SRSNode *pNode = pRoot_;

   if (!PathName.empty()) {
      pathtokens = CSLTokenizeStringComplex(PathName.c_str(), "|", TRUE, FALSE);

      if (CSLCount(pathtokens) < 1) {
         return NULL;
      }

      // Verifica nombre de la raiz
      int i = 0;
      if (std::string(pathtokens[i]) != std::string(pNode->GetValue())) {
         pNode = NULL;
      }
      i++;

      for (; pathtokens[i] != NULL && pNode->FindChild(pathtokens[i]) >= 0; i++)
         pNode = pNode->GetChild(pNode->FindChild(pathtokens[i]));

      if (pathtokens[i] != NULL) {
         pNode = NULL;
      }

      CSLDestroy(pathtokens);
   }

   return pNode;
}

/**
 * Compara dos Wkt para ver si son iguales.
 * Toma los numeros con decimal y los trunca para que tengan la misma precision,
 * el resto es una comparacion de strings.
 * @param lhs Lado izquierdo de la comparacion
 * @param rhs Lado derecho de la comparacion
 * @return True si los string son iguales
 * @return True si luego de truncar los valores de punto flotante a la misma
 *         precision, los string siguen siendo iguales
 * @return False Los string no son iguales, incluso luego de truncar los numeros
 *         de punto flotante.
 */
bool WktCompare(const std::string &lhs, const std::string &rhs) {
   if (lhs == rhs) {
      return true;
   }
   std::string::size_type lpointpos = lhs.find('.'), loffset = 0, rpointpos = rhs.find(
         '.'), roffset = 0;
   // busco los valores reales, cuando lo encuentro los dejo con la menor precision
   // encontrada.
   // lo utilizo en forma generica para indicar la consistencia de los wkts
   bool wktconsistent = true;
   // wkts temporales arreglados para que comparen iguales como string
   std::string fixedlhs, fixedrhs;
   while (lpointpos != std::string::npos && rpointpos != std::string::npos
         && wktconsistent) {
      // acumulo los wkt hasta el punto
      fixedlhs += lhs.substr(loffset, lpointpos + 1 - loffset);
      fixedrhs += rhs.substr(roffset, rpointpos + 1 - roffset);
      // busco el fin de los decimales
      std::string::size_type ldecpos = lhs.find_first_not_of("0123456789",
                                                             lpointpos + 1);
      std::string::size_type rdecpos = rhs.find_first_not_of("0123456789",
                                                             rpointpos + 1);
      // si la longitud de los decimales es la misma, copio y sigo
      if (ldecpos - lpointpos == rdecpos - rpointpos) {
         fixedlhs += lhs.substr(lpointpos + 1, ldecpos - lpointpos + 1);
         fixedrhs += rhs.substr(rpointpos + 1, rdecpos - rpointpos + 1);
      } else {
         // copio la menor cantidad de decimales
         std::string::size_type minsize = std::min(ldecpos - lpointpos,
                                                   rdecpos - rpointpos) - 1;
         fixedlhs += lhs.substr(lpointpos + 1, minsize);
         fixedrhs += rhs.substr(rpointpos + 1, minsize);
      }
      loffset = ldecpos;
      roffset = rdecpos;
      lpointpos = lhs.find('.', loffset);
      rpointpos = rhs.find('.', roffset);
      wktconsistent = fixedlhs == fixedrhs;
   }
   // acumulo los wkt hasta el final
   fixedlhs += lhs.substr(loffset);
   fixedrhs += rhs.substr(roffset);
   return fixedlhs == fixedrhs;
}
} /*namespace suri*/

