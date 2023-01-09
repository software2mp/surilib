/*! \file Configuration.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#include <map>
#include <stdexcept>
#include "suri/Configuration.h"

/** includes de wx */
#include "wx/wx.h"
#include "wx/xml/xml.h"
#include "wx/textfile.h"
#include "wx/filename.h"

/** namespace suri */
namespace suri {
// ------------------------------------------------------------------------------
// --------FUNCIONES PARA VERIFICAR ARCHIVOS NECESARIOS PARA LA EJECUCION--------
// ------------------------------------------------------------------------------

/**
 * Objetivo: Devuelve los url de los archivos necesarios para que el sistema
 * corra. Los mismos estan detallados en el archivo manifiesto.
 * @return vector con los path de los archivos.
 */
std::vector<std::string> GetManifestFiles() {
   // el archivo de manifiesto
   std::string manifiestpath = Configuration::GetParameter("app_needed_files",
                                                           wxT(""));

   // obtengo la ruta de data
   std::string datapath = Configuration::GetParameter("app_data_dir", wxT("./"));

   wxTextFile txtfile;
   if (manifiestpath.empty() || !txtfile.Open(manifiestpath)) {
      return std::vector<std::string>();
   }

   std::vector<std::string> pathfiles;
   // recorro el archivo insertando las lineas en el vector pathfiles.
   wxString line = wxT("");
   for (line = txtfile.GetFirstLine(); !txtfile.Eof(); line = txtfile.GetNextLine())
      if (!line.IsEmpty()) {
         line.Prepend(datapath);
         pathfiles.push_back(line.c_str());
      }

   // cargo la ultima linea.
   if (!line.IsEmpty()) {
      pathfiles.push_back(line.c_str());
   }

   return pathfiles;
}

/**
 * Objetivo: Verifica la existencia de los archivos cuyos paths se detallan en
 * el vector Files.
 * @param[out] MissingFiles vector con los paths de los archivos inexistentes.
 * @return existen_todos_los_archivos? true : false;
 */
bool CheckManifestFiles(std::vector<std::string> &MissingFiles) {
   return CheckFiles(GetManifestFiles(), MissingFiles);
}

/**
 * Objetivo: Verifica la existencia de los archivos cuyos paths se detallan en
 * el vector Files.
 * @param[in]  Files vector con los paths de los archivos a verificar.
 * @param[out] MissingFiles vector con los paths de los archivos inexistentes.
 * @return existen_todos_los_archivos? true : false;
 */
bool CheckFiles(const std::vector<std::string> &Files,
                std::vector<std::string> &MissingFiles) {
   MissingFiles.clear();
   for (size_t i = 0; i < Files.size(); i++) {
      wxFileName fn(Files.at(i));
      if (!fn.FileExists()) {
         MissingFiles.push_back(Files.at(i));
      }
   }

   return MissingFiles.empty();
}

// ------------------------------------------------------------------------------
// ------FIN FUNCIONES PARA VERIFICAR ARCHIVOS NECESARIOS PARA LA EJECUCION------
// ------------------------------------------------------------------------------

// Se reserva memoria aca porque la clase es estatica
wxXmlDocument* Configuration::pXmlDoc_ = new wxXmlDocument;
wxXmlDocument* Configuration::pXmlUserDoc_= new wxXmlDocument;
wxXmlDocument* Configuration::pXmlSystemDoc_= new wxXmlDocument;
std::string Configuration::configurationFile_ = wxT("configuracion.xml");
std::string Configuration::configurationUserFile_ = wxT("configuracion.xml");;
std::string Configuration::configurationSystemFile_ = wxT("configuracion.xml");
std::map<std::string, std::string> Configuration::memUserParams_;
std::map<std::string, std::string> Configuration::memSystemParams_;
std::map<std::string, std::string> Configuration::memParams_;
std::map<std::string, std::string > Configuration::memBackup_;

bool Configuration::changed_=false;

/** Constructor */
Configuration::Configuration() {
}

/** Destructor */
Configuration::~Configuration() {
}

// FUNCIONES DE GET--------------------------------------------------------------
/** Devuelve el valor del parametro Name como String. */
/**
 *  Usa GetParamenter y devuelve un std::string
 *
 * @param[in] Name clave
 * @param[in] DefaultValue valor por defecto del contenido de la clave
 * @return Valor del parametro Name o DefaultValue si no se encuentra
 */
std::string Configuration::GetParameter(const std::string &Name,
                                        const std::string &DefaultValue) {
   return GetParameter(Name.c_str(), DefaultValue.c_str());
}




/** Devuelve el valor del parametro Name como String. */
/**
 *  Usa GetParamenter y devuelve un std::string
 *
 * @param[in] pName clave
 * @param[in] DefaultValue valor por defecto del contenido de la clave
 * @return Valor del parametro pName o DefaultValue si no se encuentra
 */
std::string Configuration::GetParameter(const char *pName,
                                        const std::string &DefaultValue) {
   return GetParameter(pName, DefaultValue.c_str());
}

std::string Configuration::GetParameter(std::string Name, const char* Value){
   std::string name(Name);
   std::string value(Value);
   return GetParameter(name,value);
}

/** Devuelve el valor del parametro Name como int */
/**
 *  Usa GetParamenter y devuelve un int.
 *
 * @param[in] Name clave
 * @param[in] DefaultValue valor por defecto del contenido de la clave
 * @return Valor del parametro Name o DefaultValue si no se encuentra
 */
int Configuration::GetParameter(const std::string &Name, const int DefaultValue) {
   return static_cast<int>(GetParameter(Name, static_cast<long>(DefaultValue)));
}

std::string Configuration::GetParameter(std::string &Name, std::string & Value){
   std::string result = GetParameterFromMem(Name);
   if (result != "") return result;
   result = GetParameterFromXml(Name);
   if (result != "") return result;
   return Value;
}

std::string Configuration::GetParameter(const char *pName, const char *pDefaultValue){
   std::string name(pName);
   std::string value(pDefaultValue);
   return GetParameter(name,value);
}

// ------------------------------------------------------------------------------
/** Devuelve el valor del parametro Name como Long. */
/**
 *  Usa GetParamenter y devuelve un long.
 *
 * @param[in] Name clave
 * @param[in] DefaultValue valor por defecto del contenido de la clave
 * @return Valor del parametro Name o DefaultValue si no se encuentra
 */
long Configuration::GetParameter(const std::string &Name, const long DefaultValue) {
   std::string str = Configuration::GetParameter(Name.c_str(), "");

   // Castea el parametro a Long
   wxString wxstr(str.c_str(), wxConvUTF8);
   long value = DefaultValue;

   if (!wxstr.ToLong(&value))
      value = DefaultValue;

   return value;
}

// ------------------------------------------------------------------------------
/** Devuelve el valor del parametro Name como Double. */
/**
 *  Usa GetParamenter y devuelve un double.
 *
 * @param[in] Name clave
 * @param[in] DefaultValue valor por defecto del contenido de la clave
 * @return Valor del parametro Name o DefaultValue si no se encuentra
 */

double Configuration::GetParameter(const std::string &Name, const double DefaultValue) {
   std::string str = Configuration::GetParameter(Name, "");

   // Castea el parametro a double
   wxString wxstr(str.c_str(), wxConvUTF8);
   double value = DefaultValue;

   if (!wxstr.ToDouble(&value))
      value = DefaultValue;

   return value;
}

/** Devuelve el valor del parametro Name como Bool. */
/**
 *  Usa GetParamenter y devuelve un bool.
 *
 * @param[in] Name clave
 * @param[in] DefaultValue valor por defecto del contenido de la clave
 * @return Valor del parametro Name o DefaultValue si no se encuentra
 */
bool Configuration::GetParameter(const std::string &Name, bool DefaultValue) {
   std::string str = Configuration::GetParameter(Name.c_str(), "");
   return str == "" ? DefaultValue : str != "0";
}

/**
 * Devuelve el valor del parametro Name como String desde un cache en memoria.
 *
 * @param[in] Name Clave
 * @param[in] DefaultValue Valor por defecto del contenido de la clave
 * @return Valor del parametro Name o DefaultValue si no se encuentra
 */
std::string Configuration::GetParameterEx(const std::string& Name,
                                          const std::string& DefaultValue) {
   std::string retval = DefaultValue;
   if (memParams_.find(Name) != memParams_.end())
      retval = memParams_[Name];
   return retval;
}

/** Devuelve el valor del parametro pName como String del xml. */
/**
 * @param[in] pName clave
 * @param[in] pDefaultValue valor por defecto del contenido de la clave
 * @return Valor del parametro pName o pDefaultValue si no se encuentra
 */
std::string Configuration::GetParameterXml(const char *pName, const char *pDefaultValue) {
   if (!Load()) {
      return pDefaultValue;
   }

   wxString wxname(pName, wxConvUTF8);
   // Recorre el documento
   wxXmlNode *pchild = pXmlDoc_->GetRoot()->GetChildren();
   while (pchild) {
      // Si encuentra el elemento devuelve true, asigna a Value el valor y
      // corta la ejecucian
      if (pchild->GetName() == wxname) {
         wxString wxstr = pchild->GetNodeContent();
         std::string value = std::string(wxstr.mb_str());
         Expand(value);
         return value;
      }
      pchild = pchild->GetNext();
   }
   // Si no encontra el elemento devuelve false
   std::string defaultexpanded = pDefaultValue;
   Expand(defaultexpanded);
   return defaultexpanded;
}

/**
 * Devuelve el valor del parametro Name como wxXmlNode.
 * lo obtiene del configuracion.xml
 */
wxXmlNode* Configuration::GetParameterNode(const std::string& Name, const wxXmlNode* pDefaultValue) {
   pXmlDoc_=pXmlSystemDoc_;
   configurationFile_=configurationSystemFile_;
   if (!Load())
      return const_cast<wxXmlNode*>(pDefaultValue);
   wxString wxname(Name.c_str(), wxConvUTF8);
   wxXmlNode* pchild = pXmlDoc_->GetRoot()->GetChildren();
   while (pchild) {
      if (pchild->GetName() == wxname)
         return pchild;
      pchild = pchild->GetNext();
   }
   return const_cast<wxXmlNode*>(pDefaultValue);
}
/** retorna parametro de memoria, si no existe string vacio **/
std::string Configuration::GetParameterFromMem(std::string& Name) {
   std::string result = "";
   if (memUserParams_.find(Name) != memUserParams_.end()) {
      result = memUserParams_.at(Name);
   } else if (memSystemParams_.find(Name) != memSystemParams_.end()) {
      result = memSystemParams_.at(Name);
   } else {
         return "";
   }
   return result;
}

/** retorna parametro de xml, primero busca en configuracion.xml
 * , si no lo encuentra busca en SoPI.xml, si encuentra el campo antes de retornarlo
 * lo carga en el map correspondiente**/

std::string Configuration::GetParameterFromXml(std::string& Name){
   std::string result = "";
   pXmlDoc_ = pXmlSystemDoc_;
   configurationFile_ = configurationSystemFile_;
   result = GetParameterXml(Name.c_str(), "");
   if (result != "") {
      memSystemParams_[Name] = result;
      return result;
   }
   pXmlDoc_ = pXmlUserDoc_;
   configurationFile_ = configurationUserFile_;
   result = GetParameterXml(Name.c_str(), "");
   if (result != "")
      memUserParams_[Name] = result;
   return result;
}
// FUNCIONES DE SET--------------------------------------------------------------
#ifdef __UNUSED_CODE__
/**
 * Establece el valor Value en el parametro Name.
 * En caso que el parametro Name no exista en el nivel 1 del
 * configuracion.xml lo agrega
 * @param[in] Name parametro al cual se le quiere asignar un valor
 * @param[in] Value valor que se quiere asignar al parametro
 */
void Configuration::SetParameter(const std::string &Name, const std::string &Value) {
   if (!Load()) {
      return;
   }

   wxString wxnodename(Name.c_str(), wxConvUTF8);
   wxString wxnodevalue(Value.c_str(), wxConvUTF8);

   bool found = false;

   // Recorre el documento
   wxXmlNode *pnode = pXmlDoc_->GetRoot()->GetChildren();
   while (pnode) {
      // Si encuentra el elemento setea el contenido
      if (pnode->GetName() == wxnodename) {
         if (!pnode->GetChildren()) {
            pnode->SetChildren(
                  new wxXmlNode(pnode, wxXML_TEXT_NODE, wxnodename, wxnodevalue));
         } else {
            pnode->GetChildren()->SetContent(wxnodevalue);
         }
         found = true;
      }
      pnode = pnode->GetNext();
   }

   // Si no encontra el elemento lo agrega en el documento
   if (!found) {
      wxXmlNode* pnewnode = new wxXmlNode(pXmlDoc_->GetRoot(), wxXML_ELEMENT_NODE,
                                          wxnodename);
      wxXmlNode* pnewtextnode = new wxXmlNode(pnewnode, wxXML_TEXT_NODE, wxnodename);
      pnewnode->SetContent("");
      pnewtextnode->SetContent(wxnodevalue);
   }
}
#endif
// ------------------------------------------------------------------------------
/**
 * Establece el valor Value en el parametro Name.
 * En caso que el parametro Name no exista en el nivel 1 del
 * configuracion.xml lo agrega
 * @param[in] Name parametro al cual se le quiere asignar un valor
 * @param[in] Value valor que se quiere asignar al parametro
 */
void Configuration::SetParameter(const std::string &Name, const long Value) {
   wxString wxstr;
   wxstr << Value;
   std::string str = std::string(wxstr.mb_str());
   Configuration::SetParameter(Name, str);
}

// ------------------------------------------------------------------------------
/**
 * Establece el valor Value en el parametro Name.
 * En caso que el parametro Name no exista en el nivel 1 del
 * configuracion.xml lo agrega
 * @param[in] Name parametro al cual se le quiere asignar un valor
 * @param[in] Value valor que se quiere asignar al parametro
 */
void Configuration::SetParameter(const std::string &Name, const double Value) {
   wxString wxstr;
   wxstr << Value;
   std::string str = std::string(wxstr.mb_str());
   Configuration::SetParameter(Name, str);
}


/**
 * Establece el valor Value en el parametro Name en un cache en memoria.
 */
void Configuration::SetParameterEx(const std::string& Name, const std::string& Value) {
   memParams_[Name] = Value;
}

void Configuration::SetParameterEx(const std::string& Name, int Value) {
   wxString wxstr;
   wxstr<<Value;
   memParams_[Name] = wxstr.mb_str();
}

int Configuration::GetParameterEx(const std::string& Name, const int Value) {
   wxString wxstr;
   wxstr = Configuration::GetParameterEx(Name, "");
   long int value=Value;
   if (!wxstr.ToLong(&value))
        value =Value;
   return static_cast<int>(value);
}

/**
 * Remueve el valor con la clave Name del cache en memoria.
 */
void Configuration::RemoveParameterEx(const std::string& Name) {
   memParams_.erase(Name);
}

// ------------------------------------------------------------------------------
/** Indica el nombre y ruta completos y absolutos del archivo a usar */
/**
 * @param Filename Archivo de configuracion
 * @return true Si existe el archivo
 * @return false Si no existe el archivo
 */
bool Configuration::SetConfigFile(const std::string &UserFilename, const std::string &SystemFilename ) {
   wxLogNull nolog;
   configurationFile_ = UserFilename;
   configurationUserFile_ = UserFilename;
   configurationSystemFile_ = SystemFilename;
   /**
    * Se comento la porcion de linea "&& Load()" debido a que cuando
    * se comenzo la implementacion de las opciones de configuracion ( Ticket #6494 )
    * generaba un error de escritura, acusando un "user-mapped error" al momento
    * de querer guardar los cambios.
    */
   return wxFile::Exists(configurationFile_.c_str()) ; //&&Load();
}

/**
 * Guarda la configuracion en el archivo
 * @param[in] ConfigurationFile nombre del archivo de configuracion
 */
void Configuration::Save(const std::string &ConfigurationFile) {
   std::string configfile = ConfigurationFile.empty() ? configurationUserFile_ : ConfigurationFile;
   // actualiza los valores del map de usuario al xml
   SetParameters();
   // Guarda el documento en disco
   if (pXmlDoc_->Save(configfile.c_str(), wxXML_NO_INDENTATION)) {
      SetConfigFile(configfile, configurationSystemFile_);
   }
}

/**
 * Carga la configuracion
 * @return true si puede cargar el archivo configuracion
 * @return false en otro caso
 */
bool Configuration::Load() {
   // Fix (Windows unicamente): quiere cargar la configuracion antes de que se instancie pXmlDoc_
   if (!pXmlDoc_)
      pXmlDoc_ = new wxXmlDocument;
   // Carga el documento en memoria. Si falla devuelve ""
   if (!pXmlDoc_->IsOk()) {
      if (!wxFileExists(configurationFile_))
         return false;

      return pXmlDoc_->Load(configurationFile_, wxLocale::GetSystemEncodingName(),
                            wxXMLDOC_KEEP_WHITESPACE_NODES);
   }
   return true;
}

/** Expande las variables */
/**
 *  Expande del string las variables de configuracion referenciadas como ${NAME}
 * que no pueden tener adentro el caracter }
 *
 * @param[in] Value Valor de la variable
 * @param[out] Value Variable con los reemplazos realizados
 * @return True en caso de haber hecho al menos una expansion
 */
bool Configuration::Expand(std::string &Value) {
   std::string::size_type pos = Value.find("${");
   if (pos == std::string::npos) {
      return false;
   }
   std::string name = Value.substr(pos + 2, Value.find("}") - 2 - pos);
   std::string value = wxT("");
   value = GetParameter(name.c_str(), value.c_str());
   // reemplazo la variable entre ${ } con su contenido
   std::string::size_type inic = Value.find(name);
   Value = Value.substr(0, inic - 2) + value + Value.substr(inic + 1 + name.length());
   Expand(Value);
   return true;
}



void Configuration::SetAttribute(wxXmlNode *Pnode, const wxString& name, const wxString& value) {
    wxXmlProperty* attr=Pnode->GetProperties();
    while (attr) {
        if (attr->GetName() == name) {
            attr->SetValue(value);
            return;
        }
        attr = attr->GetNext();
    }
    // non-exist, create it
    Pnode->AddProperty(name, value);
}



/** Guarda el map memUserParams_ en el SoPI.xml
 * solo si hubo algun cambio**/
void Configuration::SetParameters(){
   pXmlDoc_ = pXmlUserDoc_;
   configurationFile_ = configurationUserFile_;
   if (!Load() || !changed_) {
         return;
   }
   wxXmlNode *pnode = pXmlDoc_->GetRoot()->GetChildren();
   std::map<std::string, std::string> auxMap = memUserParams_;
   wxString wxnodename;
   wxString wxnodevalue;
   while (pnode) {
      wxnodename = pnode->GetName();
      wxnodevalue = auxMap[wxnodename.c_str()];
      if (auxMap.find(wxnodename.c_str()) != auxMap.end()) {
         if (wxnodevalue != "") {
            wxXmlNode* pchild = pnode->GetChildren();
            if (pchild) pchild->SetContent(wxnodevalue);
         }
         auxMap.erase(wxnodename.c_str());
      }
      pnode = pnode->GetNext();
   }
   std::map<std::string, std::string>::const_iterator it=auxMap.begin();
   for (; it != auxMap.end(); ++it) {
      wxnodename = wxT((*it).first);
      wxnodevalue = wxT((*it).second);
      wxXmlNode* pnewnode = new wxXmlNode(pXmlDoc_->GetRoot(), wxXML_ELEMENT_NODE,
                                                  wxnodename);
      wxXmlNode* pnewtextnode = new wxXmlNode(pnewnode, wxXML_TEXT_NODE, wxnodename);
      pnewnode->SetContent("");
      pnewtextnode->SetContent(wxnodevalue);
   }
   changed_=false;
}

void Configuration::SetParameter(std::string& Name, std::string Value){
   memBackup_=memUserParams_;
   memUserParams_[Name] = Value;
   changed_=true;
}

void Configuration::DiscardChanges(){
   memUserParams_=memBackup_;
}
void Configuration::SetParameter(const std::string& Name, const std::string& Value){
   memBackup_=memUserParams_;
   memUserParams_[Name] = Value;
   changed_=true;
}

}
