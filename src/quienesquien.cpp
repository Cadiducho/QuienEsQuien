
#include "../include/quienesquien.h"
#include "../include/funciones.h"

#include <cmath>
#include <numeric>

QuienEsQuien::QuienEsQuien() = default;

QuienEsQuien::QuienEsQuien(const QuienEsQuien &quienEsQuien) {
    this->personajes = quienEsQuien.personajes;
    this->atributos = quienEsQuien.atributos;
    this->tablero = quienEsQuien.tablero;
    this->arbol = quienEsQuien.arbol;
    this->jugada_actual = quienEsQuien.jugada_actual;
}

QuienEsQuien& QuienEsQuien::operator= (const QuienEsQuien &quienEsQuien) {
    if (this != &quienEsQuien) {
        this->limpiar();
        this->personajes = quienEsQuien.personajes;
        this->atributos = quienEsQuien.atributos;
        this->tablero = quienEsQuien.tablero;
        this->arbol = quienEsQuien.arbol;
        this->jugada_actual = quienEsQuien.jugada_actual;
    }

    return *this;
}

QuienEsQuien::~QuienEsQuien() {
    this->limpiar();
}

void QuienEsQuien::limpiar() {
    personajes.clear();
    atributos.clear();

    for (auto &it : tablero) {
        it.clear();
    }

    tablero.clear();

    arbol.clear();
    jugada_actual = bintree<Pregunta>::node();
}

void QuienEsQuien::mostrar_estructuras_leidas(){
    cout << "personajes: "<< (this->personajes) << endl;
    cout << "atributos:  "<< (this->atributos)  << endl;
    cout << "tablero:    "<< endl;


    // Escribe la cabecera del tablero
    for (auto &atributo : this->atributos) {
        cout << atributo << "\t";
    }
    cout << endl;

    int indice_personaje = 0;
    for (auto &it_tablero_atributos : tablero) {

        string personaje = this->personajes[indice_personaje];
        int indice_atributo = 0;
        for (auto &&it_tablero_atributo : it_tablero_atributos) {

            cout << it_tablero_atributo <<"\t";

            indice_atributo++;
        }

        cout << personaje << endl;

        indice_personaje++;
    }

}

istream& operator >> (istream& is, QuienEsQuien &quienEsQuien) {
    quienEsQuien.limpiar();

    if(is.good()){
        string linea;
        getline(is, linea, '\n');

        linea = limpiar_string(linea,"\r");

        while(linea.find('\t') != string::npos ){
            string atributo = linea.substr(0,linea.find('\t'));
            quienEsQuien.atributos.push_back(atributo);
            linea = linea.erase(0,linea.find('\t')+1);
        }

        assert(linea ==  "Nombre personaje");
    }

    while( is.good() ) {
        string linea;
        getline(is, linea, '\n');
        linea = limpiar_string(linea,"\r");

        //Si la linea contiene algo extrae el personaje. Si no lo es, la ignora.
        if(!linea.empty()){;
            vector<bool> atributos_personaje;

            int indice_atributo=0;
            while(linea.find('\t') != string::npos){
                string valor = linea.substr(0,linea.find('\t'));

                assert(valor == "0" || valor == "1");

                bool valor_atributo = valor == "1";

                atributos_personaje.push_back(valor_atributo);

                linea = linea.erase(0,linea.find('\t')+1);
                indice_atributo++;
            }

            string nombre_personaje = linea;

            quienEsQuien.personajes.push_back(nombre_personaje);
            quienEsQuien.tablero.push_back(atributos_personaje);
        }
    }

    return is;
}

ostream& operator << (ostream& os, const QuienEsQuien &quienEsQuien){

    //Escribimos la cabecera, que contiene los atributos y al final una columna para el nombre
    for (const auto &atributo : quienEsQuien.atributos) {
        os  << atributo << "\t";
    }
    os << "Nombre personaje" << endl;

    //Rellenamos con ceros y unos cada línea y al final ponemos el nombre del personaje.
    for (int indice_personaje=0;indice_personaje<quienEsQuien.personajes.size();indice_personaje++){
        for (int indice_atributo=0;indice_atributo<quienEsQuien.atributos.size();indice_atributo++){

            os  << quienEsQuien.tablero[indice_personaje][indice_atributo] << "\t";
        }
        os  << quienEsQuien.personajes[indice_personaje] << endl;
    }

    return os;
}

void QuienEsQuien::tablero_aleatorio(int numero_de_personajes){
    srand(0);

    this->limpiar();

    auto log_2_numero_de_personajes = static_cast<float>(log(numero_de_personajes) / log(2));

    auto numero_de_atributos = static_cast<int>(ceil(log_2_numero_de_personajes));

    cout << "Petición para generar "<< numero_de_personajes<<" personajes ";
    cout << "con "<<numero_de_atributos<< " atributos"<<endl;
    cout << "Paso 1: generar "<< pow(2,numero_de_atributos) << " personajes"<<endl;

    //Fase 1: completar el tablero con todos los personajes posibles
    //Completo el tablero y los nombres de personajes a la vez
    for(int indice_personaje=0;indice_personaje< pow(2,numero_de_atributos);indice_personaje++){
        vector<bool> atributos_personaje = convertir_a_vector_bool(indice_personaje,numero_de_atributos);
        string nombre_personaje = "Personaje_"+to_string(indice_personaje);

        this->personajes.push_back(nombre_personaje);
        this->tablero.push_back(atributos_personaje);
    }

    // Completo los nombres de los atributos.
    for(int indice_atributo=0;indice_atributo<numero_de_atributos;indice_atributo++){
        string nombre_atributo = "Atr_"+to_string(indice_atributo);
        this->atributos.push_back(nombre_atributo);
    }

    cout << "Paso 2: eliminar "<< (pow(2,numero_de_atributos)-numero_de_personajes) << " personajes"<<endl;
    //Fase 2. Borrar personajes aleatoriamente hasta que quedan solo los que hemos pedido.
    while (personajes.size() > numero_de_personajes){
        int personaje_a_eliminar = generaEntero(0,personajes.size());

        personajes.erase(personajes.begin()+personaje_a_eliminar);
        tablero.erase(tablero.begin()+personaje_a_eliminar);
    }
}

bintree<Pregunta> QuienEsQuien::crear_arbol() {

    //la raiz del arbol será la pregunta que más discretice
    vector<int> usados;
    int mejorAtr = mejor_atributo(usados);
    usados.push_back(mejorAtr);

    Pregunta raiz = Pregunta(*(atributos.begin() + mejorAtr), personajes.size());
    bintree<Pregunta> arbol(raiz);

    //se rellena el arbol creado con sus atributos
    rellenar_ramas(arbol, arbol.root(), usados);

    //finalmente se recorre el arbol utilizado para colgar a sus personajes
    bintree<Pregunta>::node nodo_raiz = arbol.root();
    for (auto it = tablero.begin(); it != tablero.end(); it++) {
        int index = it - tablero.begin();
        Pregunta p(*(personajes.begin() + index), 1);

        colgar_hoja_personaje(p, arbol, nodo_raiz, it->begin(), it->end());
    }

    return arbol;
}

void QuienEsQuien::colgar_hoja_personaje(Pregunta &pregunta, bintree<Pregunta> &arbol, bintree<Pregunta>::node &nodo, vector<bool>::const_iterator it, vector<bool>::const_iterator end) {
    if (it == end)
        return;

    if (nodo.right().null() && nodo.left().null()) { //si es una hoja
        if (*it != 0) {
            arbol.insert_left(nodo, pregunta);
        } else {
            arbol.insert_right(nodo, pregunta);
        }
    } else {
        auto nodoSiguiente = *it != 0 ? nodo.left() : nodo.right();
        if (!nodoSiguiente.null()) {
            (*nodoSiguiente).incrementarPersonajes();
            colgar_hoja_personaje(pregunta, arbol, nodoSiguiente, it + 1, end);
        }
    }
}

void QuienEsQuien::rellenar_ramas(bintree<Pregunta> &arbol, const bintree<Pregunta>::node &root, vector<int> usados) const {
    int mejor = mejor_atributo(usados);

    //si no es -1, quedan aún atributos por rellenar
    if (mejor < 0) {
        return;
    }

    usados.push_back(mejor);
    Pregunta p = Pregunta(*(atributos.begin() + mejor), 0);

    arbol.insert_left(root, p);
    arbol.insert_right(root, p);

    rellenar_ramas(arbol, root.left(), usados);
    rellenar_ramas(arbol, root.right(), usados);
}

double QuienEsQuien::calcular_entropia(int atributo) const {
    int positivos = 0;
    int negativos = 0;
    for (const auto &it : tablero) {
        auto it2 = it.begin() + atributo;
        if (*(it2)) positivos++;
        else negativos++;
    }

    int total = positivos + negativos;
    double ratioPositivo = (double) positivos / total;
    double ratioNegativo = (double) negativos / total;

    if (ratioPositivo != 0) ratioPositivo = -1 * (ratioPositivo) * log(ratioPositivo);
    if (ratioNegativo != 0) ratioNegativo = -1 * (ratioNegativo) * log(ratioNegativo);

    double entropia = ratioPositivo + ratioNegativo;
    return entropia;
}

int QuienEsQuien::mejor_atributo(const vector<int> &utilizados) const {
    double entropia = INFINITY;
    int atributo = -1;

    for (int i = 0; i < atributos.size(); i++) {

        //si ese atributo ya ha sido utilizado, continuar el bucle
        if (std::find(utilizados.begin(), utilizados.end(), i) != utilizados.end())
            continue;

        //calcular su entropía
        double e = calcular_entropia(i);
        if (e < entropia) {
            entropia = e;
            atributo = i;
        }
    }

    return atributo;
}

void QuienEsQuien::usar_arbol(bintree<Pregunta> arbol_nuevo) {
    this->arbol = arbol_nuevo;
}

void QuienEsQuien::iniciar_juego() {
    this->jugada_actual = this->arbol.root();

    char opcion;
    int opt;
    bool jugar = true;

    cout << "Quien es Quien: " << endl << endl;

    while (jugar) {
        jugada_actual = arbol.root();

        while (!(*jugada_actual).es_personaje()) {
            cout << endl << "Escribe tu acción:" << endl;
            cout << "1 -- Siguiente turno" << endl
                 << "2 -- Información de la jugada" << endl
                 << "3 -- Camino del arbol tomado" << endl;

            opt = readInt();

            if (opt == 1) {
                cout << *(jugada_actual) << "( s / n ): " << endl;
                opcion = readChar();

                if (opcion == 's') {
                    jugada_actual = jugada_actual.left();
                } else {
                    jugada_actual = jugada_actual.right();
                }
            } else if (opt == 2) {
                set<string> restantes = informacion_jugada(jugada_actual);

                cout << "Personajes aún no tumbados:" << endl;

                for (auto it = restantes.begin(); it != restantes.end(); ++it) {
                    cout << *it << endl;
                }
            } else {
                set<string> preguntas = preguntas_formuladas(jugada_actual);

                cout << "Personajes con estos datos:" << endl;

                for (auto it = preguntas.begin(); it != preguntas.end(); ++it) {
                    cout << *it << endl;
                }

                cout << "Aún no sé seguro cuál es" << endl;
            }
        }

        cout << '\n' << (*jugada_actual).obtener_personaje() << endl;

        cout << "¿Volver a jugar? ( s / n ): " << endl;
        opcion = readChar();

        if (opcion != 's') jugar = false;

        cout << endl << endl;
    }
}

set<string> QuienEsQuien::informacion_jugada(bintree<Pregunta>::node jugada_actual) {
    set<string> personajes_levantados;
    bintree<Pregunta> aux;
    bintree<Pregunta>::level_iterator it;

    bool isRoot = jugada_actual == arbol.root();
    bintree<Pregunta>::node parent = isRoot ? bintree<Pregunta>::node() : jugada_actual.parent();
    bool isLeftSon = !isRoot && jugada_actual == parent.left();

    if (isRoot) {
        aux = arbol;
    } else {
        if (isLeftSon) {
            arbol.prune_left(parent, aux);
        } else {
            arbol.prune_right(parent, aux);
        }
    }

    for (it = aux.begin_level(); it != aux.end_level(); ++it) {
        if ((*it).es_personaje()) {
            personajes_levantados.insert((*it).obtener_personaje());
        }
    }

    if (isRoot) { //si la jugada es la raiz borrarla
        aux.clear();
    } else {
        if (isLeftSon) {
            arbol.insert_left(parent, aux);
        } else {
            arbol.insert_right(parent, aux);
        }
    }

    return personajes_levantados;
}

void QuienEsQuien::escribir_arbol_completo() const{
    string pre;
    escribir_esquema_arbol(cout,this->arbol,this->arbol.root(), pre);
}


void QuienEsQuien::eliminar_nodos_redundantes() {
    arbol = eliminar_nodos_redundantes(arbol, arbol.root());
}

bintree<Pregunta> QuienEsQuien::eliminar_nodos_redundantes(bintree<Pregunta> &arbol, const bintree<Pregunta>::node &nodo) {
    if ((nodo.right().null() && nodo.left().null())) {
        return bintree<Pregunta>(*nodo);
    }

    bintree<Pregunta>::node left = nodo.left();
    bintree<Pregunta>::node right = nodo.right();
    bintree<Pregunta> nuevoArbol;

    if (!(!left.null() && (*left).obtener_num_personajes() > 0)) {
        bintree<Pregunta> rightTree = eliminar_nodos_redundantes(arbol, nodo.right());
        nuevoArbol.assign_subtree(rightTree, rightTree.root());
    } else if (!(!right.null() && (*right).obtener_num_personajes() > 0)) {
        bintree<Pregunta> leftTree = eliminar_nodos_redundantes(arbol, nodo.left());
        nuevoArbol.assign_subtree(leftTree, leftTree.root());
    } else {
        bintree<Pregunta> leftTree = eliminar_nodos_redundantes(nuevoArbol, nodo.left());
        bintree<Pregunta> rightTree = eliminar_nodos_redundantes(nuevoArbol, nodo.right());

        nuevoArbol = bintree<Pregunta>(*nodo);
        nuevoArbol.insert_left(nuevoArbol.root(), leftTree);
        nuevoArbol.insert_right(nuevoArbol.root(), rightTree);
    }

    return nuevoArbol;
}

int QuienEsQuien::numero_personajes() {
    int total = 0;
    for (auto it = arbol.begin_inorder(); it != arbol.end_inorder(); ++it) {
        if ((*it).es_personaje()) total++;
    }
    return total;
}

int QuienEsQuien::numero_preguntas() {
    int total = 0;
    for (auto it = arbol.begin_inorder(); it != arbol.end_inorder(); ++it) {
        if ((*it).es_pregunta()) total++;
    }
    return total;
}

float QuienEsQuien::profundidad_promedio_hojas() {
    vector<int> profundidades;

    for (bintree<Pregunta>::preorder_iterator it = arbol.begin_preorder(); it != arbol.end_preorder(); ++it) {

        //busco por niveles hasta que es personaje
        if ((*it).es_personaje()) {
            bintree<Pregunta>::node node = it.nodo();
            int level = 0;

            //subo hasta encontrar al padre para contar la profundidad del personaje
            while (!node.null()) {
                level++;
                node = node.parent();
            }

            profundidades.push_back(level);
        }
    }

    float total = 0;
    for (auto &i : profundidades) {
        total += i;
    }

    return total / profundidades.size();
}

float QuienEsQuien::altura_arbol(bintree<Pregunta>::node node) {
    if (node.null()) return 0;
    float leftHeight = altura_arbol(node.left());
    float rightHeight = altura_arbol(node.right());

    float max = leftHeight > rightHeight ? leftHeight : rightHeight;

    return max + 1;
}

set<string> QuienEsQuien::preguntas_formuladas(bintree<Pregunta>::node jugada) {
    set<string> preguntas;
    bintree<Pregunta>::node padre = jugada.parent();
    bintree<Pregunta>::node nodeAux = jugada;

    while (!padre.null()) {
        string cadena((*padre).obtener_pregunta());

        if (padre.left() == nodeAux)
            cadena += " - si";
        else
            cadena += " - no";

        preguntas.insert(cadena);
        nodeAux = nodeAux.parent();
        padre = padre.parent();
    }

    return preguntas;
}
