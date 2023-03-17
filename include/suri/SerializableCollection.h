/*! \file SerializableCollection.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SERIALIZABLECOLLECTION_H_
#define SERIALIZABLECOLLECTION_H_

#include <map>

#include "suri/Serializer.h"
#include "suri/Serializable.h"
#include "suri/SerializableFactory.h"
#include "suri/SerializableVector.h"
#include "suri/XmlFunctions.h"
#include "logmacros.h"

namespace suri {

/**
 * Contendor generico (map) que permite guardar en el mismo contenedor valores
 * de cualquier tipo asociandolo con una clave.
 * Puede almacenar valores de cualquier tipo, con un wrapper no serializable
 * como también guardar instancias de Serializable que luego serán serializadas
 * al llamar a SerializeXml de la colección.
 */
class SerializableCollection : public Serializable{
public:
   /**
    * Ctor.
    */
   SerializableCollection() : defaultSerializer_() { }

   /**
    * Dtor.
    */
   virtual ~SerializableCollection();

   /**
    * Verifica si existe algun elemento con la clave dada.
    *
    * @param[in] KeyP Clave que se quiere verificar.
    * @return true si existe algun elemento con esa clave.
    */
   bool Contains(const std::string &KeyP) const;

   /**
    * Agrega un elemento de cualquier tipo al contenedor asociado a una clave.
    *
    * @param[in] KeyP Clave del elemento.
    * @param[in] ValueP Elemento que se desea guardar, se realiza una copia
    * en el heap, y debe ser serializable.
    */
   template<class T>
   void AddSerializableValue(const std::string& KeyP, const Serializable* pValueP) {
      if (params_.find(KeyP) != params_.end()){
    	  delete params_[KeyP];
      }
      params_[KeyP] = new T(*static_cast<const T*>(pValueP));
   }

   /**
    * Agrega un elemento de cualquier tipo al contenedor asociado a una clave.
    *
    * @param[in] KeyP Clave del elemento.
    * @param[in] ValueP Elemento que se desea guardar, se realiza una copia
    * en el heap, y debe ser serializable.
    */
   template<class T>
   void AddSerializableValue(const std::string& KeyP, const std::vector<T>& vect) {
      if (params_.find(KeyP) != params_.end()){
        delete params_[KeyP];
      }
      params_[KeyP] = new SerializableVector<T>(vect);
   }

   /**
    * Agrega un elemento de cualquier tipo al contenedor asociado a una clave.
    *
    * @param[in] KeyP Clave del elemento.
    * @param[in] ValueP Elemento que se desea guardar, se realiza una copia
    * en el heap, y puede ser cualquier cosa.
    */
   template<class T>
   void AddValue(const std::string& KeyP, T* pValueP) {
      if (params_.find(KeyP) != params_.end()){
         delete params_[KeyP];
      }
      params_[KeyP] = new NonSerializable<T>(*pValueP);
   }

   /**
    * Obtiene un elemento del contendor basado en la clave dada.
    *
    * @param[in] KeyP Clave del elemento.
    * @param[in] ValueP Refencia en donde se va a guardar el elemento.
    * @return true si el elemento fue encontrado.
    */
   template<class T>
   bool GetValue(const std::string& KeyP, T& ValueP) const{
      T* pValue = NULL;
      if (Contains(KeyP)) {
         pValue = (T*) params_.find(KeyP)->second->GetValue();
         ValueP = *pValue;
         return true;
      }
      return false;
   }

   /**
    * Agrega un serializador custom a la colección.
    * El mismo es eliminado en el destructor de la clase.
    */
   void AddSerializer(const std::string& KeyP, Serializer* Serializer);

   wxXmlNode* SerializeXml() const;

   static SerializableCollection* DeserializeXml(wxXmlNode* pRoot);

private:
   std::map<std::string, Serializable*> params_;
   std::map<std::string, Serializer*> serializerMap_;
   const DefaultSerializer defaultSerializer_;
   /**
    * Serializa un parámetro y devuelve una instancia nueva de wxXmlNode
    */
   wxXmlNode* SerializeParam(const std::string& Name,
                             Serializable* Serializable) const;

   /**
    * Deserializa un parámetro y lo agrega al mapa
    */
   void DeserializeParam(const wxXmlNode* pParam);
   /**
    * Obtiene el serializador asociado a un parámetro.
    * En caso de que no haya ningún serializador, se devuelve el default
    */
   Serializer* GetSerializer(const std::string& KeyP) const;
};

}

#endif /* SERIALIZABLECOLLECTION_H_ */
