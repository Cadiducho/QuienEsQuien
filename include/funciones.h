#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <iostream>
#include <cctype>
#include <string>
#include <iterator>
#include <algorithm>

using namespace std;

/**
 * @brief Pausa el programa y espera que el usuario inserte un char
 * @return char insertado
 */
char readChar() {
    char opcion;

    do {
        cin >> opcion;
        opcion = static_cast<char>(tolower(opcion));
    } while (opcion != 's' && opcion != 'n');

    return opcion;
}


/**
 * @brief Pausa el programa y espera que el usuario inserte un int
 * @return int insertado
 */
int readInt() {
    int opcion;

    do {
        cin >> opcion;

        if (cin.fail()) {
            cin.clear();
            cin.ignore();
        }
    } while (opcion < 1 || opcion > 3);

    return opcion;
}

/**
 * @brief Dibuja el esquema de un arbol
 * @param ss stream donde escribirlo
 * @param a arbol que escribir
 * @param n nodo donde empezar
 * @param pre string donde escribir
 */
void escribir_esquema_arbol(ostream& ss, const bintree<Pregunta>& a, bintree<Pregunta>::node n, string& pre){
    if (n.null()){
        ss << pre << "-- x" << endl;
    } else {
        ss << pre << "-- " << (*n) << endl;
        if ( !n.right().null() || !n.left().null()) {// Si no es una hoja
            pre += "   |";
            escribir_esquema_arbol(ss,a, n.right(), pre);
            pre.replace(pre.size()-4, 4, "    ");
            escribir_esquema_arbol (ss,a, n.left(), pre);
            pre.erase(pre.size()-4, 4);
        }
    }
}

/**
 * @brief Genera numero enteros positivos aleatorios en el rango [min,max).
**/
int generaEntero(int min, int max){
    int tam = max - min;
    return ((rand()%tam)+min);
}

/**
  * @brief Devuelve una copia de la cadena original sin las subcadenas no deseadas.
  *
  * @param cadena_original Cadena de la que se eliminan las subcadenas no deseadas.
  * @param cadena_a_eliminar Subcadena que se busca y se elimina.
  *
  * @return Copia de la cadena original sin las subcadenas no deseadas.
  */
string limpiar_string(string cadena_original, string cadena_a_eliminar){
    string linea(cadena_original);

    while(linea.find_first_of(cadena_a_eliminar) != std::string::npos){
        linea.erase(linea.find_first_of(cadena_a_eliminar),cadena_a_eliminar.length());
    }

    return linea;
}

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
    if ( !v.empty() ) {
        out << '[';
        std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
        out << "\b\b]";
    }
    return out;
}


/**
  * @brief Convierte un número a un vector de bool que corresponde
  *        con su representación en binario con un numero de digitos
  *        fijo.
  *
  * @param n Número a convertir en binario.
  * @param digitos Número de dígitos de la representación binaria.
  *
  * @return Vector de booleanos con la representación en binario de @e n
  *      con el número de elementos especificado por @e digitos.

  */
vector<bool> convertir_a_vector_bool(int n, int digitos) {
    vector<bool> ret;
    while(n) {
        ret.push_back((n & 1) != 0);
        n>>=1;
    }

    while(ret.size() < digitos){
        ret.push_back(false);
    }

    reverse(ret.begin(),ret.end());
    return ret;
}

#endif //FUNCIONES_H
