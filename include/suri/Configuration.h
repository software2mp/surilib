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

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

// Include standard
#include <string>
#include <vector>
#include <map>

// Include suri
#include "suri.h"

#define VETYPE_PARAM_NAME        "VEType"
#define VETYPE_PARAM_SIMULATED   "VETypeSimulated"
#define VETYPE_PARAM_UNDEFINED   "VETypeUndefined"

#define VETYPE_PARAM_MODE_NAME         "VETypeMode"
#define VETYPE_PARAM_MODE_CREATE       "VETypeModeCreate"
#define VETYPE_PARAM_MODE_EDIT         "VETypeModeEdit"
#define VETYPE_PARAM_MODE_UNDEFINED    "VETypeModeUndefined"
#define VETYPE_PARAM_MODE_DELETE_NODE  "VETypeModeDeleteNode"

// forward wx
class wxXmlDocument;
class wxXmlNode;

/** namespace suri */
namespace suri {
/**
 * Estas son 3 funciones que validan la existencia de los archivos necesarios
 * para la ejecucion del sistema. Los mismos estan detallados en el archivo
 * "manifiesto".
 */

/** Devuelve los url de los archivos detallados en el archivo manifiesto. */
std::vector<std::string> GetManifestFiles();

/** Verifica la existencia de los archivos detallados en el archivo manifiesto. */
bool CheckManifestFiles(std::vector<std::string> &MissingFiles);

/** Verifica la existencia de los archivos detallados en el vector. */
bool CheckFiles(const std::vector<std::string> &Files,
                std::vector<std::string> &MissingFiles);

/** Clase que implementa un sistema de basqueda y seteo de pares CLAVE=VALOR */
/**
 * Clase que implementa un sistema de basqueda y seteo de pares CLAVE=VALOR
 * dentro del xml configuracion.xml que debe estar en el directorio del
 * ejecutable.
 * El configuracion.xml tiene un nodo raiz configuration y dentro de este
 * deben ir todos los nodos.
 * Por el momento no soporta anidamiento de nodos sino que solo soporta un nivel
 * por debajo de configuration
 */

class Configuration {
   /** Constructor de la Configuration */
   Configuration();
   /** Constructor de la Configuration */
   virtual ~Configuration();
public:
   /** Devuelve el valor del parametro Name como String. */
   static std::string GetParameter(const std::string &Name,
                                   const std::string &DefaultValue);
   /** Devuelve el valor del parametro pName como String. */
   static std::string GetParameter(const char *pName, const char *pDefaultValue);
   /** Devuelve el valor del parametro Name como Double. */
   static double GetParameter(const std::string &Name, const double DefaultValue);
   /** Devuelve el valor del parametro Name como Bool. */
   static bool GetParameter(const std::string &Name, bool DefaultValue);
   /** Establece el valor Value en el parametro Name. */
   static void SetParameter(const std::string &Name, const std::string &Value);
   /** Establece el valor Value en el parametro Name. */
   static void SetParameter(const std::string &Name, const long Value);
   /** Establece el valor Value en el parametro Name. */
   static void SetParameter(const std::string &Name, const double Value);
   /** Devuelve el valor del parametro Name como wxXmlNode. */
   static wxXmlNode* GetParameterNode(const std::string& Name, const wxXmlNode* pDefaultValue);
   /** Devuelve el valor del parametro Name como Long. */
   static long GetParameter(const std::string &Name, const long DefaultValue);
   /** Devuelve el valor del parametro Name como Long. */
   static int GetParameter(const std::string &Name, const int DefaultValue);
   /** Devuelve el valor del parametro Name como String desde un cache en memoria. */
   static std::string GetParameterEx(const std::string& Name, const std::string& DefaultValue);
   /** Establece el valor Value en el parametro Name en un cache en memoria. */
   static void SetParameterEx(const std::string& Name, const std::string& Value);
   /** Remueve el valor con la clave Name del cache en memoria. */
   static void RemoveParameterEx(const std::string& Name);
   /** Indica el nombre y ruta completos y absolutos del archivo a usar */
   static bool SetConfigFile(const std::string &UserFilename,const std::string &SystemFilename="");
   /** Guarda la configuracion en el archivo */
   static void Save(const std::string &ConfigurationFile = "");
   /** Carga la configuracion */
   static bool Load();
   /** Expande las variables */
   static bool Expand(std::string &Value);
   static std::string GetParameter(std::string Name, const char* Value);
   static void SetParameter(std::string &Name, std::string Value);
   static std::string GetParameter(std::string &Name, std::string& Value);
   /** Devuelve el valor del parametro Name como String. */
   static void SetParameter(std::string& Name, std::string& Value);
   static std::string GetParameter(const char *pName, const std::string &DefaultValue);
   static void DiscardChanges();
   static void SetParameterEx(const std::string& Name, int Value);
   static int GetParameterEx(const std::string& Name, const int Value);
private:
   /** Devuelve el valor del parametro Name como String. */
   static std::string GetParameterXml(const char * Name, const char *pDefaultValue);
   static bool changed_; /*indica si se modificaron los datos para cuando hace save**/
   static void SetParameters();
   static void SetAttribute(wxXmlNode *Pnode, const wxString& name, const wxString& value);
   static wxXmlDocument *pXmlDoc_; /*! documento XML configuracion de sistema */
   static wxXmlDocument *pXmlUserDoc_; /* documento con configuracion de usuario */
   static wxXmlDocument *pXmlSystemDoc_;
   static std::string configurationFile_; /*! archivo de configuracion */
   static std::string configurationUserFile_;
   static std::string configurationSystemFile_;
   static std::string GetParameterFromMem(std::string&Name);
   static std::string GetParameterFromXml(std::string& Name);
   static std::map<std::string, std::string> memParams_;
   static std::map<std::string, std::string> memUserParams_;
   static std::map<std::string, std::string> memSystemParams_;
   static std::map<std::string, std::string> memBackup_; /* para volver atras los cambios */
};
}
#endif /*CONFIGURATION_H_*/
