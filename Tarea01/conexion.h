/* 
 * File:   conexion.h
 * Author: ignacio
 *
 * Created on 1 de octubre de 2014, 07:25 PM
 */

#ifndef CONEXION_H
#define	CONEXION_H

class conexion {
private:
	PGconn* cnn;
	PGresult* result;

	// private methods

	PGresult* getResult(const char* query) const;
	// mostrar consulta
	void DisplayResult(void);
public:
	conexion(const char* host, const char* port, const char* dataBase,
			const char* user, const char* passwd) {
		this->cnn = PQsetdbLogin(host, port, NULL, NULL, dataBase, user,
				passwd);
	}
	void showQuery(const char* query) {

		if (PQstatus(cnn) != CONNECTION_BAD) {
			// estamos conectados!
			this->result = PQexec(cnn, query);

			if (this->result != NULL) {
				int tuplas = PQntuples(this->result);
				int campos = PQnfields(this->result);

				for (int i = 0; i < campos; i++) {
					cout << PQfname(this->result, i) << "\t\t" << " | ";
				}
				cout << endl;
				for (int i = 0; i < tuplas; i++) {
					for (int j = 0; j < campos; j++) {
						cout << PQgetvalue(this->result, i, j) << "\t\t"
								<< " | ";
					}
					cout << endl;
				}
			}

			// Ahora nos toca liberar la memoria
			PQclear(this->result);

		} else {
			cout << "Error de conexion" << endl;
		}
	}

};

#endif	/* CONEXION_H */

