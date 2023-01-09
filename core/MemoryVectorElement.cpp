/*! \file MemoryVectorElement.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes suri
#include "suri/MemoryVectorElement.h"
#include "MemoryVector.h"

// Includes wx
#include "wx/filename.h"

// Includes ORG
#include "ogrsf_frmts.h"
#include "wx/dir.h"

/** namespace suri */
namespace suri {
// ------------------------------------------------------------------------------
// --------------------------MEMORY VECTOR ELEMENT-------------------------------
// ------------------------------------------------------------------------------
/** Macro para registrar el tipo de elemento automaticamente */
AUTO_REGISTER_CLASS(Element, MemoryVectorElement, 0)

/** ctor */
MemoryVectorElement::MemoryVectorElement() :
      pMemVector_(NULL), pSavedVector_(NULL) {
}

/** dtor */
MemoryVectorElement::~MemoryVectorElement() {
   Vector::Close(pMemVector_);
   Vector::Close(pSavedVector_);
}

/** Funcion estatica para crear un MemoryVectorElement */
/**
 *  Crea un MemoryVectorElement, genera un vector usando Filename, spatialreference,
 * Layers y Type.
 * Si tuvo exito inicializa el elemento.
 * @param[in] Filename nombre para el vector generado.
 * @param[in] SpatialReference referencia espacial.
 * @param[in] Layers numero de capas
 * @param[in] Type tipo de elemento vectorial
 * \attention el MemoryVectorElement retornado(y su eliminacion) es responsabilidad del invocante
 */
MemoryVectorElement* MemoryVectorElement::Create(const std::string &Filename,
                                                 const std::string &SpatialReference,
                                                 int Layers,
                                                 const Vector::VectorType Type) {
   MemoryVectorElement *pelement = new MemoryVectorElement;
   if (pelement->Initialize((MEMORY + Filename + COLON + SpatialReference).c_str())
         != pelement) {
      delete pelement;
      return NULL;
   }
   return pelement;
}

/** Funcion estatica para crear un MemoryVectorElement */
/**
 *  Crea un MemoryVectorElement, genera un vector usando Filename, spatialreference,
 * Layers y Type.
 * Si tuvo exito inicializa el elemento.
 * @param[in] Filename nombre para el vector generado.
 * @param[in] Layers numero de capas
 * @param[in] Type tipo de elemento vectorial
 * \attention el MemoryVectorElement retornado(y su eliminacion) es responsabilidad del invocante
 */
MemoryVectorElement* MemoryVectorElement::Create(const std::string &Filename,
                                                 int Layers,
                                                 const Vector::VectorType Type) {
   MemoryVectorElement *pelement = new MemoryVectorElement;
   if (pelement->Initialize((MEMORY + Filename).c_str())
         != pelement) {
      delete pelement;
      return NULL;
   }
   return pelement;
}


/**
 * Funcion que salva en un archivo el vector en memoria
 * Guardo vector como ESRI Shapefile y acutalilzo URL y Dependencias en XML
 * @param[in] Filename path del archivo
 * @return true si pudo salvar el vector en memoria a un archivo, false en otro caso
 * \post El elemento deja de estar en edicion
 */
bool MemoryVectorElement::Save(const std::string &Filename) {
   wxFileName fn(Filename);
   if (fn.GetDirCount() == 0) {
      fn.PrependDir(".");
   }
   if (!fn.IsAbsolute()) {
      fn.MakeAbsolute();
   }
   std::string filename = (fn.GetVolume() + fn.GetVolumeSeparator()).c_str();
   if (!fn.IsDir()) {  // en caso de directorio, no quiero el ultimo /
      filename += fn.GetFullPath(wxPATH_UNIX).c_str();
   } else {
      filename += fn.GetPath(wxPATH_GET_VOLUME, wxPATH_UNIX).c_str();
   }
   GetVector()->Save(filename, "ESRI Shapefile");
   // destruyo el vector en memoria
   Vector::Close(pMemVector_);
   // asigno el vector salvado
   pSavedVector_ = Vector::Open(filename.c_str(), Vector::ReadWrite);

#ifndef __ENABLE_DIRECTORY_AS_VECTOR_URL___
   if (wxDir::Exists(filename)) {
      filename += wxFileName::GetPathSeparator(wxPATH_UNIX);
      filename += GetVector()->GetLayer(GetActiveLayer())->GetLayerDefn()->GetName();
      filename += ".shp";
   }
#endif

   // cambio el URL (Propaga cambios)
   SetUrl(filename);
   SetDependencies();
   SetEdited(false);
   SaveVectorExtent();
   return !pMemVector_ && pSavedVector_;
}

/**
 * Funcion que salva en un archivo el vector en memoria
 * @return url del elemento salvado
 */
std::string MemoryVectorElement::Save() {
   std::string filename = GetUrl().c_str();
   filename = GET_NAME(filename);
   if (Save(filename)) {
      return GetUrl().c_str();
   }
   return wxT("");
}

/**
 * Crea un vector en memoria
 * @param[in] Filename Nombre del archivo deseado
 * @param[in] Layers capas del vector
 * @param[in] Type tipo de vector
 * @return puntero al vector creado, NULL si ya existe en memoria un vector con
 *       ese url
 * \attention el Vector* retornado es responsabilidad de esta clase(no eliminar externamente)
 */
Vector *MemoryVectorElement::NewVector(const std::string &Filename, int Layers,
                                       const Vector::VectorType Type) {
   // lo intento abrir sin crear, para verificar si es un vector existente
   Vector *pvector = Vector::Open(Filename, Vector::ReadOnly, Layers, Type);
   if (pvector) {
      SetUrl(Filename);
   } else {
      REPORT_AND_RETURN_VALUE("D:El vector en memoria ya existe", NULL);
   }
   pMemVector_ = pvector;
   return pvector;
}

/**
 * Metodo para obtener la instancia del vector asociada
 * No se debe cerrar nunca.
 * @return instancia del vector asociada, puede ser el vector en memoria o el
 *    vector salvado.
 * @return NULL si existen ambos vectores (memoria y salvado) o no existe ninguno
 * \attention el Vector* retornado es responsabilidad de esta clase(no eliminar externamente)
 */
Vector *MemoryVectorElement::GetVector() const {
   if (pMemVector_ && pSavedVector_) {
      REPORT_DEBUG("D:Inconsistencia. Ambos vectores existen.");
   }
   // si se trata de un vector en memoria lo retorno
   if (pMemVector_) {
      return pMemVector_;
   }
   if (pSavedVector_) {
      return pSavedVector_;
   }REPORT_AND_FAIL_VALUE("D:No hay vector asociado", NULL);

   return NULL;
}

/**
 * Crea un vector en memoria y lo inicializa con el url recibido por parametro,
 * cero capas, y tipo indefinido.
 * @param[in] Filename url del elemento
 * @return puntero a this (instancia del vector en memoria), NULL si fallo la creacion.
 */
MemoryVectorElement* MemoryVectorElement::Initialize(const std::string &Filename) {
   SetEdited();
   suri::Vector *pvector = NewVector(Filename.c_str(), 0, Vector::Undefined);
   // Fallo la creacion del vector
   if (!pvector) {
      REPORT_AND_FAIL_VALUE("D:Error al crear el vector", NULL);
   }
   InitVector(GetVector(), GetUrl().c_str());
   return this;
}

// ------------------------------------------------------------------------------
// ------------------------FIN MEMORY VECTOR ELEMENT-----------------------------
// ------------------------------------------------------------------------------
}

