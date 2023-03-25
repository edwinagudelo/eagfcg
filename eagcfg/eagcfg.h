// eagcfg.h

#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Data::SQLite;
using namespace System::Data;
using namespace System::Windows::Forms;

namespace eagcfg {

	public ref class CConfig
	{
		// TODO: Add your methods for this class here.
	public:
		SQLiteConnection^ conexion;
		SQLiteCommand^ comando;
		SQLiteDataReader^ lector;

		// Funcion que se encarga de crear los parametros de acuerdo a la BD
		bool crear_parametro(String^ parametro,String^ valor,String^ comentario,String^ path,String^ bd){
			bool retorno = false;
			conexion = gcnew SQLiteConnection();
			comando = gcnew SQLiteCommand("Insert Into configuracion(parametro,valor,comentario) Values('" + parametro + "','" + valor + "','" + comentario + "')");
			conexion->ConnectionString = "Data Source = " + path + "\\" + bd + "; Version=3;New=false;Compress=true;";
			try{
				conexion->Open();
				comando->Connection = conexion;
				comando->ExecuteNonQuery();
				conexion->Close();
				retorno = true;
			}catch(SQLiteException^ ex){
				MessageBox::Show("Error Procesando la creacion del parametro->" + ex->Message,"Error",MessageBoxButtons::OK,MessageBoxIcon::Error);
				retorno = false;
			}
			delete conexion;
			delete comando;
			return retorno;
		}

		String^ leer_parametro(String^ parametro, String^ path,String^ bd){
			String^ retorno = "NULO";
			conexion = gcnew SQLiteConnection();
			comando = gcnew SQLiteCommand("Select valor From configuracion Where parametro = '" + parametro + "'");
			conexion->ConnectionString = "Data Source = " + path + "\\" + bd + "; Version=3;New=false;Compress=true;";
			try{
				conexion->Open();
				comando->Connection = conexion;
				lector = comando->ExecuteReader();
				if(!lector->Read()){
					retorno = "NULO";
				}
				else{
					retorno = lector->GetString(0);
				}
				lector->Close();
				conexion->Close();
			}catch(SQLiteException^ ex){
				MessageBox::Show("Error Procesando la lectura  del parametro " + parametro + "->" + ex->Message,"Error",MessageBoxButtons::OK,MessageBoxIcon::Error);
				retorno = "NULO";
			}
			delete conexion;
			delete comando;
			return retorno;
		}

		bool actualizar_parametro(String^ parametro,String^ valor, String^ path,String^ bd){
			bool retorno = false;
			conexion = gcnew SQLiteConnection();
			comando = gcnew SQLiteCommand("Update configuracion Set valor = '" + valor + "' Where parametro = '" + parametro + "'");
			conexion->ConnectionString = "Data Source = " + path + "\\" + bd + "; Version=3;New=false;Compress=true;";
			try{
				conexion->Open();
				comando->Connection = conexion;
				comando->ExecuteNonQuery();
				conexion->Close();
				retorno = true;
			}catch(SQLiteException^ ex){
				MessageBox::Show("Error Procesando la actualizacion del parametro->" + ex->Message,"Error",MessageBoxButtons::OK,MessageBoxIcon::Error);
				retorno = false;
			}
			delete conexion;
			delete comando;
			return retorno;
		}

		bool borrar_parametro(String^ parametro,String^ path,String^ bd){
			bool retorno = false;
			conexion = gcnew SQLiteConnection();
			comando = gcnew SQLiteCommand("Delete From configuracion Where parametro = '" + parametro + "'");
			conexion->ConnectionString = "Data Source = " + path + "\\" + bd + "; Version=3;New=false;Compress=true;";
			try{
				conexion->Open();
				comando->Connection = conexion;
				comando->ExecuteNonQuery();
				conexion->Close();
				retorno = true;
			}catch(SQLiteException^ ex){
				MessageBox::Show("Error Procesando el borrado del parametro->" + ex->Message,"Error",MessageBoxButtons::OK,MessageBoxIcon::Error);
				retorno = false;
			}
			delete conexion;
			delete comando;
			return retorno;
		}

		bool crear_bd(String^ path,String^ bd)
		{
			bool retorno = false;
			//SQLiteConnection::CreateFile(path + "\\" + bd);
			conexion = gcnew SQLiteConnection();
			comando = gcnew SQLiteCommand();
			conexion->ConnectionString = "Data Source = " + path + "\\" + bd + "; Version=3;New=True;Compress=true;";
			comando->CommandText = "CREATE TABLE configuracion (id INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , parametro VARCHAR, valor VARCHAR, comentario VARCHAR)";
			try{
				conexion->Open();
				comando->Connection = conexion;
				comando->ExecuteNonQuery();
				conexion->Close();
				retorno = true;
			}catch(SQLiteException^ ex){
				MessageBox::Show("Error Procesando el borrado del parametro->" + ex->Message,"Error",MessageBoxButtons::OK,MessageBoxIcon::Error);
				retorno = false;
			}
			delete conexion;
			delete comando;
			return retorno;
		}
	};
}
