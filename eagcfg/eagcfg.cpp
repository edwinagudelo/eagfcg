#include "stdafx.h"

#include "eagcfg.h"

using namespace eagcfg;

CConfig::CConfig() {
	ruta = "";
	bd = "configuracion.db";
	conexion = nullptr;
	comando = nullptr;
	connectionString = buildConnectionString();
}

CConfig::CConfig(String^ path, String^ db) : ruta(path), bd(db) {
	conexion = nullptr;
	comando = nullptr;
	connectionString = buildConnectionString();
}

CConfig::~CConfig() {
	if (comando != nullptr) {
		delete comando;
	}
	if (conexion->State != ConnectionState::Closed) {
		conexion->Close();
	}
	if (conexion != nullptr){
		delete conexion;
	}
}

bool CConfig::crear_parametro(String^ parametro, String^ valor, String^ comentario, String^ path, String^ bd) {
	bool retorno = false;
	
	this->ruta = path;
	this->bd = bd;
	
	if (!this->connect()) {
		return retorno;
	}
	this->prepareSQL("Insert Into configuracion(parametro,valor,comentario) Values(@paramName, @paramValue, @paramComment)");
	this->comando->Prepare();
	this->comando->Parameters->AddWithValue("@paramName", parametro);
	this->comando->Parameters->AddWithValue("@paramValue", valor);
	this->comando->Parameters->AddWithValue("@paramComment", comentario);
	try {
		comando->ExecuteNonQuery();
		retorno = true;
	}
	catch (SQLiteException^ ex) {
		_logger->Error("Error Procesando la creacion del parametro->" + ex->Message);
		retorno = false;
	}
	return retorno;
}

String^ CConfig::leer_parametro(String^ parametro, String^ path, String^ bd) {
	String^ retorno = "NULO";
	this->ruta = path;
	this->bd = bd;
	if (!this->connect()) {
		return nullptr;
	}
	this->prepareSQL("Select valor From configuracion Where parametro = @paramName");
	this->comando->Parameters->AddWithValue("@paramName", parametro);
	try {
		this->lector = comando->ExecuteReader();
		if (!this->lector->Read()) {
			retorno = nullptr;
		}
		else {
			retorno = lector->GetString(0);
		}
		lector->Close();
	}
	catch (SQLiteException^ ex) {
		_logger->Error("Error Procesando la lectura  del parametro " + parametro + "->" + ex->Message);
		retorno = nullptr;
	}
	return retorno;
}

bool CConfig::actualizar_parametro(String^ parametro, String^ valor, String^ path, String^ bd) {
	bool retorno = false;
	this->ruta = path;
	this->bd = bd;
	if (!this->connect()) {
		return false;
	}
	this->prepareSQL("Update configuracion Set valor = @paramValue Where parametro = @paramName");
	this->comando->Parameters->AddWithValue("@paramName", parametro);
	this->comando->Parameters->AddWithValue("@paramValue", valor);
	try {
		comando->ExecuteNonQuery();
		retorno = true;
	}
	catch (SQLiteException^ ex) {
		_logger->Error("Error Procesando la actualizacion del parametro->" + ex->Message);
		retorno = false;
	}
	return retorno;
}

bool CConfig::borrar_parametro(String^ parametro, String^ path, String^ bd) {
	bool retorno = false;
	this->ruta = path;
	this->bd = bd;
	if (!this->connect()) {
		return false;
	}
	this->prepareSQL("Delete From configuracion Where parametro = @paramName");
	this->comando->Parameters->AddWithValue("@paramName", parametro);
	try {
		comando->ExecuteNonQuery();
		retorno = true;
	}
	catch (SQLiteException^ ex) {
		_logger->Error("Error Procesando el borrado del parametro->" + ex->Message);
		retorno = false;
	}
	return retorno;
}

bool CConfig::crear_bd(String^ path, String^ bd)
{
	bool retorno = false;
	String^ completePath = path + "\\" + bd;
	this->ruta = path;
	this->bd = bd;
	if (!File::Exists(completePath)) {
		SQLiteConnection::CreateFile(completePath);
	}
	if (!this->connect()) {
		return false;
	}
	this->prepareSQL("CREATE TABLE configuracion (id INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , parametro VARCHAR, valor VARCHAR, comentario VARCHAR)");
	try {
		comando->ExecuteNonQuery();
		retorno = true;
	}
	catch (SQLiteException^ ex) {
		_logger->Error("Error Procesando el borrado del parametro->" + ex->Message);
		retorno = false;
	}
	return retorno;
}

String^ CConfig::buildConnectionString(){
	String^ connectionStr = "Data Source = " + this->ruta + "\\" + this->bd + "; Version=3;New=false;Compress=true;";
	this->connectionString = connectionStr;
	return connectionStr;
}

bool CConfig::connect()
{
	try {
		if (this->conexion == nullptr) {
			this->conexion = gcnew SQLiteConnection();
			this->connectionString = buildConnectionString();
		}
		if (this->conexion->State != ConnectionState::Open) {
			this->conexion->Open();
		}
	}
	catch (Exception^ ex) {
		_logger->Error("Error conectando a la base de datos ->" + ex->Message);
		return false;
	}
	return true;
}

void CConfig::prepareSQL(String^ strsql) {
	if (this->comando == nullptr) {
		this->comando = gcnew SQLiteCommand();
	}
	this->comando->Parameters->Clear();
	this->comando->CommandText = strsql;
	this->comando->Connection = this->conexion;
}
